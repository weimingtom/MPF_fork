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
    // ����
    //
    frame->RunFrame();
    frame = NULL;

    //
    // MPF��������
    //
    suic::ExitUIWgx();

	return 0;
}
