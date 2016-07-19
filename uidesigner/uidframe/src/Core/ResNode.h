///
/// 文件名：ColorNode.h
///
/// 功  能：Color资源
///
/// 作  者：汪荣
///

#ifndef _RESNODE_H
#define _RESNODE_H

#include <Core/XamlSceneNode.h>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include <System/Windows/ResourceUri.h>
#include <System/Graphics/RadialGradientBrush.h>

#include <Framework/Controls/GridView.h>

class ResNode;
class DesignElement;

typedef rapidxml::xml_document<char> XmlDoc;
typedef rapidxml::xml_node<char> XmlNode;
typedef rapidxml::xml_attribute<char> XmlAttr;

typedef rapidxml::xml_document<suic::Char> UXmlDoc;
typedef rapidxml::xml_node<suic::Char> UXmlNode;
typedef rapidxml::xml_attribute<suic::Char> UXmlAttr;

typedef suic::shared<ResNode> ResNodePtr;

int ReadResFromFile(const char* filename, const char* mode, Mulstr& buff);

class FileWriter
{
public:

    FileWriter()
    {
        _f = NULL;
    }

    ~FileWriter()
    {
        Close();
    }

    void Write(const String& info)
    {
        if (_f != NULL)
        {
            Mulstr str(info.c_str());
            str.EncodeUtf8();
            fwrite(str.c_str(), str.Length(), 1, _f);
        }
    }

    bool Open(String strFile)
    {
        Close();

        _f = fopen(Mulstr(strFile.c_str()).c_str(), "w");

        return (NULL != _f);
    }

    void Close()
    {
        if (_f)
        {
            fclose(_f);
            _f = NULL;
        }
    }

private:

    FILE* _f;
};

//struct ResPath
//{
//    suic::String resPath;
//    // 资源ID
//    suic::String resFlag;
//    // 资源类型(res,res+,auto,file)
//    suic::String resType;
//
//    void SetPath(const String& path);
//    String GetXml(const String& resId);
//};

class ResNode : public XamlNodeItem
{
public:

    static String OFFSET1;
    static String OFFSET2;
    static String OFFSET3;
    
    RTTIOfAbstractClass(ResNode)

    virtual ~ResNode() {}

    static ResNode* CreateResNode(suic::Object* val, ResNodePtr& obj);

    virtual void CloneNode(ResNodePtr& obj);
    virtual bool IsSingleValue() = 0;
    virtual void SetValue(suic::Object* val) {}
    virtual suic::Object* GetValue() { return NULL; }

    virtual suic::String GetSingleXml()
    {
        return suic::String();
    }

    virtual suic::String GetResXml(const String& offset)
    {
        return suic::String();
    }

    virtual RTTIOfInfo* GetValueRTTIType()
    {
        if (GetValue() == NULL)
        {
            return NULL;
        }
        else
        {
            return GetValue()->GetRTTIType();
        }
    }

    suic::String GetKey() const
    {
        return _key;
    }

    void SetKey(const suic::String& key)
    {
        _key = key;
    }

    String ToString()
    {
        if (NULL == GetValue())
        {
            return GetKey();
        }
        return GetKey() + _U("(") + GetValue()->GetRTTIType()->typeName + _U(")");
    }

private:

    suic::String _key;
};

class NullResNode : public ResNode
{
public:

    static NullResNode* Value;

    RTTIOfClass(NullResNode)

    virtual bool IsSingleValue()
    {
        return true;
    }

    virtual void SetValue(suic::Object* val)
    {
    }

    virtual suic::Object* GetValue()
    {
        return DpProperty::UnsetValue();
    }

    virtual suic::String GetSingleXml()
    {
        return suic::String();
    }

    virtual suic::String GetResXml(const String& offset)
    {
        return suic::String();
    }

private:

    NullResNode()
    {
        ref();
        setAutoDelete(false);
        sealAutoDelete();
    }
};

class ResValueNode : public ResNode
{
public:

    RTTIOfAbstractClass(ResValueNode)

    ResValueNode();
    ResValueNode(suic::Object* val);
    virtual ~ResValueNode();

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();

protected:

    void Dispose();

private:

    suic::Object* _value;
};

class SingleResNode : public ResValueNode
{
public:

    RTTIOfClass(SingleResNode)

    SingleResNode();
    SingleResNode(suic::Object* val);
    virtual ~SingleResNode();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);
};

class StringResNode : public SingleResNode
{
public:

    RTTIOfClass(StringResNode)

    StringResNode();
    StringResNode(suic::OString* val);
    virtual ~StringResNode();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);
};

class BaseResNode : public SingleResNode
{
public:

    RTTIOfClass(BaseResNode)

    BaseResNode();
    BaseResNode(suic::Object* val);
    virtual ~BaseResNode();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);
};

class CursorResNode : public ResNode
{
public:

    RTTIOfClass(CursorResNode)

    CursorResNode();
    CursorResNode(suic::OCursor* val);
    virtual ~CursorResNode();

    static String CursorToString(suic::OCursor* pCursor);

    virtual bool IsSingleValue();
    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();

private:

    suic::OCursor* _value;
};

class ExtensionResNode : public ResNode
{
public:

    RTTIOfClass(ExtensionResNode)

    ExtensionResNode();
    ExtensionResNode(suic::Extension* val);

    virtual ~ExtensionResNode();

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);

protected:

    suic::String GetBindingModeXml(suic::Binding* binding);
    suic::String GetUpdateSourceTriggerXml(suic::Binding* binding);

private:

    suic::Extension* _value;
};

class GridViewResNode : public ResNode
{
public:

    RTTIOfClass(GridViewResNode)

    GridViewResNode();
    GridViewResNode(suic::GridView* val);

    virtual ~GridViewResNode();

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);

private:

    suic::GridView* _value;
};

class BrushResNode : public ResNode
{
public:

    RTTIOfClass(BrushResNode)

    BrushResNode();
    BrushResNode(suic::Brush* val);

    virtual ~BrushResNode();

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);

protected:

    bool GetSolidColorBrushResXml(const String& offset, String& strXml);

private:

    suic::Brush* _value;
};

class SolidColorBrushResNode : public BrushResNode
{
public:

    SolidColorBrushResNode();
    SolidColorBrushResNode(SolidColorBrush* val);

    virtual ~SolidColorBrushResNode();
    
    virtual bool IsSingleValue();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);
};

class LinearGradientBrushResNode : public BrushResNode
{
public:

    LinearGradientBrushResNode();
    LinearGradientBrushResNode(suic::LinearGradientBrush* val);

    virtual ~LinearGradientBrushResNode();

    virtual suic::String GetResXml(const String& offset);
};

class RadialGradientBrushResNode : public BrushResNode
{
public:

    RadialGradientBrushResNode();
    RadialGradientBrushResNode(suic::RadialGradientBrush* val);

    virtual ~RadialGradientBrushResNode();

    virtual suic::String GetResXml(const String& offset);
};

class ImageBrushResNode : public BrushResNode
{
public:

    ImageBrushResNode();
    ImageBrushResNode(suic::ImageBrush* val);
    virtual ~ImageBrushResNode();

    ResourceUri* GetResPath();
    suic::ImageBrush* GetImageBrush();

    void RefleshImageSource();
    void SetImageBrush(suic::ImageBrush* val);
    void SetImageSource(const String& imgSource);

    virtual suic::String GetResXml(const String& offset);

private:

    suic::ResourceUri _resPath;
};

class SourceResNode : public ResNode
{
public:

    RTTIOfClass(SourceResNode)

    SourceResNode();
    SourceResNode(suic::ImageSource* val);
    virtual ~SourceResNode();

    virtual void CloneNode(ResNodePtr& obj);

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();
    virtual suic::String GetSingleXml();

    ImageSource* GetImageSource();
    void ParseResPath(const String& path);
    void SetResPath(const String& comp, const String& path);

private:

    suic::ImageSource* _source;
};

class ImageSourceResNode : public ResNode
{
public:

    RTTIOfClass(ImageSourceResNode)

    ImageSourceResNode();
    ImageSourceResNode(suic::ImageSource* val, const suic::String& strVal);

    virtual ~ImageSourceResNode();

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();

    virtual suic::String GetSingleXml();
    virtual suic::String GetResXml(const String& offset);

private:

    suic::ImageSource* _value;
    suic::ResourceUri _resPath;
};

#endif
