#include "stdafx.h"
#include "DiskVolume.h"
#include "Log.h"



CDiskVolume::CDiskVolume()
{
	SetPath(_T("E:\\dummy.txt"));
}

void CDiskVolume::SetPath(TCHAR *_path)
{
	_stprintf_s(path, MAX_PATH, _path );
	_tsplitpath_s( path, drive, MAX_PATH, NULL, 0, NULL, 0, NULL, 0 );
	_tcscat_s(drive, MAX_PATH, _T("\\"));
}

// 指定されたドライブの情報を取得する
bool CDiskVolume::GetDiskVolumeInfo(TCHAR *_path)
{
	if (_path != NULL)
	{
		SetPath(_path);
	}

	// ダミーファイルを削除する
	DeleteFile(path);

	// ログ
	CLog log;

	// 取得処理
	BOOL bSuccess = GetDiskFreeSpaceEx(
		drive,
		(PULARGE_INTEGER)&FreeBytesAvailable,    // 呼び出し側が利用できるバイト数
		(PULARGE_INTEGER)&TotalNumberOfBytes,    // ディスク全体のバイト数
		(PULARGE_INTEGER)&TotalNumberOfFreeBytes // ディスク全体の空きバイト数
		);
	log.AddResult(_T("ディスク容量取得"), bSuccess != 0);

	if (bSuccess == 0)
	{
		return false;
	}

	log.Add(_T("\tディスクチェックするドライブ"), drive);
	log.Add(_T("\t呼び出し側が利用できるバイト数"), FreeBytesAvailable);
	log.Add(_T("\tディスク全体のバイト数"), TotalNumberOfBytes);
	log.Add(_T("\tディスク全体の空きバイト数"), TotalNumberOfFreeBytes);
	log.Add(_T("----------"));

	bSuccess = GetDiskFreeSpace(
		drive,					  // ルートパス
		&SectorsPerCluster,     // クラスタ当たりのセクタ数
		&BytesPerSector,        // セクタ当たりのバイト数
		&NumberOfFreeClusters,  // 空きクラスタ数
		&TotalNumberOfClusters  // 全クラスタ数
		);

	log.AddResult(_T("GetDiskFreeSpace"), bSuccess != 0);
	log.Add(_T("\tクラスタ当たりのセクタ数"), SectorsPerCluster);
	log.Add(_T("\tセクタ当たりのバイト数"), BytesPerSector);
	log.Add(_T("\t空きクラスタ数"), NumberOfFreeClusters);
	log.Add(_T("\t全クラスタ数"), TotalNumberOfClusters);
	log.Add(_T("----------"));




	if (bSuccess == 0)
	{
		return false;
	}


	TCHAR VolumeNameBuffer[_MAX_PATH];        // ボリューム名バッファ
	DWORD VolumeSerialNumber;     // ボリュームのシリアル番号
	DWORD MaximumComponentLength; // ファイル名の最大の長さ
	DWORD FileSystemFlags;        // ファイルシステムのオプション
	TCHAR FileSystemNameBuffer[_MAX_PATH];    // ファイルシステム名を格納するバッファ


	// 指定されたルートディレクトリのファイルシステムとボリュームに関する情報を取得します。
	bSuccess = GetVolumeInformation(
		drive,           // ルートディレクトリ
		VolumeNameBuffer,        // ボリューム名バッファ
		sizeof(VolumeNameBuffer),            // 名前バッファのサイズ
		&VolumeSerialNumber,     // ボリュームのシリアル番号
		&MaximumComponentLength, // ファイル名の最大の長さ
		&FileSystemFlags,        // ファイルシステムのオプション
		FileSystemNameBuffer,    // ファイルシステム名を格納するバッファ
		sizeof(FileSystemNameBuffer)	// ファイルシステム名を格納するバッファのサイズ
		);
	log.AddResult(_T("GetVolumeInformation"), bSuccess != 0);
	log.Add(_T("\tボリューム名"), VolumeNameBuffer);
	log.Add(_T("\tボリュームのシリアル番号"), VolumeSerialNumber);
	log.Add(_T("\tファイル名の最大の長さ"), MaximumComponentLength);
	log.Add(_T("\tファイルシステムのオプション"), FileSystemFlags);
	log.Add(_T("\tファイルシステム名"), FileSystemNameBuffer);
	log.Add(_T("----------"));

	if (bSuccess == 0)
	{
		return false;
	}

	bSupportsSparseFiles = FileSystemFlags & FILE_SUPPORTS_SPARSE_FILES ? true : false;


	return true;
}


TCHAR * CDiskVolume::GetPath()
{
	return path;
}



DWORD CDiskVolume::GetBytesPerSector()
{
	return BytesPerSector;
}

DWORD CDiskVolume::GetSectorsPerCluster()
{
	return SectorsPerCluster;
}


DWORD CDiskVolume::GetNumberOfFreeClusters()
{
	return NumberOfFreeClusters;
}

unsigned __int64 CDiskVolume::GetTotalNumberOfFreeBytes()
{
	return TotalNumberOfFreeBytes;
}

bool CDiskVolume::IsSupportsSparseFiles()
{
	return bSupportsSparseFiles;
}

