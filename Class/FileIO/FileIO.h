#pragma once
#include "Windows.h"

class CFileIO
{
public:
	CFileIO(void);
	~CFileIO(void);


	bool FileOpen( HANDLE *hFile, TCHAR *path, DWORD dwFlagsAndAttributes );
	bool FileClose(HANDLE *hFile);

};
