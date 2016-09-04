
#include "stdafx.h"

#include <time.h>
#include <locale>

#include <main/Encrypt.h>
#include <tools/Utils.h>
#include <Core/ResNode.h>
#include <Core/FileFinder.h>

time_t Utils::_startRunTime = time(NULL);

bool Utils::CheckUseTimeout(const String& info)
{
    return false;
    const time_t cst_timeout = 20 * 60;
    time_t curTime = time(NULL);

    if (curTime - _startRunTime > cst_timeout)
    {
        if (!info.Empty())
        {
            InfoBox::Show(info, _U("��ʾ"));
        }
        return true;
    }
    else
    {
        return false;
    }
}

static FILE* __TiFOpen(const char* filename, const char* mode)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    FILE* fp = 0;
    errno_t err = fopen_s(&fp, filename, mode);

    if (!err && fp)
    {
        return fp;
    }
    else
    {
        return 0;
    }
#else
    return fopen(filename, mode);
#endif
}

int Utils::ReadResFromFile(const char* filename, const char* mode, Mulstr& buff)
{
    FILE* f = __TiFOpen(filename, mode);

    if (f)
    {
        long length = 0;
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buff.Resize(length);

        if (fread(buff.c_str(), 1, length, f) != 1) 
        {
        }
        else
        {
            ;
        }

        fclose(f);
        return length;
    }
    else
    {
        return 0;
    }
}

String Utils::CheckUIXmlRoot(const String& strPath)
{
    String strName;
    XmlDoc xmlDoc;
    XmlNode* root = NULL;
    suic::Mulstr filename(strPath.c_str());
    suic::Mulstr buff;

    ReadResFromFile(filename.c_str(), "rb", buff);

    try
    {
        xmlDoc.parse<0>(buff.c_str());
        root = xmlDoc.first_node();

        if (NULL != root)
        {
            strName = root->name();
        }
    }
    catch (...)
    {
    }

    return strName;
}

bool Utils::ZipFolder(const String& strZip, const String& strPath, const String& zipExt, const String& pwd)
{
    XRZip xrZip;
    if (xrZip.OpenFromFile((TCHAR*)(strZip.c_str())))
    {
        ZipFileFolder(&xrZip, strPath, strPath, zipExt);
        xrZip.Close();

        // ����
        if (!pwd.Empty())
        {
            UIEncrypt::Encrypt(pwd, strZip);
        }

        return true;
    }
    else
    {
        return false;
    }
}

void Utils::ZipFileFolder(XRZip* pZip, const String& strRoot, const String& strPath, const String& zipExt)
{
    FileFinder fileFinder;

    if (!fileFinder.Open(strPath, _U("*.*")))
    {
        return;
    }

    bool bSearch = true;
    while (bSearch)
    {
        if (fileFinder.FindNext())
        {
            if (fileFinder.IsDot())
            {
                continue;
            }

            // Ŀ¼
            if (fileFinder.IsDir())
            {
                String strSubPath;
                String strFilename(fileFinder.GetFileName());
                strFilename.Trim();

                if (strFilename.IndexOf(_U(".")) == -1)
                {
                    strSubPath.Format(_U("%s%s\\"), strPath.c_str(), strFilename.c_str());
                    ZipFileFolder(pZip, strRoot, strSubPath, zipExt);
                }
            }
            else
            {
                String strFilePath;
                String strSection;
                String strFilename(fileFinder.GetFileName());
                strFilename.Trim();

                int iPos = strFilename.IndexOf(_U("."));
                if (iPos != -1)
                {
                    String strExt = strFilename.Substring(iPos);
                    strExt.ToLower();

                    if (zipExt.IndexOf(strExt) != -1)
                    {
                        strFilePath.Format(_U("%s%s"), strPath.c_str(), strFilename.c_str());
                        strSection = strFilePath.Substring(strRoot.Length());

                        strSection.Replace(_U("\\"), _U("/"));

                        pZip->AddFile((TCHAR*)(strSection.c_str()), (TCHAR*)(strFilePath.c_str()));
                    }
                }
            }
        }
        else
        {
            if (GetLastError() == ERROR_NO_MORE_FILES)
            {
                bSearch = false;
            }
            else 
            {
                return;
            }
        }
    }
}

