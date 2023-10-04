
#include "../Help/WindowsWin.h"

#include <ShlObj_core.h>
#include <Windows.h>
#include <shobjidl.h> 
#include <algorithm>

std::string help::SelectFile() {
    HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(result))
        return std::string();

    //...
    IFileOpenDialog* fileSystem;
    result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileSystem));
    if (FAILED(result)) {
        CoUninitialize();
        return std::string();
    }

    //...
    result = fileSystem->Show(NULL);
    if (FAILED(result)) {
        fileSystem->Release();
        CoUninitialize();
        return std::string();
    }

    //...
    IShellItem* files;
    result = fileSystem->GetResult(&files);
    if (FAILED(result)) {
        fileSystem->Release();
        CoUninitialize();
        return std::string();
    }

    //...
    PWSTR filePath;
    result = files->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
    if (FAILED(result)) {
        files->Release();
        fileSystem->Release();
        CoUninitialize();
        return std::string();
    }

    //...
    std::wstring path(filePath);
    std::string sFilePath(path.begin(), path.end());

    //...
    CoTaskMemFree(filePath);
    files->Release();
    fileSystem->Release();
    CoUninitialize();

    return sFilePath;
}
