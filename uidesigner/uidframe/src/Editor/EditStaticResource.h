// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����EditStaticResource.h
// ��  �ܣ��󶨱༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _EDITSTATICRESOURCE_H_
# define _EDITSTATICRESOURCE_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class EditStaticResourceWindow : public suic::Window
{
public:

    EditStaticResourceWindow(ResourceDictionaryNode* resDicNode, DesignElement* target, DpProperty* dp, ResNode* resNode);
    ~EditStaticResourceWindow();

    String GetStaticResource();

    void OnButtonClick(Element* sender, RoutedEventArg* e);
    void OnItemSelectionChanged(Element* sender, SelectionChangedEventArg* e);

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    SetterNode* HandlePropItem(SetterEditor* setterEditor, DpItem* dpItem, SetterNode* pSetter);

private:

    ResourceDictionaryNode* _resDicNode;
    ResNode* _resNode;

    DesignElement* _target;
    DpProperty* _dp;

    String _strResourceKey;
    EditorControl* _prevEditCtrl;

};

#endif
