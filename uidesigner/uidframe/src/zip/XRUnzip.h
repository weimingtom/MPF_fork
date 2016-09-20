
#ifndef _UIXRUNZIP_H_
#define _UIXRUNZIP_H_

#include "xunzip.h"
#include <System/Types/StringCore.h>

class XRUnzip
{
public:

    XRUnzip()
        : _hzip(0)
    {
    }

    ~XRUnzip()
    {
        Close();
    }

    HZIP GetHZip() const
    {
        return _hzip;
    }

    bool OpenFromFile(TCHAR* lpszName)
    {
        Close();
        _hzip = OpenZip((void*)lpszName, 0, ZIP_FILENAME);
        return (NULL != _hzip);
    }

    bool OpenFromMemory(void* data, int len)
    {
        Close();
        _hzip = OpenZip(data, len, ZIP_MEMORY);
        return (NULL != _hzip);
    }

    void Close()
    {
        if (NULL != _hzip)
        {
            CloseZip(_hzip);
            _hzip = NULL;
        }
    }

    char* GetZipItem(TCHAR* lpszName, int& iSize)
    {
        ZIPENTRYW ze; 
        int i = -1;
        iSize = 0;

        FindZipItem(_hzip, lpszName, true, &i, &ze);

        if (i == -1)
        {
            return NULL;
        }
        else
        {
            iSize = ze.unc_size;
            char *ibuf = new char[ze.unc_size];
            UnzipItem(_hzip, i, ibuf, ze.unc_size, ZIP_MEMORY);

            return ibuf;
        }
    }

	int GetZipItemData(int index, suic::Mulstr& data, suic::String& name)
    {
        ZIPENTRYW ze; 
        int iSize = 0;
        data = "";
		
        ZRESULT zipRes = ::GetZipItemW(_hzip, index, &ze);
		
		if (zipRes == ZR_OK)
		{
			iSize = ze.unc_size;
            name = ze.name;

            if (iSize > 0)
            {
                data.Resize(iSize);
			    UnzipItem(_hzip, index, data.c_str(), ze.unc_size, ZIP_MEMORY);
            }
		}
        else
        {
            iSize =  -1;
        }
		
		return iSize;
    }
	
    int GetZipItemData(suic::Char* lpszName, suic::Mulstr& data)
    {
        ZIPENTRYW ze; 
        int i = -1;
        int iSize = 0;
        data = "";

        FindZipItem(_hzip, lpszName, true, &i, &ze);

        if (i == -1)
        {
            return iSize;
        }
        else
        {
            iSize = ze.unc_size;
            data.Resize(iSize);
            UnzipItem(_hzip, i, data.c_str(), ze.unc_size, ZIP_MEMORY);

            return iSize;
        }
    }

protected:

    HZIP _hzip;
};

#endif
