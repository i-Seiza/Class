#pragma once
#include "def.h"

class Registry
{
public:
	Registry();
	~Registry();

private:
	HKEY hKey;


	void Close();
	bool Open(HKEY key, const _TCHAR *subKey, REGSAM  samDesired);
	std::pair<DWORD, std::vector<BYTE> > RegQueryValueEx(const _TCHAR *valueName);

	template <typename T> T ReadForNumeric(const _TCHAR *valueName)
	{
		std::pair<DWORD, std::vector<BYTE> > pair = RegQueryValueEx(valueName);
		if (pair.first == REG_DWORD && pair.second.size() == sizeof(DWORD))
		{
			return boost::numeric_cast<T>(*reinterpret_cast<const DWORD *>(&pair.second[0]));
		}
		else if (pair.first == REG_QWORD && pair.second.size() == sizeof(ULONGLONG))
		{
			return boost::numeric_cast<T>(*reinterpret_cast<const ULONGLONG *>(&pair.second[0]));
		}
		return T();
	}

	template <typename T> T ReadForNumeric(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName)
	{
		// ä˘Ç…äJÇ¢ÇƒÇ¢ÇÈèÍçáÇÕàÍíUï¬Ç∂ÇÈ
		Close();

		bool result = Open(key, subKey, KEY_READ);
		if (result)
		{
			return ReadForNumeric<T>(valueName);
		}
		return T();
	}

	tstring Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName);
public:

	template <typename T> T Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName) {};
	template <> DWORD Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName) { return ReadForNumeric<DWORD>(key, subKey, valueName); };
	template <> ULONGLONG Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName) { return ReadForNumeric<ULONGLONG>(key, subKey, valueName);	};

	template <> tstring Read(HKEY key, const _TCHAR *subKey, const _TCHAR *valueName) { return Read(key, subKey, valueName); };

};

