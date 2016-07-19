/////////////////////////////////////////////////////
// ResSelectorWindow.cpp

#include "stdafx.h"

#include <Main/DesignPanel.h>
#include <Editor/ResSelectorWindow.h>
#include <Core/StyleNode.h>

#include <Framework/Controls/Rectangle.h>
#include <Framework/Controls/UniformGrid.h>

ResSelectorWindow::ResSelectorWindow()
{
    _isDbClicked = false;
}

ResSelectorWindow::~ResSelectorWindow()
{
}

void ResSelectorWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);
    _isDbClicked = false;

    ListBox* pList = FindElem<ListBox>(_U("resList"));
    if (NULL != pList)
    {
        pList->SetItemsSource(ResTypePool::GetTypeItems());
        pList->AddSelectionChanged(new SelectionChangedEventHandler(this, &ResSelectorWindow::OnSelectedChanged));
        pList->AddPreviewMouseDoubleClick(new MouseButtonEventHandler(this, &ResSelectorWindow::OnDbListClick));
    }

    ListBox* pCtrlList = FindElem<ListBox>(_U("ctrlList"));
    if (NULL != pCtrlList)
    {
        TypePool::GetTypeItems(pCtrlList->GetItemsSource());
        pCtrlList->AddPreviewMouseDoubleClick(new MouseButtonEventHandler(this, &ResSelectorWindow::OnCtrlListDbClick));
    }
}

void ResSelectorWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
}

void ResSelectorWindow::OnSelectedChanged(suic::Element* sender, suic::SelectionChangedEventArg* e)
{
    if (e->GetAddedItems()->GetCount() == 1)
    {
        e->SetHandled(true);
        ListBox* pList = FindElem<ListBox>(_U("ctrlList"));
        UniformGrid* pPanel = FindElem<UniformGrid>(_U("gridPanel"));

        ResTypeItem* resItem = dynamic_cast<ResTypeItem*>(e->GetAddedItems()->GetItem(0));
        if (!IsCanClose(resItem))
        {
            pPanel->SetColumns(2);
            pList->SetVisibility(Visibility::Visible);
        }
        else
        {
            pPanel->SetColumns(1);
            pList->SetVisibility(Visibility::Collapsed);
        }
        pPanel->UpdateLayout();
    }
}

void ResSelectorWindow::OnDbListClick(Element* sender, MouseButtonEventArg* e)
{
    if (e->GetMouseButton() != suic::MouseButton::mbLeftButton)
    {
        return;
    }

    e->SetHandled(true);

    if (IsCanClose(GetCurrentSelItem()))
    {
        _isDbClicked = true;
        AsyncClose();
    }
}

void ResSelectorWindow::OnCtrlListDbClick(Element* sender, MouseButtonEventArg* e)
{
    if (e->GetMouseButton() != suic::MouseButton::mbLeftButton)
    {
        return;
    }

    e->SetHandled(true);

    _isDbClicked = true;
    AsyncClose();
}

ResTypeItem* ResSelectorWindow::GetSelectedItem()
{
    if (_isDbClicked)
    {
        return GetCurrentSelItem();
    }
    else
    {
        return NULL;
    }
}

TypeItem* ResSelectorWindow::GetSelectedControl()
{
    if (_isDbClicked)
    {
        TypeItem* typeItem = NULL;
        ListBox* pList = FindElem<ListBox>(_U("ctrlList"));
        typeItem = dynamic_cast<TypeItem*>(pList->GetSelectedItem());
        return typeItem;
    }
    else
    {
        return NULL;
    }
}

ResTypeItem* ResSelectorWindow::GetCurrentSelItem()
{
    ListBox* pList = FindElem<ListBox>(_U("resList"));
    if (NULL != pList)
    {
        return dynamic_cast<ResTypeItem*>(pList->GetSelectedItem());
    }
    else
    {
        return NULL;
    }
}

bool ResSelectorWindow::IsCanClose(ResTypeItem* resItem)
{
    RTTIOfInfo* rttiInfo = resItem->GetType();
    if (rttiInfo->InheritFrom(FrameworkTemplate::RTTIType()) || 
        rttiInfo->InheritFrom(Style::RTTIType()) || 
        rttiInfo->InheritFrom(Element::RTTIType()))
    {
        return false;
    }
    else
    {
        return true;
    }
}
