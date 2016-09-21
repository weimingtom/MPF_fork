// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����VSWindow.h
// ��  �ܣ�VS�����򵼽��洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _VSWINDOW_H_
# define _VSWINDOW_H_

using namespace suic;

class VSWindow : public suic::Window
{
public:

    BeginRoutedEvent(VSWindow, suic::Window)
        MemberRouted(OnButtonClick)
        MemberRouted(OnVSButtonClick)
        MemberRouted(OnOpenDirButtonClick)
    EndRoutedEvent()

    VSWindow();
    ~VSWindow();

    static void StartVSCreator();

protected:

    void OnButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);
    void OnVSButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);
    void OnOpenDirButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    suic::String _checkedVs;
};

#endif

