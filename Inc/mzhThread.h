#pragma once

#ifndef _MZH_THREAD
#define _MZH_THREAD

#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CThreadT : public CHandleT<t_bManaged>
{
public:
	CThreadT(HANDLE hThread = NULL)
		: CHandleT<t_bManaged>(hThread)
	{}
	virtual ~CThreadT()
	{
		if (t_bManaged)
			Release();
	}

public:
	DWORD GetTID() const
	{
		ASSERT(IsValid());
		return ::GetThreadId(*this);
	}
	DWORD GetPID() const
	{
		ASSERT(IsValid());
		return ::GetProcessIdOfThread(*this);
	}
	int GetPriority() const
	{
		ASSERT(IsValid());
		return ::GetThreadPriority(*this);
	}
	BOOL SetPriority(int nPriority)
	{
		ASSERT(IsValid());
		return ::SetThreadPriority(*this, nPriority);
	}
	BOOL GetPriorityBoost(BOOL& bDisablePriorityBoost) const
	{
		ASSERT(IsValid());
		return ::GetThreadPriorityBoost(*this, &bDisablePriorityBoost);
	}
	BOOL SetPriorityBoost(BOOL bDisablePriorityBoost)
	{
		ASSERT(IsValid());
		return ::SetThreadPriorityBoost(*this, bDisablePriorityBoost);
	}
	BOOL GetIOPendingFlag(BOOL& bIOIsPending) const
	{
		ASSERT(IsValid());
		return ::GetThreadIOPendingFlag(*this, &bIOIsPending);
	}
	BOOL GetTimes(LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime) const
	{
		ASSERT(IsValid());
		return ::GetThreadTimes(*this, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
	}
	BOOL QueryCycleTime(ULONG64& CycleTime) const
	{
		ASSERT(IsValid());
		return ::QueryThreadCycleTime(*this, &CycleTime);
	}
	DWORD Suspend()
	{
		ASSERT(IsValid());
		return ::SuspendThread(*this);
	}
	DWORD Resume()
	{
		ASSERT(IsValid());
		return ::ResumeThread(*this);
	}
	BOOL Terminate(DWORD dwExitCode)
	{
		ASSERT(IsValid());
		return ::TerminateThread(*this, dwExitCode);
	}
	BOOL GetExitCode(DWORD& dwExitCode) const
	{
		ASSERT(IsValid());
		return ::GetExitCodeThread(*this, &dwExitCode)
	}
	DWORD_PTR SetAffinityMask(DWORD_PTR dwThreadAffinityMask)
	{
		ASSERT(IsValid());
		return ::SetThreadAffinityMask(*this, dwThreadAffinityMask);
	}
	DWORD SetIdealProcessor(DWORD dwIdealProcessor)
	{
		ASSERT(IsValid());
		return ::SetThreadIdealProcessor(*this, dwIdealProcessor);
	}
	BOOL GetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor) const
	{
		ASSERT(IsValid());
		return ::GetThreadIdealProcessorEx(*this, lpIdealProcessor);
	}
	BOOL SetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor, PPROCESSOR_NUMBER lpPreviousIdealProcessor)
	{
		ASSERT(IsValid());
		return ::SetThreadIdealProcessorEx(*this, lpIdealProcessor, lpPreviousIdealProcessor);
	}
	BOOL GetInformation(THREAD_INFORMATION_CLASS InformationClass, LPVOID Information, DWORD InformationSize) const
	{
		ASSERT(IsValid());
		return ::GetThreadInformation(*this, InformationClass, Information, InformationSize);
	}
	BOOL SetInformation(THREAD_INFORMATION_CLASS InformationClass, LPVOID Information, DWORD InformationSize)
	{
		ASSERT(IsValid());
		return ::SetThreadInformation(*this, InformationClass, Information, InformationSize);
	}
	BOOL GetContext(LPCONTEXT lpContext) const
	{
		ASSERT(IsValid());
		return ::GetThreadContext(*this, lpContext);
	}
	BOOL SetContext(const CONTEXT* lpContext)
	{
		ASSERT(IsValid());
		return ::SetThreadContext(*this, lpContext);
	}
	BOOL GetSelectorEntry(DWORD dwSelector, LPLDT_ENTRY lpSelectorEntry) const
	{
		ASSERT(IsValid());
		return ::GetThreadSelectorEntry(*this, dwSelector, lpSelectorEntry);
	}

