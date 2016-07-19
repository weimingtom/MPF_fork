// 华勤科技版权所有 2010-2011
// 
// 文件名：uidframeimpl.h
// 功  能：实现设计器框架导出接口定义。
// 
// 作  者：汪荣
// 时  间：2012-07-02
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
