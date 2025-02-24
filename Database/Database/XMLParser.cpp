#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "XMLParser.h"
#include "FileUtils.h"

bool XmlNode::GetBoolAttr(const WCHAR *key, bool defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    // Wide 문자열 2개 대소문자 구분없이 비교
    if (attr)
        return ::_wcsicmp(attr->value(), L"true") == 0;

    return defaultValue;
}

INT8 XmlNode::GetInt8Attr(const WCHAR *key, INT8 defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    if (attr)
        return static_cast<INT8>(::_wtoi(attr->value()));

    return defaultValue;
}

SHORT XmlNode::GetInt16Attr(const WCHAR *key, SHORT defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    if (attr)
        return static_cast<SHORT>(::_wtoi(attr->value()));

    return defaultValue;
}

INT XmlNode::GetInt32Attr(const WCHAR *key, INT defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    if (attr)
        return static_cast<INT>(::_wtoi(attr->value()));

    return defaultValue;
}

INT64 XmlNode::GetInt64Attr(const WCHAR *key, INT64 defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    if (attr)
        return static_cast<INT64>(::_wtoi64(attr->value()));

    return defaultValue;
}

float XmlNode::GetFloatAttr(const WCHAR *key, float defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    if (attr)
        return static_cast<float>(::_wtof(attr->value()));

    return defaultValue;
}

double XmlNode::GetDoubleAttr(const WCHAR *key, double defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    // 필요하다면 _wtof_l, locale값 주는 걸로 사용해도 된다.
    if (attr)
        return ::_wtof(attr->value());

    return defaultValue;
}

const WCHAR *XmlNode::GetStringAttr(const WCHAR *key, const WCHAR *defaultValue)
{
    XmlAttrType *attr = _node->first_attribute(key);
    if (attr)
        return attr->value();

    return defaultValue;
}

bool XmlNode::GetBoolValue(bool defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return _wcsicmp(value, L"true") == 0;

    return defaultValue;
}

INT8 XmlNode::GetInt8Value(INT8 defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return static_cast<INT8>(_wtoi(value));

    return defaultValue;
}

SHORT XmlNode::GetInt16Value(SHORT defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return static_cast<INT16>(_wtoi(value));

    return defaultValue;
}

INT XmlNode::GetInt32Value(INT defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return _wtoi(value);

    return defaultValue;
}

INT64 XmlNode::GetInt64Value(INT64 defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return static_cast<INT64>(_wtoi64(value));

    return defaultValue;
}

float XmlNode::GetFloatValue(float defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return static_cast<float>(_wtof(value));

    return defaultValue;
}

double XmlNode::GetDoubleValue(double defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return _wtof(value);

    return defaultValue;
}

const WCHAR *XmlNode::GetStringValue(const WCHAR *defaultValue)
{
    WCHAR *value = _node->value();
    if (value != nullptr)
        return value;

    return defaultValue;
}

XmlNode XmlNode::FindNode(const WCHAR *key)
{
    return XmlNode(_node->first_node(key));
}

vector<XmlNode> XmlNode::FindNodeList(const WCHAR *key)
{
    vector<XmlNode> ret;

    XmlNodeType *node = _node->first_node(key);
    while (node)
    {
        ret.push_back(XmlNode(node));
        node = node->next_sibling(key);
    }

    return ret;
}

/*
    XmlParser
*/
bool XmlParser::ParserFromFile(const WCHAR *path, XmlNode &root)
{
    vector<BYTE> file = FileUtils::ReadFile(path);
    if (file.size() == 0)
        return false;

    _data = FileUtils::Convert(string(file.begin(), file.end()));
    if (_data.empty())
        return false;

    _document = make_shared<XmlDocType>();
    _document->parse<0>(&_data[0]);
    root = XmlNode(_document->first_node());
    return true;
}
