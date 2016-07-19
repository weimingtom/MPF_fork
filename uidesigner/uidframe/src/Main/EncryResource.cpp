/////////////////////////////////////////////////////
// EncryResource.cpp

#include "stdafx.h"
#include "EncryResource.h"

EncryResource::EncryResource(Project* pPrj)
{
    _project = pPrj;
}

EncryResource::~EncryResource()
{
}

void EncryResource::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);

    Button* pBtn(FindElem<Button>(_T("OpenEncryFile")));

    if (pBtn)
    {
        // ע�ᰴť�ĵ���¼�
        pBtn->AddClick(new suic::ClickEventHandler(this, &EncryResource::OnOpenFile));
    }

    pBtn = FindElem<Button>(_T("EncryBtn"));

    if (pBtn)
    {
        // ע�ᰴť�ĵ���¼�
        pBtn->AddClick(new suic::ClickEventHandler(this, &EncryResource::OnEncry));
    }

    pBtn = FindElem<Button>(_T("Cancel"));

    if (pBtn)
    {
        // ע�ᰴť�ĵ���¼�
        pBtn->AddClick(new suic::ClickEventHandler(this, &EncryResource::OnCancel));
    }

    AddHandler(PreKeyDownEvent, new suic::KeyboardEventHandler(this, &EncryResource::OnEnter), false);
}

void EncryResource::OnEnter(suic::Element* sender, suic::KeyboardEventArg* e)
{
    if (e->GetKey() == Key::kReturn)
    {
        e->SetHandled(true);

        //suic::RoutedEventArg e(this);
        //OnEncry(this, e);
    }
}

void EncryResource::OnLoaded(suic::LoadedEventArg* e)
{
    __super::OnLoaded(e);
}

void EncryResource::OnOpenFile(suic::Element* pItem, suic::RoutedEventArg* e)
{
    FileBrowser fb;

    if (fb.Save(this))
    {
        suic::TextBox* pathPtr(FindElem<suic::TextBox>(_T("EncryFile")));

        pathPtr->SetText(fb.GetFilePath());
        pathPtr->InvalidateForce();
    }
}

void EncryResource::OnEncry(suic::Element* pItem, suic::RoutedEventArg* e)
{
    suic::TextBox* pathPtr(FindElem<suic::TextBox>(_T("EncryFile")));
    suic::PasswordBox* pwdPtr(FindElem<suic::PasswordBox>(_T("Pwd")));

    suic::String strFile = pathPtr->GetText();
    suic::String strPwd = pwdPtr->GetPassword();

    if (strFile.Empty())
    {
        suic::InfoBox::Show(_T("�����뵼���ļ�!"), _T("��ʾ"));
        return;
    }

    if (!_project->Zip(strFile, strPwd))
    {
        suic::InfoBox::Show(_T("����ʧ��!"), _T("��ʾ"));
        return ;
    }

    Close();
    suic::InfoBox::Show(__T("�����ɹ�!"), _T("��ʾ"));  

    /*if (0 != UIEncrypt::Encrypt(strPwd, strFile))
    {
        suic::InfoBox::Show(_T("��ʾ"), _T("����ʧ��!"));
    }
    else
    {
        Close();
        suic::InfoBox::Show(_T("��ʾ"), _T("�����ɹ�!"));  
    }*/
}

void EncryResource::OnCancel(suic::Element* pItem, suic::RoutedEventArg* e)
{
    Close();
}