void Utils::RemoveFiles(const String& strDir, bool bRemoveDir)
{
    InternalRemoveFiles(strDir, bRemoveDir);
    if (bRemoveDir)
    {
        ::RemoveDirectory(strDir.c_str());
    }
}

void Utils::InternalRemoveFiles(const String& strDir, bool bRemoveSubDir)
{
    FileFinder fileFinder;
    bool bSearch = true;

    if (!fileFinder.Open(strDir, _U("*.*")))
    {
        return;
    }

    while (bSearch)
    {
        if (fileFinder.FindNext())
        {
            if (fileFinder.IsDot())
            {
                continue;
            }


            // Ŀ¼
            if (fileFinder.IsDir())
            {
                String strCurPath;
                strCurPath.Format(_U("%s\\%s\\"), strDir.c_str(), fileFinder.GetFileName().c_str());

                // ��ȡ��Ŀ¼
                if (bRemoveSubDir)
                {
                    InternalRemoveFiles(strCurPath, bRemoveSubDir);
                    ::RemoveDirectory(strCurPath.c_str());
                }
            }
            else
            {
                String strFilename(fileFinder.GetFilePath());
                ::DeleteFile(strFilename.c_str());
            }
        }
        else
        {
            if (GetLastError() == ERROR_NO_MORE_FILES)
            {
                bSearch = false;
            }
            else 
            {
                return;
            }
        }
    }
}

