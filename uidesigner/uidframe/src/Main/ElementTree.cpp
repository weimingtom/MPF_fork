/////////////////////////////////////////////////////
// ElementTree.cpp

#include "stdafx.h"
#include <shellapi.h>
#include "ElementTree.h"
#include "AddNameWindow.h"
#include "DesignPanel.h"
#include "SlnTreeManager.h"

#include <tools/Utils.h>
#include <System/Tools/EventHelper.h>

#include <Editor/ThemeEditorWindow.h>
#include <Main/ImageSelectorWindow.h>

#pragma comment(lib, "Shell32.lib")

ImplementRTTIOfClass(ElementTreeItem, suic::TreeViewItem)
ImplementRTTIOfClass(TreeItemGrid, suic::Grid)
ImplementRTTIOfClass(TreeItemDockPanel, suic::StackPanel)

ImplementRTTIOfClass(ElementTree, suic::TreeView)
ImplementRTTIOfClass(ProjectTree, suic::TreeView)

suic::DpProperty* ElementTreeItem::IsActiveMouseOverProperty;

bool ElementTreeItem::StaticInit()
{
    if (NULL == IsActiveMouseOverProperty)
    {
        IsActiveMouseOverProperty = suic::DpProperty::RegisterAttached(_U("IsActiveMouseOver"), RTTIType(), suic::Boolean::RTTIType()
            , new suic::PropMetadata(suic::Boolean::False, PropMetadataOptions::AffectsRender));
    }
    return true;
}

void ElementTreeItem::SetActiveMouseOver(bool val)
{
    SetValue(IsActiveMouseOverProperty, BOOLTOBOOLEAN(val));
}

bool ElementTreeItem::IsItemItsOwnContainer(suic::Object* item)
{
    return (item->GetRTTIType()->InheritFrom(ElementTreeItem::RTTIType()));
}

suic::Element* ElementTreeItem::GetContainerForItemOverride()
{
    return new ElementTreeItem();
}

void ElementTreeItem::ArrangeCore(const suic::Rect& arrangeRect)
{
    if (ReadFlag(suic::CoreFlags::IsMeasureDirty))
    {
        UpdateMeasure();
    }

    suic::TreeView* parent = GetParentTreeView();
    suic::ScrollViewer* scrollView = parent->GetScrollHost();

    _offset.x = 0;
    _offset.y = arrangeRect.top;

    _screenOffset = parent->GetScreenOffset();
    _screenOffset.y = _offset.y + GetUIParent()->GetScreenOffset().y;

    _renderSize.cx = scrollView->GetViewportWidth();
    _renderSize.cy = arrangeRect.Height();

    if (GetVisualChildrenCount() == 1)
    {
        TreeItemDockPanel* child = RTTICast<TreeItemDockPanel>(GetVisualChild(0));
        if (NULL != child)
        {
            child->ArrangeChild(this, scrollView);
        }
    }

    _descendantBounds = Rect(_screenOffset, _renderSize);
}

void TreeItemDockPanel::ArrangeCore(const suic::Rect& arrangeRect)
{
}

void TreeItemDockPanel::ArrangeChild(ElementTreeItem* parent, suic::ScrollViewer* scrollView)
{
    suic::Rect rcArrange;

    _offset.x = 0;
    _offset.y = 0;
    
    _screenOffset = parent->GetScreenOffset();
    _renderSize = parent->GetRenderSize();
    _renderSize.cx = scrollView->GetViewportWidth();

    suic::Debug::Trace(_U("TreeItemStackPanel: %d\n"), _renderSize.cx);

    if (GetVisualChildrenCount() == 2)
    {
        TreeItemGrid* child0 = RTTICast<TreeItemGrid>(GetVisualChild(0));
        Element* child1 = GetVisualChild(1);

        rcArrange.right = _renderSize.Width();
        rcArrange.top = child0->GetDesiredSize().Height();
        rcArrange.bottom = rcArrange.top + child1->GetDesiredSize().Height();

        child0->ArrangeChild(this, scrollView);
        child1->Arrange(rcArrange);
    }
}

