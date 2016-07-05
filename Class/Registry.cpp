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
		subKey,//キー
		0,//予約
		NULL,//指定
		REG_OPTION_NON_VOLATILE,//不揮発性
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

	DWORD dwType;		// 値の種類を受け取る
	DWORD dwSize;		// データのサイズを受け取る
	
	//データのサイズを取得
	LONG result = ::RegQueryValueEx(
		hKey,	// 現在オープンしているキーのハンドル
		valueName,	// 取得する値の「名前」が入った文字列へのポインタ
		NULL,	// 予約パラメータ。NULLを指定する
		&dwType,	// 値の「種類」を受け取る
		NULL,		// 値の「データ」を受け取る。NULLを指定することも可能だが、データは受け取れない
		&dwSize		// 終端文字'\0'を含んだDataのサイズを取得する
		);
	if (result != ERROR_SUCCESS)	return std::make_pair(REG_NONE, std::vector<BYTE>());
	
	std::vector<BYTE> buffer(dwSize);
	//実際にデータを取得（サイズの指定が正しくないと失敗することがある）
	result = ::RegQueryValueEx(
		hKey,	// 現在オープンしているキーのハンドル
		valueName,	// 取得する値の「名前」が入った文字列へのポインタ
		NULL,	// 予約パラメータ。NULLを指定する
		&dwType,	// 値の「種類」を受け取る
		&buffer[0],	// 値の「データ」を受け取る。NULLを指定することも可能だが、データは受け取れない
		&dwSize		// Dataのサイズを指定する
		);

	if (result != ERROR_SUCCESS)	return std::make_pair(REG_NONE, std::vector<BYTE>());
	return std::make_pair(dwType, buffer);
}

tstring Registry::Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName)
{
	// 既に開いている場合は一旦閉じる
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