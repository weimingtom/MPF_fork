// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ExpressionWindow.h
// ��  �ܣ����ʽ���洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _EXPRESSIONWINDOW_H_
# define _EXPRESSIONWINDOW_H_

using namespace suic;

class ExpressionWindow : public suic::Window
{
public:

    BeginRoutedEvent(ExpressionWindow, suic::Window)
        MemberRouted(OnOkButtonClick)
    EndRoutedEvent()

    ExpressionWindow();
    ExpressionWindow(bool fromTemp);
    ~ExpressionWindow();

    String GetValue() const;
    void SetPreviousValue(const String& val);

    void OnOkButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

protected:

    bool _fromTemplate;
    suic::String _value;
};

#endif

