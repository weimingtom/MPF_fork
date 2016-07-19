/////////////////////////////////////////////////////
// PropSelectorWindow.cpp

#include "stdafx.h"

#include <Main/DesignPanel.h>
#include <Editor/PropSelectorWindow.h>
#include <Core/StyleNode.h>

#include <Framework/Controls/Rectangle.h>

PropSelectorWindow::PropSelectorWindow()
{
    _targetElements = NULL;
}

PropSelectorWindow::~PropSelectorWindow()
{
}

String PropSelectorWindow::GetTargetName()
{
    String strName;

    if (NULL != _targetElements)
    {
        ComboBox* cmbBox = FindElem<ComboBox>(_U("targetType"));
        if (cmbBox->GetSelectedIndex() > 0)
        {
            strName = cmbBox->GetSelectedItem()->ToString();
        }
    }

    return strName;
}

void PropSelectorWindow::SetTargetElements(suic::ItemCollection* itemColl)
{
    _targetElements = itemColl;
}

void PropSelectorWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);

    ListBox* pList = FindElem<ListBox>(_U("propList"));
    if (NULL != pList && NULL != _cond.target)
    {
        suic::ItemCollection* itemColl = pList->GetItemsSource();

        DpManager::Ins()->QueryDpItems(&_cond, itemColl);
        pList->AddPreviewMouseDoubleClick(new MouseButtonEventHandler(this, &PropSelectorWindow::OnDbListClick));
    }

    ComboBox* cmbBox = FindElem<ComboBox>(_U("targetType"));

    if (NULL != _targetElements && _targetElements->GetCount() > 0)
    {
        cmbBox->SetItemsSource(_targetElements);
        cmbBox->SetSelectedIndex(0);
    }
    else if (NULL != _cond.target)
    {
        cmbBox->AddChild(new TargetTypeItem(_cond.target));
        cmbBox->SetSelectedIndex(0);
    }
}

void PropSelectorWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
    /*ComboBox* cmbBox = FindElem<ComboBox>(_U("targetType"));
    if (NULL != cmbBox && cmbBox->GetCount() == 0 && NULL != _cond.target)
    {
        cmbBox->AddChild(new TargetTypeItem(_cond.target));
        cmbBox->SetSelectedIndex(0);
    }*/
}

void PropSelectorWindow::OnDbListClick(Element* sender, MouseButtonEventArg* e)
{
    if (e->GetMouseButton() != suic::MouseButton::mbLeftButton)
    {
        return;
    }

    e->SetHandled(true);
    AsyncClose();
}

void PropSelectorWindow::SetQueryDpCond(const QueryDpCond& cond)
{
    _cond = cond;
}

DpItem* PropSelectorWindow::GetSelectedItem()
{
    ListBox* pList = FindElem<ListBox>(_U("propList"));
    if (NULL != pList)
    {
        return dynamic_cast<DpItem*>(pList->GetSelectedItem());
    }
    else
    {
        return NULL;
    }
}
