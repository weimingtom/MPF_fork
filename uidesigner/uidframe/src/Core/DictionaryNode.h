///
/// �ļ�����DictionaryNode.h
///
/// ��  �ܣ�Dictionary��Դ
///
/// ��  �ߣ�����
///


#ifndef _DICTIONARYNODE_H
#define _DICTIONARYNODE_H

#include <Core/XamlSceneNode.h>

class DictionaryNode : public XamlNodeItem
{
public:

    DictionaryNode();

private:

    suic::XDictionary* _value;
};

#endif