/*
// ����ļ��������

// strExt: Ҫ������չ��(����: ".txt")

// strAppKey: ExeName��չ����ע����еļ�ֵ(����: "txtfile")

// ����TRUE: ��ʾ�ѹ�����FALSE: ��ʾδ����

BOOL CheckFileRelation(const char *strExt, const char *strAppKey)

{

    int nRet=FALSE;

    HKEY hExtKey;

    char szPath[_MAX_PATH];

    DWORD dwSize=sizeof(szPath);

    if(RegOpenKey(HKEY_CLASSES_ROOT,strExt,&hExtKey)==ERROR_SUCCESS)

    {

        RegQueryValueEx(hExtKey,NULL,NULL,NULL,(LPBYTE)szPath,&dwSize);

        if(_stricmp(szPath,strAppKey)==0)

        {

            nRet=TRUE;

        }

        RegCloseKey(hExtKey);

        return nRet;

    }

    return nRet;

}

//---------------------------------------------------------------------------

// ע���ļ�����

// strExe: Ҫ������չ��(����: ".txt")

// strAppName: Ҫ������Ӧ�ó�����(����: "C:\MyApp\MyApp.exe")

// strAppKey: ExeName��չ����ע����еļ�ֵ(����: "txtfile")

// strDefaultIcon: ��չ��ΪstrAppName��ͼ���ļ�(����: "C:\MyApp\MyApp.exe,0")

// strDescribe: �ļ���������

void RegisterFileRelation(char *strExt, char *strAppName, char *strAppKey, char *strDefaultIcon, char *strDescribe)

{

    char strTemp[_MAX_PATH];

    HKEY hKey;

   

    RegCreateKey(HKEY_CLASSES_ROOT,strExt,&hKey);

    RegSetValue(hKey,"",REG_SZ,strAppKey,strlen(strAppKey)+1);

    RegCloseKey(hKey);   

    RegCreateKey(HKEY_CLASSES_ROOT,strAppKey,&hKey);

    RegSetValue(hKey,"",REG_SZ,strDescribe,strlen(strDescribe)+1);

    RegCloseKey(hKey);

    sprintf(strTemp,"%s\\DefaultIcon",strAppKey);

    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);

    RegSetValue(hKey,"",REG_SZ,strDefaultIcon,strlen(strDefaultIcon)+1);

    RegCloseKey(hKey);

    sprintf(strTemp,"%s\\Shell",strAppKey);

    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);

    RegSetValue(hKey,"",REG_SZ,"Open",strlen("Open")+1);

    RegCloseKey(hKey);

    sprintf(strTemp,"%s\\Shell\\Open\\Command",strAppKey);

    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);

    sprintf(strTemp,"%s \"%%1\"",strAppName);

    RegSetValue(hKey,"",REG_SZ,strTemp,strlen(strTemp)+1);

    RegCloseKey(hKey);

}

 

//ʹ���������������й�����ʾ������

char strExt[10] = ".REC";

char strAppKey[20] = "WellTest.REC.1.0";         

BOOL relationExists = CheckFileRelation(strExt, strAppKey);

if(!relationExists)

{
����char strAppName[MAX_PATH + 1] = "C:\\WellTest\\trunk\\bin\\WellTest.exe";                        

    char strDefaultIcon[MAX_PATH + 1] = "C:\\WellTest\\trunk\\bin\\WellTest.exe,0";                                     

    char strDescribe[100] = "WellTest Interpretation Files";

    RegisterFileRelation(strExt, strAppName, strAppKey, strDefaultIcon, strDescribe);

}


void WatchDirectory(LPTSTR lpDir)  
{  
   DWORD dwWaitStatus;  
   HANDLE dwChangeHandles[3];  
   TCHAR lpDrive[4];//�洢���̷�  
   TCHAR lpFile[_MAX_FNAME];//���ڴ洢�ļ���  
   TCHAR lpExt[_MAX_EXT];//���ڴ洢��Ӧ�ļ��ĺ�׺  
   
   _tsplitpath(lpDir, lpDrive,NULL, lpFile, lpExt);  
   lpDrive[2] = (TCHAR)'\\';  
   lpDrive[3] = (TCHAR)'\0';  
     
   //�ֱ����ļ�����·�������ļ����ݵ��޸�   
   dwChangeHandles[0] =FindFirstChangeNotification(   
      lpDir,                         
      TRUE,                         
     FILE_NOTIFY_CHANGE_FILE_NAME); //�ļ���     
   if (dwChangeHandles[0] ==INVALID_HANDLE_VALUE)   
         ExitProcess(GetLastError());   
     
   dwChangeHandles[1] =FindFirstChangeNotification(   
      lpDrive,                     
      TRUE,  
     FILE_NOTIFY_CHANGE_DIR_NAME); //·����    
   if (dwChangeHandles[1] ==INVALID_HANDLE_VALUE)   
    ExitProcess(GetLastError());  
   
   dwChangeHandles[2] =FindFirstChangeNotification(   
      lpDir,  
      TRUE,  
     FILE_NOTIFY_CHANGE_LAST_WRITE); //�ļ�����/����˵��󱣴�ʱ��    
   if (dwChangeHandles[2] ==INVALID_HANDLE_VALUE)   
                   ExitProcess(GetLastError());   
     
   //�ı�֪ͨ�Ѿ�������ɣ�����ֻ��ȴ���Щ֪ͨ��������Ȼ������Ӧ����  
   while (TRUE)   
   {   
         dwWaitStatus= WaitForMultipleObjects(3, dwChangeHandles, FALSE, INFINITE);   
         time(&ChangeTime);  
              
         switch(dwWaitStatus)  
         {  
         caseWAIT_OBJECT_0:   
                   //ִ��ĳ��Ӧ����  
                   RefreshDirectory(lpDir); //ĳ�����ӿ�  
                   if( FindNextChangeNotification(dwChangeHandles[0]) == FALSE )   
                            ExitProcess(GetLastError());   
                   break;  
         caseWAIT_OBJECT_0 + 1:  
                   //ִ��ĳ��Ӧ����  
                   RefreshTree(lpDrive);//ĳ�����ӿ�  
                   if(FindNextChangeNotification(dwChangeHandles[1]) == FALSE)   
                            ExitProcess(GetLastError());  
                   break;  
         caseWAIT_OBJECT_0 + 2:  
                   //ִ��ĳ��Ӧ����  
                   RefreshFile(lpDrive);//ĳ�����ӿ�  
                   if(FindNextChangeNotification(dwChangeHandles[2]) == FALSE)   
                            ExitProcess(GetLastError());  
                   break;       
         default:  
                   ExitProcess(GetLastError());  
         }  
   }  
}  
*/