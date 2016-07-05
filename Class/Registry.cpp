#include "stdafx.h"
#include "Registry.h"
#include "Text.h"

Registry::Registry()
{
	hKey = nullptr;
}


Registry::~Registry()
{
	Close();
}

void Registry::Close()
{
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = nullptr;
	}
}

bool Registry::Open(HKEY key, const _TCHAR * subKey, REGSAM  samDesired)
{
	DWORD dwDisposition;
	LONG result = RegCreateKeyEx(key,
		//HKEY_CLASSES_ROOT
		//HKEY_CURRENT_CONFIG
		//HKEY_CURRENT_USER
		//HKEY_LOCAL_MACHINE
		//HKEY_USERS
		subKey,//�L�[
		0,//�\��
		NULL,//�w��
		REG_OPTION_NON_VOLATILE,//�s������
		samDesired,
		NULL,
		&hKey,
		&dwDisposition);
	if (result != ERROR_SUCCESS)
	{
		Close();
	}
	return result == ERROR_SUCCESS;
}

std::pair<DWORD, std::vector<BYTE> > Registry::RegQueryValueEx(const _TCHAR * valueName)
{
	if (hKey == nullptr)	return std::make_pair(REG_NONE, std::vector<BYTE>());

	DWORD dwType;		// �l�̎�ނ��󂯎��
	DWORD dwSize;		// �f�[�^�̃T�C�Y���󂯎��
	
	//�f�[�^�̃T�C�Y���擾
	LONG result = ::RegQueryValueEx(
		hKey,	// ���݃I�[�v�����Ă���L�[�̃n���h��
		valueName,	// �擾����l�́u���O�v��������������ւ̃|�C���^
		NULL,	// �\��p�����[�^�BNULL���w�肷��
		&dwType,	// �l�́u��ށv���󂯎��
		NULL,		// �l�́u�f�[�^�v���󂯎��BNULL���w�肷�邱�Ƃ��\�����A�f�[�^�͎󂯎��Ȃ�
		&dwSize		// �I�[����'\0'���܂�Data�̃T�C�Y���擾����
		);
	if (result != ERROR_SUCCESS)	return std::make_pair(REG_NONE, std::vector<BYTE>());
	
	std::vector<BYTE> buffer(dwSize);
	//���ۂɃf�[�^���擾�i�T�C�Y�̎w�肪�������Ȃ��Ǝ��s���邱�Ƃ�����j
	result = ::RegQueryValueEx(
		hKey,	// ���݃I�[�v�����Ă���L�[�̃n���h��
		valueName,	// �擾����l�́u���O�v��������������ւ̃|�C���^
		NULL,	// �\��p�����[�^�BNULL���w�肷��
		&dwType,	// �l�́u��ށv���󂯎��
		&buffer[0],	// �l�́u�f�[�^�v���󂯎��BNULL���w�肷�邱�Ƃ��\�����A�f�[�^�͎󂯎��Ȃ�
		&dwSize		// Data�̃T�C�Y���w�肷��
		);

	if (result != ERROR_SUCCESS)	return std::make_pair(REG_NONE, std::vector<BYTE>());
	return std::make_pair(dwType, buffer);
}

tstring Registry::Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName)
{
	// ���ɊJ���Ă���ꍇ�͈�U����
	Close();

	bool result = Open(key, subKey, KEY_READ);
	if (result)
	{
		std::pair<DWORD, std::vector<BYTE> > pair = RegQueryValueEx(valueName);
		if (pair.first == REG_SZ || pair.first == REG_EXPAND_SZ)
		{
			if (pair.second.empty())
			{
				return tstring();
			}
			else
			{
				const LPCTSTR str = reinterpret_cast<LPCTSTR>(&pair.second[0]);
				return (pair.first == REG_SZ ? str : Text::ExpandEnvironmentStrings(str));
			}
		}
	}
	return tstring();
}