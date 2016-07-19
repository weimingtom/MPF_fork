
#include "stdafx.h"

#include <Core/ResNode.h>
#include <Core/TypeManager.h>
#include <Core/XamlElementNode.h>

#include <System/Resources/StaticResource.h>
#include <System/Resources/DynamicResource.h>

ImplementRTTIOfClass(ResNode, XamlNodeItem)

ImplementRTTIOfClass(GridViewResNode, ResNode)
ImplementRTTIOfClass(NullResNode, ResNode)
ImplementRTTIOfClass(ResValueNode, ResNode)
ImplementRTTIOfClass(SingleResNode, ResNode)
ImplementRTTIOfClass(StringResNode, SingleResNode)
ImplementRTTIOfClass(BaseResNode, SingleResNode)
ImplementRTTIOfClass(CursorResNode, ResNode)
ImplementRTTIOfClass(ExtensionResNode, ResNode)
ImplementRTTIOfClass(BrushResNode, ResNode)
ImplementRTTIOfClass(SourceResNode, ResNode)
ImplementRTTIOfClass(ImageSourceResNode, ResNode)

String ResNode::OFFSET1 = _U("    ");
String ResNode::OFFSET2 = _U("        ");
String ResNode::OFFSET3 = _U("            ");

NullResNode* NullResNode::Value = new NullResNode();

ResNode* ResNode::CreateResNode(suic::Object* val, ResNodePtr& obj)
{
    if (suic::RTTICast<suic::ImageBrush>(val))
    {
        obj = new ImageBrushResNode((suic::ImageBrush*)(val));
    }
    else if (suic::RTTICast<suic::SolidColorBrush>(val))
    {
        obj = new SolidColorBrushResNode((suic::SolidColorBrush*)(val));
    }
    else if (suic::RTTICast<suic::LinearGradientBrush>(val))
    {
        obj = new LinearGradientBrushResNode((suic::LinearGradientBrush*)(val));
    }
    else if (suic::RTTICast<suic::RadialGradientBrush>(val))
    {
        obj = new RadialGradientBrushResNode((suic::RadialGradientBrush*)(val));
    }
    else if (suic::RTTICast<Extension>(val))
    {
        obj = new ExtensionResNode((Extension*)(val));
    }
    else if (suic::RTTICast<suic::GridView>(val))
    {
        obj = new GridViewResNode(suic::RTTICast<suic::GridView>(val));
    }
    else if (suic::RTTICast<suic::OCursor>(val))
    {
        obj = new CursorResNode((suic::OCursor*)(val));
    }
    else if (suic::RTTICast<ImageSource>(val))
    {
        obj = new SourceResNode((suic::ImageSource*)(val));
    }
    else if (suic::RTTICast<OString>(val))
    {
        obj = new StringResNode((suic::OString*)(val));
    }
    else if (suic::RTTICast<ResNode>(val))
    {
        obj = suic::RTTICast<ResNode>(val);
    }
    else
    {
        obj = new SingleResNode(val);
    }

    return obj.get();
}

void ResNode::CloneNode(ResNodePtr& obj)
{
    CreateResNode(GetValue(), obj);
}

ResValueNode::ResValueNode()
{
    _value = NULL;
}

ResValueNode::ResValueNode(suic::Object* val)
{
    _value = val;
    if (_value != NULL)
    {
        _value->ref();
    }
}

ResValueNode::~ResValueNode()
{
    FREEREFOBJ(_value);
}

bool ResValueNode::IsSingleValue()
{
    return true;
}

void ResValueNode::SetValue(suic::Object* val)
{
    SETREFOBJ(_value, val);
}

suic::Object* ResValueNode::GetValue()
{
    return _value;
}

void ResValueNode::Dispose()
{
    FREEREFOBJ(_value);
}

//====================================================
// SingleResNode

SingleResNode::SingleResNode()
{
}

SingleResNode::SingleResNode(suic::Object* val)
    : ResValueNode(val)
{
}

SingleResNode::~SingleResNode()
{
}

