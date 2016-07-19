// 华勤科技版权所有 2010-2011
// 
// 文件名：PropEditorWindow.h
// 功  能：属性编辑界面。
// 
// 作  者：汪荣
// 时  间：2013-01-02
// 
// ============================================================================

# ifndef _PROPEDITORWINDOW_H_
# define _PROPEDITORWINDOW_H_

#include <Editor/SetterEditorWindow.h>

using namespace suic;

class ThemeEditorWindow;

class PropEditorItem
{
public:

    // 模版打开
    bool templateMode;
    SetterNode* currentNode;
    SetterCollectionNode* setterColl;
};

class PropEditorWindow : public SetterEditorWindow
{
public:

    PropEditorWindow(SetterCollectionNode* setterColl, ThemeEditorWindow* owner);
    ~PropEditorWindow();

    TreeView* GetTreeView();

    void ResetQueryDpCond();
    QueryDpCond* GetQueryDpCond();

    void UpdateSelectedPropShow();
    void UpdateSetterColl(TemplateRootItem* tempRoot, SetterCollectionNode* setterColl);

    void OnDbClickTree(Element* sender, MouseButtonEventArg* e);
    void OnPropItemSelectedChanged(suic::Element* sender, suic::RoutedPropChangedEventArg* e);

protected:

    void InitTreeDoc();

    void AddPropertyItem(DpItem* dpItem, String targetName);

    // 响应增加属性按钮
    void OnAddPropButton(Element* sender, RoutedEventArg* e); 
    void OnOpPropButton(Element* sender, RoutedEventArg* e);

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);
    void OnIsVisibleChanged(suic::IsVisibleChangedEventArg* e);

    SetterNode* HandlePropItem(SetterEditor* setterEditor, DpItem* dpItem, SetterNode* pSetter);

protected:

    QueryDpCond _cond;

    SetterCollectionNode* _setterColl;

    TreeView* _propTreeView;

    TemplateRootItem* _tempRootItem;

    bool _initTargetElements;
    suic::ObservableCollection _targetElements;
};

#endif

