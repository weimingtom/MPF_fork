/////////////////////////////////////////////////////
// VSManager.cpp

#include "stdafx.h"
#include "VSManager.h"

#include <tools/Utils.h>
#include <zip/XRUnzip.h>

static suic::Mulstr VSTEMPLATENAME = "MPFTemplate";

void VSManager::UnzipTo(const suic::Mulstr& data, const suic::String& strDir, const suic::String& name)
{
	XRUnzip xrZip;
    suic::Byte* prjData = new suic::Byte[data.Length()];
    memcpy(prjData, data.c_str(), data.Length());

	if (xrZip.OpenFromMemory(prjData, data.Length()))
	{
        int index = 0;

		for (;;)
		{
			suic::String shortFile;
			suic::Mulstr unzipData;
			int iSize = xrZip.GetZipItemData(index, unzipData, shortFile);
			if (iSize < 0)
			{
				break;
			}

            FileWriter fs;
			suic::String strPath;
			
			if (!name.Empty())
			{
                shortFile.Replace(VSTEMPLATENAME.c_str(), name);
                strPath.Format(_U("%s\\%s\\%s"), strDir.c_str(), name.c_str(), shortFile.c_str());
			}
            else
            {
                strPath.Format(_U("%s\\%s"), strDir.c_str(), shortFile.c_str());
            }
            
            if (0 == iSize)
            {
                strPath += _U("\\");
            }

            FileDir::DupCreateDir(strPath);
	
			if (iSize > 0 && fs.Open(strPath))
			{
                unzipData.Replace("\r", "");
				fs.WriteAscii(unzipData.c_str());
				fs.Close();
			}

            index++;
		}
	}
}

bool VSManager::CreateVSProject(const suic::String& strVer, const suic::String& name, const suic::String& strDir)
{
	// 获取对应的VS工程数据
	suic::Mulstr data;
    suic::String strPath;
    FileReader fReader;

    strPath = FileDir::CalculatePath(String().Format(_U("resource\\uidesign\\VSTemplate\\%s\\trunk.zip"), strVer.c_str()));

    Utils::ReadResFromFile(suic::Mulstr(strPath.c_str()).c_str(), "rb", data);
	// 解压到指定目录
	UnzipTo(data, strDir, name);
	
	// 用name替换模板的工程项目名称
	ReplacePrjName(name, strDir + name);

    strPath = FileDir::CalculatePath(String().Format(_U("resource\\uidesign\\VSTemplate\\common\\common.zip")));
    Utils::ReadResFromFile(suic::Mulstr(strPath.c_str()).c_str(), "rb", data);
	// 解压到指定目录
	UnzipTo(data, strDir, name);

    return true;
}

void VSManager::ReplacePrjName(const suic::String& name, const suic::String& strDir)
{
	FileFinder fileFinder;
    if (!fileFinder.Open(strDir, _U("*.*")))
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

            suic::String strPath = fileFinder.GetFilePath();

            // 目录
            if (fileFinder.IsDir())
            {
                ReplacePrjName(name, strPath);
			}
            else
            {
                FileWriter fWriter;
                suic::Mulstr data;

                Utils::ReadResFromFile(suic::Mulstr(strPath.c_str()).c_str(), "rb", data);
                FileDir::RemoveFile(strPath);

                data.Replace(VSTEMPLATENAME.c_str(), name.c_str());

                fWriter.Open(strPath);
                fWriter.WriteAscii(data);
            }
		}
        else
        {
            break;
        }
	}
}
