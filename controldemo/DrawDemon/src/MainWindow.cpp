/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
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
    CenterWindow();

    //
    // ע�ᰴť�¼�����
    //
    RegisterButtonEvent();
}

void MainWindow::OnButtonClicked(suic::Element* sender, suic::RoutedEventArg* e)
{
    if (sender->GetName().Equals("NorBtn"))
    {
    }
    else if (sender->GetName().Equals("BindBtn"))
    {
    }
}

void MainWindow::RegisterButtonEvent()
{
}
