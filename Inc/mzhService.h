#pragma once

#ifndef _MZH_SERVICE
#define _MZH_SERVICE

#include <Winsvc.h>
#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CSCHandleT : public CHObjT<SC_HANDLE, t_bManaged>
{
public:
	CSCHandleT(SC_HANDLE hSC = NULL)
		: CHObjT<SC_HANDLE, t_bManaged>(hSC)
	{ }
	virtual ~CSCHandleT()
	{
		if(t_bManaged)
			Release();
	}

public:
	virtual BOOL Close()
	{
		if(!::CloseServiceHandle(_hObj))
			return FALSE;
		_hObj = NULL;
		return TRUE;
	}
};

template<bool t_bManaged>
class CSCManagerT : public CSCHandleT<t_bManaged>
{
public:
	CSCManagerT(SC_HANDLE hSCM = NULL)
		: CSCHandleT<t_bManaged>(hSCM)
	{ }
	virtual ~CSCManagerT()
	{ }

public:
	BOOL Open(LPCTSTR lpMachineName = NULL, LPCTSTR lpDatabaseName = NULL, DWORD dwDesiredAccess = SC_MANAGER_ALL_ACCESS)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::OpenSCManager(lpMachineName, lpDatabaseName, dwDesiredAccess));
	}

public:
	BOOL GetServiceDisplayName(LPCTSTR lpServiceName, LPTSTR lpDisplayName, DWORD& cchBuffer)
	{
		ASSERT(IsValid());
		return ::GetServiceDisplayName(*this, lpServiceName, lpDisplayName, &cchBuffer);
	}
	BOOL GetServiceKeyName(LPCTSTR lpDisplayName, LPTSTR lpServiceName, DWORD& cchBuffer)
	{
		ASSERT(IsValid());
		return ::GetServiceKeyName(*this, lpDisplayName, lpServiceName, &cchBuffer);
	}
	BOOL EnumServicesStatus(
		DWORD                   dwServiceType,
		DWORD                   dwServiceState,
		LPENUM_SERVICE_STATUSA  lpServices,
		DWORD                   cbBufSize,
		LPDWORD                 pcbBytesNeeded,
		LPDWORD                 lpServicesReturned,
		LPDWORD                 lpResumeHandle)
	{
		ASSERT(IsValid());
		return ::EnumServicesStatus(*this, dwServiceType, dwServiceState, lpServices, cbBufSize, pcbBytesNeeded, lpServicesReturned, lpResumeHandle);
	}
	BOOL EnumServicesStatusEx(
		SC_ENUM_TYPE	InfoLevel,
		DWORD			dwServiceType,
		DWORD			dwServiceState,
		LPBYTE			lpServices,
		DWORD			cbBufSize,
		LPDWORD			pcbBytesNeeded,
		LPDWORD			lpServicesReturned,
		LPDWORD			lpResumeHandle,
		LPCTSTR			pszGroupName = NULL)
	{
		ASSERT(IsValid());
		return ::EnumServicesStatusEx(*this, InfoLevel, dwServiceType, dwServiceState, lpServices, cbBufSize, pcbBytesNeeded, lpServicesReturned, lpResumeHandle, pszGroupName);
	}

};
typedef CSCManagerT<true> CSCManager;
typedef CSCManagerT<false> CSCManagerHandle;
template<bool t_bManaged>
class CServiceT : public CSCHandleT<t_bManaged>
{
public:
	CServiceT(SC_HANDLE hSC = NULL)
		: CSCHandleT<t_bManaged>(hSC)
	{ }
	virtual ~CServiceT()
	{ }

public:
	BOOL Open(SC_HANDLE hSCManager, LPCTSTR lpServiceName, DWORD dwDesiredAccess = SERVICE_ALL_ACCESS)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::OpenService(hSCManager, lpServiceName, dwDesiredAccess));
	}
	BOOL Create(SC_HANDLE hSCManager,
				LPCTSTR lpBinaryPathName,
				LPCTSTR lpServiceName,
				LPCTSTR lpDisplayName		= NULL,
				DWORD	dwDesiredAccess		= SERVICE_ALL_ACCESS,
				DWORD	dwServiceType		= SERVICE_WIN32_OWN_PROCESS,
				DWORD	dwStartType			= SERVICE_AUTO_START,
				DWORD	dwErrorControl		= SERVICE_ERROR_NORMAL,
				LPCTSTR lpLoadOrderGroup	= NULL,
				LPDWORD lpdwTagId			= NULL,
				LPCTSTR lpDependencies		= NULL,
				LPCTSTR lpServiceStartName	= _T("NT AUTHORITY\\LocalService"),
				LPCTSTR lpPassword			= _T(""))
	{
		ASSERT(!IsValid());
		if(!lpDisplayName)
			lpDisplayName = lpServiceName;
		return VALID_ATTACH(::CreateService(hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType, dwStartType,
			dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagId, lpDependencies, lpServiceStartName, lpPassword));
	}
	BOOL Delete()
	{
		ASSERT(IsValid());
		return ::DeleteService(*this);
	}