suic::String SingleResNode::GetSingleXml()
{
    if (NULL == GetValue())
    {
        return suic::String();
    }
    suic::OfRect* fObj = RTTICast<suic::OfRect>(GetValue());
    if (NULL != fObj)
    {
        suic::fRect rect = fObj->TofRect();
        return suic::String().Format(_U("%.2f,%.2f,%.2f,%.2f"), 
            rect.left, rect.top, rect.right, rect.bottom);
    }
    suic::OfPoint* pObj = RTTICast<suic::OfPoint>(GetValue());
    if (NULL != pObj)
    {
        suic::fPoint point = pObj->TofPoint();
        return suic::String().Format(_U("%.2f,%.2f"), point.x, point.y);
    }
    suic::OFloat* flObj = RTTICast<suic::OFloat>(GetValue());
    if (NULL != flObj)
    {
        return suic::String().Format(_U("%.2f"), flObj->ToFloat());
    }

    return GetValue()->ToString();
}

suic::String SingleResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    suic::String strTypeName;
    
    if (NULL != GetValue())
    {
        strTypeName = _U("sys:");
        strTypeName += GetValue()->GetRTTIType()->typeName;

        strXml = offset + _U("<");
        strXml += strTypeName;
        strXml += _U(" ");

        if (!GetKey().Empty())
        {
            strXml += _U("x:Key=\"");
            strXml += GetKey();
            strXml += _U("\">");
        }

        strXml += GetSingleXml();
        strXml += _U("</");
        strXml += strTypeName;
        strXml += _U(">\n");
    }   

    return strXml;
}


//====================================================
// StringResNode

StringResNode::StringResNode()
{
}

StringResNode::StringResNode(suic::OString* val)
    : SingleResNode(val)
{

}

StringResNode::~StringResNode()
{

}

suic::String StringResNode::GetSingleXml()
{
    if (NULL != GetValue())
    {
        return GetValue()->ToString();
    }
    return suic::String();
}

suic::String StringResNode::GetResXml(const String& offset)
{
    suic::String strXml;

    if (NULL != GetValue())
    {
        strXml = offset + _U("<sys:String ");

        if (!GetKey().Empty())
        {
            strXml += _U("x:Key=\"");
            strXml += GetKey();
            strXml += _U("\"");
        }

        strXml += _U(">");
        strXml += GetValue()->ToString();
        strXml += _U("</sys:String>\n");
    }   

    return strXml;
}

//====================================================
// BaseResNode

BaseResNode::BaseResNode()
{
}

BaseResNode::BaseResNode(suic::Object* val)
    : SingleResNode(val)
{
}

BaseResNode::~BaseResNode()
{
}

suic::String BaseResNode::GetSingleXml()
{
    return suic::String();
}

suic::String BaseResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    suic::String strTypeName;

    strTypeName = ResTypePool::GetResTypeName(GetValue()->GetRTTIType());

    strXml = offset + _U("<");
    strXml += strTypeName;
    strXml += _U(" ");

    if (!GetKey().Empty())
    {
        strXml += _U("x:Key=\"");
        strXml += GetKey();
        strXml += _U("\" ");
    }

    strXml += GetValue()->ToString();
    strXml += _U(" />\n");

    return strXml;
}

//================================================
// CursorResNode

CursorResNode::CursorResNode()
{
    _value = NULL;
}

CursorResNode::CursorResNode(suic::OCursor* val)
{
    _value = val;
    if (_value != NULL)
    {
        _value->ref();
    }
}

CursorResNode::~CursorResNode()
{
    if (_value != NULL)
    {
        _value->unref();
    }
}

String CursorResNode::CursorToString(suic::OCursor* pCursor)
{
    if (pCursor == suic::OCursor::Arrow)
    {
        return _U("Arrow");
    }
    else if (pCursor == suic::OCursor::Hand)
    {
        return _U("Hand");
    }
    else if (pCursor == suic::OCursor::Wait)
    {
        return _U("Wait");
    }
    else if (pCursor == suic::OCursor::Cross)
    {
        return _U("Cross");
    }
    else if (pCursor == suic::OCursor::Resize)
    {
        return _U("Resize");
    }
    else if (pCursor == suic::OCursor::Splith)
    {
        return _U("Splith");
    }
    else if (pCursor == suic::OCursor::Ibeam)
    {
        return _U("Ibeam");
    }
    else if (pCursor == suic::OCursor::Splitv)
    {
        return _U("Splitv");
    }
    else if (pCursor == suic::OCursor::SizeNwse)
    {
        return _U("SizeNwse");
    }
    else if (pCursor == suic::OCursor::Splitv)
    {
        return _U("Splitv");
    }
    else if (pCursor == suic::OCursor::SizeNesw)
    {
        return _U("SizeNesw");
    }
    else if (pCursor == suic::OCursor::SizeWe)
    {
        return _U("SizeWe");
    }
    else if (pCursor == suic::OCursor::SizeNs)
    {
        return _U("SizeNs");
    }
    else if (pCursor == suic::OCursor::SizeAll)
    {
        return _U("SizeAll");
    }
    else
    {
        return _U("");
    }
}

