#include "stdafx.h"
#include "inifile.h"
#include <iostream>
#include <fstream>


// 构造函数，会初始化注释字符集合flags_（容器），目前只使用#和;作为注释前缀
IniFile::IniFile()
:commentDelimiter("#")
{
}

//解析一行数据，得到键值
/* --------------------------------------------------------------------------*/
/**
 * @brief   parse
 *
 * @param   content 数据源指针
 * @param   key     键名
 * @param   value   键值
 * @param   c       分隔符
 *
 * @return  bool
 */
/* ----------------------------------------------------------------------------*/
bool IniFile::parse(const _tstring &content, _tstring *key, _tstring *value)
{
    return split(content, "=", key, value);
}

int IniFile::UpdateSection(const _tstring &cleanLine, const _tstring &comment,
                           const _tstring &rightComment, IniSection **section)
{
    IniSection *newSection;
    // 查找右中括号
    size_t index = cleanLine.find_first_of(']');
    if (index == _tstring::npos) {
        errMsg = _tstring("no matched ] found");
        return ERR_UNMATCHED_BRACKETS;
    }

    int len = index - 1;
    // 若段名为空，继续下一行
    if (len <= 0) {
        errMsg = _tstring("section name is empty");
        return ERR_SECTION_EMPTY;
    }

    // 取段名
    _tstring s(cleanLine, 1, len);

    trim(s);

    //检查段是否已存在
    if (getSection(s) != NULL) {
        errMsg = _tstring("section ") + s + _tstring("already exist");
        return ERR_SECTION_ALREADY_EXISTS;
    }

    // 申请一个新段，由于map容器会自动排序，打乱原有顺序，因此改用std::vector存储（sections_vt）
    newSection = new IniSection();
    // 填充段名
    newSection->name = s;
    // 填充段开头的注释
    newSection->comment = comment;
    newSection->rightComment = rightComment;

    sections_vt.push_back(newSection);

    *section = newSection;

    return 0;
}

int IniFile::AddKeyValuePair(const _tstring &cleanLine, const _tstring &comment,
                             const _tstring &rightComment, IniSection *section)
{
    _tstring key, value;

    if (!parse(cleanLine, &key, &value)) {
        errMsg = _tstring("parse line failed:") + cleanLine;
        return ERR_PARSE_KEY_VALUE_FAILED;
    }

    IniItem item;
    item.key = key;
    item.value = value;
    item.comment = comment;
    item.rightComment = rightComment;

    section->items.push_back(item);

    return 0;
}

int IniFile::Load(const _tstring &filePath)
{
    int err;
    _tstring line;  // 带注释的行
    _tstring cleanLine;  // 去掉注释后的行
    _tstring comment;
    _tstring rightComment;
    IniSection *currSection = NULL;  // 初始化一个字段指针

    release();

    iniFilePath = filePath;
    std::ifstream ifs(iniFilePath);
    if (!ifs.is_open()) {
        errMsg = _tstring("open") +iniFilePath+ _tstring(" file failed");
        return ERR_OPEN_FILE_FAILED;
    }

    //增加默认段，即 无名段""
    currSection = new IniSection();
    currSection->name = "";
    sections_vt.push_back(currSection);

    // 每次读取一行内容到line
    while (std::getline(ifs, line)) {
        trim(line);

        // step 0，空行处理，如果长度为0，说明是空行，添加到comment，当作是注释的一部分
        if (line.length() <= 0) {
            comment += delim;
            continue;
        }

        // step 1
        // 如果行首不是注释，查找行尾是否存在注释
        // 如果该行以注释开头，添加到comment，跳过当前循环，continue
        if (IsCommentLine(line)) {
            comment += line + delim;
            continue;
        }

        // 如果行首不是注释，查找行尾是否存在注释，若存在，切割该行，将注释内容添加到rightComment
        split(line, commentDelimiter, &cleanLine, &rightComment);

        // step 2，判断line内容是否为段或键
        //段开头查找 [
        if (cleanLine[0] == '[') {
            err = UpdateSection(cleanLine, comment, rightComment, &currSection);
        } else {
            // 如果该行是键值，添加到section段的items容器
             err = AddKeyValuePair(cleanLine, comment, rightComment, currSection);
        }

        if (err != 0) {
            ifs.close();
            return err;
        }

        // comment清零
        comment = "";
        rightComment = "";
    }

    ifs.close();

    return 0;
}

int IniFile::Save()
{
    return SaveAs(iniFilePath);
}

