#pragma once
#include "log.h"

class CLogFile :
	public CLog
{
private:
	TCHAR	m_LogName[MAX_PATH];


public:
	CLogFile(void);
	CLogFile(TCHAR *_path);
	~CLogFile(void);
	template <typename T>	void Add(std::wstring sTitle, T value )
	{
		std::wstring wtext = boost::lexical_cast<std::wstring>(value);
		Log(sTitle + L":\t" + wtext);
	}
	template <typename T>	void Add(T value)
	{
		std::wstring wtext = boost::lexical_cast<std::wstring>(value);
		Log(wtext);
	}


	void DeleteLog();


private:
	void Log( std::wstring str );
	void SetDefault( TCHAR *path = NULL );
};
