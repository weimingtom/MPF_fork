// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����MainWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�Sharpui������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

//
// <Summary>
//  ��ʾ��ť��Command�����ݰ󶨹��ܣ�
//  Command���������û���������ݽ��ղ�����ť�ĵ������
//  ���ݰ󶨹�����ʾ�˰�ť�����ݸ����û����ݱ仯���仯��
// </Summary>
//
class BindData : public suic::NotifyPropChanged
{
public:

    BeginMember(BindData, suic::NotifyPropChanged)
        MemberString(Name)
    EndMember()

    BindData()
    {
        _name = "Bind TextBlock";
    }

    suic::String GetName()
    {
        return _name;
    }

    void SetName(suic::String val)
    {
        _name = val;
        NotifyChanged("Name");
    }

    suic::String ToString()
    {
        return _name;
    }

private:

    suic::String _name;
};

class MainWindow : public suic::Window
{
public:

    MainWindow();
    ~MainWindow();

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnButtonClicked(suic::Element* sender, suic::RoutedEventArg* e);
    void OnEditStylePropClick(suic::Element* sender, suic::RoutedEventArg* e);

    void OnHyperlinkClick(suic::Element* sender, suic::RoutedEventArg* e);
 
protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    void RegisterButtonEvent();
};

#endif