public:
	BOOL Control(DWORD dwControl, LPSERVICE_STATUS lpServiceStatus)
	{
		ASSERT(IsValid());
		return ::ControlService(*this, dwControl, lpServiceStatus);
	}
	BOOL ControlEx(DWORD dwControl, DWORD dwInfoLevel, PVOID pControlParams)
	{
		ASSERT(IsValid());
		return ::ControlServiceEx(*this, dwControl, dwInfoLevel, pControlParams);
	}
	BOOL Start(DWORD dwNumServiceArgs = 0, LPCTSTR* lpServiceArgVectors = NULL)
	{
		ASSERT(IsValid());
		return ::StartService(*this, dwNumServiceArgs, lpServiceArgVectors);
	}

public:
	BOOL QueryConfig(LPQUERY_SERVICE_CONFIG lpServiceConfig, DWORD cbBufSize, LPDWORD pcbBytesNeeded)
	{
		ASSERT(IsValid());
		return ::QueryServiceConfig(*this, lpServiceConfig, cbBufSize, pcbBytesNeeded);
	}
	BOOL ChangeConfig(
		DWORD	dwServiceType		= SERVICE_NO_CHANGE,
		DWORD	dwStartType			= SERVICE_NO_CHANGE,
		DWORD	dwErrorControl		= SERVICE_NO_CHANGE,
		LPCTSTR lpBinaryPathName	= NULL,
		LPCTSTR lpLoadOrderGroup	= NULL,
		LPDWORD lpdwTagId			= NULL,
		LPCTSTR lpDependencies		= NULL,
		LPCTSTR lpServiceStartName	= NULL,
		LPCTSTR lpPassword			= NULL,
		LPCTSTR lpDisplayName		= NULL)
	{
		ASSERT(IsValid());
		return ::ChangeServiceConfig(*this, dwServiceType, dwStartType, dwErrorControl, lpBinaryPathName, lpLoadOrderGroup,
			lpdwTagId, lpDependencies, lpServiceStartName, lpPassword, lpDisplayName);
	}

	BOOL QueryConfig2(DWORD dwInfoLevel, LPBYTE lpBuffer, DWORD cbBufSize, LPDWORD pcbBytesNeeded)
	{
		ASSERT(IsValid());
		return ::QueryServiceConfig2(*this, dwInfoLevel, lpBuffer, cbBufSize, pcbBytesNeeded);
	}
	BOOL ChangeConfig2(DWORD dwInfoLevel, LPVOID lpInfo)
	{
		ASSERT(IsValid());
		return ::ChangeServiceConfig2(*this, dwInfoLevel, lpInfo);
	}
	BOOL QueryObjectSecurity(SECURITY_INFORMATION dwSecurityInformation, PSECURITY_DESCRIPTOR lpSecurityDescriptor, DWORD cbBufSize, LPDWORD pcbBytesNeeded)
	{
		ASSERT(IsValid());
		return ::QueryServiceObjectSecurity(*this, dwSecurityInformation, lpSecurityDescriptor, cbBufSize, pcbBytesNeeded);
	}
	BOOL SetObjectSecurity(SECURITY_INFORMATION dwSecurityInformation, PSECURITY_DESCRIPTOR lpSecurityDescriptor)
	{
		ASSERT(IsValid());
		return ::SetServiceObjectSecurity(*this, dwSecurityInformation, lpSecurityDescriptor);
	}
	BOOL QueryStatus(LPSERVICE_STATUS lpServiceStatus)
	{
		ASSERT(IsValid());
		return ::QueryServiceStatus(*this, lpServiceStatus);
	}
	BOOL QueryStatusEx(SC_STATUS_TYPE InfoLevel, LPBYTE lpBuffer, DWORD cbBufSize, LPDWORD pcbBytesNeeded)
	{
		ASSERT(IsValid());
		return ::QueryServiceStatusEx(*this, InfoLevel, lpBuffer, cbBufSize, pcbBytesNeeded);
	}

	DWORD NotifyStatusChange(DWORD dwNotifyMask, PSERVICE_NOTIFY pNotifyBuffer)
	{
		ASSERT(IsValid());
		return ::NotifyServiceStatusChange(*this, dwNotifyMask, pNotifyBuffer);
	}

	BOOL EnumDependentServices(DWORD dwServiceState, LPENUM_SERVICE_STATUS lpServices, DWORD cbBufSize, LPDWORD pcbBytesNeeded, LPDWORD lpServicesReturned)
	{
		ASSERT(IsValid());
		return ::EnumDependentServices(*this, dwServiceState, lpServices, cbBufSize, pcbBytesNeeded, lpServicesReturned);
	}
};
typedef CServiceT<true> CService;
typedef CServiceT<false> CServiceHandle;

_MZH_END

#endif