#pragma once
#include <string>

namespace WinAPI {
    bool DownloadFile(const std::string& url, const std::string& savePath);
    bool UnzipFile(const std::string& zipPath, const std::string& extractPath);
    bool DeleteFile(const std::string& path);
}