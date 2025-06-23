#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <comdef.h>
#include "compat.hpp"

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ole32.lib")
namespace WinAPI {
    bool DownloadFile(const std::string& url, const std::string& savePath) {
        HINTERNET hInternet = InternetOpenA("SkyInstaller", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return false;

        HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hUrl) {
            InternetCloseHandle(hInternet);
            return false;
        }

        std::string dir = savePath.substr(0, savePath.find_last_of("\\/"));
        CreateDirectoryA(dir.c_str(), NULL);

        HANDLE hFile = CreateFileA(savePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            InternetCloseHandle(hUrl);
            InternetCloseHandle(hInternet);
            return false;
        }

        DWORD bytesRead;
        BYTE buffer[1024];
        BOOL result = TRUE;
        DWORD totalRead = 0;
        
        while (result) {
            result = InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead);
            if (bytesRead == 0) break;
            
            DWORD bytesWritten;
            WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
            totalRead += bytesRead;
        }

        CloseHandle(hFile);
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        
        return result;
    }

    bool UnzipFile(const std::string& zipPath, const std::string& extractPath) {
        IShellDispatch* pShell = NULL;
        Folder* pZipFolder = NULL;
        Folder* pDestFolder = NULL;
        
        CoInitialize(NULL);
        
        HRESULT hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void**)&pShell);
        if (FAILED(hr)) return false;
        
        _bstr_t wZipPath(zipPath.c_str());
        _bstr_t wExtractPath(extractPath.c_str());
        
        VARIANT vZipFile;
        VariantInit(&vZipFile);
        vZipFile.vt = VT_BSTR;
        vZipFile.bstrVal = wZipPath;
        
        VARIANT vDestDir;
        VariantInit(&vDestDir);
        vDestDir.vt = VT_BSTR;
        vDestDir.bstrVal = wExtractPath;
        
        hr = pShell->NameSpace(vZipFile, &pZipFolder);
        if (FAILED(hr) || !pZipFolder) {
            pShell->Release();
            return false;
        }
        
        hr = pShell->NameSpace(vDestDir, &pDestFolder);
        if (FAILED(hr) || !pDestFolder) {
            pZipFolder->Release();
            pShell->Release();
            return false;
        }
        
        FolderItems* pItems = NULL;
        pZipFolder->Items(&pItems);
        
        VARIANT vItems;
        VariantInit(&vItems);
        vItems.vt = VT_DISPATCH;
        vItems.pdispVal = pItems;
        
        VARIANT vOptions;
        VariantInit(&vOptions);
        vOptions.vt = VT_I4;
        vOptions.lVal = 0;
        
        hr = pDestFolder->CopyHere(vItems, vOptions);
        
        pItems->Release();
        pDestFolder->Release();
        pZipFolder->Release();
        pShell->Release();
        CoUninitialize();
        
        return SUCCEEDED(hr);
    }
    
    bool DeleteFile(const std::string& path) {
        return DeleteFileA(path.c_str());
    }
}