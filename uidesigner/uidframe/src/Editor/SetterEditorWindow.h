// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����SetterEditorWindow.h
// ��  �ܣ����Ա༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _SETTEREDITORWINDOW_H_
# define _SETTEREDITORWINDOW_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>

#include <Editor/PropSelectorWindow.h>

#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>
#include <Core/ValueEditor.h>
#include <Core/TypeManager.h>
#include <Core/TypeControls.h>

using namespace suic;

class SetterEditorWindow : public suic::Window
{
public:

    SetterEditorWindow(ThemeEditorWindow* themeWnd);
    ~SetterEditorWindow();

    SetterNode* SwitchToProperty(DpItem* dpItem, SetterNode* pSetter, String targetName, bool fromTrigger);
    bool OpenPropSelectorWindow(Element* sender, PropSelectorWindow* propWnd, PlacementMode mode);

    void ShowSetterNode(SetterNode* resNode, SetterCollectionNode* setterColl, bool fromTrigger);

protected:

    void HandlePrevSetterNode();

    virtual TreeView* GetTreeView();
    virtual void SelectToSetterNode(SetterNode* pSetterNode);
    virtual SetterNode* HandlePropItem(SetterEditor* setterEditor, DpItem* dpItem, SetterNode* pSetter);

protected:

    ThemeEditorWindow* _themeWnd;
    SetterEditor* _prevSetterEditor;
};

#endif

