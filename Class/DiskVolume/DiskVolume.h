#pragma once
#include "windows.h"

// ディスク容量についての処理を行う
class CDiskVolume
{
public:
	CDiskVolume();
	bool GetDiskVolumeInfo(TCHAR *_path = NULL);
	DWORD GetBytesPerSector();
	TCHAR *GetPath();

	DWORD GetSectorsPerCluster();
	DWORD GetNumberOfFreeClusters();

	unsigned __int64 GetTotalNumberOfFreeBytes();
	bool IsSupportsSparseFiles();

protected:
private:

	void SetPath( TCHAR *_path );



	TCHAR path[_MAX_PATH];
	TCHAR drive[_MAX_PATH];

	DWORD SectorsPerCluster;     // クラスタ当たりのセクタ数
	DWORD BytesPerSector;        // セクタ当たりのバイト数
	DWORD NumberOfFreeClusters;  // 空きクラスタ数
	DWORD TotalNumberOfClusters;  // 全クラスタ数

	unsigned __int64  FreeBytesAvailable;    // 呼び出し側が利用できるバイト数
	unsigned __int64  TotalNumberOfBytes;    // ディスク全体のバイト数
	unsigned __int64  TotalNumberOfFreeBytes; // ディスク全体の空きバイト数

	bool bSupportsSparseFiles;	// SparseFileをサポートしているか

};