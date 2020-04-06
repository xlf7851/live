#pragma once



#define RET_OK 0
// 没有找到匹配的]
#define ERR_UNMATCHED_BRACKETS 2
// 段为空
#define ERR_SECTION_EMPTY 3
// 段名已经存在
#define ERR_SECTION_ALREADY_EXISTS 4
// 解析key value失败
#define ERR_PARSE_KEY_VALUE_FAILED 5
// 打开文件失败
#define ERR_OPEN_FILE_FAILED 6
// 内存不足
#define ERR_NO_ENOUGH_MEMORY 7
// 没有找到对应的key
#define ERR_NOT_FOUND_KEY 8
// 没有找到对应的section
#define ERR_NOT_FOUND_SECTION 9


const char delim[] = "\n";
struct IniItem {
    _tstring key;
	_tstring value;
	_tstring comment;  // 每个键的注释，都是指该行上方的内容
	_tstring rightComment;
};

struct IniSection {
    typedef std::vector<IniItem>::iterator IniItem_it;  // 定义一个迭代器，即指向键元素的指针
    IniItem_it begin() {
        return items.begin();  // 段结构体的begin元素指向items的头指针
    }

    IniItem_it end() {
        return items.end();  // 段结构体的begin元素指向items的尾指针
    }

	_tstring name;
	_tstring comment;  // 每个段的注释，都是指该行上方的内容
	_tstring rightComment;
    std::vector<IniItem> items;  // 键值项数组，一个段可以有多个键值，所有用vector来储存
};

class IniFile
{
 public:
    IniFile();
    ~IniFile() {
        release();
    }

 public:
    /* 打开并解析一个名为fname的INI文件 */
    int Load(const _tstring &fname);
    /* 将内容保存到当前文件 */
    int Save();
    /* 将内容另存到一个名为fname的文件 */
    int SaveAs(const _tstring &fname);

    /* 获取section段第一个键为key的string值，成功返回0，否则返回错误码 */
    int GetStringValue(const _tstring &section, const _tstring &key, string *value);
    /* 获取section段第一个键为key的int值，成功返回0，否则返回错误码 */
    int GetIntValue(const _tstring &section, const _tstring &key, int *value);
    /* 获取section段第一个键为key的double值，成功返回0，否则返回错误码 */
    int GetDoubleValue(const _tstring &section, const _tstring &key, double *value);
    /* 获取section段第一个键为key的bool值，成功返回0，否则返回错误码 */
    int GetBoolValue(const _tstring &section, const _tstring &key, bool *value);
    /* 获取注释，如果key=""则获取段注释 */
    int GetComment(const _tstring &section, const _tstring &key, _tstring *comment);
    /* 获取行尾注释，如果key=""则获取段的行尾注释 */
    int GetRightComment(const _tstring &section, const _tstring &key, _tstring *rightComment);

    /* 获取section段第一个键为key的string值，成功返回获取的值，否则返回默认值 */
    void GetStringValueOrDefault(const _tstring &section, const _tstring &key, _tstring *value, const _tstring &defaultValue);
    /* 获取section段第一个键为key的int值，成功返回获取的值，否则返回默认值 */
    void GetIntValueOrDefault(const _tstring &section, const _tstring &key, int *value, int defaultValue);
    /* 获取section段第一个键为key的double值，成功返回获取的值，否则返回默认值 */
    void GetDoubleValueOrDefault(const _tstring &section, const _tstring &key, double *value, double defaultValue);
    /* 获取section段第一个键为key的bool值，成功返回获取的值，否则返回默认值 */
    void GetBoolValueOrDefault(const _tstring &section, const _tstring &key, bool *value, bool defaultValue);

    /* 获取section段所有键为key的值,并将值赋到values的vector中 */
    int GetValues(const _tstring &section, const _tstring &key, std::vector<_tstring> *values);

    /* 获取section列表,并返回section个数 */
    int GetSections(std::vector<_tstring> *sections);
    /* 获取section个数，至少存在一个空section */
    int GetSectionNum();
    /* 是否存在某个section */
    bool HasSection(const _tstring &section);
    /* 获取指定section的所有ken=value信息 */
    IniSection *getSection(const _tstring &section = "");
    
    /* 是否存在某个key */
    bool HasKey(const _tstring &section, const _tstring &key);

    /* 设置字符串值 */
    int SetStringValue(const _tstring &section, const _tstring &key, const _tstring &value);
    /* 设置整形值 */
    int SetIntValue(const _tstring &section, const _tstring &key, int value);
    /* 设置浮点数值 */
    int SetDoubleValue(const _tstring &section, const _tstring &key, double value);
    /* 设置布尔值 */
    int SetBoolValue(const _tstring &section, const _tstring &key, bool value);
    /* 设置注释，如果key=""则设置段注释 */
    int SetComment(const _tstring &section, const _tstring &key, const _tstring &comment);
    /* 设置行尾注释，如果key=""则设置段的行尾注释 */
    int SetRightComment(const _tstring &section, const _tstring &key, const _tstring &rightComment);

    /* 删除段 */
    void DeleteSection(const _tstring &section);
    /* 删除特定段的特定参数 */
    void DeleteKey(const _tstring &section, const _tstring &key);
    /*设置注释分隔符，默认为"#" */
    void SetCommentDelimiter(const _tstring &delimiter);

    const _tstring &GetErrMsg();
 private:
    /* 获取section段所有键为key的值,并将值赋到values的vector中,,将注释赋到comments的vector中 */
    int GetValues(const _tstring &section, const _tstring &key, std::vector<_tstring> *value, std::vector<_tstring> *comments);

    /* 同时设置值和注释 */
    int setValue(const _tstring &section, const _tstring &key, const _tstring &value, const _tstring &comment = "");

    /* 去掉str前面的c字符 */
    static void trimleft(_tstring &str, char c = ' ');
    /* 去掉str后面的c字符 */
    static void trimright(_tstring &str, char c = ' ');
    /* 去掉str前面和后面的空格符,Tab符等空白符 */
    static void trim(_tstring &str);
    /* 将字符串str按分割符delim分割成多个子串 */
 private:
    int UpdateSection(const _tstring &cleanLine, const _tstring &comment,
                      const _tstring &rightComment, IniSection **section);
    int AddKeyValuePair(const _tstring &cleanLine, const _tstring &comment,
                        const _tstring &rightComment, IniSection *section);

    void release();
    bool split(const _tstring &str, const _tstring &sep, _tstring *left, _tstring *right);
    bool IsCommentLine(const _tstring &str);
    bool parse(const _tstring &content, _tstring *key, _tstring *value);
    // for debug
    void print();

 private:
    /* 获取section段第一个键为key的值,并将值赋到value中 */
    int getValue(const _tstring &section, const _tstring &key, _tstring *value);
    /* 获取section段第一个键为key的值,并将值赋到value中,将注释赋到comment中 */
    int getValue(const _tstring &section, const _tstring &key, _tstring *value, _tstring *comment);

    bool StringCmpIgnoreCase(const _tstring &str1, const _tstring &str2);
    bool StartWith(const _tstring &str, const _tstring &prefix);

 private:
    typedef std::vector<IniSection*>::iterator IniSection_it;
    std::vector<IniSection*> sections_vt;  // 用于存储段集合的vector容器
	_tstring iniFilePath;
	_tstring commentDelimiter;
	_tstring errMsg;  // 保存出错时的错误信息
};



