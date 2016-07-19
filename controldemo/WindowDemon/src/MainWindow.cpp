/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"
#include "MainWindow.h"

ImplementRTTIOfClass(ButtonData, suic::NotifyPropChanged)

MainWindow::MainWindow()
    : _openWnd(NULL)
{
}

MainWindow::~MainWindow()
{
    if (NULL != _openWnd)
    {
        _openWnd->unref();
    }
}

bool MainWindow::OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj)
{
    return false;
}

void MainWindow::OnConnect(suic::IXamlNode* pNode, suic::Object* target)
{
    
}

void MainWindow::OnInitialized(suic::EventArg* e)
{
    suic::Window::OnInitialized(e);
}

void MainWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);

    //
    // ���ھ���
    //
    CenterWindow(0);

    //
    // ע�ᰴť�¼�����
    //
    RegisterButtonEvent();
}

void MainWindow::OnButtonClicked(suic::Element* sender, suic::RoutedEventArg* e)
{
    if (sender->GetName().Equals("OpenWnd"))
    {
        if (NULL == _openWnd)
        {
            _openWnd = new ChildWindow();
            _openWnd->ref();
            suic::Builder::BuildUri(_openWnd, "ControlDemo/WindowDemon/ChildWindow.xml");
        }

        _openWnd->ShowInTaskBar(false);
        _openWnd->Show();
    }
    else if (sender->GetName().Equals("BindBtn"))
    {
    }
}

void MainWindow::RegisterButtonEvent()
{
    suic::ButtonBase* pBtn = NULL;

    // �������ҵ���ΪNorBtn�İ�ť����
    pBtn = FindElem<suic::ButtonBase>("OpenWnd");
    if (NULL != pBtn)
    {
        // �����Ƕ���ĵ���������ҽӵ���ť�ĵ���¼�
        pBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnButtonClicked));
    }

    pBtn = FindElem<suic::ButtonBase>("BindBtn");
    if (NULL != pBtn)
    {
        ButtonData* btnData = new ButtonData();
        //
        // �����Ƕ�������ݰ󶨵���ť��
        //
        pBtn->SetContent(btnData);

        // �������������ģ��������ǿ���ֱ���޸�ҵ����������̬�ı�󶨵������ϵ�����
        pBtn->SetDataContext(btnData);
    }
}
