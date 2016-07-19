/////////////////////////////////////////////////////
// PropEditorWindow.cpp

#include "stdafx.h"

#include <Main/DesignPanel.h>

#include <Editor/EditRootPanel.h>
#include <Editor/ThemeEditorWindow.h>
#include <Editor/PropEditorWindow.h>
#include <Core/StyleNode.h>

#include <Framework/Controls/Rectangle.h>

SetterEditorWindow::SetterEditorWindow(ThemeEditorWindow* themeWnd)
{
    _prevSetterEditor = NULL;
    _themeWnd = themeWnd;
}

SetterEditorWindow::~SetterEditorWindow()
{

}

void SetterEditorWindow::SelectToSetterNode(SetterNode* pSetterNode)
{
    GetTreeView()->SetSelectedItem(pSetterNode);
    GetTreeView()->ScrollIntoView(pSetterNode, true);
}

void SetterEditorWindow::HandlePrevSetterNode()
{
    if (NULL != _prevSetterEditor)
    {
        _prevSetterEditor->SetVisibility(Visibility::Collapsed);
        _prevSetterEditor->SetTargetElement(NULL);
        _prevSetterEditor = NULL;
    }
}

SetterNode* SetterEditorWindow::SwitchToProperty(DpItem* dpItem, SetterNode* pSetter, String targetName, bool fromTrigger)
{
    HandlePrevSetterNode();

    SetterNode* pSetterNode = NULL;
    String editorName = DpManager::Ins()->GetEditorName(dpItem);
    SetterEditor* setterEditor = FindElem<SetterEditor>(editorName);

    if (NULL != setterEditor)
    {
        BrushResSetterEditor* resSetterEditor = RTTICast<BrushResSetterEditor>(setterEditor);
        setterEditor->ResetSetterEditor();

        if (resSetterEditor != NULL)
        {
            resSetterEditor->ResetBrushEditor();
        }
        
        pSetterNode = HandlePropItem(setterEditor, dpItem, pSetter);
        Brush* valBrush = RTTICast<Brush>(pSetterNode->GetValue());
        ExtensionResNode* extRes = RTTICast<ExtensionResNode>(pSetterNode->GetResNode());

        if (pSetterNode != NULL && !targetName.Empty())
        {
            pSetterNode->SetTargetName(targetName);
        }

        if (NULL == extRes && NULL == valBrush && resSetterEditor != NULL)
        {
            // 对Brush生成默认的画刷
            pSetterNode->SetValue(SolidColorBrush::White);
        }

        setterEditor->SetVisibility(Visibility::Visible);
        setterEditor->SetSetterNode(pSetterNode, fromTrigger);
        setterEditor->UpdateLayout();

        SelectToSetterNode(pSetterNode);

        _prevSetterEditor = setterEditor;
        setterEditor->SetTargetElement(_themeWnd->GetEditElem());
    }

    return pSetterNode;
}

SetterNode* SetterEditorWindow::HandlePropItem(SetterEditor* setterEditor, DpItem* dpItem, SetterNode* pSetter)
{
    return NULL;
}

TreeView* SetterEditorWindow::GetTreeView()
{
    return NULL;
}

bool SetterEditorWindow::OpenPropSelectorWindow(Element* sender, PropSelectorWindow* propWnd, PlacementMode mode)
{
    String strPath("/mpfuid;/resource/uidesign/layout/editor/PropSelectorWindow.xaml");

    Builder::BuildUri(propWnd, strPath);

    if (NULL != propWnd)
    {
        Popup popup;
        popup.setAutoDelete(false);
        popup.SetPlacementTarget(sender);
        popup.SetPlacement(mode);
        popup.SetChild(propWnd);
        popup.TrackingPopup();
        return true;
    }
    else
    {
        return false;
    }
}

//======================================================
// PropEditorWindow

PropEditorWindow::PropEditorWindow(SetterCollectionNode* setterColl, ThemeEditorWindow* owner)
    : SetterEditorWindow(owner)
{
    _setterColl = setterColl;
    _prevSetterEditor = NULL;
    _propTreeView = NULL;
    _tempRootItem = NULL;

    _initTargetElements = false;;
    _targetElements.setAutoDelete(false);
}   

PropEditorWindow::~PropEditorWindow()
{
}

TreeView* PropEditorWindow::GetTreeView()
{
    if (NULL == _propTreeView)
    {
        _propTreeView = FindElem<suic::TreeView>(_U("SettersTree"));
    }
    return _propTreeView;
}

