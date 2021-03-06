#pragma once

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned long long uint64;
typedef long long int64;


#ifdef _UNICODE
typedef std::wstring _tstring;
#else
typedef std::string _tstring;
#endif

typedef std::vector<void*> CPtrVector;