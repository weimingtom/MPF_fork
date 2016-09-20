/////////////////////////////////////////////////////
// VSManager.cpp

#include "stdafx.h"
#include "VSManager.h"

#include <zip/XRUnzip.h>

static suic::Mulstr VSTEMPLATENAME = "MPFTemplate";

void VSManager::UnzipTo(const suic::Mulstr& data, const suic::String& strDir, const suic::String& name)
{
	XRUnzip xrZip;
	if (xrZip.OpenFromMemory((void*)data.c_str(), data.Length()))
	{
        int index = 0;

		for (;;)
		{
			suic::String shortFile;
			suic::Mulstr unzipData;
			int iSize = xrZip.GetZipItemData(index, unzipData, shortFile);
			if (iSize <= 0)
			{
				break;
			}
			
			if (!name.Empty())
			{
                shortFile.Replace(VSTEMPLATENAME.c_str(), name);
			}

			FileWriter fs;
			suic::String strPath;
			
			strPath.Format(_U("%s\\%s"), strDir.c_str(), shortFile.c_str());
	
			if (fs.Open(strPath))
			{
				fs.Write(unzipData.c_str());
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

    fReader.Open(strPath);
    fReader.Read(data);

	// 解压到指定目录
	UnzipTo(data, strDir, name);
	
	// 用name替换模板的工程项目名称
	ReplacePrjName(name, strDir);
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

            // 目录
            if (fileFinder.IsDir())
            {
			}
            else
            {
                FileReader fReader;
                FileWriter fWriter;
                suic::Mulstr data;
                fReader.Open(fileFinder.GetFilePath());
                fReader.Read(data);
                fReader.Close();

                FileDir::RemoveFile(fileFinder.GetFilePath());

                data.Replace(VSTEMPLATENAME.c_str(), name.c_str());
                fWriter.Open(fileFinder.GetFilePath());
                fWriter.WriteAscii(data);
            }
		}
	}
}
