// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ResEditorWindow.h
// ��  �ܣ���Դ�༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _RESEDITORWINDOW_H_
# define _RESEDITORWINDOW_H_

#include <Main/EditNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Core/XamlRootElement.h>

using namespace suic;

class ResEditorWindow : public suic::Window
{
public:

    ResEditorWindow(ResourceDictionary* resElem);
    ~ResEditorWindow();

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

protected:

    ObjTreeManager* _objTree;
};

#endif

