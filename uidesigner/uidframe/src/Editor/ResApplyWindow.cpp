/////////////////////////////////////////////////////
// ResApplyWindow.cpp

#include "stdafx.h"

#include <Main/DesignPanel.h>
#include <Editor/ResApplyWindow.h>
#include <Core/StyleNode.h>

ResApplyWindow::ResApplyWindow(FileRootItem* rootItem)
{
    _rootItem = rootItem;
    _listBox = NULL;
    _listBoxApply = NULL;
}

ResApplyWindow::~ResApplyWindow()
{
}

void ResApplyWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);
    suic::Array<ResourceDicRootItem*> resRootItems;
    _rootItem->GetProject()->FindAllResourceDicRootItems(resRootItems);

    _listBox = FindElem<ListBox>(_U("listBox"));
    _listBoxApply = FindElem<ListBox>(_U("applyListBox"));

    // 初始化已经应用的资源
    InitAppliedResource();

    if (NULL != _listBox)
    {
        suic::ItemCollection* itemColl = _listBox->GetItemsSource();
        for (int i = 0; i < resRootItems.GetCount(); ++i)
        {
            if (!ExistResourceDicRootItem(resRootItems.GetItem(i)))
            {
                itemColl->AddItem(new ResApplyItem(resRootItems.GetItem(i)));
            }
        }
    }
}

bool ResApplyWindow::ExistResourceDicRootItem(ResourceDicRootItem* dicRootItem)
{
    suic::ItemCollection* itemColl = _listBoxApply->GetItemsSource();
    for (int i = 0; i < itemColl->GetCount(); ++i)
    {
        ResApplyItem* resItem = dynamic_cast<ResApplyItem*>(itemColl->GetItem(i));
        if (resItem->GetResourceDicRootItem() == dicRootItem)
        {
            return true;
        }
    }
    return false;
}

void ResApplyWindow::InitAppliedResource()
{
    ResourceDictionaryNode* resDicNode = NULL;
    ElementRootItem* elemRootItem = suic::RTTICast<ElementRootItem>(_rootItem);
    if (NULL != elemRootItem)
    {
        resDicNode = elemRootItem->GetRootElement()->GetResourceDictionary();
    }
    else
    {
        ApplicationRootItem* appRootItem = suic::RTTICast<ApplicationRootItem>(_rootItem);
        if (NULL != appRootItem)
        {
            resDicNode = appRootItem->GetApplicationNode()->GetResourceDictionary();
        }
    }

    if (NULL != resDicNode)
    {
        MergedDictionariesNode* mergedDic = resDicNode->GetMergedDictionariesNode();
        if (NULL != mergedDic)
        {
            for (int i = 0; i < mergedDic->GetCount(); ++i)
            {
                ResourceDictionaryNode* resDic = mergedDic->GetResourceDictionaryNode(i);
                if (NULL != resDic->GetSourceResource())
                {
                    _listBoxApply->AddChild(new ResApplyItem(resDic->GetSourceResource()));
                }
            }
        }
    }
}

void ResApplyWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
}

void ResApplyWindow::OnApplyButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    suic::Object* selObj = _listBox->GetSelectedItem();
    if (selObj != NULL && selObj != suic::DpProperty::UnsetValue())
    {
        _listBoxApply->AddChild(selObj);
        _listBox->GetItemsSource()->RemoveItem(selObj);
    }
}

void ResApplyWindow::OnRemoveButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    suic::Object* selObj = _listBoxApply->GetSelectedItem();
    if (selObj != NULL && selObj != suic::DpProperty::UnsetValue())
    {
        _listBox->AddChild(selObj);
        _listBoxApply->GetItemsSource()->RemoveItem(selObj);
    }
}

void ResApplyWindow::OnUpMoveButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
}

void ResApplyWindow::OnDownMoveButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
}

void ResApplyWindow::OnOkButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
}

void ResApplyWindow::OnCancelButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    AsyncClose();
}