bool CursorResNode::IsSingleValue()
{
    return true;
}

suic::String CursorResNode::GetSingleXml()
{
    return CursorToString(_value);
}

suic::String CursorResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    return strXml;
}

void CursorResNode::SetValue(suic::Object* val)
{
    if (_value != NULL)
    {
        _value->unref();
        _value = NULL;
    }

    _value = RTTICast<suic::OCursor>(val);
    if (NULL != _value)
    {
        _value->ref();
    }
}

suic::Object* CursorResNode::GetValue()
{
    return _value;
}

//================================================
// ExpressionResNode

ExtensionResNode::ExtensionResNode()
{
    _value = NULL;
}

ExtensionResNode::ExtensionResNode(suic::Extension* val)
{
    _value = val;
    ADDREFOBJ(_value);
}

ExtensionResNode::~ExtensionResNode()
{
    FREEREFOBJ(_value);
}

bool ExtensionResNode::IsSingleValue()
{
    return true;
}

void ExtensionResNode::SetValue(suic::Object* val)
{
    SETREFOBJ(_value, suic::RTTICast<suic::Extension>(val));
}

suic::Object* ExtensionResNode::GetValue()
{
    return _value;
}

suic::String ExtensionResNode::GetBindingModeXml(suic::Binding* binding)
{
    suic::String strXml;

    if (binding->GetBindMode() != suic::BindingMode::TwoWay)
    {
        switch (binding->GetBindMode())
        {
        case suic::BindingMode::OneTime:
            strXml += _U("OneTime");
            break;

        case suic::BindingMode::OneWay:
            strXml += _U("OneWay");
            break;

        case suic::BindingMode::OneWayToSource:
            strXml += _U("OneWayToSource");
            break;
        }
    }

    if (!strXml.Empty())
    {
        strXml = _U("Mode=") + strXml;
    }

    return strXml;
}

suic::String ExtensionResNode::GetUpdateSourceTriggerXml(suic::Binding* binding)
{
    suic::String strXml;

    if (binding->GetUpdateSourceTrigger() != suic::UpdateSourceTrigger::ustPropertyChanged)
    {
        switch (binding->GetUpdateSourceTrigger())
        {
        case suic::UpdateSourceTrigger::ustLostFocus:
            strXml += _U("LostFocus");
            break;

        case suic::UpdateSourceTrigger::ustExplicit:
            strXml += _U("Explicit");
            break;
        }
    }

    if (!strXml.Empty())
    {
        strXml = _U("UpdateSourceTrigger=") + strXml;
    }

    return strXml;
}

