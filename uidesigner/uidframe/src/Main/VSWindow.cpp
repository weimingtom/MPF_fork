/////////////////////////////////////////////////////
// VSWindow.cpp

#include "stdafx.h"
#include "VSWindow.h"
#include <Core/ResNode.h>
#include <Tools/Utils.h>
#include <main/VSManager.h>

VSWindow::VSWindow()
{
}

VSWindow::~VSWindow()
{
}

void VSWindow::StartVSCreator()
{
    VSWindow vsWnd;
    suic::String strUri(_U("/mpfuid;/resource/uidesign/layout/VSWindow.xaml"));

    vsWnd.setAutoDelete(false);
    vsWnd.ShowDialog(strUri);
}

void VSWindow::OnOpenDirButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    suic::DirBrowser dirOpen;
    suic::TextBox* prjDir = FindElem<suic::TextBox>(_U("PrjDir"));

    if (dirOpen.Open())
    {
        prjDir->SetText(dirOpen.GetDir());
    }
}

void VSWindow::OnButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    suic::Element* pElem = suic::RTTICast<suic::Element>(sender);
    suic::TextBox* prjName = FindElem<suic::TextBox>(_U("PrjName"));
    suic::TextBox* prjDir = FindElem<suic::TextBox>(_U("PrjDir"));

    suic::String strName = prjName->GetText();
    suic::String strDir = prjDir->GetText();

    strName.Trim();
    strDir.Trim();

    e->SetHandled(true);

    if (strName.Empty())
    {
        suic::Toast::Show(_U("请输入工程名称！"), Toast::DelayClose::ShortTime);
        return;
    }

    if (strDir.Empty())
    {
        suic::Toast::Show(_U("请输入工程路径！"), Toast::DelayClose::ShortTime);
        return;
    }

    if (strName.Equals(_U("OKBtn")))
    {
        if (_checkedVs.Empty())
        {
            return;
        }

        VSManager::CreateVSProject(_checkedVs, strName, strDir);
    }
    AsyncClose();
}

void VSWindow::OnVSButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    suic::RadioButton* pElem = suic::RTTICast<suic::RadioButton>(sender);
    suic::String strName = pElem->GetName();

    _checkedVs = pElem->GetContent()->ToString();

    e->SetHandled(true);
}

bool VSWindow::OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj)
{
    return false;
}

void VSWindow::OnConnect(suic::IXamlNode* pNode, suic::Object* target)
{
}

void VSWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);
}

void VSWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
}
