#include "StdAfx.h"
#include "Log.h"


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


/////////////////////////
// コンストラクタ
CLog::CLog(void)
{
	_tsetlocale(LC_ALL, _T(""));
}


CLog::~CLog(void)
{

}


/////////////////////////
// 表示
void CLog::Log(std::wstring str)
{
	str += L"\n";
	::OutputDebugStringW(str.c_str());
	wprintf(str.c_str());
}

void CLog::Log(std::string str)
{
	str += "\n";
	::OutputDebugStringA(str.c_str());
	printf(str.c_str());
}

/////////////////////////
// 結果表示
void CLog::AddLastError()
{
	Add(GetLastError());
}

void CLog::AddResult(tstring sTitle, bool bSuccess)
{
	tstring wtext = sTitle + _T(":\t");
	if (bSuccess)
	{
		wtext += _T("Success");
	}
	else
	{
		wtext += (_T("failed(") + boost::lexical_cast<tstring>(GetLastError()) + _T(")"));
	}

	Add(wtext);
}

void CLog::Add(tstring sTitle, GUID guid)
{
	OLECHAR strGuid[40];
	int ret = ::StringFromGUID2(guid, strGuid, sizeof(strGuid));

#if _UNICODE
	if (ret)	Add(sTitle, strGuid);
#else
	if(ret)	Add(sTitle, Text::ToAnsi(strGuid));
#endif // _UNICODE
	else	Add(_T("GUIDの出力に失敗"));

}