suic::String ExtensionResNode::GetSingleXml()
{
    suic::Binding* binding = suic::RTTICast<suic::Binding>(GetValue());
    if (NULL != binding)
    {
        suic::String strXml;
        suic::SourceRef* srcRef = binding->GetSourceRef();

        strXml = _U("{Binding ");

        if (!binding->GetPath().Path.Empty())
        {
            strXml += _U("Path=");
            strXml += binding->GetPath().Path;
            strXml += _U(",");
        }

        if (!binding->GetConverterName().Empty())
        {
            strXml += _U("Converter=");
            strXml += binding->GetConverterName();
            strXml += _U(",");
        }

        if (!binding->GetConverterParam().Empty())
        {
            strXml += _U("ConverterParam=");
            strXml += binding->GetConverterParam();
            strXml += _U(",");
        }

        if (srcRef != NULL)
        {
            suic::RelativeSource* relativeRef = dynamic_cast<suic::RelativeSource*>(srcRef);
            suic::String strMode = GetBindingModeXml(binding);
            suic::String strTrigger = GetUpdateSourceTriggerXml(binding);

            if (!strMode.Empty())
            {
                strXml += strMode;
                strXml += _U(",");
            }

            if (!strTrigger.Empty())
            {
                strXml += strTrigger;
                strXml += _U(",");
            }

            if (NULL != relativeRef)
            {
                strXml += _U("RelativeSource=");
                if (relativeRef->Mode == RelativeSourceMode::Self)
                {
                    strXml += _U("{RelativeSource Self}");
                }
                else if (relativeRef->Mode == RelativeSourceMode::TemplatedParent)
                {
                    strXml += _U("{RelativeSource TemplatedParent}");
                }
                else if (relativeRef->Mode == RelativeSourceMode::FindAncestor)
                {
                    strXml += _U("{RelativeSource FindAncestor,AncestorType={x:Type ");
                    strXml += relativeRef->AncestorType->typeName;
                    strXml += _U("}, AncestorLevel=");
                    strXml += String().FromInt(relativeRef->AncestorLevel);
                    strXml += _U("}");
                }
            }
            else
            {
                suic::ElementNameSource* elementRef = dynamic_cast<suic::ElementNameSource*>(srcRef);
                if (NULL != elementRef)
                {
                    strXml += _U("ElementName=");
                    strXml += elementRef->Name;
                }
            }
        }
        else
        {
            if (!binding->GetPath().Path.Empty())
            {
                strXml = _U("{Binding ");
                strXml += _U("Path=");
                strXml += binding->GetPath().Path;
                
                suic::String strMode = GetBindingModeXml(binding);
                suic::String strTrigger = GetUpdateSourceTriggerXml(binding);

                if (!strMode.Empty())
                {
                    strXml += _U(",");
                    strXml += strMode;
                }

                if (!strTrigger.Empty())
                {
                    strXml += _U(",");
                    strXml += strTrigger;
                }
            }
        }

        strXml += _U("}");
        return strXml;
    }

    suic::SRExtension* staticRes = suic::RTTICast<suic::SRExtension>(GetValue());
    if (NULL != staticRes)
    {
        String strXml = _U("{StaticResource ");

        strXml += staticRes->GetResourceKey();
        strXml += _U("}");
        return strXml;
    }

    suic::DRExtension* dynamicRes = suic::RTTICast<suic::DRExtension>(GetValue());
    if (NULL != dynamicRes)
    {
        String strXml = _U("{DynamicResource ");

        strXml += dynamicRes->GetResourceKey();
        strXml += _U("}");
        return strXml;
    }

    suic::TemplateBindingExtension* tempRes = suic::RTTICast<suic::TemplateBindingExtension>(GetValue());
    if (NULL != tempRes && NULL != tempRes->GetProp())
    {
        String strXml = _U("{TemplateBinding ");

        strXml += tempRes->GetProp()->GetName();
        strXml += _U("}");
        return strXml;
    }

    return String();
}

suic::String ExtensionResNode::GetResXml(const String& offset)
{
    return suic::String();
}

//================================================
// GridViewResNode

GridViewResNode::GridViewResNode()
{
}

GridViewResNode::GridViewResNode(suic::GridView* val)
{
}

GridViewResNode::~GridViewResNode()
{
}

bool GridViewResNode::IsSingleValue()
{
    return false;
}

void GridViewResNode::SetValue(suic::Object* val)
{
}

suic::Object* GridViewResNode::GetValue()
{
    return _value;
}

suic::String GridViewResNode::GetSingleXml()
{
    suic::String strXml;
    return strXml;
}

suic::String GridViewResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    return strXml;
}

//================================================
// BrushResNode

BrushResNode::BrushResNode()
{
    _value = NULL;
}

BrushResNode::BrushResNode(suic::Brush* val)
{
    _value = val;
    ADDREFOBJ(_value);
}

BrushResNode::~BrushResNode()
{
    FREEREFOBJ(_value);
}

bool BrushResNode::IsSingleValue()
{
    return false;
}

void BrushResNode::SetValue(suic::Object* val)
{
    SETREFOBJ(_value, suic::RTTICast<suic::Brush>(val));
}

suic::Object* BrushResNode::GetValue()
{
    return _value;
}

suic::String BrushResNode::GetSingleXml()
{
    return suic::String();
}

suic::String BrushResNode::GetResXml(const String& offset)
{
    return suic::String();
}

bool BrushResNode::GetSolidColorBrushResXml(const String& offset, String& strXml)
{
    SolidColorBrush* brush = RTTICast<SolidColorBrush>(GetValue());

    if (NULL == brush)
    {
        return false;
    }

    strXml = offset + _U("<SolidColorBrush ");

    if (!GetKey().Empty())
    {
        strXml += _U("x:Key=\"");
        strXml += GetKey();
        strXml += _U("\" ");
    }

    strXml += _U("Color=\"");
    strXml += GetValue()->ToString();
    strXml += _U("\"/>\n");

    return true;
}

