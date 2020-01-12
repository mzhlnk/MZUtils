#pragma once

#ifndef _MZHSQLITE
#define _MZHSQLITE

#include <Windows.h>
#include <mzhBase.h>

_MZH_BEGIN

namespace SQLite3
{

#include <sqlite3.h>
#pragma comment(lib, "sqlite3.lib")

	typedef sqlite3*		SQL_HANDLE;
	typedef sqlite3_stmt*	OP_HANDLE;
	typedef sqlite3_value	SQL_VALUE, *PSQL_VALUE;
	typedef const SQL_VALUE* PCSQL_VALUE;
	typedef sqlite3_destructor_type SQL_DESTRUCTOR_TYPE;

	enum EVALTYPE {
		typeINT = SQLITE_INTEGER,
		typeFLOAT = SQLITE_FLOAT,
		typeTEXT = SQLITE_TEXT,
		typeBLOB = SQLITE_BLOB,
		typeNULL = SQLITE_NULL,
	};

	inline void SQLITE_FINALIZE_S(OP_HANDLE& hOp)
	{
		if (hOp)
		{
			sqlite3_finalize(hOp);
			hOp = NULL;
		}
	}

	template<bool t_bManaged>
	class CSQLiteT
	{
	public:
		CSQLiteT(SQL_HANDLE hDB = NULL)
			: m_hDB(hDB)
		{}
		virtual ~CSQLiteT()
		{
			if (t_bManaged)
				close();
		}

	protected:
		SQL_HANDLE m_hDB;

	public:
		operator SQL_HANDLE() const { return m_hDB; }

	public:
		int open(LPCSTR lpszFileName)
		{
			close();
			int ret = sqlite3_open(lpszFileName, &m_hDB);
			if (SQLITE_OK != ret)
			{
				close();
				TRACE1("failed to open datebase: %d", ret);
			}
			return ret;
		}
		int open(LPCWSTR lpszFileName)
		{
			close();
			int ret = sqlite3_open16(lpszFileName, &m_hDB);
			if (SQLITE_OK != ret)
			{
				close();
				TRACE1("failed to open datebase: %d", ret);
			}
			return ret;
		}
		void close()
		{
			if (m_hDB)
			{
				sqlite3_close(m_hDB);
				m_hDB = NULL;
			}
		}

	public:
		OP_HANDLE prepare(PCSTR pszSQL, int cch = -1)
		{
			OP_HANDLE hOp(NULL);
			int ret = sqlite3_prepare(m_hDB, pszSQL, cch, &hOp, NULL);
			if (SQLITE_OK != ret)
			{
				SQLITE_FINALIZE_S(hOp);
				TRACE1("failed to prepare: %d\n", ret);
			}
			return hOp;
		}
		OP_HANDLE prepare(PCWSTR pszSQL, int cch = -1)
		{
			OP_HANDLE hOp(NULL);
			int ret = sqlite3_prepare16(m_hDB, pszSQL, (cch > 0 ? sizeof(WCHAR) * cch : -1), &hOp, NULL);
			if (SQLITE_OK != ret)
			{
				SQLITE_FINALIZE_S(hOp);
				TRACE1("failed to prepare: %d\n", ret);
			}
			return hOp;
		}
		OP_HANDLE prepare_v2(PCSTR pszSQL, int cch = -1)
		{
			OP_HANDLE hOp(NULL);
			int ret = sqlite3_prepare_v2(m_hDB, pszSQL, cch, &hOp, NULL);
			if (SQLITE_OK != ret)
			{
				SQLITE_FINALIZE_S(hOp);
				TRACE1("failed to prepare v2: %d\n", ret);
			}
			return hOp;
		}
		OP_HANDLE prepare_v2(PCWSTR pszSQL, int cch = -1)
		{
			OP_HANDLE hOp(NULL);
			int ret = sqlite3_prepare16_v2(m_hDB, pszSQL, (cch > 0 ? sizeof(WCHAR) * cch : -1), &hOp, NULL);
			if (SQLITE_OK != ret)
			{
				SQLITE_FINALIZE_S(hOp);
				TRACE1("failed to prepare v2: %d\n", ret);
			}
			return hOp;
		}

	public:
		template<typename PCTEXT>
		bool doSQL(PCTEXT pszCmd, int nChars = -1)
		{
			OP_HANDLE hOp = prepare_v2(pszCmd, nChars);
			if (NULL == hOp)
			{
				TRACE0("failed to prepare sql\n");
				return false;
			}
			int ret = sqlite3_step(hOp);
			if (SQLITE_DONE != ret)
			{
				TRACE1("failed to do sql: %d\n", ret);
				sqlite3_finalize(hOp);
				return false;
			}
			ret = sqlite3_finalize(hOp);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to end sql: %d\n", ret);
				return false;
			}
			return true;
		}