int IniFile::SaveAs(const _tstring &filePath)
{
    _tstring data = "";

    /* 载入section数据 */
    for (IniSection_it sect = sections_vt.begin(); sect != sections_vt.end(); ++sect) {
        if ((*sect)->comment != "") {
            data += (*sect)->comment;
        }

        if ((*sect)->name != "") {
            data += _tstring("[") + (*sect)->name + _tstring("]");
            data += delim;
        }

        if ((*sect)->rightComment != "") {
            data += " " + commentDelimiter +(*sect)->rightComment;
        }

        /* 载入item数据 */
        for (IniSection::IniItem_it item = (*sect)->items.begin(); item != (*sect)->items.end(); ++item) {
            if (item->comment != "") {
                data += item->comment;
                if (data[data.length()-1] != '\n') {
                    data += delim;
                }
            }

            data += item->key + "=" + item->value;

            if (item->rightComment != "") {
                data += " " + commentDelimiter + item->rightComment;
            }

            if (data[data.length()-1] != '\n') {
                data += delim;
            }
        }
    }

    std::ofstream ofs(filePath);
    ofs << data;
    ofs.close();
    return 0;
}

IniSection *IniFile::getSection(const _tstring &section /*=""*/)
{
    for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
        if ((*it)->name == section) {
            return *it;
        }
    }

    return NULL;
}

int IniFile::GetSections(std::vector<_tstring> *sections)
{
    for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
        sections->push_back((*it)->name);
    }

    return sections->size();
}

int IniFile::GetSectionNum()
{
    return sections_vt.size();
}

int IniFile::Get_tstringValue(const _tstring &section, const _tstring &key, _tstring *value)
{
    return getValue(section, key, value);
}

int IniFile::GetIntValue(const _tstring &section, const _tstring &key, int *intValue)
{
    int err;
    _tstring strValue;

    err = getValue(section, key, &strValue);

    *intValue = atoi(strValue.c_str());

    return err;
}

int IniFile::GetDoubleValue(const _tstring &section, const _tstring &key, double *value)
{
    int err;
    _tstring strValue;

    err = getValue(section, key, &strValue);

    *value = atof(strValue.c_str());

    return err;
}

int IniFile::GetBoolValue(const _tstring &section, const _tstring &key, bool *value)
{
    int err;
    _tstring strValue;

    err = getValue(section, key, &strValue);

    if (_tstringCmpIgnoreCase(strValue, "true") || _tstringCmpIgnoreCase(strValue, "1")) {
        *value = true;
    } else if (_tstringCmpIgnoreCase(strValue, "false") || _tstringCmpIgnoreCase(strValue, "0")) {
        *value = false;
    }

    return err;
}

/* 获取section段第一个键为key的_tstring值，成功返回获取的值，否则返回默认值 */
void IniFile::Get_tstringValueOrDefault(const _tstring &section, const _tstring &key,
                                      _tstring *value, const _tstring &defaultValue)
{
    if (Get_tstringValue(section, key, value) != 0) {
        *value = defaultValue;
    }

    return;
}

/* 获取section段第一个键为key的int值，成功返回获取的值，否则返回默认值 */
void IniFile::GetIntValueOrDefault(const _tstring &section, const _tstring &key, int *value, int defaultValue)
{
    if (GetIntValue(section, key, value) != 0) {
        *value = defaultValue;
    }

    return;
}

/* 获取section段第一个键为key的double值，成功返回获取的值，否则返回默认值 */
void IniFile::GetDoubleValueOrDefault(const _tstring &section, const _tstring &key, double *value, double defaultValue)
{
    if (GetDoubleValue(section, key, value) != 0) {
        *value = defaultValue;
    }

    return;
}

/* 获取section段第一个键为key的bool值，成功返回获取的值，否则返回默认值 */
void IniFile::GetBoolValueOrDefault(const _tstring &section, const _tstring &key, bool *value, bool defaultValue)
{
    if (GetBoolValue(section, key, value) != 0) {
        *value = defaultValue;
    }

    return;
}

/* 获取注释，如果key=""则获取段注释 */
int IniFile::GetComment(const _tstring &section, const _tstring &key, _tstring *comment)
{
    IniSection *sect = getSection(section);

    if (sect == NULL) {
        errMsg = _tstring("not find the section ")+section;
        return ERR_NOT_FOUND_SECTION;
    }

    if (key == "") {
        *comment = sect->comment;
        return RET_OK;
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            *comment = it->comment;
            return RET_OK;
        }
    }

    errMsg = _tstring("not find the key ")+section;
    return ERR_NOT_FOUND_KEY;
}

/* 获取行尾注释，如果key=""则获取段的行尾注释 */
int IniFile::GetRightComment(const _tstring &section, const _tstring &key, _tstring *rightComment)
{
    IniSection *sect = getSection(section);

    if (sect == NULL) {
        errMsg = _tstring("not find the section ")+section;
        return ERR_NOT_FOUND_SECTION;
    }

    if (key == "") {
        *rightComment = sect->rightComment;
        return RET_OK;
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            *rightComment = it->rightComment;
            return RET_OK;
        }
    }

    errMsg = _tstring("not find the key ")+key;
    return ERR_NOT_FOUND_KEY;
}

