
#include "stdafx.h"

#include <Core/SingleUndefined.h>

ImplementRTTIOfClass(SingleUndefined, suic::Object)
ImplementRTTIOfClass(NodeUndefined, suic::Object)
ImplementRTTIOfClass(SingleUndefinedResNode, ResNode)
ImplementRTTIOfClass(NodeUndefinedResNode, ResNode)

SingleUndefinedResNode::SingleUndefinedResNode()
{
    _value = NULL;
}

SingleUndefinedResNode::~SingleUndefinedResNode()
{
    FREEREFOBJ(_value)
}

void SingleUndefinedResNode::CloneNode(ResNodePtr& obj)
{
}

bool SingleUndefinedResNode::IsSingleValue()
{
    return true;
}

void SingleUndefinedResNode::SetValue(suic::Object* val)
{
    if (NULL != val)
    {
        val->ref();
        SingleUndefined* pVal = suic::RTTICast<SingleUndefined>(val);
        SETREFOBJ(_value, pVal);
        val->unref();
    }
}

suic::Object* SingleUndefinedResNode::GetValue()
{
    return _value;
}

suic::String SingleUndefinedResNode::GetSingleXml()
{
    if (_value != NULL)
    {
        return _value->GetValue();
    }
    return suic::String();
}

suic::String SingleUndefinedResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    return strXml;
}

void NodeUndefinedResNode::CloneNode(ResNodePtr& obj)
{
    NodeUndefinedResNode* pNode = new NodeUndefinedResNode();
    obj = pNode;
    _holdItem.CloneTo(&(pNode->_holdItem));
}

bool NodeUndefinedResNode::IsSingleValue()
{
    return false;
}

void NodeUndefinedResNode::SetValue(suic::Object* val)
{
}

suic::Object* NodeUndefinedResNode::GetValue()
{
    return NULL;
}

suic::String NodeUndefinedResNode::GetSingleXml()
{
    return suic::String();
}

suic::String NodeUndefinedResNode::GetResXml(const String& offset)
{
    return _holdItem.GetResXml(offset);
}

void NodeUndefinedResNode::InitResNode(suic::IXamlNode* pNode)
{
    _holdItem.Clear();
    _holdItem.InitNode(pNode);
}