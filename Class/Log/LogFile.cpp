#include "StdAfx.h"
#include "LogFile.h"

#define _LOG_FILE_NAME _T("log.txt")


////////////////////////////
CLogFile::CLogFile(void)
{
	SetDefault();
}

CLogFile::CLogFile(TCHAR *_path)
{
	SetDefault(_path);
}


CLogFile::~CLogFile(void)
{
}

////////////////////////////
void CLogFile::SetDefault( TCHAR *path )
{
	_tsetlocale(LC_ALL, _T(""));

	TCHAR *p = path == nullptr ? _LOG_FILE_NAME : path;
	_stprintf_s( m_LogName, MAX_PATH, p);
}

////////////////////////////
void CLogFile::DeleteLog()
{
	::DeleteFile(m_LogName);
}

void CLogFile::Log( std::wstring str )
{
	CLog::Log(str);

	FILE *fp = NULL;
	_tfopen_s( &fp, m_LogName, _T("a"));
	if(fp)
	{
		fputws( str.c_str(), fp );
		fclose(fp);
	}
}