void TreeItemGrid::RenderChildren(suic::Drawing* drawing)
{
    if (GetVisualChildrenCount() == 2)
    {
        Element* child0 = GetVisualChild(0);
        Element* child1 = GetVisualChild(1);
        suic::fRect clip(0, 0, _renderSize.cx - 40, _renderSize.cy);

        suic::Debug::Trace(_U("TreeItemGrid::RenderChildren: %d\n"), _renderSize.cx);
        
        if (drawing->ContainsClip(clip))
        {
            drawing->Save();
            drawing->ClipRect(clip, suic::ClipOp::OpIntersect);
            child0->Render(drawing);
            drawing->Restore();
        }

        child1->Render(drawing);
    }
}

void TreeItemGrid::OnArrange(const suic::Size& rect)
{
    //suic::Grid::OnArrange(rect);
}

void TreeItemGrid::ArrangeCore(const suic::Rect& arrangeRect)
{
}

void TreeItemGrid::ArrangeChild(Element* parent, suic::ScrollViewer* scrollView)
{
    suic::Rect rcArrange;

    _offset.x = 0;
    _offset.y = 0;

    _screenOffset = parent->GetScreenOffset();
    _renderSize.cx = parent->GetActualWidth();//scrollView->GetViewportWidth();
    _renderSize.cy = GetDesiredSize().Height();

    suic::Debug::Trace(_U("TreeItemGrid: %d\n"), _renderSize.cx);

    if (GetVisualChildrenCount() == 2)
    {
        Element* child0 = GetVisualChild(0);
        Element* child1 = GetVisualChild(1);

        rcArrange.left = -scrollView->GetHorizontalOffset();
        rcArrange.right = rcArrange.left + child0->GetDesiredSize().Width();

        rcArrange.bottom = child0->GetDesiredSize().Height();
        child0->Arrange(rcArrange);

        rcArrange.left = 0;
        rcArrange.right = _renderSize.Width();

        child1->Arrange(rcArrange);
    }
    else
    {
        suic::Debug::Trace(_U("************************* TreeItemStackPanel: %d\n"), _renderSize.cx);
    }

    _renderSize.cx += 6;
    _descendantBounds = Rect(_screenOffset, _renderSize);
}

ProjectTree::ProjectTree()
{
    _prjTreeMenu = new PrjTreeMenu();
    _prjTreeMenu->ref();
}

ProjectTree::~ProjectTree()
{
    _prjTreeMenu->unref();
}

SlnTreeManager* ProjectTree::GetSlnTree()
{
    return _prjTreeMenu->GetSlnTree();
}

ObjTreeManager* ProjectTree::GetObjTree()
{
    return GetSlnTree()->GetObjTreeManager();
}

Project* ProjectTree::GetProject()
{
    if (NULL != GetObjTree())
    {
        return GetObjTree()->GetProject();
    }
    else
    {
        return NULL;
    }
}

void ProjectTree::SetRootDir(const String& strDir)
{
    _rootDir = strDir;
}

String ProjectTree::GetRootDir() const
{
    return _rootDir;
}

bool ProjectTree::InitImageMenu(bool canOpenImage)
{
    _canOpenImage = canOpenImage;
    return true;
}

bool ProjectTree::InitTreeDoc(SlnTreeManager* slnMana, Solution* root)
{
    _prjTreeMenu->Initialize(slnMana, this);
    SetItemsSource(root->GetProjects());
    InitImageMenu(true);

    return true;
}