#ifdef _WIN64
	DWORD Wow64Suspend()
	{
		ASSERT(IsValid());
		return ::Wow64SuspendThread(*this);
	}
	BOOL Wow64GetContext(PWOW64_CONTEXT lpContext) const
	{
		ASSERT(IsValid());
		return ::Wow64GetThreadContext(*this, lpContext);
	}
	BOOL Wow64SetContext(const WOW64_CONTEXT* lpContext)
	{
		ASSERT(IsValid());
		return ::Wow64SetThreadContext(*this, lpContext);
	}
	BOOL Wow64GetSelectorEntry(DWORD dwSelector, PWOW64_LDT_ENTRY lpSelectorEntry) const
	{
		ASSERT(IsValid());
		return ::Wow64GetThreadSelectorEntry(*this, dwSelector, lpSelectorEntry);
	}
#endif
public:
	BOOL IsActive() const
	{
		DWORD dwExitCode(0);
		if (!GetExitCode(dwExitCode))
		{
			TRACE1("Failed to get thread exit code: %d\n", GetLastError());
		}
		return (STILL_ACTIVE == dwExitCode);
	}
	BOOL Join(DWORD dwTimeout = INFINITE) const
	{
		if(!IsValid())
			return TRUE;
		DWORD dwVal = ::WaitForSingleObject(*this, dwTimeout);
		if (WAIT_OBJECT_0 == dwVal)
			return TRUE;
		if (WAIT_FAILED == dwVal)
			TRACE1("Failed to wait thread exit: %d\n", GetLastError());
		return FALSE;
	}

public:
	virtual BOOL PostMessage(UINT Msg, WPARAM wParam = 0U, LPARAM lParam = 0L)
	{
		return ::PostThreadMessage(GetTID(), Msg, wParam, lParam);
	}
	virtual BOOL Open(DWORD dwThreadId, DWORD dwDesiredAccess = THREAD_ALL_ACCESS, BOOL bInheritHandle = FALSE)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(OpenThread(dwDesiredAccess, bInheritHandle, dwThreadId));
	}
};

typedef CThreadT<true> CThread;
typedef CThreadT<false> CThreadHandle;


template<bool t_bDeleteThis>
class CSimpleThreadT : public CThread
{
public:
	CSimpleThreadT()
		: m_dwThreadId(0)
		, m_dwExitCode(0)
	{ }
	virtual ~CSimpleThreadT()
	{
		if(!t_bDeleteThis)
			Join();
	}

public:
	virtual BOOL InitInstance() { return TRUE; }
	virtual int Run() = 0;
	virtual BOOL Stop() = 0;
	virtual void ExitInstance() { }

public:
	DWORD	m_dwThreadId;
	DWORD	m_dwExitCode;

public:
	BOOL Create(DWORD dwCreateFlags = 0, UINT nStackSize = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL)
	{
		if (IsValid())
			return FALSE;
		return VALID_ATTACH(::CreateThread(lpSecurityAttrs, nStackSize, (LPTHREAD_START_ROUTINE)_ThreadProc, this, dwCreateFlags, &m_dwThreadId));
	}

protected:	// cannot be used
	using CThread::Open;
	using CThread::operator =;
	using CThread::Detach;
	using CThread::Attach;

public:
	virtual BOOL PostMessage(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(GetTID() == m_dwThreadId);
		return ::PostThreadMessage(m_dwThreadId, Msg, wParam, lParam);
	}

protected:
	static DWORD WINAPI _ThreadProc(CSimpleThreadT<t_bDeleteThis>* pThis)
	{
		if (pThis->InitInstance())
		{
			pThis->m_dwExitCode = pThis->Run();
			pThis->ExitInstance();
		}
		DWORD dwRet = pThis->m_dwExitCode;
		if (t_bDeleteThis)
			delete pThis;
		return dwRet;
	}
};

typedef CSimpleThreadT<false> CSimpleThread;
typedef CSimpleThreadT<true> CAutoSimpleThread;


template<bool t_bDeleteThis>
class CWinThreadT : public CSimpleThreadT<t_bDeleteThis>
{
public:
	CWinThreadT()
		: CSimpleThreadT()
	{ }
	virtual ~CWinThreadT()
	{
	}

public:
	// -------
};



_MZH_END

#endif

