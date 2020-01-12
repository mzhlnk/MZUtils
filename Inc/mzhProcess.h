#pragma once

#ifndef _MZH_PROCESS
#define _MZH_PROCESS

#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CProcessT : public CHandleT<t_bManaged>
{
public:
	CProcessT(HANDLE hProcess = NULL)
		: CHandleT<t_bManaged>(hProcess)
	{ }
	virtual ~CProcessT()
	{
		if (t_bManaged)
			Release();
	}

public:
	DWORD GetPID() const
	{
		ASSERT(IsValid());
		return ::GetProcessId(*this);
	}
	BOOL GetTimes(LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime) const
	{
		ASSERT(IsValid());
		return ::GetProcessTimes(*this, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
	}
	BOOL QueryCycleTime(ULONG64& CycleTime) const
	{
		ASSERT(IsValid());
		return ::QueryProcessCycleTime(*this, &CycleTime);
	}
	BOOL Terminate(UINT uExitCode)
	{
		ASSERT(IsValid());
		return ::TerminateProcess(*this, uExitCode);
	}
	BOOL GetExitCode(DWORD& dwExitCode) const
	{
		ASSERT(IsValid());
		return ::GetExitCodeProcess(*this, &dwExitCode)
	}
	BOOL GetAffinityMask(PDWORD_PTR lpProcessAffinityMask, PDWORD_PTR lpSystemAffinityMask) const
	{
		ASSERT(IsValid());
		return ::GetProcessAffinityMask(*this, lpProcessAffinityMask, lpSystemAffinityMask);
	}
	DWORD_PTR SetAffinityMask(DWORD_PTR dwProcessAffinityMask)
	{
		ASSERT(IsValid());
		return ::SetProcessAffinityMask(*this, dwProcessAffinityMask);
	}
	BOOL GetHandleCount(DWORD& dwHandleCount) const
	{
		ASSERT(IsValid());
		return ::GetProcessHandleCount(*this, &dwHandleCount);
	}
	BOOL GetIoCounters(IO_COUNTERS& IoCounters) const
	{
		ASSERT(IsValid());
		return ::GetProcessIoCounters(*this, &IoCounters);
	}
	BOOL GetPriorityBoost(BOOL& bDisablePriorityBoost) const
	{
		ASSERT(IsValid());
		return ::GetProcessPriorityBoost(*this, &bDisablePriorityBoost);
	}
	BOOL SetPriorityBoost(BOOL bDisablePriorityBoost)
	{
		ASSERT(IsValid());
		return ::SetProcessPriorityBoost(*this, bDisablePriorityBoost);
	}
	BOOL GetInformation(PROCESS_INFORMATION_CLASS InformationClass, LPVOID Information, DWORD InformationSize) const
	{
		ASSERT(IsValid());
		return ::GetProcessInformation(*this, InformationClass, Information, InformationSize);
	}
	BOOL SetInformation(PROCESS_INFORMATION_CLASS InformationClass, LPVOID Information, DWORD InformationSize)
	{
		ASSERT(IsValid());
		return ::SetProcessInformation(*this, InformationClass, Information, InformationSize);
	}
	DWORD GetGuiResources(DWORD uiFlags) const
	{
		ASSERT(IsValid());
		return ::GetGuiResources(*this, uiFlags);
	}
	DWORD GetPriorityClass() const
	{
		ASSERT(IsValid());
		return ::GetPriorityClass(*this);
	}
	DWORD SetPriorityClass(DWORD dwPriorityClass)
	{
		ASSERT(IsValid());
		return ::SetPriorityClass(*this, dwPriorityClass);
	}
	BOOL GetWorkingSetSize(PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize) const
	{
		ASSERT(IsValid());
		return ::GetProcessWorkingSetSize(*this, lpMinimumWorkingSetSize, lpMaximumWorkingSetSize);
	}
	BOOL SetWorkingSetSize(SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize)
	{
		ASSERT(IsValid());
		return ::SetProcessWorkingSetSize(*this, dwMinimumWorkingSetSize, dwMaximumWorkingSetSize);
	}
	BOOL GetWorkingSetSizeEx(PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize, PDWORD Flags) const
	{
		ASSERT(IsValid());
		return ::GetProcessWorkingSetSizeEx(*this, lpMinimumWorkingSetSize, lpMaximumWorkingSetSize, Flags);
	}
	BOOL SetWorkingSetSizeEx(SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize, DWORD Flags)
	{
		ASSERT(IsValid());
		return ::SetProcessWorkingSetSizeEx(*this, dwMinimumWorkingSetSize, dwMaximumWorkingSetSize, Flags);
	}
	BOOL QueryFullImageName(DWORD dwFlags, LPTSTR lpExeName, DWORD& dwSize) const
	{
		ASSERT(IsValid());
		return ::QueryFullProcessImageName(*this, dwFlags, lpExeName, &dwSize);
	}
	BOOL SetAffinityUpdateMode(DWORD dwFlags = PROCESS_AFFINITY_ENABLE_AUTO_UPDATE)
	{
		ASSERT(IsValid());
		return ::SetProcessAffinityUpdateMode(*this, dwFlags);
	}
	BOOL DebugBreak()
	{
		ASSERT(IsValid());
		return ::DebugBreakProcess(*this);
	}
	BOOL IsWow64(BOOL& Wow64Process) const
	{
		ASSERT(IsValid());
		return ::IsWow64Process(*this, &Wow64Process);
	}

public:
	DWORD GetVersion() const
	{
		return ::GetProcessVersion(GetPID());
	}
	BOOL IsActive() const
	{
		DWORD dwExitCode(0);
		if (!GetExitCode(dwExitCode))
			TRACE1("Failed to get Process exit code: %d\n", GetLastError());
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
			TRACE1("Failed to wait Process exit: %d\n", GetLastError());
		return FALSE;
	}
protected:
	using operator =;
};

typedef CProcessT<false> CProcessHandle;

class CProcess : public CProcessT<true>
{
public:
	CProcess(HANDLE hProcess = NULL)
		: CProcessT<true>(hProcess)
	{ }
	virtual ~CProcess()
	{ }

public:
	BOOL Open(DWORD dwProcessId, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS, BOOL bInheritHandle = FALSE)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId));
	}

