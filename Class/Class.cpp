// Class.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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

