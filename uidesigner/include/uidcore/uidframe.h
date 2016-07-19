// 华勤科技版权所有 2010-2011
// 
// 文件名：uidframe.h
// 功  能：设计器框架导出接口定义。
// 
// 作  者：汪荣
// 时  间：2012-07-02
// 
// ============================================================================

#ifndef _UIDFRAME_H_
#define _UIDFRAME_H_

#include <System/Windows/Element.h>

class IUIDFrame : public suic::Object
{
public:

    virtual ~IUIDFrame() {}

    virtual bool LoadRes(const suic::String& path, const suic::String& pwd) = 0;
    virtual suic::ElementPtr LoadFrame(const suic::String& path) = 0;

    virtual bool RunFrame(const suic::String& path) = 0;
    virtual bool RunWindow(suic::ElementPtr main) = 0;

    virtual void Dispose() = 0;
};

typedef suic::shared<IUIDFrame> UIDFramePtr;

extern "C" void UIDGetFrame(UIDFramePtr& frame);

#endif
