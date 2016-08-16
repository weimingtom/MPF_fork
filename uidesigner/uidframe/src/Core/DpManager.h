// 华勤科技版权所有 2010-2011
// 
// 文件名：DpManager.h
// 功  能：管理可编辑的属性。
// 
// 作  者：汪荣
// 时  间：2013-01-02
// 
// ============================================================================

#ifndef _DPMANAGER_H_
#define _DPMANAGER_H_

#include <Core/ResNode.h>
#include <Main/XamlRootItem.h>
#include <System/Types/Const.h>

struct DpItem : public Object
{
    suic::String name;
    // 属性编辑器名字
    suic::String editor;
    // 属性类型
    suic::RTTIOfInfo* type;
    // 对应属性个数
    suic::Array<DpProperty*> dps;

    DpItem(const String& propName, suic::RTTIOfInfo* t)
    {
        name = propName;
        type = t;
    }
   
    String ToString()
    {
        return name;
    }

    // 获取属性注册时的元数据
    PropMetadata* GetMetatata(suic::RTTIOfInfo* owner)
    {
        PropMetadata* meta = NULL;
        for (int i = 0; i < dps.GetCount(); ++i)
        {
            meta = dps[i]->FindMetadata(owner);
            if (meta != NULL)
            {
                break;
            }
        }
        return meta;
    }

    // 根据创建属性时的Owner类型查询对应的属性
    // 如果bAttach为true则返回附加属性
    DpProperty* GetDp(suic::RTTIOfInfo* owner, bool bAttach)
    {
        PropMetadata* meta = NULL;
        for (int i = 0; i < dps.GetCount(); ++i)
        {
            meta = dps[i]->FindMetadata(owner);
            if (meta != NULL || (dps[i]->IsAttached() && bAttach))
            {
                return dps[i];
            }
        }
        return NULL;
    }

    DpProperty* GetDpEx(suic::RTTIOfInfo* owner, bool bAttach, bool& bOwner)
    {
        PropMetadata* meta = NULL;
        bOwner = false;

        for (int i = 0; i < dps.GetCount(); ++i)
        {
            meta = dps[i]->FindMetadata(owner);
            if (meta != NULL)
            {
                bOwner = true;
                return dps[i];
            }
            else if (dps[i]->IsAttached() && bAttach)
            {
                return dps[i];
            }
        }
        return NULL;
    }

    void AddDp(DpProperty* dp)
    {
        dps.Add(dp);
    }

    virtual String GetEditorName();
    virtual int GetSelectIndex();

    virtual suic::String ValueToString(ResNode* val)
    {
        return val->GetSingleXml();
    }

    virtual void FillAddChild(IAddChild* sender) {}
};

// 定义查询依赖属性的条件结构
struct QueryDpCond
{
    RTTIOfInfo* target;
    
    // 依赖属性的返回值集合
    ItemCollection* propFilter;
    ItemCollection* trgFilter;

    // 是否是Trigger里的属性
    bool fromTrigger;
    // 是否是模版里的属性
    bool inTemplate;
    // 是否跳过模版
    bool filterTemp;

    QueryDpCond()
    {
        target = NULL;
        propFilter = NULL;
        trgFilter = NULL;

        fromTrigger = false;
        inTemplate = false;
        filterTemp = false;
    }
};

/// <Summary>
///  统一管理系统中所有的依赖属性，因此在设计器里必须先
///  注册控件和其定义的依赖属性，这样，才可以自动把需要编辑的依赖
///  纳入统一的管理。
/// </Summary>
class DpManager
{
public:

    static DpManager* Ins();

    static suic::String NameFromDb(DpProperty* dp);

    /// <summary>
    ///   根据依赖属性名称查询对应的所有依赖属性
    /// </summary>
    /// <remarks>
    ///   名称相同的依赖属性可以属于不同的控件对象
    /// </remarks>
    /// <param name="name">依赖属性名称</param>
    /// <returns>依赖属性列表集合</returns>
    DpItem* FindDpItem(const String& name);
    DpItem* FindDpItem(DpProperty* dp);
    DpProperty* FindDp(const String& name);
    DpProperty* FindDp(const String& name, RTTIOfInfo* owner, bool isAttach);
    String GetEditorName(DpItem* pItem);

