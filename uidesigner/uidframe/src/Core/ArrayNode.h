///
/// �ļ�����ArrayNode.h
///
/// ��  �ܣ������Element��Ӧ����Դ����
///
/// ��  �ߣ�����
///

#ifndef _ARRAYNODE_H
#define _ARRAYNODE_H

#include <Core/XamlSceneNode.h>

class ArrayNode : public XamlNodeItem
{
public:

    ArrayNode();

private:

    suic::XArray* _value;
};

#endif
