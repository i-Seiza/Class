// Class.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "Log\Log.h"
int main()
{
	CLog log;
	log.Add(_T("test"), _T("test2"));

	GUID guid;
	::CoCreateGuid(&guid);
	log.Add(_T("guid"), guid);
    return 0;
}

