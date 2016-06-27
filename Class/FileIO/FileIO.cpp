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

// �t�@�C�����J��
bool CFileIO::FileOpen(HANDLE *hFile, TCHAR *path, DWORD dwFlagsAndAttributes)
{
	CLog log;

	// ��U�t�@�C����open
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
		// �t�@�C�����J���Ȃ������B�ǂ����悤�H
		log.AddResult(_T("�t�@�C���I�[�v��"), false);
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

// �t�@�C���̓��e��S�ēǂݍ���
std::vector< tstring> CFileIO::Read( const _TCHAR *path )
{
	//ifstream�̏ꍇ
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

// �t�@�C���ɏ�������
void CFileIO::Write( tstring data, std::ios_base::open_mode mode, const _TCHAR *path )
{
	ofstream ofs;
	ofs.open( path, mode );
	ofs << data << std::endl;
	ofs.close();
}

// �t�@�C����T��
std::vector<tstring> CFileIO::FindFile( const _TCHAR *path, const _TCHAR *name )
{
	std::vector<tstring> vec;

	// �w�肵���t�@�C���܂��̓t�H���_�����݂��Ȃ��ꍇ�͔�����
	if( !boost::filesystem::exists( path ))	return vec;

	// �w�肵�����̂̓t�@�C�����H
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

// �t�H���_��T��
std::vector<tstring> CFileIO::FindFolder( const _TCHAR *path, const _TCHAR *name )
{
	std::vector<tstring> vec;

	// �w�肵���t�@�C���܂��̓t�H���_�����݂��Ȃ��ꍇ�͔�����
	if( !boost::filesystem::exists( path ))	return vec;

	// �w�肵�����̂̓t�@�C�����H
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
		std::cout << "�G���[�����I : " << ex.what() << std::endl;
	}

	return boost::posix_time::ptime();
}

/*
��1�����́A���ƂȂ�t�@�C���̃p�X�B
��2�����́A�ړ���̃t�@�C���p�X�A�������͐V���ȃt�@�C�����B
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