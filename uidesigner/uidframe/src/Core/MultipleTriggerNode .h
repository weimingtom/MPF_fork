///
/// �ļ�����MultipleTriggerNode.h
///
/// ��  �ܣ�MultipleTrigger��Դ
///
/// ��  �ߣ�����
///

#ifndef _MULTIPLETRIGGERNODE_H
#define _MULTIPLETRIGGERNODE_H

#include <Core/XamlSceneNode.h>

class MultipleTriggerNode : public XamlNodeItem
{
public:

    MultipleTriggerNode();

private:

    suic::MultiTrigger* _value;
};

#endif


