
#include "stdafx.h"
#include "FileFinder.h"

FileFinder::FileFinder()
{
    _fileHandle = NULL;
}

FileFinder::~FileFinder()
{
    Close();
}

bool FileFinder::Open(const String& strDir, const String& strFilter)
{
    Close();

    _findDir = strDir;
    _filter = strFilter;

    TCHAR PathToSearchInto [MAX_PATH] = {0};

    _tcscpy(PathToSearchInto, strDir.c_str());
    _tcscat(PathToSearchInto, _T("\\"));
    _tcscat(PathToSearchInto, strFilter.c_str());

    _fileHandle = FindFirstFile(PathToSearchInto,&_findFileData);
    if (_fileHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool FileFinder::FindNext()
{
    if (NULL == _fileHandle)
    {
        return false;
    }
    else if (::FindNextFile(_fileHandle, &_findFileData))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool FileFinder::IsDot() const
{
    if (NULL == _fileHandle)
    {
        return false;
    }
    else if ((_tcscmp(_findFileData.cFileName, _T(".")) == 0) ||
        (_tcscmp(_findFileData.cFileName, _T("..")) == 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool FileFinder::IsDir() const
{
    if (NULL == _fileHandle)
    {
        return false;
    }
    else if ((_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        return true;
    }
    else
    {
        return false;
    }
}

String FileFinder::GetFileName() const
{
    if (NULL == _fileHandle)
    {
        return String();
    }
    else
    {
        return _findFileData.cFileName;
    }
}

String FileFinder::GetFilePath() const
{
    String strPath;

    if (NULL == _fileHandle)
    {
        return strPath;
    }
    else
    {
        strPath.Format(_U("%s\\%s"), _findDir.c_str(), _findFileData.cFileName);
    }

    return strPath;
}

String FileFinder::GetCurrentDir() const
{
    return _findDir;
}

void FileFinder::Close()
{
    if (NULL != _fileHandle)
    {
        FindClose(_fileHandle);
    }
}
