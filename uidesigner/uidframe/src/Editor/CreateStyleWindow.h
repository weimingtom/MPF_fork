// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����CreateStyleWindow.h
// ��  �ܣ��༭��ʽ��ؼ�ģ��ѡ����档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _CREATESTYLEWINDOW_H_
# define _CREATESTYLEWINDOW_H_

#include <main/CommonInterface.h>

using namespace suic;

class CreateStyleWindow : public suic::Window
{
public:

    BeginRoutedEvent(CreateStyleWindow, suic::Window)
        MemberRouted(OnOkButtonClick)
        MemberRouted(OnCancelButtonClick)
    EndRoutedEvent()

    CreateStyleWindow(bool fromStyle);
    ~CreateStyleWindow();

    bool IsFromBlank() const
    {
        return _fromBlank;
    }

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnOkButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);
    void OnCancelButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);

private:

    bool _fromStyle;
    bool _fromBlank;
    suic::RadioButton* _rBlank;
    suic::RadioButton* _rBackup;
};

#endif
