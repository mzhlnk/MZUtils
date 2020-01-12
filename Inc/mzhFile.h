#pragma once

#ifndef _MZH_FILE
#define _MZH_FILE

#include <mzhBase.h>
#include <mzhStr.h>

_MZH_BEGIN

const HANDLE hFileNull = INVALID_HANDLE_VALUE;

template<bool t_bManaged>
class CFileT : public CHandleT<t_bManaged, hFileNull>
{
public:
	//static void Rename(LPCTSTR lpszOldName, LPCTSTR lpszNewName);
	//static void Remove(LPCTSTR lpszFileName);
	//static BOOL GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus);
	//static void SetStatus(LPCTSTR lpszFileName, const CFileStatus& status);

public:
	// Constructors
	explicit CFileT(HANDLE hFile = hFileNull)
		: CHandleT<t_bManaged, hFileNull>(hFile)
	{ }
	explicit CFileT(LPCSTR lpFileName,
		DWORD dwDesiredAccess = GENERIC_ALL,
		DWORD dwCreationDisposition = OPEN_ALWAYS,
		DWORD dwShareMode = FILE_SHARE_READ,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		HANDLE hTemplateFile = NULL)
	{
		__super::_hFile = ::CreateFileA(lpFileName,
			dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile);
	}
	explicit CFileT(LPCWSTR lpFileName,
		DWORD dwDesiredAccess = GENERIC_ALL,
		DWORD dwCreationDisposition = OPEN_ALWAYS,
		DWORD dwShareMode = FILE_SHARE_READ,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		HANDLE hTemplateFile = NULL)
	{
		__super::_hFile = ::CreateFileW(lpFileName,
			dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile);
	}

public:
	BOOL Open(LPCSTR lpFileName,
		DWORD dwDesiredAccess = GENERIC_ALL,
		DWORD dwCreationDisposition = OPEN_ALWAYS,
		DWORD dwShareMode = FILE_SHARE_READ,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		HANDLE hTemplateFile = NULL)
	{
		ASSERT(!__super::IsValid());
		return VALID_ATTACH(::CreateFileA(lpFileName,
			dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile));
	}
	BOOL Open(LPCWSTR lpFileName,
		DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE,
		DWORD dwCreationDisposition = OPEN_ALWAYS,
		DWORD dwShareMode = FILE_SHARE_READ,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		HANDLE hTemplateFile = NULL)
	{
		ASSERT(!__super::IsValid());
		return __super::VALID_ATTACH(::CreateFileW(lpFileName,
			dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile));
	}

public:
	BOOL GetFileTime(LPFILETIME lpModifiedTime,
		LPFILETIME lpCreationTime = NULL,
		LPFILETIME lpLastAccessTime = NULL) const
	{
		ASSERT(__super::IsValid());
		return ::GetFileTime(__super::_hFile, lpCreationTime, lpLastAccessTime, lpModifiedTime);
	}
	BOOL SetFileTime(LPFILETIME lpModifiedTime,
		LPFILETIME lpCreationTime = NULL,
		LPFILETIME lpLastAccessTime = NULL)
	{
		ASSERT(__super::IsValid());
		return ::SetFileTime(__super::_hFile, lpCreationTime, lpLastAccessTime, lpModifiedTime);
	}
	ULONGLONG GetPosition() const
	{
		ASSERT(__super::IsValid());
		LARGE_INTEGER liPos;
		liPos.QuadPart = 0;
		liPos.LowPart = ::SetFilePointer(__super::_hFile, liPos.LowPart, &liPos.HighPart, FILE_CURRENT);
		if (liPos.LowPart == (DWORD)-1)
			if (::GetLastError() != NO_ERROR)
				liPos.QuadPart = MAXULONGLONG;
		return liPos.QuadPart;
	}
	ULONGLONG Seek(LONGLONG lOff, DWORD dwFrom = FILE_CURRENT)
	{
		ASSERT(__super::IsValid());
		LARGE_INTEGER liOff;
		liOff.QuadPart = lOff;
		liOff.LowPart = ::SetFilePointer(__super::_hFile, liOff.LowPart, &liOff.HighPart, dwFrom);
		if (liOff.LowPart == (DWORD)-1)
			if (::GetLastError() != NO_ERROR)
				liOff.QuadPart = MAXULONGLONG;
		return liOff.QuadPart;
	}
	ULONGLONG SeekTo(LONGLONG lOff)
	{ return Seek(lOff, FILE_BEGIN); }
	ULONGLONG SeekToEnd()
	{ return Seek(0LL, FILE_END); }
	void SeekToBegin()
	{ Seek(0LL, FILE_BEGIN); }