	public:
		typedef int(*ExecProc)(LPVOID, int, PSTR*, PSTR*);
		int exec(PCSTR pszCmd, ExecProc lpExecFunc = NULL, LPVOID argv = NULL, PSTR* errmsg = NULL)
		{
			return sqlite3_exec(m_hDB, pszCmd, lpExecFunc, argv, errmsg);
		}

	public:
		INT64 last_insert_rowid()
		{
			return sqlite3_last_insert_rowid(m_hDB);
		}

	public:
		OP_HANDLE enumTable()
		{
			PCSTR pszSql = "SELECT name FROM sqlite_master WHERE type=\'table\' AND name NOT LIKE \'sqlite_%%\'";
			OP_HANDLE hOp = prepare_v2(pszSql);
			return hOp;
		}

	public:
		bool delTable(LPCSTR pszTable)
		{
			ASSERT(pszTable);
			const size_t len = strlen(pszTable) + 20;
			PSTR pszCmd = new CHAR[len];
			sprintf_s(pszCmd, len, "DROP TABLE %s", pszTable);
			bool bret = doSQL(pszCmd);
			delete[] pszTable;
			return bret;
		}
		bool delTable(LPCWSTR pszTable)
		{
			ASSERT(pszTable);
			const size_t len = wcslen(pszTable) + 20;
			PWSTR pszCmd = new WCHAR[len];
			swprintf_s(pszCmd, len, L"DROP TABLE %s", pszTable);
			bool bret = doSQL(pszCmd);
			delete[] pszTable;
			return bret;
		}
		bool renTable(LPCSTR pszTable, LPCSTR pszNewTable)
		{
			ASSERT(pszTable && pszNewTable);
			const size_t len = strlen(pszTable) + strlen(pszNewTable) + 32;
			PSTR pszCmd = new CHAR[len];
			sprintf_s(pszCmd, len, "ALTER TABLE %s RENAME TO %s", pszTable, pszNewTable);
			bool bret = doSQL(pszCmd);
			delete[] pszTable;
			return bret;
		}
		bool renTable(LPCWSTR pszTable, LPCWSTR pszNewTable)
		{
			ASSERT(pszTable && pszNewTable);
			const size_t len = wcslen(pszTable) + wcslen(pszNewTable) + 32;
			PWSTR pszCmd = new WCHAR[len];
			swprintf_s(pszCmd, len, L"ALTER TABLE %s RENAME TO %s", pszTable, pszNewTable);
			bool bret = doSQL(pszCmd);
			delete[] pszTable;
			return bret;
		}
	};
	typedef CSQLiteT<true> CSQLite;
	typedef CSQLiteT<false> CSQLiteHandle;

	class COperate
	{
	public:
		COperate(OP_HANDLE hOp = NULL)
			: m_hOp(hOp)
		{ }
		COperate(SQL_HANDLE hDB, LPCSTR pszSql)
		{
			m_hOp = CSQLiteHandle(hDB).prepare(pszSql);
		}
		COperate(SQL_HANDLE hDB, LPCWSTR pszSql)
		{
			m_hOp = CSQLiteHandle(hDB).prepare(pszSql);
		}
		virtual ~COperate()
		{
			finalize();
		}

	protected:
		OP_HANDLE m_hOp;

	public:
		operator OP_HANDLE() const { return m_hOp; }

	public:
		bool attach(OP_HANDLE hOp)
		{
			if (hOp != m_hOp)
			{
				finalize();
				m_hOp = hOp;
			}
			return true;
		}
		OP_HANDLE dettach()
		{
			OP_HANDLE hOp = m_hOp;
			m_hOp = NULL;
			return hOp;
		}

	public:
		int step()
		{
			ASSERT(m_hOp); return sqlite3_step(m_hOp);
		}
		int finalize()
		{
			if (m_hOp)
			{
				int ret = sqlite3_finalize(m_hOp);
				m_hOp = NULL;
				return ret;
			}
			return SQLITE_OK;
		}

	public:
		bool done()
		{
			int ret = step();
			if (SQLITE_DONE != ret)
			{
				TRACE1("failed to operate: %d\n", ret);
				return false;
			}
			return true;
		}
		bool reset()
		{
			int ret = sqlite3_reset(m_hOp);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to reset: %d\n", ret);
				return false;
			}
			return true;
		}
		bool end()
		{
			ASSERT(m_hOp);
			int ret = sqlite3_finalize(m_hOp);
			m_hOp = NULL;
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to end: %d\n", ret);
				return false;
			}
			return true;
		}

	public:
		bool bindValue(int nCol, INT val)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_int(m_hOp, nCol, val);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind INT: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindValue(int nCol, INT64 val)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_int64(m_hOp, nCol, val);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind INT64: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindValue(int nCol, DOUBLE val)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_double(m_hOp, nCol, val);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind DOUBLE: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindValue(int nCol, PCSQL_VALUE pVal)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_value(m_hOp, nCol, pVal);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind ZEROBLOB: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindNULL(int nCol)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_null(m_hOp, nCol);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind NULL: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindText(int nCol, PCSTR pszText, int len = -1, SQL_DESTRUCTOR_TYPE desType = SQLITE_STATIC)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_text(m_hOp, nCol, pszText, len, desType);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind TEXTA: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindText(int nCol, PCWSTR pszText, int len = -1, SQL_DESTRUCTOR_TYPE desType = SQLITE_STATIC)
		{
			ASSERT(m_hOp);
			if (len > 0) len *= sizeof(pszText[0]);
			int ret = sqlite3_bind_text16(m_hOp, nCol, pszText, len, desType);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind TEXTW: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindBLOB(int nCol, LPCVOID pData, int len)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_blob(m_hOp, nCol, pData, len, SQLITE_TRANSIENT);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind BLOB: %d\n", ret);
				return false;
			}
			return true;
		}
		bool bindBLOB(int nCol, int len)
		{
			ASSERT(m_hOp);
			int ret = sqlite3_bind_zeroblob(m_hOp, nCol, len);
			if (SQLITE_OK != ret)
			{
				TRACE1("failed to bind ZEROBLOB: %d\n", ret);
				return false;
			}
			return true;
		}

	public:
		bool bindValue(int nCol, UINT val)
		{
			return bindValue(nCol, (INT)val);
		}
		bool bindValue(int nCol, UINT64 val)
		{
			return bindValue(nCol, (INT64)val);
		}
	};

	class CEnum : public COperate
	{
	public:
		CEnum(OP_HANDLE hOp = NULL)
			: COperate(hOp)
		{}
		CEnum(SQL_HANDLE hDB, LPCSTR pszSql)
			: COperate(hDB, pszSql)
		{}
		CEnum(SQL_HANDLE hDB, LPCWSTR pszSql)
			: COperate(hDB, pszSql)
		{}
		~CEnum()
		{}

	public:
		bool next()
		{
			int ret = step();
			if (SQLITE_ROW == ret)
				return true;
			if (SQLITE_DONE != ret)
				TRACE1("failed to enum: %d\n", ret);
			return false;
		}

	public:
		int colCount()
		{
			ASSERT(m_hOp); return sqlite3_column_count(m_hOp);
		}
		int colType(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_type(m_hOp, nCol);
		}
		PCWSTR colDeclType(int nCol)
		{
			ASSERT(m_hOp); return (PCWSTR)sqlite3_column_decltype16(m_hOp, nCol);
		}
		PCWSTR colName(int nCol)
		{
			ASSERT(m_hOp); return (PCWSTR)sqlite3_column_name16(m_hOp, nCol);
		}

	public:
		INT colInt(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_int(m_hOp, nCol);
		}
		INT64 colInt64(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_int64(m_hOp, nCol);
		}
		LPCVOID colBlob(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_blob(m_hOp, nCol);
		}
		INT colBytes(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_bytes(m_hOp, nCol);
		}
		INT colBytes16(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_bytes16(m_hOp, nCol);
		}
		DOUBLE colDouble(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_double(m_hOp, nCol);
		}
		PSQL_VALUE colValue(int nCol)
		{
			ASSERT(m_hOp); return sqlite3_column_value(m_hOp, nCol);
		}
		LPCSTR colTextA(int nCol)
		{
			ASSERT(m_hOp); return (LPCSTR)sqlite3_column_text(m_hOp, nCol);
		}
		LPCWSTR colTextW(int nCol)
		{
			ASSERT(m_hOp); return (LPCWSTR)sqlite3_column_text16(m_hOp, nCol);
		}
#ifdef _UNICODE
#define colText	colTextW
#else
#define colText colTextA
#endif
	};

	class CTransAction
	{
	public:
		CTransAction(SQL_HANDLE hDB = NULL, bool bInit = true)
			: m_DB(hDB)
			, m_begin(false)
		{
			if(bInit)
				VERIFY(begin());
		}
		~CTransAction()
		{
			if (m_begin)
				VERIFY(rollback());
		}

	protected:
		CSQLiteHandle	m_DB;
		bool			m_begin;

	public:
		operator bool() const { return m_begin; }
	public:
		bool begin()
		{
			ASSERT(m_DB && !m_begin);
			if (m_begin)
				return false;
			if (m_DB.exec("BEGIN TRANSACTION") == SQLITE_OK)
				m_begin = true;
			return m_begin;
		}
		bool commit()
		{
			ASSERT(m_DB && m_begin);
			if (m_begin && m_DB.exec("COMMIT") == SQLITE_OK)
			{
				m_begin = false;
				return true;
			}
			return false;
		}
		bool rollback()
		{
			ASSERT(m_DB && m_begin);
			if (m_begin && m_DB.exec("ROLLBACK") == SQLITE_OK)
			{
				m_begin = false;
				return true;
			}
			return false;
		}
	};

}

_MZH_END

#endif
