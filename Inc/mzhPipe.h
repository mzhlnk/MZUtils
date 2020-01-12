#pragma once

#ifndef _MZH_PIPE
#define _MZH_PIPE

#include <mzhBase.h>

_MZH_BEGIN

const HANDLE hNullPipe = INVALID_HANDLE_VALUE;

template<bool t_bManaged>
class CPipeT : public CHandleT<t_bManaged, hNullPipe>
{
public:
	CPipeT(HANDLE hPipe = hNullPipe)
		: CHandleT<t_bManaged, hNullPipe>(hPipe)
	{ }
	virtual ~CPipeT()
	{ }

public:
	BOOL Create(
		LPCTSTR	lpName,
		DWORD	dwOpenMode		= PIPE_ACCESS_DUPLEX,
		DWORD	dwPipeMode		= PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		DWORD	nMaxInstances	= PIPE_UNLIMITED_INSTANCES,
		DWORD	nOutBufferSize	= 4096,
		DWORD	nInBufferSize	= 4096,
		DWORD	nDefaultTimeOut	= INFINITE,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
	{
		ASSERT(IsInvalid());
		return VALID_ATTACH(::CreateNamedPipe(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes));
	}
	BOOL Open(
		LPCTSTR		lpName,
		DWORD		dwDesiredAccess	= GENERIC_READ | GENERIC_WRITE,
		DWORD		dwShareMode		= 0,
		DWORD		dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
	{
		ASSERT(IsInvalid());
		return VALID_ATTACH(::CreateFile(lpName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, OPEN_EXISTING, dwFlagsAndAttributes, NULL));
	}
	void CancelIo()
	{
		ASSERT(IsValid());
		VERIFY(::CancelIo(m_handle));
	}

	BOOL Connect(LPOVERLAPPED lpOverlapped = NULL)
	{
		ASSERT(IsValid());
		BOOL bl = ::ConnectNamedPipe(m_handle, lpOverlapped);
		ASSERT((bl && !lpOverlapped) || (!bl && lpOverlapped));
		if (lpOverlapped)
		{
			if (bl)
			{
				DWORD dwErr = GetLastError();
				if (ERROR_IO_PENDING != dwErr)
				{
					TRACE1("Failed to ConnectNamedPipe: %d\n", dwErr);
					return FALSE;
				}
			}
		}
		else
		{
			if (!bl)
			{
				TRACE1("Failed to ConnectNamedPipe: %d\n", GetLastError());
				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL Disconnect()
	{
		if (IsValid())
		{
			if (::DisconnectNamedPipe(m_handle))
			{
				return TRUE;
			}
			TRACE1("Failed to DisconnectNamedPipe: %d\n", GetLastError());
		}
		return FALSE;
	}

	INT Send(LPVOID lpBuffer, INT nLen)
	{
		ASSERT(lpBuffer && nLen > 0);
		DWORD nWritten(0);
		if (!WriteFile(m_handle, lpBuffer, nLen, &nWritten, NULL))
		{
			TRACE1("Failed to WriteFile: %d\n", GetLastError());
			return -1;
		}
		return nWritten;
	}
	INT Recv(LPVOID lpBuffer, INT nLen)
	{
		ASSERT(lpBuffer && nLen > 0);
		DWORD nRead(0);
		if (!ReadFile(m_handle, lpBuffer, nLen, &nRead, NULL))
		{
			TRACE1("Failed to ReadFile: %d\n", GetLastError());
			return -1;
		}
		return nRead;
	}

	BOOL Send(LPVOID lpBuffer, INT nLen, LPOVERLAPPED lpOverlapped)
	{
		ASSERT(lpBuffer && nLen > 0 && lpOverlapped);

		BOOL bl = WriteFile(m_handle, lpBuffer, nLen, NULL, lpOverlapped);
		if (!bl)
		{
			DWORD dwErr = GetLastError();
			if (dwErr && ERROR_IO_PENDING != dwErr)
			{
				TRACE1("Failed to WriteFile: %d\n", dwErr);
				return FALSE;
			}
		}

		return TRUE;
	}
	BOOL Recv(LPVOID lpBuffer, INT nLen, LPOVERLAPPED lpOverlapped)
	{
		ASSERT(lpBuffer && nLen > 0 && lpOverlapped);

		BOOL bl = ReadFile(m_handle, lpBuffer, nLen, NULL, lpOverlapped);
		if (!bl/* && ERROR_NO_MORE_ITEMS != lpOverlapped->Internal*/)
		{
			DWORD dwErr = GetLastError();
			if (dwErr && ERROR_IO_PENDING != dwErr)
			{
				TRACE1("Failed to ReadFile: %d\n", dwErr);
				return FALSE;
			}
		}
		return TRUE;
	}

	BOOL SetState(LPDWORD lpMode, LPDWORD lpMaxCollectionCount = NULL, LPDWORD lpCollectDataTimeout = NULL)
	{
		ASSERT(lpMode);
		if (!::SetNamedPipeHandleState(m_handle, lpMode, lpMaxCollectionCount, lpCollectDataTimeout))
		{
			TRACE1("Failed to SetNamedPipeHandleState: %d\n", GetLastError());
			return FALSE;
		}
		return TRUE;
	}

	BOOL GetOverlappedResult(LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait)
	{
		ASSERT(lpOverlapped);
		if (!::GetOverlappedResult(m_handle, lpOverlapped, lpNumberOfBytesTransferred, bWait))
		{
			TRACE1("Failed to GetOverlappedResult: %d\n", GetLastError());
			return FALSE;
		}
		return TRUE;
	}

public:
	static BOOL Wait(LPCTSTR lpszNamedPipe, DWORD nTimeout = INFINITE)
	{
		if (!::WaitNamedPipe(lpszNamedPipe, nTimeout))
		{
			TRACE1("Failed to WaitNamedPipe: %d\n", GetLastError());
			return FALSE;
		}
		return TRUE;
	}

};

typedef CPipeT<true> CPipe;
typedef CPipeT<false> CPipeHandle;

_MZH_END

#endif
