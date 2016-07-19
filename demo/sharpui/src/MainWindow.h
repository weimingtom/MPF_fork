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

using namespace suic;

enum ViewMode
{
    vmHome,
    vmControl,
};

/// <Summary>
///  �û����ݽṹʵ��ICommand�ӿڣ����Խ������簴ť֮��ĵ�������¼�
///  �¼�����Ϊ��ť�������¼��ǿ�·�ɵģ�����������ListBox��TreeView��
///  �����Զ�������ģ���ж���İ�ť�ĵ���¼�
/// <Summary/>
class NameField : public NotifyPropChanged
{
public:

    BeginMember(NameField, NotifyPropChanged)
        MemberInt(Number)
        MemberString(Name)
        MemberString(Tel)
        MemberString(Sexy)
        MemberString(PersonId)
        MemberString(Addr)
        MemberBool(CheckRow)
    EndMember()

    NameField(int num, String name)
    {
        _number = num;
        _name = name;
        _addr = _U("�й��㶫����");
        _tel = _U("13500013800");
        _personId = _U("52020298888081231");
    }

    int GetNumber()
    {
        return _number;
    }

    bool GetCheckRow() 
    {
        return true;
    }

    void SetCheckRow(bool val)
    {
        NotifyChanged(_U("CheckRow"));
    }

    void SetNumber(int val)
    {
        _number = val;
    }

    String GetName()
    {
        return _name;
    }

    void SetName(String name)
    {
        _name = name;
    }

    String GetAddr()
    {
        return _addr;
    }

    void SetAddr(String val)
    {
        _addr = val;
    }

    String GetTel()
    {
        return _tel;
    }

    void SetTel(String val)
    {
        _tel = val;
    }

    String GetSexy()
    {
        return _sexy;
    }

    void SetSexy(String val)
    {
        _sexy = val;
    }

    String GetPersonId()
    {
        return _personId;
    }

    void SetPersonId(String val)
    {
        _personId = val;
    }

private:

    int _number;
    String _name;
    String _addr;
    String _tel;
    String _sexy;
    String _personId;
};

class GroupData : public suic::ViewGroup
{
public:

    BeginMember(GroupData, suic::ViewGroup)
        MemberGetString(Name)
    EndMember()

    GroupData()
    {

    }

    GroupData(suic::String name)
    {
        _name = name;
    }

    bool IsBottomLevel()
    {
        return false;
    }

    suic::String GetName()
    {
        return _name;
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

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnLeftAniEnd(Object* sender, EventArg* e);
    void OnRightAniEnd(Object* sender, EventArg* e);
    void OnCtrlAreaStart(Object* sender, EventArg* e);

    void OnButtonClicked(Element* sender, RoutedEventArg* e);
    void OnSetPosBtnClicked(Element* sender, RoutedEventArg* e);
    void OnControlSwitchComplete(Object* sender, EventArg* e);

    void OnControlMouseEnter(Element* sender, MouseButtonEventArg* e);

    void OnAppSelected(Element* sender, MouseButtonEventArg* e);
    void OnControlSelected(Element* sender, SelectionChangedEventArg* e);

    void OnTimer(Object* sender, EventArg* e);

protected:

    void LeftPanelAnimate(bool bHome);
    void RightPanelAnimate(bool bHome);
    void SwitchToControl(String name);

    GroupData* NewGroupData(suic::String name);
    void InitGroupListData(suic::ListBox* pListBox);

protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    int _currentMode;
    bool _isPendingHome;
    bool _isPendingCtrl;
    AssignerTimer* _timer;
    StringAutoDic<FrameworkElement> _ctrls;
};

#endif
