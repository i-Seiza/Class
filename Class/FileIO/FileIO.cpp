#include "StdAfx.h"
#include <locale>
#include "FileIO.h"
#include "Log.h"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

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

// ファイルの内容を全て読み込む
std::vector< tstring> CFileIO::Read( const _TCHAR *path )
{
	//ifstreamの場合
	ifstream ifs( path );

	std::locale::global(std::locale("japanese"));
	_tsetlocale(LC_ALL, _T("Japanese"));

	tstring str;
	std::vector< tstring > vec;

	if(ifs.fail()) {
		return vec;
	}

	while(getline(ifs, str)) {
		vec.push_back( str );
	}
	return vec;
}

// ファイルに書き込む
void CFileIO::Write( tstring data, std::ios_base::open_mode mode, const _TCHAR *path )
{
	ofstream ofs;
	ofs.open( path, mode );
	ofs << data << std::endl;
	ofs.close();
}

// ファイルを探す
std::vector<tstring> CFileIO::FindFile( const _TCHAR *path, const _TCHAR *name )
{
	std::vector<tstring> vec;

	// 指定したファイルまたはフォルダが存在しない場合は抜ける
	if( !boost::filesystem::exists( path ))	return vec;

	// 指定したものはファイルか？
	if( !boost::filesystem::is_directory( path ))	return vec;

	tstring str = tstring( path ) + tstring( _T("\\*.*") );

	BOOST_FOREACH(const boost::filesystem::path& p, std::make_pair(boost::filesystem::recursive_directory_iterator(path),
		boost::filesystem::recursive_directory_iterator())) {
		if (!boost::filesystem::is_directory(p))
		{
			if (!p.filename().compare(boost::filesystem::path(name)))
			{
#ifdef _UNICODE
				vec.push_back(p.c_str());
#else
				vec.push_back(Text::ToAnsi(p.c_str()));
#endif // _UNICODE
			}
		}
	}

	return vec;
}

// フォルダを探す
std::vector<tstring> CFileIO::FindFolder( const _TCHAR *path, const _TCHAR *name )
{
	std::vector<tstring> vec;

	// 指定したファイルまたはフォルダが存在しない場合は抜ける
	if( !boost::filesystem::exists( path ))	return vec;

	// 指定したものはファイルか？
	if( !boost::filesystem::is_directory( path ))	return vec;

	const boost::filesystem::path fsPath( path );
	BOOST_FOREACH(const boost::filesystem::path& p, std::make_pair(boost::filesystem::recursive_directory_iterator(fsPath),
		boost::filesystem::recursive_directory_iterator())) {
		if (boost::filesystem::is_directory(p) && p.filename() == name )
		{
#ifdef _UNICODE
			vec.push_back(p.c_str());
#else
			vec.push_back(Text::ToAnsi(p.c_str()));
#endif // _UNICODE
		}
	}

	return vec;
}

boost::posix_time::ptime CFileIO::GetLastWriteTime(const _TCHAR *path)
{
	try {
		const std::time_t last_update = boost::filesystem::last_write_time(boost::filesystem::path(path));

		return boost::posix_time::from_time_t(last_update);
		std::cout << time << std::endl;
	}
	catch (boost::filesystem::filesystem_error& ex) {
		std::cout << "エラー発生！ : " << ex.what() << std::endl;
	}

	return boost::posix_time::ptime();
}

/*
第1引数は、元となるファイルのパス。
第2引数は、移動先のファイルパス、もしくは新たなファイル名。
*/
bool CFileIO::rename(const _TCHAR* path, const _TCHAR* dest)
{
	try {
		boost::filesystem::rename(
			boost::filesystem::path(path),
			boost::filesystem::path(dest)
		);
		return true;
	}
	catch (boost::filesystem::filesystem_error& ex) {
		std::cout << ex.what() << std::endl;
	}

	return false;
}