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

#include "ChildWindow.h"

//
// <Summary>
//  ��ʾ��ť��Command�����ݰ󶨹��ܣ�
//  Command���������û���������ݽ��ղ�����ť�ĵ������
//  ���ݰ󶨹�����ʾ�˰�ť�����ݸ����û����ݱ仯���仯��
// </Summary>
//
class ButtonData : public suic::ICommand
                 , public suic::NotifyPropChanged
{
public:

    BeginMember(ButtonData, suic::NotifyPropChanged)
        MemberString(ButtonName)
    EndMember()

    RTTIOfClass(ButtonData)

    ButtonData()
    {
        _btnName = "�����ҵ������";
    }

    bool CanExecute(suic::Object* target, suic::Object* param)
    {
        return true;
    }

    void Execute(suic::Object* target, suic::Object* param)
    {
        SetButtonName("�󶨵�ҵ������");
    }

    suic::String GetButtonName()
    {
        return _btnName;
    }

    void SetButtonName(suic::String val)
    {
        _btnName = val;
        NotifyChanged("ButtonName");
    }

    suic::String ToString()
    {
        return _btnName;
    }

private:

    suic::String _btnName;
};

class MainWindow : public suic::Window
{
public:

    MainWindow();
    ~MainWindow();

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnButtonClicked(suic::Element* sender, suic::RoutedEventArg* e);
 
protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    void RegisterButtonEvent();

private:

    ChildWindow* _openWnd;
};

#endif