void PropEditorWindow::ResetQueryDpCond()
{
    _cond = QueryDpCond();
}

QueryDpCond* PropEditorWindow::GetQueryDpCond()
{
    return &_cond;
}

void PropEditorWindow::UpdateSetterColl(TemplateRootItem* tempRoot, SetterCollectionNode* setterColl)
{
    _tempRootItem = tempRoot;

    if (NULL != _prevSetterEditor)
    {
        _prevSetterEditor->SetVisibility(Visibility::Collapsed);
        _prevSetterEditor = NULL;
    }

    _setterColl = setterColl;

    _cond.target = _setterColl->GetOwnerType();
    _cond.propFilter = _setterColl->GetChildren();

    suic::TreeView* settersTree = GetTreeView();

    if (NULL != settersTree)
    {
        settersTree->SetItemsSource(_cond.propFilter);
        if (_cond.propFilter->GetCount() > 0)
        {
            SetterNode* resNode = RTTICast<SetterNode>(_cond.propFilter->GetItem(0));
            settersTree->SetSelectedItem(resNode);
        }
    }

    UpdateSelectedPropShow();
}

void PropEditorWindow::InitTreeDoc()
{
}

void SetterEditorWindow::ShowSetterNode(SetterNode* resNode, SetterCollectionNode* setterColl, bool fromTrigger)
{
    //_themeWnd->GetEditRootPanel()->SetTemplateParent(NULL);

    if (NULL != resNode)
    {
        suic::RTTIOfInfo* ownerRtti = setterColl->GetOwnerType();
        DpItem* dpItem = DpManager::Ins()->FindDpItem(resNode->GetName());
        if (NULL != dpItem)
        {
            SwitchToProperty(dpItem, resNode, _U(""), fromTrigger);
        }
    }
}

void PropEditorWindow::OnPropItemSelectedChanged(suic::Element* sender, suic::RoutedPropChangedEventArg* e)
{
    Object* selObj = e->GetNewValue();
    SetterNode* resNode = RTTICast<SetterNode>(selObj);
    e->SetHandled(true);

    ShowSetterNode(resNode, _setterColl, false);
}

void PropEditorWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);

    suic::Button* pAddProp = FindElem<suic::Button>(_U("AddPropBtn"));
    if (NULL != pAddProp)
    {
        pAddProp->AddClick(new ClickEventHandler(this, &PropEditorWindow::OnAddPropButton));
    }

    //-----------------------------------------------------------------------

    suic::Button* pOpRes = FindElem<suic::Button>(_U("DelPropBtn"));
    if (NULL != pOpRes)
    {
        pOpRes->AddClick(new ClickEventHandler(this, &PropEditorWindow::OnOpPropButton));
    }

    pOpRes = FindElem<suic::Button>(_U("UpPropBtn"));
    if (NULL != pOpRes)
    {
        pOpRes->AddClick(new ClickEventHandler(this, &PropEditorWindow::OnOpPropButton));
    }

    pOpRes = FindElem<suic::Button>(_U("DownPropBtn"));
    if (NULL != pOpRes)
    {
        pOpRes->AddClick(new ClickEventHandler(this, &PropEditorWindow::OnOpPropButton));
    }

    //---------------------------------------------------------------------

    if (NULL != GetTreeView())
    {
        _propTreeView->AddSelectedItemChanged(new suic::RoutedPropChangedEventHandler(this, &PropEditorWindow::OnPropItemSelectedChanged));
        _propTreeView->AddPreviewMouseDoubleClick(new MouseButtonEventHandler(this, &PropEditorWindow::OnDbClickTree));
    }
}

void PropEditorWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
    
}

void PropEditorWindow::UpdateSelectedPropShow()
{
    if (NULL != _propTreeView)
    {
        Object* selObj = _propTreeView->GetSelectedItem();
        SetterNode* pSetter = NULL;

        if (NULL == selObj && _propTreeView->GetCount() > 0)
        {
            _propTreeView->SetSelectedItem(_propTreeView->GetItem(0));
            selObj = _propTreeView->GetSelectedItem();
        }

        pSetter = RTTICast<SetterNode>(selObj);

        if (pSetter != NULL)
        {
            ShowSetterNode(pSetter, _setterColl, false);
        }
    }
}

void PropEditorWindow::OnIsVisibleChanged(suic::IsVisibleChangedEventArg* e)
{
}

