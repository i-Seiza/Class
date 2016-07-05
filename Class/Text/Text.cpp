#include "stdafx.h"
#include "Text.h"

const int BUFFER_COUNT_THRESHOLD = 1024 * 1024;

Text::Text()
{
}


Text::~Text()
{
}

int Text::MultiByteToWideCharSafe(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	int ret = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	return ret;
}

int Text::WideCharToMultiByteSafe(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
	int ret = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
	return ret;
}


std::string Text::ToAnsiImpl(boost::wstring_ref src, UINT codePage)
{
	if (src.empty())
	{
		return std::string();
	}

	int srcSize = boost::numeric_cast<int>(src.size());
	int dstSizeTmp =
		srcSize < BUFFER_COUNT_THRESHOLD
		? srcSize * 3
		: WideCharToMultiByteSafe(codePage, 0, src.data(), srcSize, nullptr, 0, nullptr, nullptr);

	std::string ret(static_cast<std::size_t>(dstSizeTmp), '\0');
	int dstSize = WideCharToMultiByteSafe(codePage, 0, src.data(), srcSize, &ret[0], dstSizeTmp, nullptr, nullptr);
	ret.resize(static_cast<std::size_t>(dstSize));

	return ret;
}

std::wstring Text::ToWideImpl(boost::string_ref src, UINT codePage)
{
	if (src.empty())
	{
		return std::wstring();
	}

	int srcSize = boost::numeric_cast<int>(src.size());
	int dstSizeTmp =
		srcSize < BUFFER_COUNT_THRESHOLD
		? srcSize
		: MultiByteToWideCharSafe(codePage, 0, src.data(), srcSize, nullptr, 0);

	std::wstring ret(static_cast<std::size_t>(dstSizeTmp), '\0');
	int dstSize = MultiByteToWideCharSafe(codePage, 0, src.data(), srcSize, &ret[0], dstSizeTmp);
	ret.resize(static_cast<std::size_t>(dstSize));

	return ret;
}

std::string Text::ToAnsi(boost::string_ref src)
{
	return src.to_string();
}

std::string Text::ToAnsi(boost::wstring_ref src)
{
	return ToAnsiImpl(src, CP_ACP);
}

std::string Text::ToAnsi(const boost::wformat &src)
{
	return ToAnsi(src.str());
}

std::vector<std::string> Text::ToAnsi(const std::vector<std::wstring> &src)
{
	std::string(*f)(boost::wstring_ref src) = &ToAnsi;
	std::vector<std::string> result;
	result.reserve(src.size());
	std::transform(src.begin(), src.end(), std::back_inserter(result), f);
	return result;
}

std::wstring Text::ToWide(boost::string_ref src)
{
	return ToWideImpl(src, CP_ACP);
}

std::wstring Text::ToWide(boost::wstring_ref src)
{
	return src.to_string();
}

std::vector<std::wstring> Text::ToWide(const std::vector<std::string> &src)
{
	std::wstring(*f)(boost::string_ref src) = &ToWide;
	std::vector<std::wstring> result;
	result.reserve(src.size());
	std::transform(src.begin(), src.end(), std::back_inserter(result), f);
	return result;
}

std::wstring Text::ToWide(const boost::format &src)
{
	return ToWide(src.str());
}

std::wstring Text::ToWide(const boost::wformat &src)
{
	return src.str();
}

tstring Text::ExpandEnvironmentStrings(const _TCHAR *lpSrc)
{
	tstring buffer;
	buffer.resize(256);
	for (;;)
	{
		DWORD result = ::ExpandEnvironmentStrings(lpSrc, &buffer[0], boost::numeric_cast<DWORD>(buffer.size()));
		if (result == 0)
		{
			return tstring();
		}
		else if (result <= buffer.size())
		{
			buffer.resize(result - 1);
			return buffer;
		}
		else
		{
			buffer.resize(result);
		}
	}
}