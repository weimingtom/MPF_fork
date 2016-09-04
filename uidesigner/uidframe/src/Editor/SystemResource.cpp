/////////////////////////////////////////////////////
// SystemResource.cpp

#include "stdafx.h"

#include <Editor/SystemResource.h>

SystemResource* SystemResource::Ins()
{
    static SystemResource _ins;
    return &_ins;
}

ResourceDicRootItem* SystemResource::GetDefaultResDic()
{
    return _resRootItem;
}

ResNode* SystemResource::FindResNode(const suic::String& strName)
{
    if (NULL != _resRootItem)
    {
        ResourceDictionaryNode* resDicNode = _resRootItem->GetResourceDicNode();
        return resDicNode->SearchResItem(strName);
    }
    else
    {
        return NULL;
    }
}

TemplateRootItem* SystemResource::FindControlTemplate(const suic::String& strType)
{
    TemplateRootItem* tempRootItem = NULL;
    suic::String strName = strType + _U("Template");
    tempRootItem = suic::RTTICast<TemplateRootItem>(FindResNode(strName));

    if (NULL == tempRootItem)
    {
        SetterNode* setterNode = NULL;
        StyleNode* styleNode = FindStyle(strType);
        if (NULL != styleNode)
        {
            setterNode = styleNode->GetSetterCollection()->FindSetter(_U("Template"));
            if (NULL != setterNode)
            {
                tempRootItem = suic::RTTICast<TemplateRootItem>(setterNode->GetResNode());
            }
        }
    }

    return tempRootItem;
}

StyleNode*  SystemResource::FindStyle(const suic::String& strType)
{
    return suic::RTTICast<StyleNode>(FindResNode(strType));
}

bool  SystemResource::InitDefaultResDic()
{
    if (NULL == _resRootItem)
    {
        static suic::String strDefRes(_U("resource/uidesign/Template/theme.xaml"));

        suic::String strPath = suic::FileDir::CalculatePath(strDefRes);
        XamlLoader xamlLoader;
        _resRootItem = new ResourceDicRootItem();
        _resRootItem->ref();
        return xamlLoader.LoadResourceDicRootXaml(_resRootItem, strPath);
    }
    return (NULL != _resRootItem);
}

SystemResource::SystemResource()
{
    _resRootItem = NULL;
}

SystemResource::~SystemResource()
{
    if (NULL != _resRootItem)
    {
        _resRootItem->unref();
        _resRootItem = NULL;
    }
}