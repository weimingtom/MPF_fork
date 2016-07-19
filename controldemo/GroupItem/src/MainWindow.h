// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����MainWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�MPF������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

class GroupData : public suic::ViewGroup
{
public:

    BeginMember(GroupData, suic::ViewGroup)
        MemberGetString(Name)
        MemberBool(IsExpander)
    EndMember()

    GroupData()
    {
        _isExpander = false;
    }

    GroupData(suic::String name)
    {
        _name = name;
        _isExpander = false;
    }

    bool IsBottomLevel()
    {
        return false;
    }

    suic::String GetName()
    {
        return _name;
    }

    bool GetIsExpander() 
    {
        return _isExpander;
    }

    void SetIsExpander(bool val)
    {
        _isExpander = val;
    }

    suic::String ToString()
    {
        return _name;
    }

private:

    bool _isExpander;
    suic::String _name;
};

class MainWindow : public suic::Window
{
public:

    BeginRoutedEvent(MainWindow, suic::Window)
        MemberRouted(OnResetButtonClick)
    EndRoutedEvent()

    MainWindow();
    ~MainWindow();

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnSelectedDateChanged(suic::Element* sender, suic::RoutedEventArg* e);
    void OnResetButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e);
    void OnStopButtonClick(suic::Element* sender, suic::RoutedEventArg* e);
    void OnDeleteButtonClick(suic::Element* sender, suic::RoutedEventArg* e);

    void OnSelModeButtonClick(suic::Element* sender, suic::RoutedEventArg* e);
 
protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    void RegisterButtonEvent();
    GroupData* NewGroupData(suic::String name);
};

#endif

