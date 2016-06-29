#pragma once

#include "def.h"

// �T�[�r�X�𑀍삷��N���X
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