void ProjectTree::OnContextMenu(ContextMenuEventArg* e)
{
    suic::Element* elem = suic::RTTICast<suic::Element>(e->GetOriginalSource());
    FilterNode* filterNode = RTTICast<FilterNode>(elem->GetDataContext());
    suic::ObservableCollection* pMenu = NULL;
    RootItem* rootItem = RTTICast<RootItem>(filterNode);

    if (NULL == filterNode)
    {
        return;
    }

    // ͼ��ڵ�
    if (filterNode->GetRTTIType()->InheritFrom(ImageRootItem::RTTIType()))
    {
        pMenu = _prjTreeMenu->images;
    }
    // .xml�ڵ㣬��Ҫ��ȡ
    else if (rootItem != NULL)
    {
        if (!filterNode->GetRTTIType()->InheritFrom(ApplicationRootItem::RTTIType()))
        {
            pMenu = _prjTreeMenu->rootElem;
            RootItemCmd* pItem = DynamicCast<RootItemCmd>(pMenu->GetItem(4));
            RootItemCmd* pDelItem = DynamicCast<RootItemCmd>(pMenu->GetItem(2));

            pDelItem->SetShowItem(true);
            pItem->SetEnabled(true);

            ResourceDicRootItem* resRootItem = RTTICast<ResourceDicRootItem>(rootItem);

            // ������Դ�����ش˲˵���
            if (resRootItem != NULL)
            {
                pItem->SetName(_U("Ӧ����Դ"));
                pDelItem->SetShowItem(false);
            }
            else
            {
                ElementRootItem* elemRootItem = suic::RTTICast<ElementRootItem>(rootItem);
                if (NULL != elemRootItem)
                {
                    pItem->SetEnabled(true);
                    if (rootItem == rootItem->GetProject()->GetMainWindow())
                    {
                        pDelItem->SetShowItem(false);
                        pItem->SetName(_U("�Ƴ�����"));
                    }
                    else
                    {
                        pItem->SetName(_U("��Ϊ����"));
                    }
                }
                else
                {
                    pItem->SetEnabled(false);
                }
            }
        }
    }
    // ����ڵ�
    else
    {
        pMenu = _prjTreeMenu->filter;
        Project* pPrj = RTTICast<Project>(filterNode);
        FilterNodeCmd* pItem = DynamicCast<FilterNodeCmd>(pMenu->GetItem(0));

        if (NULL != pPrj)
        {
            pItem->SetShowItem(true);
        }
        else
        {
            pItem->SetShowItem(false);
        }
    }

    if (NULL != pMenu)
    {
        suic::ContextMenu* ctxMenu = new suic::ContextMenu();

        ctxMenu->ref();
        ctxMenu->SetItemsSource(pMenu);
        SetSelectedItem(filterNode);
        UpdateLayout();

        ctxMenu->SetTag(filterNode);
        ctxMenu->SetPlacementTarget(this);
        ctxMenu->TrackContextMenu();

        ctxMenu->unref();
    }
}

void ProjectTree::OnSelectedItemChanged(RoutedPropChangedEventArg* e)
{
    e->SetHandled(true);
    if (NULL != GetProject())
    {
        GetProject()->SwitchToCurrent();
    }
    //GetProject()->SwitchToCurrent();
}