    void FillPropByDp(ItemCollection* itemColl, RTTIOfInfo* owner, DpProperty* dp);

    bool ExistOnItemCollection(ItemCollection* filterColl, DpItem* dpItem);
    void QueryDpItems(QueryDpCond* cond, ItemCollection* result);

    void GetDpItems(Collection<DpItem*>& dpItems)
    {
        _dpItems.GetValues(dpItems);
    }

private:

    void AddDpItem(DpItem* pItem);

    void InitFilterDps();
    void MergeDpItems();
    void InitDefaultDpItems();

private:

    Collection<DpProperty*> _dps;
    Collection<DpItem*> _dpItemColl;
    StringAutoDic<DpItem> _dpItems;

    StringDic<int> _filterDps;
    PointerDic<DpProperty*, int> _filterTagDps;
    StringDic<int> _hideDps;

    Collection<DpItem*> _trgOnlyDps;
};

struct OpacityDpItem : public DpItem
{
public:

    OpacityDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : DpItem(propName, t)
    {
        editor = _U("Ratio");
        name = propName;
    }
};

struct ToolTipDpItem : public DpItem
{
public:

    ToolTipDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : DpItem(propName, t)
    {
        editor = _U("String");
        name = propName;
    }
};

struct WidthDpItem : public DpItem
{
public:

    WidthDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : DpItem(propName, t)
    {
        editor = _U("Width");
        name = propName;
    }

    virtual suic::String ValueToString(ResNode* val)
    {
        suic::Integer* pInteger = RTTICast<suic::Integer>(val->GetValue());
        
        if (NULL != pInteger && pInteger->ToInt() < 0)
        {
            return _U("Auto");
        }
        else
        {
            return DpItem::ValueToString(val);
        }
    }
};

struct ChoiceDpItem : public DpItem
{
public:

    ChoiceDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : DpItem(propName, t)
    {
        editor = _U("ComboBox");
        name = propName;
    }

    virtual suic::String ValueToString(ResNode* val)
    {
        suic::String strXml;
        ExtensionResNode* exten = RTTICast<ExtensionResNode>(val);
        if (NULL != exten)
        {
            strXml = exten->GetSingleXml();
        }
        return strXml;
    }

    String GetEditorName()
    {
        return _U("ComboBox");
    }

    suic::String ValueToXml(ResNode* val)
    {
        return ValueToString(val);
    }

    int GetChoiceIndex(ResNode* val, Integer* pDef)
    {
        int iVal = 0;

        pDef->ref();

        if (NULL == val->GetValue())
        {
            val->SetValue(pDef);
        }

        pDef->unref();
        
        iVal = val->GetValue()->ToInt();
        return iVal;
    }
};

struct AlignVertDpItem : public ChoiceDpItem
{
public:

    AlignVertDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
        editor = _U("AlignVert");
        name = propName;
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, VertAlignBox::StretchBox);

        if (iVal == VertAlign::vaCenter)
        {
            return _U("Center");
        }
        else if (iVal == VertAlign::vaTop)
        {
            return _U("Top");
        }
        else if (iVal == VertAlign::vaBottom)
        {
            return _U("Bottom");
        }
        else
        {
            return _U("Stretch");
        }
    }
};

struct AlignHoriDpItem : public ChoiceDpItem
{
public:

    AlignHoriDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
        editor = _U("AlignHori");
        name = propName;
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, HoriAlignBox::StretchBox);

        if (iVal == HoriAlign::haCenter)
        {
            return _U("Center");
        }
        else if (iVal == HoriAlign::haLeft)
        {
            return _U("Left");
        }
        else if (iVal == HoriAlign::haRight)
        {
            return _U("Right");
        }
        else
        {
            return _U("Stretch");
        }
    }
};

struct DockDpItem : public ChoiceDpItem
{
public:

    DockDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Left"));
        sender->AddText(_U("Top"));
        sender->AddText(_U("Right"));
        sender->AddText(_U("Bottom"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, DockBox::LeftBox);

        if (iVal == Dock::dockTop)
        {
            return _U("Top");
        }
        else if (iVal == Dock::dockRight)
        {
            return _U("Right");
        }
        else if (iVal == Dock::dockBottom)
        {
            return _U("Bottom");
        }
        else
        {
            return _U("Left");
        }
    }
};