	BOOL SetLength(ULONGLONG dwNewLen)
	{
		Seek(dwNewLen, FILE_BEGIN);
		return ::SetEndOfFile(__super::_hFile);
	}
	ULONGLONG GetLength() const
	{
		ULARGE_INTEGER liSize;
		liSize.LowPart = ::GetFileSize(__super::_hFile, &liSize.HighPart);
		if (liSize.LowPart == INVALID_FILE_SIZE)
			if (::GetLastError() != NO_ERROR)
				liSize.QuadPart = MAXULONGLONG;
		return liSize.QuadPart;
	}

	BOOL LockRange(ULONGLONG dwPos, ULONGLONG dwCount)
	{
		ASSERT(__super::IsValid());

		ULARGE_INTEGER liPos;
		ULARGE_INTEGER liCount;
		liPos.QuadPart = dwPos;
		liCount.QuadPart = dwCount;
		return ::LockFile(__super::_hFile, liPos.LowPart, liPos.HighPart, liCount.LowPart, liCount.HighPart);
	}
	BOOL UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount)
	{
		ASSERT(__super::IsValid());

		ULARGE_INTEGER liPos;
		ULARGE_INTEGER liCount;
		liPos.QuadPart = dwPos;
		liCount.QuadPart = dwCount;
		return ::UnlockFile(__super::_hFile, liPos.LowPart, liPos.HighPart, liCount.LowPart, liCount.HighPart);
	}

	UINT Read(void* lpBuf, UINT nCount)
	{
		ASSERT(__super::IsValid());
		ASSERT(lpBuf && nCount);
		DWORD dwRead(0);
		VERIFY(::ReadFile(__super::_hFile, lpBuf, nCount, &dwRead, NULL));
		return (UINT)dwRead;
	}
	BOOL Write(const void* lpBuf, UINT nCount)
	{
		ASSERT(__super::IsValid());
		ASSERT(lpBuf && nCount);
		DWORD nWritten;
		if (::WriteFile(__super::_hFile, lpBuf, nCount, &nWritten, NULL) && nWritten == nCount)
			return TRUE;
		ASSERT(0);
		return FALSE;
	}
	BOOL Flush()
	{
		ASSERT(__super::IsValid());
		return ::FlushFileBuffers(__super::_hFile);
	}
};
typedef CFileT<true> CFile;
typedef CFileT<false> CFileHandle;


const HANDLE hFileFindNull = INVALID_HANDLE_VALUE;

