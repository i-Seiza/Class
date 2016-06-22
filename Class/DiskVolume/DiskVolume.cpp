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

// �w�肳�ꂽ�h���C�u�̏����擾����
bool CDiskVolume::GetDiskVolumeInfo(TCHAR *_path)
{
	if (_path != NULL)
	{
		SetPath(_path);
	}

	// �_�~�[�t�@�C�����폜����
	DeleteFile(path);

	// ���O
	CLog log;

	// �擾����
	BOOL bSuccess = GetDiskFreeSpaceEx(
		drive,
		(PULARGE_INTEGER)&FreeBytesAvailable,    // �Ăяo���������p�ł���o�C�g��
		(PULARGE_INTEGER)&TotalNumberOfBytes,    // �f�B�X�N�S�̂̃o�C�g��
		(PULARGE_INTEGER)&TotalNumberOfFreeBytes // �f�B�X�N�S�̂̋󂫃o�C�g��
		);
	log.AddResult(_T("�f�B�X�N�e�ʎ擾"), bSuccess != 0);

	if (bSuccess == 0)
	{
		return false;
	}

	log.Add(_T("\t�f�B�X�N�`�F�b�N����h���C�u"), drive);
	log.Add(_T("\t�Ăяo���������p�ł���o�C�g��"), FreeBytesAvailable);
	log.Add(_T("\t�f�B�X�N�S�̂̃o�C�g��"), TotalNumberOfBytes);
	log.Add(_T("\t�f�B�X�N�S�̂̋󂫃o�C�g��"), TotalNumberOfFreeBytes);
	log.Add(_T("----------"));

	bSuccess = GetDiskFreeSpace(
		drive,					  // ���[�g�p�X
		&SectorsPerCluster,     // �N���X�^������̃Z�N�^��
		&BytesPerSector,        // �Z�N�^������̃o�C�g��
		&NumberOfFreeClusters,  // �󂫃N���X�^��
		&TotalNumberOfClusters  // �S�N���X�^��
		);

	log.AddResult(_T("GetDiskFreeSpace"), bSuccess != 0);
	log.Add(_T("\t�N���X�^������̃Z�N�^��"), SectorsPerCluster);
	log.Add(_T("\t�Z�N�^������̃o�C�g��"), BytesPerSector);
	log.Add(_T("\t�󂫃N���X�^��"), NumberOfFreeClusters);
	log.Add(_T("\t�S�N���X�^��"), TotalNumberOfClusters);
	log.Add(_T("----------"));




	if (bSuccess == 0)
	{
		return false;
	}


	TCHAR VolumeNameBuffer[_MAX_PATH];        // �{�����[�����o�b�t�@
	DWORD VolumeSerialNumber;     // �{�����[���̃V���A���ԍ�
	DWORD MaximumComponentLength; // �t�@�C�����̍ő�̒���
	DWORD FileSystemFlags;        // �t�@�C���V�X�e���̃I�v�V����
	TCHAR FileSystemNameBuffer[_MAX_PATH];    // �t�@�C���V�X�e�������i�[����o�b�t�@


	// �w�肳�ꂽ���[�g�f�B���N�g���̃t�@�C���V�X�e���ƃ{�����[���Ɋւ�������擾���܂��B
	bSuccess = GetVolumeInformation(
		drive,           // ���[�g�f�B���N�g��
		VolumeNameBuffer,        // �{�����[�����o�b�t�@
		sizeof(VolumeNameBuffer),            // ���O�o�b�t�@�̃T�C�Y
		&VolumeSerialNumber,     // �{�����[���̃V���A���ԍ�
		&MaximumComponentLength, // �t�@�C�����̍ő�̒���
		&FileSystemFlags,        // �t�@�C���V�X�e���̃I�v�V����
		FileSystemNameBuffer,    // �t�@�C���V�X�e�������i�[����o�b�t�@
		sizeof(FileSystemNameBuffer)	// �t�@�C���V�X�e�������i�[����o�b�t�@�̃T�C�Y
		);
	log.AddResult(_T("GetVolumeInformation"), bSuccess != 0);
	log.Add(_T("\t�{�����[����"), VolumeNameBuffer);
	log.Add(_T("\t�{�����[���̃V���A���ԍ�"), VolumeSerialNumber);
	log.Add(_T("\t�t�@�C�����̍ő�̒���"), MaximumComponentLength);
	log.Add(_T("\t�t�@�C���V�X�e���̃I�v�V����"), FileSystemFlags);
	log.Add(_T("\t�t�@�C���V�X�e����"), FileSystemNameBuffer);
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