struct HoriAlignDpItem : public ChoiceDpItem
{
public:

    HoriAlignDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Left"));
        sender->AddText(_U("Right"));
        sender->AddText(_U("Center"));
        sender->AddText(_U("Stretch"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, HoriAlignBox::StretchBox);

        if (iVal == HoriAlign::haLeft)
        {
            return _U("Left");
        }
        else if (iVal == HoriAlign::haRight)
        {
            return _U("Right");
        }
        else if (iVal == HoriAlign::haCenter)
        {
            return _U("Center");
        }
        else
        {
            return _U("Stretch");
        }
    }
};

struct VertAlignDpItem : public ChoiceDpItem
{
public:

    VertAlignDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Top"));
        sender->AddText(_U("Bottom"));
        sender->AddText(_U("Center"));
        sender->AddText(_U("Stretch"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, VertAlignBox::StretchBox);

        if (iVal == VertAlign::vaTop)
        {
            return _U("Top");
        }
        else if (iVal == VertAlign::vaBottom)
        {
            return _U("Bottom");
        }
        else if (iVal == VertAlign::vaCenter)
        {
            return _U("Center");
        }
        else
        {
            return _U("Stretch");
        }
    }
};

struct HitTestFilterActionDpItem : public ChoiceDpItem
{
public:

    HitTestFilterActionDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Continue"));
        sender->AddText(_U("Stop"));
        sender->AddText(_U("ContinueSkipSelf"));
        sender->AddText(_U("ContinueSkipChildren"));
        sender->AddText(_U("ContinueSkipSelfAndChildren"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, HitTestFilterAction::ContinueBox);

        if (iVal == HitTestFilterAction::Stop)
        {
            return _U("Stop");
        }
        else if (iVal == HitTestFilterAction::ContinueSkipSelf)
        {
            return _U("ContinueSkipSelf");
        }
        else if (iVal == HitTestFilterAction::ContinueSkipChildren)
        {
            return _U("ContinueSkipChildren");
        }
        else if (iVal == HitTestFilterAction::ContinueSkipSelfAndChildren)
        {
            return _U("ContinueSkipSelfAndChildren");
        }
        else
        {
            return _U("Continue");
        }
    }
};

struct OrientationDpItem : public ChoiceDpItem
{
public:

    OrientationDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Horizontal"));
        sender->AddText(_U("Vertical"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, OrientationBox::HorizontalBox);

        if (iVal == Orientation::Horizontal)
        {
            return _U("Horizontal");
        }
        else
        {
            return _U("Vertical");
        }
    }
};

struct VisibilityDpItem : public ChoiceDpItem
{
public:

    VisibilityDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Visible"));
        sender->AddText(_U("Hidden"));
        sender->AddText(_U("Collapsed"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, VisibilityBox::VisibleBox);

        if (iVal == Visibility::Collapsed)
        {
            return _U("Collapsed");
        }
        else if (iVal == Visibility::Hidden)
        {
            return _U("Hidden");
        }
        else
        {
            return _U("Visible");
        }
    }
};

struct StretchDpItem : public ChoiceDpItem
{
public:

    StretchDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("None"));
        sender->AddText(_U("Fill"));
        sender->AddText(_U("Uniform"));
        sender->AddText(_U("UniformToFill"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, StretchBox::NoneBox);

        if (iVal == Stretch::stFill)
        {
            return _U("Fill");
        }
        else if (iVal == Stretch::stUniform)
        {
            return _U("Uniform");
        }
        else if (iVal == Stretch::stUniformToFill)
        {
            return _U("UniformToFill");
        }
        else
        {
            return _U("None");
        }
    }
};

struct StretchDirectionDpItem : public ChoiceDpItem
{
public:

    StretchDirectionDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("UpOnly"));
        sender->AddText(_U("DownOnly"));
        sender->AddText(_U("Both"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, StretchDirectionBox::BothBox);

        if (iVal == StretchDirection::sdUpOnly)
        {
            return _U("UpOnly");
        }
        else if (iVal == StretchDirection::sdDownOnly)
        {
            return _U("DownOnly");
        }
        else
        {
            return _U("Both");
        }
    }
};