void ProjectTree::OnPreviewMouseDoubleClick(MouseButtonEventArg* e)
{
    suic::Element* elem = suic::RTTICast<suic::Element>(e->GetOriginalSource());
    FilterNode* filterNode = RTTICast<FilterNode>(elem->GetDataContext());

    if (NULL == filterNode || e->GetMouseButton() != suic::MouseButton::mbLeftButton)
    {
        return;
    }

    if (filterNode->GetRTTIType()->InheritFrom(RootItem::RTTIType()))
    {
        RootItem* pElemItem = (RootItem*)filterNode;
        if (!pElemItem->IsLoaded())
        {
            pElemItem->Load(false);
        }

        ResourceDicRootItem* resRootItem = RTTICast<ResourceDicRootItem>(pElemItem);

        // ��Դ�ڵ�
        if (resRootItem != NULL)
        {
            Project* pPrg = pElemItem->GetProject();
            const String strPath = "/mpfuid;/resource/uidesign/layout/ThemeEditor.xaml";
            ThemeEditorWindow* themeWnd = new ThemeEditorWindow(pElemItem, resRootItem->GetResourceDicNode(), NULL);

            e->SetHandled(true);

            themeWnd->ref();
            pElemItem->CheckLoadState();
            themeWnd->ShowDialog(strPath);
            themeWnd->unref();

            DesignHelper::EnterDesignMode();

            if (pElemItem->IsModified())
            {
                pElemItem->Save();
                pElemItem->Close();
                pElemItem->GetProject()->Reflesh(true);
            }

            DesignHelper::ExitDesignMode();
        }
        else if (pElemItem->GetRTTIType()->InheritFrom(ElementRootItem::RTTIType()))
        {
            GetObjTree()->SetTargetElement((ElementRootItem*)pElemItem);
            e->SetHandled(true);
        }
    }
    // ͼ��ڵ�
    else if (filterNode->GetRTTIType()->InheritFrom(ImageRootItem::RTTIType()))
    {
    }
}

//=======================================================================
// ElementTree

ElementTree::ElementTree()
    : _showRoot(true)
    , _treeDoc(NULL)
{
}

ElementTree::~ElementTree()
{
}

void ElementTree::UpdateElementTree()
{
    _treeDoc->UpdateElementTree();
    UpdateLayout();
}

ObjTreeManager* ElementTree::GetObjTreeMana() const
{
    return _treeDoc;
}

bool ElementTree::IsItemItsOwnContainer(suic::Object* item)
{
    return (item->GetRTTIType()->InheritFrom(ElementTreeItem::RTTIType()));
}

suic::Element* ElementTree::GetContainerForItemOverride()
{
    return new ElementTreeItem();
}

void ElementTree::OnInitialized(EventArg* e)
{
    suic::TreeView::OnInitialized(e);
}

void ElementTree::OnLoaded(suic::LoadedEventArg* e)
{
    suic::TreeView::OnLoaded(e);
}

void ElementTree::OnContextMenu(ContextMenuEventArg* e)
{
    suic::Object* item = NULL;
    suic::Element* elem = suic::RTTICast<suic::Element>(e->GetOriginalSource());

    if (elem != NULL)
    {
        item = elem->GetDataContext();
    }

    if (NULL != item)
    {
        // �Ҽ�����˲���Ԫ��
        DesignElement* elem = suic::RTTICast<DesignElement>(item);
        ElementRootItem* rootItem = _treeDoc->GetDesignPanel()->GetRootElement();

        if (rootItem != NULL && NULL != elem)
        {
            SetSelectedItem(item);
            _elemMenu.TrackElementMenu(rootItem, elem, this);
        }
    }

    e->SetHandled(true);
}

bool ElementTree::InitTreeDoc(ObjTreeManager* treeDoc)
{
    _treeDoc = treeDoc;
    return true;
}

void ElementTree::SetActiveMouseOver(ElementTreeItem* treeItem)
{
    ElementTreeItem* prevItem = _prevActive.GetElement<ElementTreeItem>();
    if (prevItem != NULL)
    {
        prevItem->SetActiveMouseOver(false);
    }

    _prevActive = treeItem;

    if (NULL != treeItem)
    {
        treeItem->SetActiveMouseOver(true);
    }
}

void ElementTree::OnSelectedItemChanged(RoutedPropChangedEventArg* e)
{
    Object* selObj = e->GetNewValue();
    DesignElement* selElem = suic::DynamicCast<DesignElement>(selObj);

    if (NULL != selElem && _treeDoc != NULL && selElem->GetUIElement() != NULL)
    {
        _treeDoc->GetDesignPanel()->SetFocusedElement(selElem, false);
    }

    e->SetHandled(true);
}
