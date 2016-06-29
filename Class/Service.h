#pragma once

#include "def.h"

// サービスを操作するクラス
class Service
{
public:
	Service();
	~Service();

private:
	SC_HANDLE handle;

public:
	bool Open(DWORD desiredAccess);
	bool GetStatus(DWORD * status);
	boost::optional<bool> IsRunning();
	boost::optional<bool> IsStopped();
	bool StartService();
	bool StopService();
	bool WaitForPendingState();
	tstring GetLastErrorCode();
};

