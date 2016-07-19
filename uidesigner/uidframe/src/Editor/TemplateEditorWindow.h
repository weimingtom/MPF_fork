// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����TemplateEditorWindow.h
// ��  �ܣ�ģ��༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _TEMPLATEEDITORWINDOW_H_
# define _TEMPLATEEDITORWINDOW_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class ThemeEditorWindow;

class TemplateEditorWindow : public suic::Window
{
public:

    TemplateEditorWindow(DesignElement* resElem, ThemeEditorWindow* owner);
    ~TemplateEditorWindow();

    void SetRootTemplateElement(TemplateRootItem* tempNode);
    void OnEditTempPropClick(DpObject* sender, RoutedEventArg* e);

    void OnIsVisibleChanged(IsVisibleChangedEventArg* e);

protected:

    void InitTreeDoc();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

protected:

    ObjTreeManager* _objTree;
    ThemeEditorWindow* _themeWnd;
    TemplateRootItem* _tempElement;

    FrameworkElement* _templateParent;
};

#endif

