// ======================================================================
//
// Copyright (c) 2008-2012 ����, Inc. All rights reserved.
//
// suiwgx��Դ����ѭCPLЭ����п�Դ���κθ��˻�����������ʹ�ã�
// �����ܾ��ڴ˿������κ���ҵ���ʵĿ����롣
//
// ======================================================================

//////////////////////////////////////////////////////////////////////////////
// uidframeimpl.cpp

#include "stdafx.h"

#include <main/Encrypt.h>
#include <Core/ValueEditor.h>
#include <Editor/SystemResource.h>
#include <System/Tools/TreeBuilder.h>

#include <main/VSManager.h>

#include "uidframeimpl.h"

static suic::String APP_URI = _U("/mpfuid;/resource/uidesign/layout/Application.xaml");

void InitDControls();

UIDFrameImpl::UIDFrameImpl()
    : _mainWnd(NULL)
    , _mode(IUIDFrame::UIDMode::umNormal)
{
    //VSManager::CreateVSProject(_U("2010"), _U("MPF999"), _U("d:\\"));

    InitDControls();

    SystemResource::Ins()->InitDefaultResDic();
}

UIDFrameImpl::~UIDFrameImpl()
{
    FREEREFOBJ(_mainWnd);
}

bool UIDFrameImpl::InitApplication()
{
    if (suic::Application::Current() == NULL)
    {
        suic::XamlReader xamlRead;
        SETREFOBJ(_mainWnd, new MainWindow());
        _mainWnd->SetUIDMode(_mode);
        xamlRead.LoadUri(NULL, _mainWnd, APP_URI.c_str());
    }

    return suic::Application::Current() != NULL;
}

bool UIDFrameImpl::LoadRes(const suic::String& path, const suic::String& pwd)
{
    //suic::ResourceHelper::SetSystemRes(path, pwd);
    //UIEncrypt::Encrypt(pwd, suic::FileDir::CalculatePath(path));
    suic::ThemeOp::AddUri(_U("mpfuid"), path, pwd);
    suic::ThemeOp::AddDir(_U("mpfuid"), FileDir::CalculatePath(_U("")));
    return true;
}

bool UIDFrameImpl::LoadProject(const suic::String& strDir, const suic::String& strTheme, bool bNoExistCreate)
{
    if (InitApplication())
    {
        suic::String strPath = strDir;
        strPath.Trim();
        if (strPath[strPath.Length() - 1] != _U('\\') && 
            strPath[strPath.Length() - 1] != _U('/'))
        {
            strPath += _U("\\");
        }
        strPath += strTheme;

        if (_mainWnd->OpenProject(strPath) != NULL)
        {
            return true;
        }
        else if (bNoExistCreate && _mainWnd->CreateProject(strTheme, strDir) != NULL)
        {
            return true;
        }
    }
    return false;
}

bool UIDFrameImpl::RunFrame()
{
     //
    // �����������Դ�ļ�
    //
    LoadRes(_U("default.sres"), _U("wrdesign"));

    suic::RunApplication(new MainWindow(), APP_URI.c_str());

    return true;
}

bool UIDFrameImpl::StartWindow(const suic::String& strUri)
{
    if (InitApplication())
    {
        _mainWnd->SetSkipStartWindow(true);
        
        _mainWnd->Show();
        _mainWnd->OpenWindow(strUri);

        _mainWnd->SetSkipStartWindow(false);
    }

    return true;
}

void UIDFrameImpl::RemoveWindow(const suic::String& strUri)
{
}

void UIDFrameImpl::SetUIDMode(IUIDFrame::UIDMode mode)
{
    _mode = mode;
}

void UIDFrameImpl::Dispose()
{

}

void UIDGetFrame(UIDFramePtr& frame)
{
    frame = new UIDFrameImpl();
}
