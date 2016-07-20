#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <locale.h>
#include <locale>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/optional.hpp>
#include <boost/xpressive/xpressive.hpp>



typedef std::basic_string<TCHAR> tstring;
#ifdef _UNICODE
typedef boost::wregex regex;
typedef boost::wsmatch smatch;
typedef std::wifstream ifstream;
typedef std::wofstream ofstream;
# define tout std::wcout
# define tin std::wcin
# define terr std::wcerr
# define tlog std::wclog

#else
typedef boost::regex regex;
typedef boost::smatch smatch;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;
# define tout std::cout
# define tin std::cin
# define terr std::cerr
# define tlog std::clog

#endif // _UNICODE
