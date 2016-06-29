#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

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

#else
typedef boost::regex regex;
typedef boost::smatch smatch;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;
#endif // _UNICODE
