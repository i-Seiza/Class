#include "StdAfx.h"
#include "FileIO.h"
#include "Log.h"

CFileIO::CFileIO(void)
{
}

CFileIO::~CFileIO(void)
{
}

// ファイルを開く
bool CFileIO::FileOpen(HANDLE *hFile, TCHAR *path, DWORD dwFlagsAndAttributes)
{
	CLog log;

	// 一旦ファイルをopen
	*hFile = CreateFile(
		path,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		dwFlagsAndAttributes,
		NULL
		);
	if (*hFile == INVALID_HANDLE_VALUE)
	{
		// ファイルが開けなかった。どうしよう？
		log.AddResult(_T("ファイルオープン"), false);
		return false;
	}

	return true;
}

bool CFileIO::FileClose(HANDLE *hFile)
{
	if (*hFile)
	{
		CloseHandle(*hFile);
	}
	return true;
}