struct WindowStateDpItem : public ChoiceDpItem
{
public:

    WindowStateDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Normal"));
        sender->AddText(_U("Minimized"));
        sender->AddText(_U("Maximized"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, WindowStateBox::NormalBox);

        if (iVal == WindowState::wsMinimized)
        {
            return _U("Minimized");
        }
        else if (iVal == WindowState::wsMaximized)
        {
            return _U("Maximized");
        }
        else
        {
            return _U("Normal");
        }
    }
};

struct WindowStyleDpItem : public ChoiceDpItem
{
public:

    WindowStyleDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("None"));
        sender->AddText(_U("SingleBorderWindow"));
        sender->AddText(_U("ThreeDBorderWindow"));
        sender->AddText(_U("ToolWindow"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, WindowStyleBox::NoneBox);

        if (iVal == WindowStyle::wsSingleBorderWindow)
        {
            return _U("SingleBorderWindow");
        }
        else if (iVal == WindowStyle::wsThreeDBorderWindow)
        {
            return _U("ThreeDBorderWindow");
        }
        else if (iVal == WindowStyle::wsToolWindow)
        {
            return _U("ToolWindow");
        }
        else
        {
            return _U("None");
        }
    }
};

struct ResizeModeDpItem : public ChoiceDpItem
{
public:

    ResizeModeDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("NoResize"));
        sender->AddText(_U("CanMinimize"));
        sender->AddText(_U("CanResize"));
        sender->AddText(_U("CanResizeWithGrip"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, ResizeModeBox::NoResizeBox);

        if (iVal == ResizeMode::CanMinimize)
        {
            return _U("CanMinimize");
        }
        else if (iVal == ResizeMode::CanResize)
        {
            return _U("CanResize");
        }
        else if (iVal == ResizeMode::CanResizeWithGrip)
        {
            return _U("CanResizeWithGrip");
        }
        else
        {
            return _U("NoResize");
        }
    }
};

struct PlacementDpItem : public ChoiceDpItem
{
public:

    PlacementDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Left"));
        sender->AddText(_U("Top"));
        sender->AddText(_U("Right"));
        sender->AddText(_U("Bottom"));

        sender->AddText(_U("Center"));
        sender->AddText(_U("Mouse"));
        sender->AddText(_U("MousePoint"));
        sender->AddText(_U("Custom"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, PlacementModeBox::MouseBox);

        if (iVal == PlacementMode::pmLeft)
        {
            return _U("Left");
        }
        else if (iVal == PlacementMode::pmTop)
        {
            return _U("Top");
        }
        else if (iVal == PlacementMode::pmRight)
        {
            return _U("Right");
        }
        else if (iVal == PlacementMode::pmBottom)
        {
            return _U("Bottom");
        }
        else if (iVal == PlacementMode::pmCenter)
        {
            return _U("Center");
        }
        else if (iVal == PlacementMode::pmMousePoint)
        {
            return _U("MousePoint");
        }
        else if (iVal == PlacementMode::pmCustom)
        {
            return _U("Custom");
        }
        else
        {
            return _U("Mouse");
        }
    }
};

struct TextAlignmentDpItem : public ChoiceDpItem
{
public:

    TextAlignmentDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Left"));
        sender->AddText(_U("Right"));
        sender->AddText(_U("Center"));
        sender->AddText(_U("Justify"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, new Integer(TextAlignment::tLeft));

        if (iVal == TextAlignment::tLeft)
        {
            return _U("Left");
        }
        else if (iVal == TextAlignment::tRight)
        {
            return _U("Right");
        }
        else if (iVal == TextAlignment::tCenter)
        {
            return _U("Center");
        }
        else
        {
            return _U("Justify");
        }
    }
};

struct TextWrappingDpItem : public ChoiceDpItem
{
public:

    TextWrappingDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("NoWrap"));
        sender->AddText(_U("Wrap"));
        sender->AddText(_U("WrapWithOverflow"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, new Integer(TextWrapping::tNoWrap));

        if (iVal == TextWrapping::tWrap)
        {
            return _U("Wrap");
        }
        else if (iVal == TextWrapping::tWrapWithOverflow)
        {
            return _U("WrapWithOverflow");
        }
        else
        {
            return _U("NoWrap");
        }
    }
};

