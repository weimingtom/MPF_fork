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

class ComboBoxData : public suic::NotifyPropChanged
{
public:

    BeginMember(ComboBoxData, suic::NotifyPropChanged)
        MemberString(Name)
    EndMember()

    RTTIOfClass(ComboBoxData)

    ComboBoxData()
    {
        _name = "combobox";
    }

    suic::String GetName()
    {
        return _name;
    }

    void SetName(suic::String val)
    {
        _name = val;
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

    void OnSelItemChanged(suic::Element* sender, suic::SelectionChangedEventArg* e);
    void OnComboBoxTextChanged(suic::DpObject* sender, suic::RoutedEventArg* e);
    void OnSwitchBtnClick(suic::Element* sender, suic::RoutedEventArg* e);

protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    void RegisterButtonEvent();
};

#endif