//======================================================
//

void PropEditorWindow::OnDbClickTree(Element* sender, MouseButtonEventArg* e)
{
    if (e->GetMouseButton() != suic::MouseButton::mbLeftButton)
    {
        return;
    }

    Element* clickElem = RTTICast<Element>(e->GetOriginalSource());
    if (NULL != clickElem && _setterColl != NULL)
    {
        SetterNode* item = RTTICast<SetterNode>(clickElem->GetDataContext());

        // 点击了模版节点
        TemplateRootItem* tempNode = RTTICast<TemplateRootItem>(item->GetResNode());
        if (NULL != tempNode)
        {
            _themeWnd->SwitchToTemplateEditor(tempNode);
        }
    }
    e->SetHandled(true);
}

SetterNode* PropEditorWindow::HandlePropItem(SetterEditor* setterEditor, DpItem* dpItem, SetterNode* pSetter)
{
    SetterNode* pSetterNode = pSetter;
    suic::RTTIOfInfo* ownerRtti = _setterColl->GetOwnerType();
    suic::TextBox* propName = FindElem<suic::TextBox>(_U("propName"));
    suic::TextBox* targetName = FindElem<suic::TextBox>(_U("targetName"));
    //DpProperty* dp = DpManager::Ins()->FindDp(dpItem->name, ownerRtti, true);
    DpProperty* dp = dpItem->GetDp(ownerRtti, true);
    IAddChild* pAddChild = DynamicCast<IAddChild>(setterEditor);

    if (NULL != propName)
    {
        propName->SetText(dpItem->name);
    }

    if (NULL != targetName)
    {
        targetName->SetText(ownerRtti->typeName);
    }

    if (NULL != pAddChild)
    {
        dpItem->FillAddChild(pAddChild);
    }

    if (NULL == pSetterNode)
    {
        pSetterNode = new SetterNode(dpItem->name, dp, NULL);
        setterEditor->InitSetterNode(ownerRtti, dp, pSetterNode);
        _setterColl->AddSetterForAdd(pSetterNode);
    }

    return pSetterNode;
}

void PropEditorWindow::AddPropertyItem(DpItem* dpItem, String targetName)
{
    SwitchToProperty(dpItem, NULL, targetName, false);
    _themeWnd->SetModified();
}

void PropEditorWindow::OnOpPropButton(Element* sender, RoutedEventArg* e)
{
    e->SetHandled(true);

    TreeView* pTreeView = GetTreeView();
    String strName = sender->GetName();
    Object* selObj = pTreeView->GetSelectedItem();
    ItemCollection* itemsColl = _setterColl->GetChildren();
    int selIndex = itemsColl->IndexOf(selObj);

    if (strName.Equals(_U("DelPropBtn")))
    {
        itemsColl->RemoveItem(selObj);
        if (selIndex >= itemsColl->GetCount())
        {
            selIndex = itemsColl->GetCount() - 1;
        }
        
        if (selIndex >= 0)
        {
            selObj = itemsColl->GetItem(selIndex);
            pTreeView->SetSelectedItem(selObj);
        }
    }
    else if (strName.Equals(_U("UpPropBtn")))
    {
        if (selIndex > 0)
        {
            itemsColl->SwapItem(selIndex, selIndex - 1);
        }
    }
    else if (strName.Equals(_U("DownPropBtn")))
    {
        if (selIndex < itemsColl->GetCount() - 1)
        {
            itemsColl->SwapItem(selIndex, selIndex + 1);
        }
    }

    _themeWnd->SetModified();

    pTreeView->ScrollIntoView(selObj, false);
    pTreeView->UpdateLayout();
}

void PropEditorWindow::OnAddPropButton(Element* sender, RoutedEventArg* e)
{
    PropSelectorWindow* propWnd = new PropSelectorWindow();

    propWnd->ref();
    propWnd->SetQueryDpCond(_cond);

    if (NULL != _tempRootItem)
    {
        _targetElements.Clear();
        _tempRootItem->GetChildAsNamed(&_targetElements);
        propWnd->SetTargetElements(&_targetElements);
    }

    if (OpenPropSelectorWindow(sender, propWnd, PlacementMode::pmRight))
    {
        DpItem* dpItem = propWnd->GetSelectedItem();
        if (NULL != dpItem)
        {
            AddPropertyItem(dpItem, propWnd->GetTargetName());
        }
    }

    _themeWnd->SetModified();
    propWnd->unref();
}