public:
	BOOL Create(
		LPCTSTR lpApplicationName,
		LPTSTR lpCommandLine,
		LPSTARTUPINFO lpStartupInfo,
		DWORD dwCreationFlags						= NORMAL_PRIORITY_CLASS,
		LPVOID lpEnvironment						= NULL,
		LPCTSTR lpCurrentDirectory					= NULL,
		BOOL bInheritHandles						= FALSE,
		LPSECURITY_ATTRIBUTES lpProcessAttributes	= NULL,
		LPSECURITY_ATTRIBUTES lpThreadAttributes	= NULL)
	{
		ASSERT(!IsValid());
		PROCESS_INFORMATION pi = {};
		if(!::CreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, &pi))
		{
			TRACE1("Failed to create process: %d\n", GetLastError());
			return FALSE;
		}
		::CloseHandle(pi.hThread);
		Attach(pi.hProcess);
		return TRUE;
	}

	BOOL CreateAsUser(
		HANDLE hToken,
		LPCTSTR lpApplicationName,
		LPTSTR lpCommandLine,
		LPSTARTUPINFO lpStartupInfo,
		DWORD dwCreationFlags						= NORMAL_PRIORITY_CLASS,
		LPVOID lpEnvironment						= NULL,
		LPCTSTR lpCurrentDirectory					= NULL,
		BOOL bInheritHandles						= FALSE,
		LPSECURITY_ATTRIBUTES lpProcessAttributes	= NULL,
		LPSECURITY_ATTRIBUTES lpThreadAttributes	= NULL)
	{
		ASSERT(!IsValid());
		PROCESS_INFORMATION pi = {};
		if(!::CreateProcessAsUser(hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, &pi))
		{
			TRACE1("Failed to create process as user: %d\n", GetLastError());
			return FALSE;
		}
		::CloseHandle(pi.hThread);
		Attach(pi.hProcess);
		return TRUE;
	}
	BOOL CreateWithLogonW(
		LPCWSTR lpDomain,
		LPCWSTR lpUsername,
		LPCWSTR lpPassword,
		DWORD dwLogonFlags,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSTARTUPINFOW lpStartupInfo,
		DWORD dwCreationFlags		= NORMAL_PRIORITY_CLASS,
		LPVOID lpEnvironment		= NULL,
		LPCWSTR lpCurrentDirectory	= NULL
		)
	{
		ASSERT(!IsValid());
		PROCESS_INFORMATION pi = {};
		if(!::CreateProcessWithLogonW(lpUsername, lpDomain, lpPassword, dwLogonFlags, lpApplicationName, lpCommandLine, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, &pi))
		{
			TRACE1("Failed to create process with logon: %d\n", GetLastError());
			return FALSE;
		}
		::CloseHandle(pi.hThread);
		Attach(pi.hProcess);
		return TRUE;
	}
	BOOL CreateWithTokenW(
		HANDLE hToken,
		DWORD dwLogonFlags,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSTARTUPINFOW lpStartupInfo,
		DWORD dwCreationFlags		= NORMAL_PRIORITY_CLASS,
		LPVOID lpEnvironment		= NULL,
		LPCWSTR lpCurrentDirectory	= NULL)
	{
		ASSERT(!IsValid());
		PROCESS_INFORMATION pi = {};
		if(!::CreateProcessWithTokenW(hToken, dwLogonFlags, lpApplicationName, lpCommandLine, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, &pi))
		{
			TRACE1("Failed to create process with token: %d\n", GetLastError());
			return FALSE;
		}
		::CloseHandle(pi.hThread);
		Attach(pi.hProcess);
		return TRUE;
	}
};

_MZH_END

#endif

