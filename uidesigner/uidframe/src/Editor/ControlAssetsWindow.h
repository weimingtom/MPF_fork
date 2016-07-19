// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ControlAssetsWindow.h
// ��  �ܣ��ؼ���Դ��ѡ����档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _CONTROLASSETSWINDOW_H_
# define _CONTROLASSETSWINDOW_H_

#include <main/CommonInterface.h>

using namespace suic;

class ControlAssetsWindow : public suic::Window
{
public:

    ControlAssetsWindow(IControlWindow* ctrlWnd);
    ~ControlAssetsWindow();

    void OnPreviewMouseDoubleClick(Element* sender, MouseButtonEventArg* e);

    void OnSelControlsChanged(Element* sender, SelectionChangedEventArg* e);
    void OnSelectedItemChanged(suic::Element* sender, suic::RoutedPropChangedEventArg* e);

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void RefleshControls();

private:

    IControlWindow* _controlWnd;
};

#endif
