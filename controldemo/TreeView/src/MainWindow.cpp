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
}

void MainWindow::RegisterButtonEvent()
{
    suic::TreeView* treeView = FindElem<suic::TreeView>("bindTree");
    if (NULL != treeView)
    {
        treeView->AddSelectedItemChanged(new suic::RoutedPropChangedEventHandler(this, &MainWindow::OnTreeItemSelectedChanged));
  
        TreeGroup* treeGrp = new TreeGroup();
        TreeGroup* gjGroup = new TreeGroup();

        treeView->AddChild(treeGrp);
        treeGrp->SetName("����");
        
        gjGroup->SetName("�й�");
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("�Ϻ�"));
        gjGroup->AddItem(new TreeGuoJia("������"));
        gjGroup->AddItem(new TreeGuoJia("�㶫����"));
        gjGroup->AddItem(new TreeGuoJia("��³ľ��"));

        treeView->SetItemIsExpanded(gjGroup, true);
        treeGrp->AddItem(gjGroup);

        gjGroup = new TreeGroup();
        gjGroup->SetName("����");
        gjGroup->AddItem(new TreeGuoJia("�׶�"));

        treeGrp->AddItem(gjGroup);
        treeGrp->AddItem(new TreeGuoJia("����"));

        treeGrp = new TreeGroup();
        treeGrp->SetName("ŷ��");

        gjGroup = new TreeGroup();
        gjGroup->SetName("�¹�");
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        gjGroup->AddItem(new TreeGuoJia("����"));
        treeGrp->AddItem(gjGroup);

        gjGroup = new TreeGroup();
        gjGroup->SetName("Ӣ��");
        treeGrp->AddItem(gjGroup);

        treeView->AddChild(treeGrp);

        treeGrp = new TreeGroup();
        treeGrp->SetName("����");
        treeView->AddChild(treeGrp);
    }
}

void MainWindow::OnTreeItemSelectedChanged(suic::Element* sender, suic::RoutedPropChangedEventArg* e)
{
    suic::Object* unselectedItem = e->GetOldValue();
    suic::Object* selectedItem = e->GetNewValue();

    if (NULL != unselectedItem)
    {
        ;
    }

    // ��ǰѡ����
    if (selectedItem != NULL)
    {
    }
}
