#pragma once


class CFunctionCallItem
{
private:
	struct FunctionParamItem
	{
		_tstring m_strKey;
		_tstring m_strValue;
	};
	typedef std::vector<FunctionParamItem*> _param_container_t;

public:
	CFunctionCallItem();
	virtual ~CFunctionCallItem();

	// 获取功能调用名称
	_tstring GetName()const;
	// 获取调用参数值
	_tstring GetParamValue(LPCTSTR lpszKey) const;
	bool IsAttributeTrue(LPCTSTR lpszKey) const;

	void SetName(LPCTSTR lpszName);
	void SetParam(LPCTSTR lpszKey, LPCTSTR lpszValue);
	void ParseParam(LPCTSTR lpszParam);
protected:
	void Release();
	FunctionParamItem* FindParamItem(LPCTSTR lKey) const;

protected:
	_tstring m_strName;
	_param_container_t m_param;

};

class IFuntionCaller
{
public:
	virtual function_call_error_code FunctionCallHandle(const _tstring& funcName, const CFunctionCallItem& callParam) = 0;
};

class CFunctinCallerManager
{
public:
	typedef std::map<_tstring, IFuntionCaller*> _dataContianer_t;
public:
	~CFunctinCallerManager(){}

	static CFunctinCallerManager* Instance();

	bool FunctionCallHandle(const _tstring& funcName, const CFunctionCallItem& callParam, function_call_error_code& error);
	void RegisterFunctionCaller(const _tstring& name, IFuntionCaller* caller);
	void UnregitserFunctinCaller(const _tstring& name);


private:
	CFunctinCallerManager(){}
private:
	_dataContianer_t m_callerContainer;
};