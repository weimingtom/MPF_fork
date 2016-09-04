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

#include "uidframeimpl.h"
#include "MainWindow.h"

void InitDControls();

UIDFrameImpl::UIDFrameImpl()
{
    InitDControls();

    SystemResource::Ins()->InitDefaultResDic();
}

UIDFrameImpl::~UIDFrameImpl()
{

}

bool UIDFrameImpl::LoadRes(const suic::String& path, const suic::String& pwd)
{
    //suic::ResourceHelper::SetSystemRes(path, pwd);
    //UIEncrypt::Encrypt(pwd, suic::FileDir::CalculatePath(path));
    suic::ResourceHelper::AddRes(_U("mpfuid"), path, pwd);
    suic::ResourceHelper::AddDir(_U("mpfuid"), FileDir::CalculatePath(_U("")));
    return true;
}

suic::ElementPtr UIDFrameImpl::LoadFrame(const suic::String& path)
{
    suic::ElementPtr main;
    return main;
}

void UIDFrameImpl::OnText(int,int,int,int)
{
}

bool UIDFrameImpl::RunFrame(const suic::String& path)
{
    suic::RunApplication(new MainWindow(), path.c_str());
    MyHand += MyHandler(this, &UIDFrameImpl::OnText);
    return true;
}

bool UIDFrameImpl::RunWindow(suic::ElementPtr main)
{
    return true;
}

void UIDFrameImpl::Dispose()
{

}

void UIDGetFrame(UIDFramePtr& frame)
{
    frame = new UIDFrameImpl();
}
