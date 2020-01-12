#pragma once

#ifndef _MZH_ARCHIVE
#define _MZH_ARCHIVE

#include <mzhFile.h>

_MZH_BEGIN

class CArchive
{
public:
	// Flag values
	enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };

	CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
	~CArchive();

	// Attributes
	BOOL IsLoading() const;
	BOOL IsStoring() const;
	BOOL IsByteSwapping() const;
	BOOL IsBufferEmpty() const;

	CFile* GetFile() const;
	UINT GetObjectSchema(); // only valid when reading a CObject*
	void SetObjectSchema(UINT nSchema);

	// pointer to document being serialized -- must set to serialize
	//  COleClientItems in a document!
	CDocument* m_pDocument;

	// Operations
	UINT Read(void* lpBuf, UINT nMax);
	void EnsureRead(void *lpBuf, UINT nCount);
	void Write(const void* lpBuf, UINT nMax);
	void Flush();
	void Close();
	void Abort();   // close and shutdown without exceptions

					// reading and writing strings
	void WriteString(LPCTSTR lpsz);
	LPTSTR ReadString(_Out_writes_z_(nMax + 1) LPTSTR lpsz, _In_ UINT nMax);
	BOOL ReadString(CString& rString);

public:
	// Object I/O is pointer based to avoid added construction overhead.
	// Use the Serialize member function directly for embedded objects.
	friend CArchive& AFXAPI operator<<(CArchive& ar, const CObject* pOb);

	friend CArchive& AFXAPI operator>>(CArchive& ar, CObject*& pOb);
	friend CArchive& AFXAPI operator>>(CArchive& ar, const CObject*& pOb);

	// insertion operations
	CArchive& operator<<(BYTE by);
	CArchive& operator<<(WORD w);
	CArchive& operator<<(LONG l);
	CArchive& operator<<(DWORD dw);
	CArchive& operator<<(float f);
	CArchive& operator<<(double d);
	CArchive& operator<<(LONGLONG dwdw);
	CArchive& operator<<(ULONGLONG dwdw);

	CArchive& operator<<(int i);
	CArchive& operator<<(short w);
	CArchive& operator<<(char ch);
#ifdef _NATIVE_WCHAR_T_DEFINED
	CArchive& operator<<(wchar_t ch);
#endif
	CArchive& operator<<(unsigned u);

	template < typename BaseType, bool t_bMFCDLL>
	CArchive& operator<<(const ATL::CSimpleStringT<BaseType, t_bMFCDLL>& str);

	template< typename BaseType, class StringTraits >
	CArchive& operator<<(const ATL::CStringT<BaseType, StringTraits>& str);

	template < typename BaseType, bool t_bMFCDLL>
	CArchive& operator>>(ATL::CSimpleStringT<BaseType, t_bMFCDLL>& str);

	template< typename BaseType, class StringTraits >
	CArchive& operator>>(ATL::CStringT<BaseType, StringTraits>& str);

	CArchive& operator<<(bool b);

	// extraction operations
	CArchive& operator>>(BYTE& by);
	CArchive& operator>>(WORD& w);
	CArchive& operator>>(DWORD& dw);
	CArchive& operator>>(LONG& l);
	CArchive& operator>>(float& f);
	CArchive& operator>>(double& d);
	CArchive& operator>>(LONGLONG& dwdw);
	CArchive& operator>>(ULONGLONG& dwdw);

	CArchive& operator>>(int& i);
	CArchive& operator>>(short& w);
	CArchive& operator>>(char& ch);
#ifdef _NATIVE_WCHAR_T_DEFINED
	CArchive& operator>>(wchar_t& ch);
#endif
	CArchive& operator>>(unsigned& u);
	CArchive& operator>>(bool& b);

	// Implementation
public:
	BOOL m_bForceFlat;  // for COleClientItem implementation (default TRUE)
	BOOL m_bDirectBuffer;   // TRUE if m_pFile supports direct buffering
	BOOL m_bBlocking;  // TRUE if m_pFile can block for unbounded periods of time
	void FillBuffer(UINT nAdditionalBytesNeeded);
	void CheckCount();  // throw exception if m_nMapCount is too large

						// special functions for reading and writing (16-bit compatible) counts
	DWORD_PTR ReadCount();
	void WriteCount(DWORD_PTR dwCount);

	// public for advanced use
	UINT m_nObjectSchema;
	CString m_strFileName;

protected:
	// archive objects cannot be copied or assigned
	CArchive(const CArchive& arSrc);
	void operator=(const CArchive& arSrc);

	BOOL m_nMode;
	BOOL m_bUserBuf;
	int m_nBufSize;
	CFile* m_pFile;
	BYTE* m_lpBufCur;
	BYTE* m_lpBufMax;
	BYTE* m_lpBufStart;

};


_MZH_END

#endif