//================================================
// SolidColorBrushResNode

SolidColorBrushResNode::SolidColorBrushResNode()
{
}

SolidColorBrushResNode::SolidColorBrushResNode(SolidColorBrush* val)
    : BrushResNode(val)
{
}

SolidColorBrushResNode::~SolidColorBrushResNode()
{
}

bool SolidColorBrushResNode::IsSingleValue()
{
    return true;
}

suic::String SolidColorBrushResNode::GetSingleXml()
{
    suic::SolidColorBrush* brush = suic::RTTICast<suic::SolidColorBrush>(GetValue());
    suic::String strXml = Color::ToString(brush->GetColor());//suic::String::Int32ToHex(brush->GetColor());
    return strXml;
}

suic::String SolidColorBrushResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    suic::SolidColorBrush* brush = RTTICast<SolidColorBrush>(GetValue());

    if (NULL != brush)
    {
        strXml = offset + _U("<SolidColorBrush ");

        if (!GetKey().Empty())
        {
            strXml += _U("x:Key=\"");
            strXml += GetKey();
            strXml += _U("\" ");
        }

        strXml += _U("Color=\"");
        strXml += Color::ToString(brush->GetColor());
        strXml += _U("\"/>\n");
    }

    return strXml;
}

//================================================
// LinearGradientBrushResNode

LinearGradientBrushResNode::LinearGradientBrushResNode()
{
}

LinearGradientBrushResNode::LinearGradientBrushResNode(suic::LinearGradientBrush* val)
    : BrushResNode(val)
{
}

LinearGradientBrushResNode::~LinearGradientBrushResNode()
{
}

suic::String LinearGradientBrushResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    LinearGradientBrush* brush = suic::RTTICast<LinearGradientBrush>(GetValue());
    GradientStops* linearInfo = brush->GetGradientStops();

    strXml = offset + _U("<LinearGradientBrush ");

    if (!GetKey().Empty())
    {
        strXml += _U("x:Key=\"");
        strXml += GetKey();
        strXml += _U("\" ");
    }

    strXml += _U("StartPoint=\"");
    strXml += String().Format(_U("%.2f,%.2f"), brush->GetStartPoint().x, brush->GetStartPoint().y);
    strXml += _U("\" EndPoint=\"");
    strXml += String().Format(_U("%.2f,%.2f"), brush->GetEndPoint().x, brush->GetEndPoint().y);

    strXml += _U("\">\n");

    for (int i = 0; i < linearInfo->colors.GetCount(); ++i)
    {
        strXml += offset + ResNode::OFFSET1 + _U("<GradientStop Offset=\"");
        strXml += String().Format(_U("%.2f"), linearInfo->ratios[i]);
        strXml += _U("\" Color=\"");
        strXml += Color::ToString(linearInfo->colors[i]);
        strXml += _U("\" />\n");
    }

    strXml += offset + _U("</LinearGradientBrush>\n");

    return strXml;
}

//================================================
// RadialGradientBrushResNode

RadialGradientBrushResNode::RadialGradientBrushResNode()
{
}

RadialGradientBrushResNode::RadialGradientBrushResNode(suic::RadialGradientBrush* val)
    : BrushResNode(val)
{
}

RadialGradientBrushResNode::~RadialGradientBrushResNode()
{
}

suic::String RadialGradientBrushResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    RadialGradientBrush* brush = suic::RTTICast<RadialGradientBrush>(GetValue());
    GradientStops* radialInfo = brush->GetGradientStops();

    strXml = offset + _U("<RadialGradientBrush ");

    if (!GetKey().Empty())
    {
        strXml += _U("x:Key=\"");
        strXml += GetKey();
        strXml += _U("\" ");
    }

    strXml += _U("GradientOrigin=\"");
    strXml += String().Format(_U("%.2f,%.2f"), brush->GetOrigin().x, brush->GetOrigin().y);
    strXml += _U("\" RadiusX=\"");
    strXml += String().Format(_U("%.2f"), brush->GetRadiusX());
    strXml += _U("\" RadiusY=\"");
    strXml += String().Format(_U("%.2f"), brush->GetRadiusY());

    strXml += _U("\">\n");

    for (int i = 0; i < radialInfo->colors.GetCount(); ++i)
    {
        strXml += offset + ResNode::OFFSET1 + _U("<GradientStop Offset=\"");
        strXml += String().Format(_U("%.2f"), radialInfo->ratios[i]);
        strXml += _U("\" Color=\"");
        strXml += Color::ToString(radialInfo->colors[i]);//String::Int32ToHex(radialInfo->colors[i]);
        strXml += _U("\" />\n");
    }

    strXml += offset + _U("</RadialGradientBrush>\n");

    return strXml;
}