struct ScrollBarVisibilityDpItem : public ChoiceDpItem
{
public:

    ScrollBarVisibilityDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Disabled"));
        sender->AddText(_U("Auto"));
        sender->AddText(_U("Hidden"));
        sender->AddText(_U("Visible"));
        sender->AddText(_U("Collapsed"));
    }

    virtual int GetSelectIndex()
    {
        return 1;
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, new Integer(ScrollBarVisibility::sbvAutoScroll));

        if (iVal == ScrollBarVisibility::sbvDisabled)
        {
            return _U("Disabled");
        }
        else if (iVal == ScrollBarVisibility::sbvHidden)
        {
            return _U("Hidden");
        }
        else if (iVal == ScrollBarVisibility::sbvVisible)
        {
            return _U("Visible");
        }
        else if (iVal == ScrollBarVisibility::sbvCollapsed)
        {
            return _U("Collapsed");
        }
        else
        {
            return _U("Auto");
        }
    }
};

struct RoleDpItem : public ChoiceDpItem
{
public:

    RoleDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("TopLevelItem"));
        sender->AddText(_U("TopLevelHeader"));
        sender->AddText(_U("SubmenuItem"));
        sender->AddText(_U("SubmenuHeader"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, MenuItemRoleBox::TopLevelItemBox);

        if (iVal == MenuItemRole::TopLevelHeader)
        {
            return _U("TopLevelHeader");
        }
        else if (iVal == MenuItemRole::SubmenuItem)
        {
            return _U("SubmenuItem");
        }
        else if (iVal == MenuItemRole::SubmenuHeader)
        {
            return _U("SubmenuHeader");
        }
        else
        {
            return _U("TopLevelItem");
        }
    }
};

struct ExpandDirectionDpItem : public ChoiceDpItem
{
public:

    ExpandDirectionDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Left"));
        sender->AddText(_U("Right"));
        sender->AddText(_U("Up"));
        sender->AddText(_U("Down"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, Integer::GetPosInt(ExpandDirection::edDown));

        if (iVal == ExpandDirection::edLeft)
        {
            return _U("Left");
        }
        else if (iVal == ExpandDirection::edRight)
        {
            return _U("Right");
        }
        else if (iVal == ExpandDirection::edUp)
        {
            return _U("Up");
        }
        else
        {
            return _U("Down");
        }
    }
};

//========================================================
struct FontWeightDpItem : public ChoiceDpItem
{
public:

    FontWeightDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Black"));
        sender->AddText(_U("Bold"));
        sender->AddText(_U("Light"));
        sender->AddText(_U("Medium"));
        sender->AddText(_U("Normal"));
        sender->AddText(_U("Thin"));
        sender->AddText(_U("SemiBold"));
        sender->AddText(_U("ExtraBold"));
        sender->AddText(_U("ExtraBlack"));
        sender->AddText(_U("ExtraLight"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, new Integer(FontWeightStyle::fwNormal));

        if (iVal == FontWeightStyle::fwExtraBold)
        {
            return _U("ExtraBold");
        }
        else if (iVal == FontWeightStyle::fwBold)
        {
            return _U("Bold");
        }
        else
        {
            return _U("Normal");
        }
    }
};

struct FontStyleDpItem : public ChoiceDpItem
{
public:

    FontStyleDpItem(const suic::String& propName, suic::RTTIOfInfo* t)
        : ChoiceDpItem(propName, t)
    {
    }

    virtual void FillAddChild(IAddChild* sender)
    {
        sender->AddText(_U("Italic"));
        sender->AddText(_U("Normal"));
    }

    suic::String ValueToString(ResNode* val)
    {
        suic::String strXml = ChoiceDpItem::ValueToString(val);
        if (!strXml.Empty())
        {
            return strXml;
        }

        int iVal = GetChoiceIndex(val, new Integer(FontStyleStyle::fssNormal));

        if (iVal == FontStyleStyle::fssItalic)
        {
            return _U("Italic");
        }
        else
        {
            return _U("Normal");
        }
    }
};

#endif
