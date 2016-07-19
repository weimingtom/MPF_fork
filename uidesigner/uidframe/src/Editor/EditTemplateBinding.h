// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����EditTemplateBinding..h
// ��  �ܣ��󶨱༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _EDITTEMPLATEBINDING_H_
# define _EDITTEMPLATEBINDING_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class EditTemplateBindingWindow : public suic::Window
{
public:

    EditTemplateBindingWindow(RTTIOfInfo* rttiInfo, DpProperty* dp);
    ~EditTemplateBindingWindow();

    String GetBindingProp() const;

    void OnButtonClick(Element* sender, RoutedEventArg* e);
    void OnItemSelectionChanged(Element* sender, SelectionChangedEventArg* e);

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

private:

    DpProperty* _dp;
    RTTIOfInfo* _rttiInfo;

    String _bindingProp;
};

#endif
