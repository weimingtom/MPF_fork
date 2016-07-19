// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����MainWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�Sharpui������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _SETTINGWINDOW_H_
#define _SETTINGWINDOW_H_

class SettingWindow : public suic::Window
{
public:

    BeginRoutedEvent(SettingWindow, suic::Window)
        MemberRouted(OnButtonClicked)
    EndRoutedEvent()

    SettingWindow();
    ~SettingWindow();

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnButtonClicked(suic::DpObject* sender, suic::RoutedEventArg* e);
 
protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    void RegisterButtonEvent();

private:

    Window* _openWnd;
};

#endif