template<bool t_bManaged>
class CFileFindT : public CHObjT<HANDLE, t_bManaged, hFileFindNull>
{
public:
	CFileFindT(HANDLE hFind = hFileFindNull)
		: CHObjT<HANDLE, t_bManaged, hFileFindNull>(hFind)
	{ }
	virtual ~CFileFindT()
	{
		if(t_bManaged)
			Release();
	}

public:
	virtual BOOL Close()
	{
		ASSERT(IsValid());
		if(!::FindClose(_hFind))
			return FALSE;
		_hFind = hFileFindNull;
		m_pData.Release();
		return TRUE;
	}

protected:
	CString						m_strRoot;
	CAutoPtrT<WIN32_FIND_DATA>	m_pData;

public:
	BOOL Find(LPCTSTR lpFileName = NULL)
	{
		Release();

		if(lpFileName == NULL)
			lpFileName = _T("*.*");
		TCHAR szRoot[MAX_PATH];
		if(::GetFullPathName(lpFileName, MAX_PATH, szRoot, NULL) >= ARRAYSIZE(szRoot))
			return FALSE;
		TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
		errno_t err = ::_tsplitpath_s(szRoot, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);
		if(err) return FALSE;
		err = ::_tmakepath_s(szRoot, _MAX_PATH, szDrive, szDir, NULL, NULL);
		if(err) return FALSE;
		m_strRoot = szRoot;
		if(!m_pData)
			m_pData = (LPWIN32_FIND_DATA)malloc(sizeof(WIN32_FIND_DATA));
		return VALID_ATTACH(::FindFirstFile(lpFileName, m_pData));
	}
	BOOL Next()
	{ ASSERT(IsValid()); return ::FindNextFile(_hFind, m_pData); }

public:
	BOOL MatchesMask(DWORD dwMask) const
	{ ASSERT(IsValid()); return m_pData && (m_pData->dwFileAttributes & dwMask); }
	BOOL IsArchived() const
	{ return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }
	BOOL IsCompressed() const
	{ return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
	BOOL IsDirectory() const
	{ return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
	BOOL IsHidden() const
	{ return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
	BOOL IsNormal() const
	{ return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
	BOOL IsReadOnly() const
	{ return MatchesMask(FILE_ATTRIBUTE_READONLY); }
	BOOL IsSystem() const
	{ return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
	BOOL IsTemporary() const
	{ return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
	BOOL IsDots() const
	{
		ASSERT(IsValid());
		if(IsDirectory() && m_pData->cFileName[0] == _T('.'))
			return m_pData->cFileName[1] == _T('\0') ||
				(m_pData->cFileName[1] == _T('.') && m_pData->cFileName[2] == _T('\0'));
		return FALSE;
	}

public:
	BOOL GetCreationTime(FILETIME& ft) const
	{
		ASSERT(IsValid());
		if(IsValid() && m_pData)
		{
			ft = m_pData->ftCreationTime;
			return TRUE;
		}
		return FALSE;
	}
	BOOL GetLastAccessTime(FILETIME& ft) const
	{
		ASSERT(IsValid());
		if(IsValid() && m_pData)
		{
			ft = m_pData->ftLastAccessTime;
			return TRUE;
		}
		return FALSE;
	}
	BOOL GetLastWriteTime(FILETIME& ft) const
	{
		ASSERT(IsValid());
		if(IsValid() && m_pData)
		{
			ft = m_pData->ftLastWriteTime;
			return TRUE;
		}
		return FALSE;
	}
	CString GetRoot() const
	{
		ASSERT(IsValid());
		return m_strRoot;
	}
	CString GetFileName() const
	{
		ASSERT(IsValid());
		CString ret;
		if(m_pData)
			ret = m_pData->cFileName;
		return ret;
	}
	CString GetFileTitle() const
	{
		ASSERT(IsValid());
		CString strFullName = GetFileName();
		TCHAR szTitle[_MAX_FNAME] = {};
		_tsplitpath_s(strFullName, NULL, 0, NULL, 0, szTitle, ARRAYSIZE(szTitle), NULL, 0);
		return CString(szTitle);
	}
	CString GetFilePath() const
	{
		ASSERT(IsValid());
		CString strResult = m_strRoot;
		LPCTSTR pszLast = _tcsdec(strResult, strResult + strResult.size());
		ASSERT(pszLast);
		if(*pszLast != _T('/') && *pszLast != _T('\\'))
			strResult += _T('\\');
		strResult += GetFileName();
		return strResult;
	}
	CString GetFileURL() const
	{
		ASSERT(IsValid());
		CString strResult(_T("file://"));
		strResult += GetFilePath();
		return strResult;
	}
};
typedef CFileFindT<true> CFileFind;
typedef CFileFindT<false> CFileFindHandle;

_MZH_END

#endif
