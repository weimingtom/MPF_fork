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
    // 初始化MPF环境
    //
    suic::InitUIWgx();

    UIDFramePtr frame;

    //
    // 获取设计器核心接口
    //
    UIDGetFrame(frame);

    //
    // 载入设计器资源文件
    //
    frame->LoadRes(_U("default.sres"), _U("wrdesign"));

    //
    // 运行
    //
    frame->RunFrame(_U("/mpfuid;/resource/uidesign/layout/Application.xaml"));

    frame = NULL;

    //
    // MPF环境清理
    //
    suic::ExitUIWgx();

	return 0;
}