int IniFile::getValue(const _tstring &section, const _tstring &key, _tstring *value)
{
    _tstring comment;
    return getValue(section, key, value, &comment);
}

int IniFile::getValue(const _tstring &section, const _tstring &key, _tstring *value, _tstring *comment)
{
    IniSection *sect = getSection(section);

    if (sect == NULL) {
        errMsg = _tstring("not find the section ")+section;
        return ERR_NOT_FOUND_SECTION;
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            *value = it->value;
            *comment = it->comment;
            return RET_OK;
        }
    }

    errMsg = _tstring("not find the key ")+key;
    return ERR_NOT_FOUND_KEY;
}

int IniFile::GetValues(const _tstring &section, const _tstring &key, std::vector<_tstring> *values)
{
    std::vector<_tstring> comments;
    return GetValues(section, key, values, &comments);
}

int IniFile::GetValues(const _tstring &section, const _tstring &key, std::vector<_tstring> *values, std::vector<_tstring> *comments)
{
    _tstring value, comment;

    values->clear();
    comments->clear();

    IniSection *sect = getSection(section);

    if (sect == NULL) {
        errMsg = _tstring("not find the section ")+section;
        return ERR_NOT_FOUND_SECTION;
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            value = it->value;
            comment = it->comment;

            values->push_back(value);
            comments->push_back(comment);
        }
    }

    if (values->size() == 0) {
        errMsg = _tstring("not find the key ")+key;
        return ERR_NOT_FOUND_KEY;
    }

    return RET_OK;
}

bool IniFile::HasSection(const _tstring &section)
{
    return (getSection(section) != NULL);
}

bool IniFile::HasKey(const _tstring &section, const _tstring &key)
{
    IniSection *sect = getSection(section);

    if (sect != NULL) {
        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                return true;
            }
        }
    }

    return false;
}

int IniFile::setValue(const _tstring &section, const _tstring &key, const _tstring &value, const _tstring &comment /*=""*/)
{
    IniSection *sect = getSection(section);

    _tstring comt = comment;

    if (comt != "") {
        comt = commentDelimiter + comt;
    }

    if (sect == NULL) {
        //如果段不存在，新建一个
        sect = new IniSection();

        if (sect == NULL) {
            errMsg = _tstring("no enough memory!");
            return ERR_NO_ENOUGH_MEMORY;
        }

        sect->name = section;
        if (sect->name == "") {
            // 确保空section在第一个
            sections_vt.insert(sections_vt.begin(), sect);
        } else {
            sections_vt.push_back(sect);
        }
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            it->value = value;
            it->comment = comt;
            return RET_OK;
        }
    }

    // not found key
    IniItem item;
    item.key = key;
    item.value = value;
    item.comment = comt;

    sect->items.push_back(item);

    return RET_OK;
}

int IniFile::Set_tstringValue(const _tstring &section, const _tstring &key, const _tstring &value)
{
    return setValue(section, key, value);
}

int IniFile::SetIntValue(const _tstring &section, const _tstring &key, int value)
{
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%d", value);
    return setValue(section, key, buf);
}

int IniFile::SetDoubleValue(const _tstring &section, const _tstring &key, double value)
{
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%f", value);
    return setValue(section, key, buf);
}

int IniFile::SetBoolValue(const _tstring &section, const _tstring &key, bool value)
{
    if (value) {
        return setValue(section, key, "true");
    } else {
        return setValue(section, key, "false");
    }
}

int IniFile::SetComment(const _tstring &section, const _tstring &key, const _tstring &comment)
{
    IniSection *sect = getSection(section);

    if (sect == NULL) {
        errMsg = _tstring("Not find the section ")+section;
        return ERR_NOT_FOUND_SECTION;
    }

    if (key == "") {
        sect->comment = comment;
        return RET_OK;
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            it->comment = comment;
            return RET_OK;
        }
    }

    errMsg = _tstring("not find the key ")+key;
    return ERR_NOT_FOUND_KEY;
}

int IniFile::SetRightComment(const _tstring &section, const _tstring &key, const _tstring &rightComment)
{
    IniSection *sect = getSection(section);

    if (sect == NULL) {
        errMsg = _tstring("Not find the section ")+section;
        return ERR_NOT_FOUND_SECTION;
    }

    if (key == "") {
        sect->rightComment = rightComment;
        return RET_OK;
    }

    for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
        if (it->key == key) {
            it->rightComment = rightComment;
            return RET_OK;
        }
    }

    errMsg = _tstring("not find the key ")+key;
    return ERR_NOT_FOUND_KEY;
}

void IniFile::SetCommentDelimiter(const _tstring &delimiter)
{
    commentDelimiter = delimiter;
}

