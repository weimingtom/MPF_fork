///
/// �ļ�����SolidColorBrushNode.h
///
/// ��  �ܣ�SolidColorBrush��Դ
///
/// ��  �ߣ�����
///

#ifndef _SOLIDCOLORBRUSHNODE_H
#define _SOLIDCOLORBRUSHNODE_H

#include <Core/XamlSceneNode.h>

class SolidColorBrushNode : public XamlNodeItem
{
public:

    SolidColorBrushNode();

private:

    suic::SolidColorBrush* _value;
};

#endif

