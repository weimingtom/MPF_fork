///
/// �ļ�����ColorNode.h
///
/// ��  �ܣ�Color��Դ
///
/// ��  �ߣ�����
///

#ifndef _COLORNODE_H
#define _COLORNODE_H

#include <Core/XamlSceneNode.h>

class ColorNode : public XamlEditNode
{
public:

    ColorNode();

    Object* GetHostElem()
    {
        return _value;
    }

private:

    suic::OColor* _value;
};

#endif