void IniFile::DeleteSection(const _tstring &section)
{
    for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ) {
        if ((*it)->name == section) {
            delete *it;
            it = sections_vt.erase(it);
            break;
        } else {
            it++;
        }
    }
}

void IniFile::DeleteKey(const _tstring &section, const _tstring &key)
{
    IniSection *sect = getSection(section);

    if (sect != NULL) {
        for (IniSection::IniItem_it it = sect->begin(); it != sect->end();) {
            if (it->key == key) {
                it = sect->items.erase(it);
                break;
            } else {
                it++;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/
/**
  @brief    release: 释放全部资源，清空容器
  @param    none
  @return   none
 */
/*--------------------------------------------------------------------------*/
void IniFile::release()
{
    iniFilePath = "";

    for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
        delete (*it);  // 清除section
    }

    sections_vt.clear();
}

/*-------------------------------------------------------------------------*/
/**
  @brief    判断是否是注释
  @param    str 一个_tstring变量
  @return   如果是注释则为真
 */
/*--------------------------------------------------------------------------*/
bool IniFile::IsCommentLine(const _tstring &str)
{
    return StartWith(str, commentDelimiter);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    print for debug
  @param    none
  @return   none
 */
/*--------------------------------------------------------------------------*/
void IniFile::print()
{
    printf("############ print start ############\n");
    printf("filePath:[%s]\n", iniFilePath.c_str());
    printf("commentDelimiter:[%s]\n", commentDelimiter.c_str());

    for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
        printf("comment :[\n%s]\n", (*it)->comment.c_str());
        printf("section :\n[%s]\n", (*it)->name.c_str());
        if ((*it)->rightComment != "") {
            printf("rightComment:\n%s", (*it)->rightComment.c_str());
        }

        for (IniSection::IniItem_it i = (*it)->items.begin(); i != (*it)->items.end(); ++i) {
            printf("    comment :[\n%s]\n", i->comment.c_str());
            printf("    parm    :%s=%s\n", i->key.c_str(), i->value.c_str());
            if (i->rightComment != "") {
                printf("    rcomment:[\n%s]\n", i->rightComment.c_str());
            }
        }
    }

    printf("############ print end ############\n");
    return;
}

const _tstring & IniFile::GetErrMsg()
{
    return errMsg;
}

bool IniFile::StartWith(const _tstring &str, const _tstring &prefix)
{
    if (strncmp(str.c_str(), prefix.c_str(), prefix.size()) == 0) {
        return true;
    }

    return false;
}

void IniFile::trimleft(_tstring &str, char c /*=' '*/)
{
    int len = str.length();

    int i = 0;

    while (str[i] == c && str[i] != '\0') {
        i++;
    }

    if (i != 0) {
        str = _tstring(str, i, len - i);
    }
}

void IniFile::trimright(_tstring &str, char c /*=' '*/)
{
    int i = 0;
    int len = str.length();

    for (i = len - 1; i >= 0; --i) {
        if (str[i] != c) {
            break;
        }
    }

    str = _tstring(str, 0, i + 1);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    trim，整理一行字符串，去掉首尾空格
  @param    str _tstring变量
 */
/*--------------------------------------------------------------------------*/
void IniFile::trim(_tstring &str)
{
    int len = str.length();

    int i = 0;

    while ((i < len) && isspace(str[i]) && (str[i] != '\0')) {
        i++;
    }

    if (i != 0) {
        str = _tstring(str, i, len - i);
    }

    len = str.length();

    for (i = len - 1; i >= 0; --i) {
        if (!isspace(str[i])) {
            break;
        }
    }

    str = _tstring(str, 0, i + 1);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    split，用分隔符切割字符串
  @param    str 输入字符串
  @param    left_str 分隔后得到的左字符串
  @param    right_str 分隔后得到的右字符串（不包含seperator）
  @param    seperator 分隔符
  @return   pos
 */
/*--------------------------------------------------------------------------*/
bool IniFile::split(const _tstring &str, const _tstring &sep, _tstring *pleft, _tstring *pright)
{
    size_t pos = str.find(sep);
    _tstring left, right;

    if (pos != _tstring::npos) {
        left = _tstring(str, 0, pos);
        right = _tstring(str, pos+1);

        trim(left);
        trim(right);

        *pleft = left;
        *pright = right;
        return true;
    } else {
        left = str;
        right = "";

        trim(left);

        *pleft = left;
        *pright = right;
        return false;
    }
}

bool IniFile::_tstringCmpIgnoreCase(const _tstring &str1, const _tstring &str2)
{
    _tstring a = str1;
    _tstring b = str2;
    transform(a.begin(), a.end(), a.begin(), towupper);
    transform(b.begin(), b.end(), b.begin(), towupper);

    return (a == b);
}

