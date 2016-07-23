// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ElementTree.h
// ��  �ܣ�Ԫ�ز������
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _ELEMENTTREE_H_
# define _ELEMENTTREE_H_

#include "ProjectTreeMenu.h"

class SlnTreeManager;
class ObjTreeManager;
class ElementTree;
class ProjectTree;

class ElementTreeItem : public suic::TreeViewItem
{
public:

    static suic::DpProperty* IsActiveMouseOverProperty;

    static bool StaticInit();

    RTTIOfClass(ElementTreeItem)

    void SetActiveMouseOver(bool val);

protected:

    bool IsItemItsOwnContainer(suic::Object* item);
    suic::Element* GetContainerForItemOverride();

    void ArrangeCore(const suic::Rect& arrangeRect);
};

class TreeItemDockPanel : public suic::StackPanel
{
public:

    RTTIOfClass(TreeItemDockPanel);

public:

    void ArrangeChild(ElementTreeItem* parent, suic::ScrollViewer* scrollView);
    void ArrangeCore(const suic::Rect& arrangeRect);
};

class TreeItemGrid : public suic::Grid
{
public:

    RTTIOfClass(TreeItemGrid);

public:

    void RenderChildren(suic::Drawing* drawing);
    void OnArrange(const suic::Size& rect);
    void ArrangeCore(const suic::Rect& arrangeRect);

    void ArrangeChild(Element* parent, suic::ScrollViewer* scrollView);
};

class ElementTree : public TreeView
{
public:

    ElementTree();
    ~ElementTree();

    RTTIOfClass(ElementTree)

    bool InitTreeDoc(ObjTreeManager* treeDoc);

    ObjTreeManager* GetObjTreeMana() const;
    void UpdateElementTree();

    void SetActiveMouseOver(ElementTreeItem* treeItem);
    void OnSelectedItemChanged(RoutedPropChangedEventArg* e);

protected:

    bool IsItemItsOwnContainer(suic::Object* item);
    suic::Element* GetContainerForItemOverride();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnContextMenu(ContextMenuEventArg* e);

protected:

    bool _showRoot;
    suic::WeakRef _prevActive;

    ObjTreeManager* _treeDoc;
    ElemTreeMenu _elemMenu;
};

typedef shared<ElementTree> ElementTreePtr;

class ProjectTree : public TreeView
{
public:

    ProjectTree();
    ~ProjectTree();

    RTTIOfClass(ProjectTree)
;
    SlnTreeManager* GetSlnTree();
    ObjTreeManager* GetObjTree();

    Project* GetProject();

    void SetRootDir(const String& strDir);
    String GetRootDir() const;

    bool InitImageMenu(bool bCanOpenImage);
    bool InitTreeDoc(SlnTreeManager* slnMana, Solution* root);

protected:

    void OnContextMenu(ContextMenuEventArg* e);
    void OnSelectedItemChanged(RoutedPropChangedEventArg* e);

    void OnPreviewMouseDoubleClick(MouseButtonEventArg* e);

private:

    String _rootDir;
    bool _canOpenImage;
    PrjTreeMenu* _prjTreeMenu;
};


#endif

