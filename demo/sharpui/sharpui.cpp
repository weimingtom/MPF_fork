// sharpui.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "./src/MainWindow.h"
#include "./src/BindConverter.h"

void RegisterBindConverter()
{
    suic::BindConverter::Ins()->Add(_U("OrientationCvt"), new OrientationConverter());
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    suic::InitUIWgx();
    int iCount = suic::DpPropMemory::GetPropCount();
    suic::WindowPtr wnd(new MainWindow());

    RegisterBindConverter();

    //try
    {
        // ������Դ�ļ�
        suic::ResourceHelper::SetSystemRes("default.sres", "wr");
        suic::String strApp = _T("resource/mpf/Application.xml");
        
        // ���벼����Դ������ϵͳ
        suic::Application::RunApp(wnd.get(), strApp.c_str());
    }
    /*catch(suic::Exception& e)
    {
        String err = e.GetErrorMsg();
    }*/

    wnd = NULL;

    suic::ExitUIWgx();

    return 0;
}

