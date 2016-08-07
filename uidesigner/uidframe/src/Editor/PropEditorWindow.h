// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����PropEditorWindow.h
// ��  �ܣ����Ա༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _PROPEDITORWINDOW_H_
# define _PROPEDITORWINDOW_H_

#include <Core/CoreDefines.h>
#include <Editor/SetterEditorWindow.h>

using namespace suic;

class ThemeEditorWindow;

class PropEditorItem
{
public:

    // ģ���
    bool templateMode;
    SetterNode* currentNode;
    SetterCollectionNode* setterColl;
};

class TargetNameComboBox : public suic::ComboBox
{
public:


};

class PropEditorWindow : public SetterEditorWindow
{
public:

    PropEditorWindow(SetterCollectionNode* setterColl, ThemeEditorWindow* owner, ePropertyType propType);
    ~PropEditorWindow();

    TreeView* GetTreeView();

    void SetPropertyType(ePropertyType val)
    {
        _propType = val;
    }

    void ResetQueryDpCond();
    QueryDpCond* GetQueryDpCond();

    void UpdateSelectedPropShow();
    void UpdateSetterColl(TemplateRootItem* tempRoot, SetterCollectionNode* setterColl);

    void OnDbClickTree(Element* sender, MouseButtonEventArg* e);
    void OnPropItemSelectedChanged(suic::Element* sender, suic::RoutedPropChangedEventArg* e);

    void OnTargetNameChanged(Element* sender, SelectionChangedEventArg* e);

protected:

    void InitTreeDoc();
    void InitTemplateTargetNames(SetterNode* pSetterNode);

    void AddPropertyItem(DpItem* dpItem, String targetName);

    // ��Ӧ�������԰�ť
    void OnAddPropButton(Element* sender, RoutedEventArg* e); 
    void OnOpPropButton(Element* sender, RoutedEventArg* e);

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);
    void OnIsVisibleChanged(suic::IsVisibleChangedEventArg* e);

    suic::ComboBox* GetTargetNameControl();
    void GetTemplateTargetNames(DesignElement* elem, ItemCollection* itemColl);
    suic::RTTIOfInfo* GetOwnerRTTIInfo();
    SetterNode* HandlePropItem(SetterEditor* setterEditor, DpItem* dpItem, SetterNode* pSetter);

protected:

    QueryDpCond _cond;
    ePropertyType _propType;

    SetterNode* _selSetterNode;
    suic::ComboBox* _targetBox;
    SetterCollectionNode* _setterColl;

    TreeView* _propTreeView;

    TemplateRootItem* _tempRootItem;

    bool _initTargetElements;
    suic::ObservableCollection _targetElements;
};

#endif

