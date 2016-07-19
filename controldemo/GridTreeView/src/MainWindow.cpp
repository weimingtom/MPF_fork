/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"
#include "MainWindow.h"

ImplementRTTIOfClass(TLData, suic::NotifyPropChanged)

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
}

void MainWindow::AddDataToListView(suic::ListView* pList, TLData* pData)
{
    pList->AddChild(pData);
    for (int i = 0; i < pData->GetCount(); ++i)
    {
        AddDataToListView(pList, pData->GetItem(i));
    }
}

void MainWindow::RegisterButtonEvent()
{
    suic::ListView* pListView = FindElem<suic::ListView>("lvTree");
    if (NULL != pListView)
    {
        OranizeData* pData = new OranizeData("�ܲ�");
        DepData* pDep = new DepData("�з�����");

        pDep->AddItem(new PersonData("���", "������ɽ"));
        pDep->AddItem(new PersonData("��ǿ", "����"));

        pData->AddItem(pDep);

        pDep = new DepData("���²�");
        pDep->AddItem(new PersonData("С��", "����"));
        pData->AddItem(pDep);

        AddDataToListView(pListView, pData);
    }
}
