// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����DpManager.h
// ��  �ܣ�����ɱ༭�����ԡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
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
    // ���Ա༭������
    suic::String editor;
    // ��������
    suic::RTTIOfInfo* type;
    // ��Ӧ���Ը���
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

    // ��ȡ����ע��ʱ��Ԫ����
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

    // ���ݴ�������ʱ��Owner���Ͳ�ѯ��Ӧ������
    // ���bAttachΪtrue�򷵻ظ�������
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

// �����ѯ�������Ե������ṹ
struct QueryDpCond
{
    RTTIOfInfo* target;
    
    // �������Եķ���ֵ����
    ItemCollection* propFilter;
    ItemCollection* trgFilter;

    // �Ƿ���Trigger�������
    bool fromTrigger;
    // �Ƿ���ģ���������
    bool inTemplate;
    // �Ƿ�����ģ��
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
///  ͳһ����ϵͳ�����е��������ԣ������������������
///  ע��ؼ����䶨����������ԣ��������ſ����Զ�����Ҫ�༭������
///  ����ͳһ�Ĺ���
/// </Summary>
class DpManager
{
public:

    static DpManager* Ins();

    static suic::String NameFromDb(DpProperty* dp);

    /// <summary>
    ///   ���������������Ʋ�ѯ��Ӧ��������������
    /// </summary>
    /// <remarks>
    ///   ������ͬ���������Կ������ڲ�ͬ�Ŀؼ�����
    /// </remarks>
    /// <param name="name">������������</param>
    /// <returns>���������б���</returns>
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
    void InitPropertyOptions();

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

struct OptionDpItem : public DpItem
{
public:

    OptionDpItem(const String& name)
        : DpItem(name, suic::Integer::RTTIType())
    {
        editor = _U("ComboBox");
    }

    virtual suic::String ValueToString(ResNode* val)
    {
        suic::String strXml;
        ExtensionResNode* exten = RTTICast<ExtensionResNode>(val);
        if (NULL != exten)
        {
            strXml = exten->GetSingleXml();
        }
        else
        {
            strXml = _options[GetChoiceIndex(val, suic::Integer::GetZeroInt())];
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

    virtual void FillAddChild(IAddChild* sender)
    {
        for (int i = 0; i < _options.GetCount(); ++i)
        {
            sender->AddText(_options[i]);
        }
    }

    void AddOption(const suic::String& val)
    {
        _options.Add(val);
    }

private:

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

private:

    suic::Array<suic::String> _options;
};

struct HoriAlignDpItem : public OptionDpItem
{
public:

    HoriAlignDpItem(const suic::String& propName)
        : OptionDpItem(propName)
    {
        editor = _U("AlignHori");

        AddOption(_U("Left"));
        AddOption(_U("Right"));
        AddOption(_U("Center"));
        AddOption(_U("Stretch"));
    }
};

struct VertAlignDpItem : public OptionDpItem
{
public:

    VertAlignDpItem(const suic::String& propName)
        : OptionDpItem(propName)
    {
        editor = _U("AlignVert");

        AddOption(_U("Top"));
        AddOption(_U("Bottom"));
        AddOption(_U("Center"));
        AddOption(_U("Stretch"));
    }
};

#endif
