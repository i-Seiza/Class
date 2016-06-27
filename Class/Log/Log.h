#pragma once
#include <Windows.h>
#include <string>

#include <Objbase.h>
#include "Text.h"
#include "def.h"

class CLog
{


public:
	CLog(void);
	~CLog(void);

	template <typename T> void Add(tstring title, T value)
	{
		tstring text = boost::lexical_cast<tstring>(value);
		Log(title + _T(":\t") + text);
	}

	template <typename T>	void Add(T value)
	{
		tstring text = boost::lexical_cast<tstring>(value);
		Log(text);
	}

	void AddLastError();
	void AddResult(tstring sTitle, bool bSuccess);

	void Add(tstring sTitle, GUID guid);



protected:
	virtual void Log(std::wstring str);
	virtual void Log(std::string str);


};
