// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ResSelectorWindow.h
// ��  �ܣ���Դ����ѡ����档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _RESTYPEWINDOW_H_
# define _RESTYPEWINDOW_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class ResSelectorWindow : public suic::Window
{
public:

    ResSelectorWindow();
    ~ResSelectorWindow();

    ResTypeItem* GetSelectedItem();
    TypeItem* GetSelectedControl();

    void OnDbListClick(Element* sender, MouseButtonEventArg* e);
    void OnCtrlListDbClick(Element* sender, MouseButtonEventArg* e);

    void OnSelectedChanged(suic::Element* sender, suic::SelectionChangedEventArg* e);

protected:

    void InitTreeDoc();

    bool IsCanClose(ResTypeItem* resItem);
    ResTypeItem* GetCurrentSelItem();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

private:

    bool _isDbClicked;
};

#endif
