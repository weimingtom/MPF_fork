// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����AddNameWindow.h
// ��  �ܣ��������洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _ADDNAMEWINDOW_H_
# define _ADDNAMEWINDOW_H_

using namespace suic;

class AddNameWindow : public suic::Window
{
public:

    AddNameWindow(const String& title, const String& name, int wid);
    ~AddNameWindow();

    String GetName() const;
    void OnOkButtonClick(suic::Element* sender, suic::RoutedEventArg* e);

protected:

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

protected:

    int _width;
    suic::String _value;
    suic::String _title;
    suic::String _name;
};

#endif

