/////////////////////////////////////////////////////
// CreateStyleWindow.cpp

#include "stdafx.h"

#include <Editor/CreateStyleWindow.h>

CreateStyleWindow::CreateStyleWindow(bool fromStyle)
    : _fromStyle(fromStyle)
    , _fromBlank(true)
    , _rBlank(NULL)
    , _rBackup(NULL)
{
}

CreateStyleWindow::~CreateStyleWindow()
{
}

void CreateStyleWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);

    SetDialogResult(-1);

    _rBlank = FindElem<suic::RadioButton>(_U("fromBlank"));
    _rBackup = FindElem<suic::RadioButton>(_U("fromBackup"));

    _rBackup->SetChecked(Boolean::True);

    if (_fromStyle)
    {
        SetTitle(_U("�༭��ʽ"));
    }
    else
    {
        SetTitle(_U("�༭�ؼ�ģ��"));
    }
}

void CreateStyleWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);
}

void CreateStyleWindow::OnOkButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);

    if (_rBlank->IsChecked())
    {
        _fromBlank = true;
    }
    else if (_rBackup->IsChecked())
    {
        _fromBlank = false;
    }
    else
    {
        return;
    }

    SetDialogResult(0);
    AsyncClose();
}

void CreateStyleWindow::OnCancelButtonClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    SetDialogResult(-1);
    AsyncClose();
}
