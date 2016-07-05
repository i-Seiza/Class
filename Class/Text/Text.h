#pragma once
#include "def.h"
#include <boost/utility/string_ref.hpp>
#include <boost/cast.hpp>
#include <boost/format.hpp>


class Text
{

public:
	Text();
	~Text();

	static std::string ToAnsi(boost::string_ref src);
	static std::string ToAnsi(boost::wstring_ref src);
	static std::string ToAnsi(const boost::wformat & src);
	static std::vector<std::string> ToAnsi(const std::vector<std::wstring>& src);
	static std::wstring ToWide(boost::string_ref src);
	static std::wstring ToWide(boost::wstring_ref src);
	static std::vector<std::wstring> ToWide(const std::vector<std::string>& src);
	static std::wstring ToWide(const boost::format & src);
	static std::wstring ToWide(const boost::wformat & src);

	static tstring Text::ExpandEnvironmentStrings(const _TCHAR *lpSrc);

private:
	static int MultiByteToWideCharSafe(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
	static int WideCharToMultiByteSafe(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);
	static std::string ToAnsiImpl(boost::wstring_ref src, UINT codePage);
	static std::wstring ToWideImpl(boost::string_ref src, UINT codePage);

};

