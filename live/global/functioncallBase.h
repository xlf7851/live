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

	// ��ȡ���ܵ�������
	_tstring GetName()const;
	// ��ȡ���ò���ֵ
	_tstring GetParamValue(LPCTSTR lpszKey) const;
	bool IsAttributeTrue(LPCTSTR lpszKey) const;

	void SetName(LPCTSTR lpszName);
	void ParseParam(LPCTSTR lpszParam);
protected:
	void Release();
	FunctionParamItem* FindParamItem(LPCTSTR lKey) const;

protected:
	_tstring m_strName;
	_param_container_t m_param;

};