//================================================
// ImageBrushResNode

/*void ResPath::SetPath(const String& path)
{
    String strSource = path;
    strSource.Trim();
    int pos = strSource.IndexOf(_U("@"));

    if (pos != -1)
    {
        resPath = strSource.Substring(pos + 1);
        StringArray strArr;
        strArr.SplitString(strSource.Substring(0, pos), _U(","));
        if (strArr.GetCount() == 2)
        {
            resType = strArr[0];
            resFlag = strArr[1];
        }
    }
    else
    {
        resPath = strSource;
    }

    resPath = resPath.Replace(_U("\\"), _U("/"));
    resPath.Trim();
    resFlag.Trim();
}

String ResPath::GetXml(const String& resId)
{
    String strXml;

    if (!resType.Empty())
    {
        strXml += resType;
        strXml += _U(",");
        strXml += resFlag;
        strXml += _U("@");
        strXml += resPath;
    }
    else if (!resId.Empty())
    {
        strXml += _U("res+,");
        strXml += resId;
        strXml += _U("@");
        strXml += resPath;

        resType = _U("res+");
        resFlag = resId;
    }
    else
    {
        strXml += resPath;
    }

    return strXml;
}*/

ImageBrushResNode::ImageBrushResNode()
{
}

ImageBrushResNode::ImageBrushResNode(suic::ImageBrush* val)
    : BrushResNode(NULL)
{
    SetImageBrush(val);
}

ImageBrushResNode::~ImageBrushResNode()
{
}

ResourceUri* ImageBrushResNode::GetResPath()
{
    return (&_resPath);
}

void ImageBrushResNode::SetImageBrush(suic::ImageBrush* val)
{
    SetValue(val);
    if (NULL != val->GetImageSource())
    {
        _resPath = val->GetImageSource()->GetUri();
    }
}

void ImageBrushResNode::SetImageSource(const String& imgSource)
{
    _resPath.Parse(imgSource);
    suic::ImageBrush* imgBr = GetImageBrush();

    if (NULL == imgBr)
    {
        imgBr = new suic::ImageBrush();
        SetValue(imgBr);
    }

    if (NULL != imgBr)
    {
        imgBr->SetImageSource(new ImageSource(imgSource));
    }
}

suic::ImageBrush* ImageBrushResNode::GetImageBrush()
{
    return RTTICast<suic::ImageBrush>(GetValue());
}

void ImageBrushResNode::RefleshImageSource()
{
    suic::ImageBrush* imgBr = GetImageBrush();

    if (NULL != imgBr)
    {
        imgBr->SetImageSource(new ImageSource(_resPath.ToString()));
    }
}

