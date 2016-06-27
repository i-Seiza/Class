#pragma once
#include "def.h"

class CFileIO
{
public:
	CFileIO(void);
	~CFileIO(void);


	std::vector< tstring>	Read( const _TCHAR *path );	// ƒtƒ@ƒCƒ‹‚ð“Ç‚Þ
	void						Write( tstring data, std::ios_base::open_mode mode, const _TCHAR *path );
	boost::posix_time::ptime GetLastWriteTime(const _TCHAR * path);
	bool rename(const _TCHAR * path, const _TCHAR * dest);
	std::vector<tstring> FindFile( const _TCHAR *path, const _TCHAR *name );
	std::vector<tstring> FindFolder( const _TCHAR *path, const _TCHAR *name );

	bool FileOpen( HANDLE *hFile, TCHAR *path, DWORD dwFlagsAndAttributes );
	bool FileClose(HANDLE *hFile);

};
