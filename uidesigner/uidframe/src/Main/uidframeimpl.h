// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����uidframeimpl.h
// ��  �ܣ�ʵ���������ܵ����ӿڶ��塣
// 
// ��  �ߣ�����
// ʱ  �䣺2012-07-02
// 
// ============================================================================

#ifndef _UIDFRAMEIMPL_H_
#define _UIDFRAMEIMPL_H_

#include "MainWindow.h"
#include <uidcore/uidframe.h>

class UIDFrameImpl : public IUIDFrame
{
public:

    UIDFrameImpl();
    virtual ~UIDFrameImpl();

    virtual bool LoadRes(const suic::String& path, const suic::String& pwd);
    virtual bool LoadProject(const suic::String& strDir, const suic::String& strTheme, bool bNoExistCreate);

    virtual bool RunFrame();
    virtual bool StartWindow(const suic::String& strUri);
    virtual void RemoveWindow(const suic::String& strUri);

    virtual void SetUIDMode(IUIDFrame::UIDMode mode);

    virtual void Dispose();

private:

    bool InitApplication();

private:

    MainWindow* _mainWnd;
    IUIDFrame::UIDMode _mode;
};

#endif