suic::String ImageBrushResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    const suic::fRect defViewbox(0, 0, 1, 1);

    ImageBrush* brush = suic::RTTICast<ImageBrush>(GetValue());

    strXml = offset + _U("<ImageBrush ");

    if (!GetKey().Empty())
    {
        strXml += _U("x:Key=\"");
        strXml += GetKey();
        strXml += _U("\" ");
    }

    strXml += _U("ImageSource=\"");
    strXml += _resPath.ToString();

    bool bEqual = (brush->GetViewport() == defViewbox);

    if (!bEqual)
    {
        strXml += _U("\" Viewport=\"");
        strXml += String().Format(_U("%.1f,%.1f,%.1f,%.1f")
            , brush->GetViewport().left
            , brush->GetViewport().top
            , brush->GetViewport().right
            , brush->GetViewport().bottom);
    }

    if (brush->GetViewportUnits() == BrushMappingMode::bmmAbsolute)
    {
        strXml += _U("\" ViewportUnits=\"Absolute");
    }

    bEqual = (brush->GetViewbox() == defViewbox);

    if (!bEqual)
    {
        strXml += _U("\" Viewbox=\"");
        strXml += String().Format(_U("%.1f,%.1f,%.1f,%.1f")
            , brush->GetViewbox().left
            , brush->GetViewbox().top
            , brush->GetViewbox().right
            , brush->GetViewbox().bottom);
    }

    if (brush->GetViewboxUnits() == BrushMappingMode::bmmAbsolute)
    {
        strXml += _U("\" ViewboxUnits=\"Absolute");
    }

    if (!brush->GetViewCorner().IsZero())
    {
        strXml += _U("\" Viewcorner=\"");
        strXml += String().Format(_U("%d,%d,%d,%d")
            , brush->GetViewCorner().left
            , brush->GetViewCorner().top
            , brush->GetViewCorner().right
            , brush->GetViewCorner().bottom);
    }

    if (brush->GetStretch() != Stretch::stFill)
    {
        strXml += _U("\" Stretch=\"");
        switch (brush->GetStretch())
        {
            case Stretch::stUniform:
                strXml += _U("Uniform");
                break;

            case Stretch::stUniformToFill:
                strXml += _U("UniformToFill");
                break;

            default:
                strXml += _U("None");
        }
    }

    if (brush->GetTileMode() != TileMode::tmNone)
    {
        strXml += _U("\" TileMode=\"");
        switch (brush->GetTileMode())
        {
        case TileMode::tmFlipX:
            strXml += _U("FlipX");
            break;

        case TileMode::tmFlipY:
            strXml += _U("FlipY");
            break;

        case TileMode::tmFlipXY:
            strXml += _U("FlipXY");
            break;

        default:
            strXml += _U("Tile");
        }
    }

    strXml += _U("\" />\n");

    return strXml;
}

//==========================================

SourceResNode::SourceResNode()
{
    _source = NULL;
}

SourceResNode::SourceResNode(suic::ImageSource* val)
{
    _source = val;
    ADDREFOBJ(_source);
}

SourceResNode::~SourceResNode()
{
    FREEREFOBJ(_source);
}

void SourceResNode::CloneNode(ResNodePtr& obj)
{
    if (NULL != _source)
    {
        suic::ImageSource* src = new suic::ImageSource();
        src->SetUri(_source->GetUri());
        obj = new SourceResNode(src);
    }
    else
    {
        obj = new SourceResNode();
    }
}

bool SourceResNode::IsSingleValue()
{
    return true;
}

suic::Object* SourceResNode::GetValue()
{
    return _source;
}

void SourceResNode::SetValue(suic::Object* val)
{

}

suic::String SourceResNode::GetSingleXml()
{
    if (NULL != _source)
    {
        return _source->GetUri().ToString();
    }
    return suic::String();
}

ImageSource* SourceResNode::GetImageSource()
{
    if (NULL == _source)
    {
        _source = new ImageSource();
        _source->ref();
    }
    return _source;
}

void SourceResNode::ParseResPath(const String& path)
{
    GetImageSource()->SetUri(path);
}

void SourceResNode::SetResPath(const String& comp, const String& path)
{
    ResourceUri uri;
    
    uri.SetComponent(comp);
    uri.SetResourcePath(path);
    GetImageSource()->SetUri(uri);
}

//===================================================================
// ImageSourceResNode

ImageSourceResNode::ImageSourceResNode()
{
    _value = NULL;
}

ImageSourceResNode::ImageSourceResNode(suic::ImageSource* val, const suic::String& strVal)
{
    _value = val;
    if (_value != NULL)
    {
        _value->ref();
    }
    _resPath.Parse(strVal);
}

ImageSourceResNode::~ImageSourceResNode()
{

}

bool ImageSourceResNode::IsSingleValue()
{
    return true;
}

void ImageSourceResNode::SetValue(suic::Object* val)
{
    if (val != NULL)
    {
        val->ref();
    }

    if (_value != NULL)
    {
        _value->unref();
    }

    _value = RTTICast<suic::ImageSource>(val);

    if (_value != NULL)
    {
        _value->ref();
    }

    if (val != NULL)
    {
        val->unref();
    }
}

suic::Object* ImageSourceResNode::GetValue()
{
    return _value;
}

suic::String ImageSourceResNode::GetSingleXml()
{
    suic::String strXml;

    return strXml;
}

suic::String ImageSourceResNode::GetResXml(const String& offset)
{
    suic::String strXml;
    
    strXml = offset;
    strXml += _U("<ImageSource x:Key=\"");
    strXml += GetKey();
    strXml += _U("\" Source=\"");
    strXml += _resPath.ToString();
    strXml += _U("\" />\n");

    return strXml;
}