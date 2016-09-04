// uidrun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <uidcore/uidframe.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    //
    // ��ʼ��MPF����
    //
    suic::InitUIWgx();
    suic::InitUILogLevel(suic::LogLevel::Trace);

    UIDFramePtr frame;

    //
    // ��ȡ��������Ľӿ�
    //
    UIDGetFrame(frame);

    //
    // �����������Դ�ļ�
    //
    frame->LoadRes(_U("default.sres"), _U("wrdesign"));

    //
    // ����
    //
    frame->RunFrame(_U("/mpfuid;/resource/uidesign/layout/Application.xaml"));

    frame = NULL;

    //
    // MPF��������
    //
    suic::ExitUIWgx();

	return 0;
}
