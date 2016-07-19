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
        MemberGetObject(Icon)
        MemberGetString(Name)
    EndMember()

    RTTIOfClass(BindData)

    BindData()
    {
        _name = "Bind Button";
        _icon = NULL;
    }

    BindData(suic::String name)
    {
        _name = name;
        _icon = NULL;
    }

    suic::Object* GetIcon()
    {
        return _icon;
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

    suic::Brush* _icon;
    suic::String _name;
};

class MyThread : public suic::Thread
{
public:

    MyThread(suic::InvokeProxy* h)
    {
        _handler = h;
    }

    void Run()
    {
        for (;;)
        {
            if (IsStopped()) 
            {
                break;
            }
            _handler->PostInvoker(0, NULL);
            Sleep(20);
        }
    }

    void OnInvoker(suic::Object* sender, suic::InvokerArg* e)
    {
        suic::String text;
        static int i = 0;
        text.Format(_U("���߳�ˢ��Invoker_%d"), ++i);
        suic::ListBox* listBox = (suic::ListBox*)_handler->GetTarget();

        if (listBox->GetItemsSource()->GetCount() > 100)
        {
            listBox->GetItemsSource()->Clear();
        }

        listBox->AddChild(new BindData(text));
        listBox->ScrollToEnd();
    }

protected:

    suic::InvokeProxy* _handler;
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
    MyThread* _thread;
    suic::InvokeProxy* _reflesh;
};

#endif

