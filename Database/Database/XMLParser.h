#pragma once
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

using XmlNodeType = xml_node<WCHAR>;
using XmlDocType = xml_document<WCHAR>;
using XmlAttrType = xml_attribute<WCHAR>;

class XmlNode
{
public:
    XmlNode(XmlNodeType *node = nullptr) : _node(node)
    {
    }

    bool IsValid()
    {
        return _node != nullptr;
    }

    bool GetBoolAttr(const WCHAR *key, bool defaultValue = false);
    INT8 GetInt8Attr(const WCHAR *key, INT8 defaultValue = 0);
    SHORT GetInt16Attr(const WCHAR *key, SHORT defaultValue = 0);
    INT GetInt32Attr(const WCHAR *key, INT defaultValue = 0);
    INT64 GetInt64Attr(const WCHAR *key, INT64 defaultValue = 0);
    float GetFloatAttr(const WCHAR *key, float defaultValue = 0);
    double GetDoubleAttr(const WCHAR *key, double defaultValue = 0);
    const WCHAR *GetStringAttr(const WCHAR *key, const WCHAR *defaultValue = L"");

    bool GetBoolValue(bool defaultValue = false);
    INT8 GetInt8Value(INT8 defaultValue = 0);
    SHORT GetInt16Value(SHORT defaultValue = 0);
    INT GetInt32Value(INT defaultValue = 0);
    INT64 GetInt64Value(INT64 defaultValue = 0);
    float GetFloatValue(float defaultValue = 0.0f);
    double GetDoubleValue(double defaultValue = 0.0);
    const WCHAR *GetStringValue(const WCHAR *defaultValue = L"");

    XmlNode FindNode(const WCHAR *key);
    vector<XmlNode> FindNodeList(const WCHAR *key);

private:
    XmlNodeType *_node = nullptr;
};

/*
    XmlParser
*/
class XmlParser
{
public:
    bool ParserFromFile(const WCHAR *path, XmlNode &root);

private:
    shared_ptr<XmlDocType> _document;
    wstring _data;
};
