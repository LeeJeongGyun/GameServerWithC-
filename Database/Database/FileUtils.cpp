#include <Windows.h>
#include <string>
#include <vector>
#include<filesystem>
#include<fstream>
#include "FileUtils.h"

namespace fs = std::filesystem;

vector<BYTE> FileUtils::ReadFile(const WCHAR *path)
{
    fs::path filePath(path);
    int fileSize = static_cast<int>(fs::file_size(filePath));

    vector<BYTE> retFile(fileSize);
    basic_ifstream<BYTE> inputStream(filePath);
    inputStream.read(&retFile[0], fileSize);

    return retFile;
}

std::wstring FileUtils::Convert(string str)
{
    const int srcLen = static_cast<int>(str.size());
    if (srcLen == 0)
        return wstring();

    const int retlen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), srcLen, NULL, 0);
    wstring ret;

    ret.resize(retlen);
    ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), srcLen, &ret[0], retlen);
    return ret;
}