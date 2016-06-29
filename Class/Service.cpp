#include "stdafx.h"
#include "Service.h"


Service::Service()
{
	handle = nullptr;
}


Service::~Service()
{
	if (handle)
	{
		::CloseServiceHandle(handle);
	}
}

bool Service::Open(DWORD desiredAccess)
{
	handle = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT| SC_MANAGER_ENUMERATE_SERVICE);
	if (handle == nullptr)	return false;
	
	handle = OpenService(handle, _T("BizServiceHost"), desiredAccess);
	if (handle == nullptr)	return false;
	return true;
}

bool Service::GetStatus(DWORD *status)
{
	bool ret = false;
	if (handle == nullptr)
	{
		ret = Open(SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_USER_DEFINED_CONTROL);
		if (ret == false)	return ret;
	}

	SERVICE_STATUS serviceStatus;
	ret = QueryServiceStatus(handle, &serviceStatus);
	if (ret == false)	return false;

	*status = serviceStatus.dwCurrentState;
	return true;
}

boost::optional<bool> Service::IsRunning()
{
	DWORD status;
	bool ret = GetStatus(&status);
	if (ret == false)	return boost::none;

	return status == SERVICE_RUNNING;
}

boost::optional<bool> Service::IsStopped()
{
	DWORD status;
	bool ret = GetStatus(&status);
	if (ret == false)	return boost::none;

	return status == SERVICE_STOPPED;
}

bool Service::StartService()
{
	boost::optional<bool> status = IsRunning();
	if (status)
	{
		if (*status == true)		return true;
		// 開始していないので開始する。
		bool ret = ::StartService(handle, 0, nullptr);
		return ret;
	}
	else
	{
		return false;
	}
}

bool Service::StopService()
{
	boost::optional<bool> status = IsStopped();
	if (status)
	{
		if (*status == true)		return true;
		// 停止していないので開始する。
		SERVICE_STATUS serviceStatus;
		bool ret = ::ControlService(handle, 133, &serviceStatus);
		return ret;
	}
	else
	{
		return false;
	}
}

// 待機中である場合は抜ける
bool Service::WaitForPendingState()
{
	const std::vector<DWORD> pending =
	{
		SERVICE_STOP_PENDING,
		SERVICE_START_PENDING,
		SERVICE_CONTINUE_PENDING,
		SERVICE_PAUSE_PENDING,
	};

	DWORD status;
	std::vector<DWORD>::const_iterator it;

	while(true)
	{
		bool ret = GetStatus(&status);
		it = std::find(pending.begin(), pending.end(), status);
		if (it == pending.end())break;
		::Sleep(1000);
	}
	return true;
}

///////////////////////////////////////////
tstring Service::GetLastErrorCode()
{
	std::map<DWORD, tstring> err = {
		{ ERROR_ACCESS_DENIED , (_T("要求されたアクセスは拒絶されました。")) },
		{ ERROR_DATABASE_DOES_NOT_EXIST , (_T("指定されたデータベースは存在しません。")) },
		{ ERROR_INVALID_PARAMETER , (_T("指定されたパラメータは無効です。")) },
		{ ERROR_INVALID_HANDLE, _T("指定されたハンドルは無効です。") },
		{ ERROR_INVALID_NAME, _T("指定されたサービス名は無効です。") },
		{ ERROR_SERVICE_DOES_NOT_EXIST, _T("指定されたサービスは存在しません。") },
	};

	DWORD errCode = GetLastError();
	std::map<DWORD, tstring>::iterator it = err.find(errCode);
	if (it != err.end())	return it->second;
	return _T("問題はありません。");
}
