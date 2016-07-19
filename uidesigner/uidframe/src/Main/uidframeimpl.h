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

#include <uidcore/uidframe.h>


typedef delegate<void(int,int,int,int)> MyHandler;

class UIDFrameImpl : public IUIDFrame
{
public:

    UIDFrameImpl();
    virtual ~UIDFrameImpl();

    virtual bool LoadRes(const suic::String& path, const suic::String& pwd);
    virtual suic::ElementPtr LoadFrame(const suic::String& path);

    virtual bool RunFrame(const suic::String& path);
    virtual bool RunWindow(suic::ElementPtr main);

    virtual void Dispose();

    void OnText(int,int,int,int);

private:

    MyHandler MyHand;
};

#endif
