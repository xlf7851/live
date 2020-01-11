#include "stdafx.h"
#include "jsonutl.h"


namespace json_utl
{
	int json_to_int(Json::Value& js, int default)
	{
		int nRet = default;
		Json::Value& v = js;
		if (v.isNull())
		{
			return nRet;
		}

		if (v.isInt() || v.isUInt() || v.isBool())
		{
			nRet = v.asInt();
		}
		else if (v.isString())
		{
			nRet = atoi(v.asCString());
		}

		return nRet;
	}

	std::string json_to_string(Json::Value& js, LPCSTR default)
	{
		std::string strRet = default;
		Json::Value& v = js;
		if (v.isNull())
		{
			return strRet;
		}

		if (v.isInt())
		{
			char sz[128] = { 0 };
			sprintf_s(sz, "%d", v.asInt());
			strRet = sz;
		}
		else if (v.isUInt())
		{
			char sz[128] = { 0 };
			sprintf_s(sz, "%u", v.asUInt());
			strRet = sz;
		}
		else if (v.isString() || v.isBool())
		{
			strRet = v.asString();
		}

		return strRet;
	}

	
		
}