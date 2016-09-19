/////////////////////////////////////////////////////
// VSManager.cpp

#include "stdafx.h"
#include "VSManager.h"

#include <zip/XRUnzip.h>

void VSManager::UnzipTo(const suic::Mulstr& data, const suic::String& strDir)
{
	XRUnzip xrZip;
	if (xrZip.OpenFromMemory(data.c_str(), data.length()))
	{
		for (;;)
		{
			suic::String name;
			suic::Mulstr unzipData;
			int iSize = xrZip.GetZipItemData(index, unzipData, name);
			if (iSize <= 0)
			{
				break;
			}
		}
	}
}

bool VSManager::CreateVSProject(const suic::String& name, const suic::String& strDir)
{
}
