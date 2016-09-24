/////////////////////////////////////////////////////
// VSWindow.cpp

#include "stdafx.h"
#include "VSWindow.h"
#include <Core/ResNode.h>
#include <Tools/Utils.h>
#include <main/VSManager.h>

VSWindow::VSWindow()
{
    _checkedVs = _U("2008");
}

VSWindow::~VSWindow()
{
}

void VSWindow::SetVSVersion(const suic::String& strVer)
{
    _createVsInfo.targetVs = strVer;
}

void VSWindow::InitVSType(const suic::String strVer)
{
    suic::RadioButton* vsType = FindElem<suic::RadioButton>(strVer);
    if (NULL != vsType)
    {
        vsType->SetChecked(suic::Boolean::True);
        _checkedVs = vsType->GetContent()->ToString();
    }
}

void VSWindow::StartVSCreator(CreateVSInfo& info)
{
    VSWindow vsWnd;
    suic::String strUri(_U("/mpfuid;/resource/uidesign/layout/VSWindow.xaml"));

    vsWnd.setAutoDelete(false);
    vsWnd.SetVSVersion(info.targetVs);
    info.createVs = info.targetVs;
	vsWnd._createVsInfo = info;

    if (vsWnd.ShowDialog(strUri) == 0)
    {
        info = vsWnd._createVsInfo;
        if (info.needOpenSln)
        {
        }
    }
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
    e->SetHandled(true);

    suic::Element* pElem = suic::RTTICast<suic::Element>(sender);

    if (pElem->GetName().Equals(_U("OkBtn")))
    {
        if (_checkedVs.Empty())
        {
            return;
        }
        
        suic::TextBox* prjName = FindElem<suic::TextBox>(_U("PrjName"));
        suic::TextBox* prjDir = FindElem<suic::TextBox>(_U("PrjDir"));
        suic::CheckBox* openSln = FindElem<suic::CheckBox>(_U("IsOpenSln"));

        suic::String strName = prjName->GetText();
        suic::String strDir = prjDir->GetText();

        strName.Trim();
        strDir.Trim();

        if (strName.Empty())
        {
            suic::Toast::Show(_U("�����빤�����ƣ�"), Toast::DelayClose::ShortTime);
            return;
        }

        if (strDir.Empty())
        {
            suic::Toast::Show(_U("�����빤��·����"), Toast::DelayClose::ShortTime);
            return;
        }

        if (VSManager::CreateVSProject(_checkedVs, strName, strDir))
        {
             SetDialogResult(0);
             if (NULL != openSln)
             {
                 _createVsInfo.needOpenSln = openSln->IsChecked();
             }
             _createVsInfo.slnPath.Format(_U("%s\\%s\\trunk\\%s.sln"), strDir.c_str(), strName.c_str(), strName.c_str());
        }
    }

    AsyncClose();
}

void VSWindow::OnVSButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    suic::RadioButton* pElem = suic::RTTICast<suic::RadioButton>(sender);
    suic::String strName = pElem->GetName();

    _createVsInfo.createVs = pElem->GetName();
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
    SetDialogResult(-1);
    if (!_createVsInfo.targetVs.Empty())
    {
        suic::String strTitle;
        strTitle.Format(_U("%s - %s"), GetTitle().c_str(), _createVsInfo.targetVs.c_str());
        SetTitle(strTitle);

        InitVSType(_createVsInfo.targetVs);
    }
}

void VSWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
}
