
#ifndef _XAMLROOTITEM_H_
#define _XAMLROOTITEM_H_

#include <Core/ResNode.h>
#include <Core/RedoUndoManager.h>
#include <Core/XamlElementNode.h>
#include <Core/TriggerCollectionNode.h>

#include <Main/FilterNode.h>

using namespace suic;

class Project;

struct RootItemData
{
    DesignElement* root;
    DesignElement* hot;
    RedoUndoManager ruMana;

    Array<DesignElement*> sels;

    RootItemData()
    {
        root = NULL;
        hot = NULL;
    }

    bool ContainsSelected(DesignElement* focused)
    {
        for (int i = 0; i < GetSelectedCount(); ++i)
        {
            if (GetSelectedElement(i) == focused)
            {
                return true;
            }
        }
        return false;
    }

    void ClearSelectedElements()
    {
        sels.Clear();
    }

    void AddSelected(DesignElement* focused)
    {
        sels.Add(focused);
    }

    void SetFocused(DesignElement* focused)
    {
        sels.Clear();
        sels.Add(focused);
    }

    int GetSelectedCount() const
    {
        return sels.GetCount();
    }

    DesignElement* GetSelectedElement(int index)
    {
        return sels.GetItem(index);
    }
};

class ObjTreeManager;

/// <Summary>
///  ��������ࣨ���粼���ļ���ͼ����Դ�ļ��ȱ����ڱ��ص��ļ���
/// </Summary>
class RootItem : public FilterNode
{
public:

    RTTIOfAbstractClass(RootItem)

    BeginMember(RootItem, FilterNode)
        MemberGetInt(ShowItem)
    EndMember()

    bool IsLeafItem()
    {
        return true;
    }

    int GetShowItem()
    {
        return Visibility::Collapsed;
    }

    RootItem();
    RootItem(const String& name, Project* pPrj);
    virtual ~RootItem();

    suic::String GetResXml(const String& offset);

    String ToString();

    bool IsModified() const;
    void SetModified(bool val);

    Project* GetProject() const;
    void SetProject(Project* pPrj);

    String GetFileName() const;
    void SetFileName(const String& name);

    ResourceUri GetResourceUri() const
    {
        return _resPath;
    }

    void SetObjTreeManager(ObjTreeManager* objTree);
    ObjTreeManager* GetObjTreeManager() const;

    bool IsLoaded() const;

    void Close();
    bool Load(bool onlyReadAppRes);
    bool CheckLoadState();

    void SetLoaded();
    void ReloadOnMemory();

    virtual void Reset() = 0;
    virtual bool Save() = 0;
    virtual bool SaveAs(const String& strPath) = 0;

protected:

    virtual bool OnLoad(bool onlyReadAppRes) = 0;
    virtual void OnClose() = 0;
    virtual bool OnLoadFromXml();

    virtual void OnModifiedChanged();

protected:

    bool _isModified;
    bool _isLoaded;

    ResourceUri _resPath;
    Project* _project;
    ObjTreeManager* _objTree;

    friend class XamlLoader;
};

class FileRootItem : public RootItem
{
public:

    RTTIOfAbstractClass(FileRootItem)
};

/// <Summary>
///  �������ļ�����Դ�ļ���Application��ResourceDictionary��Window���ļ���
/// </Summary>
class ElementRootItem : public FileRootItem
{
public:

    RTTIOfClass(ElementRootItem)

    ElementRootItem();
    ElementRootItem(DesignElement* root);
    virtual ~ElementRootItem();

    static DesignElement* HitTestElement(DesignElement* elem, Point pt);
    static SetterNode* SetElementValue(DesignElement* elem, DpProperty* dp, Object* val);

    void UpdateRootElement(DesignElement* data);

    // ���ļ��Ƿ���һ��ģ���ļ�
    bool IsTemplate() const;

    suic::String GetResXml(const String& offset);

    RootItemData* GetRootItemData() const;

    void Refresh();
    void SetRefreshFlag(bool val);

    void Reset();
    void ResetRootItem(bool bSetLoaded);
    virtual DesignElement* GetRootElement();

    virtual bool Save();
    virtual bool SaveAs(const String& strPath);

    //============================= ���ֲ�������
    void UpdateDesignWindow();
    void AddChileElement(DesignElement* pParent, DesignElement* pChild, bool bCallRedo=true);
    void ReplacePanel(DesignElement* pElem, suic::Panel* pPanel);
    void RemoveChileElement(DesignElement* pParent, DesignElement* pChild);
    bool SetElementSetter(DesignElement* pElem, SetterNode* pSetter, bool fromProp);

    //============================= �����帨����������
    DesignElement* HitTestElement(Point pt);

    FrameworkElement* GetUIElement();

    DesignElement* GetHotElement() const;
    DesignElement* GetFocusElement() const;

    void SetHotElement(DesignElement* elem);
    void SetFocusElement(DesignElement* elem);

    virtual bool IsTemplateRoot();

protected:

    virtual bool OnLoad(bool onlyReadAppRes);
    virtual void OnClose();
    virtual bool OnLoadFromXml();

protected:

    bool OnLoadApp(bool onlyReadAppRes);
    bool InternalAppOpen(const String& strPath, const String& strFilename, bool onlyReadAppRes);

protected:

    bool _isTemplate;
    bool _refreshDirty;
    DesignElement* _rootItem;
    RootItemData* _rootData;
};

class ApplicationRootItem : public FileRootItem
{
public:

    RTTIOfClass(ApplicationRootItem)

    ApplicationRootItem();
    virtual ~ApplicationRootItem();

    suic::String GetResXml(const String& offset);

    void Reset();
    void ClearOnlyRes();

    virtual bool Save();
    virtual bool SaveAs(const String& strPath);

    ApplicationNode* GetApplicationNode();
    ElementRootItem* GetMainRootItem() const;

protected:

    virtual bool OnLoad(bool onlyReadAppRes);
    virtual void OnClose();

private:

    void Clear();
    bool OnLoadApp(bool onlyReadAppRes);
    bool InternalAppOpen(const String& strPath, const String& strFilename, bool onlyReadAppRes);

private:

    ApplicationNode* _app;
};

class ResourceDicRootItem : public FileRootItem
{
public:

    RTTIOfClass(ResourceDicRootItem)

    ResourceDicRootItem();
    virtual ~ResourceDicRootItem();

    suic::String GetResXml(const String& offset);

    void Reset();
    virtual bool Save();
    virtual bool SaveAs(const String& strPath);

    ResourceDictionaryNode* GetResourceDicNode();

protected:

    virtual bool OnLoad(bool onlyReadAppRes);
    virtual void OnClose();

private:

    void Clear();

private:

    ResourceDictionaryNode* _resDic;
};

class DesignTemplateRoot : public DesignElement
{
public:

    DesignTemplateRoot(DesignElement* pChild, RTTIOfInfo* targetInfo)
    {
        FrameworkElement* pCont = NULL;
        
        _targetInfo = targetInfo;

        if (NULL != targetInfo)
        {
            pCont = RTTICast<FrameworkElement>(targetInfo->Create());
        }
        else
        {
            pCont = RTTICast<FrameworkElement>(ContentControl::RTTIType()->Create());
        }

        if (NULL != pCont)
        {
            DesignHelper::SetTemplateDesignMode(pCont);
        }

        SetNodeElement(pCont);
        SetIsTemplate(true);

        if (pChild != NULL)
        {
            pChild->SetIsTemplate(true);
            AddXamlElement(pChild);

            if (NULL != pChild->GetUIElement())
            {
                if (NULL != targetInfo)
                {
                    DesignHelper::SetElementParent(pCont, pChild->GetUIElement());
                }
                pCont->SetTemplateChild(pChild->GetUIElement());
            }
        }
    }

    DesignElement* GetRootElement()
    {
        if (GetCount() > 0)
        {
            return GetXamlElement(0);
        }
        return NULL;
    }

    virtual bool AddElement(DesignElement* elem)
    {
        Clear();
        AddXamlElement(elem);
        elem->SetIsTemplate(true);

        FrameworkElement* pCont = GetUIElement();
        FrameworkElement* feElem = elem->GetUIElement();

        if (NULL != feElem)
        {
            if (NULL != _targetInfo)
            {
                DesignHelper::SetElementParent(pCont, feElem);
            }

            pCont->SetTemplateChild(elem->GetUIElement());
        }
        else
        {
            pCont->SetTemplateChild(NULL);
        }
        return true;
    }

    virtual bool RemoveElement(DesignElement* elem)
    {
        FrameworkElement* pCont = GetUIElement();
        if (GetCount() == 1 && GetXamlElement(0) == elem)
        {
            Clear();
            pCont->SetTemplateChild(NULL);
        }
        return true;
    }

private:

    RTTIOfInfo* _targetInfo;
};

//=========================================
// 
// ����ģ��Ԫ��
class TemplateRootItem : public ElementRootItem
{
public:

    RTTIOfClass(TemplateRootItem)

    TemplateRootItem();
    ~TemplateRootItem();

    void CloneNode(ResNodePtr& obj);
    void ParseTemplateValue(Project* pPrj, FrameworkElement* fe, ObjectPtr& resVal);

    virtual String GetResXml(const String& offset);

    String ToString();
    suic::Object* GetValue();
    RTTIOfInfo* GetValueRTTIType();

    bool HasContent();

    void CheckTemplateInstantiate();

    suic::RTTIOfInfo* GetTargetType() const;
    suic::RTTIOfInfo* GetTemplateType() const;

    void SetTemplateType(suic::RTTIOfInfo* typeInfo);
    void SetTargetType(suic::RTTIOfInfo* typeInfo);

    void SetIsControlTemplate(bool v);
    bool IsControlTemplate() const;
    bool IsInitTemplate() const;
    void SealInitTemplate();

    DesignElement* GetRootElement();
    void SetRootElement(DesignElement* pElem);

    DesignElement* GetTemplateRootElement();
    TriggerCollectionNode* GetTriggerCollection();

    // ȡ���Ѿ���������Ԫ��
    void GetChildAsNamed(suic::ItemCollection* itemColl);

    bool IsTemplateRoot()
    {
        return true;
    }

protected:

    DesignTemplateRoot* GetDesignTemplateRoot();
    void GetChildAsNamed(DesignElement* pNode, suic::ItemCollection* itemColl, bool bSkip);

private:

    bool _initTemplate;
    bool _isControlTemplate;

    RTTIOfInfo* _typeInfo;
    RTTIOfInfo* _targetType;

    TriggerCollectionNode* _trgColl;

    FrameworkTemplate* _ftRoot;
};

inline String TemplateRootItem::ToString()
{
    return ResNode::ToString();
}

class ImageRootItem : public RootItem
{
public:

    RTTIOfClass(ImageRootItem)

    BeginMember(ImageRootItem, RootItem)
        MemberGetInt(ShowItem)
    EndMember()

    ImageRootItem();

    int GetShowItem()
    {
        return Visibility::Visible;
    }

    virtual void Reset()
    {

    }

    virtual String GetRootType() const;
    virtual bool Save();
    virtual bool SaveAs(const String& strPath);

protected:

    virtual bool OnLoad(bool onlyReadAppRes);
    virtual void OnClose();
    virtual void OnClear();
};

// ��ȡ�ļ�

class XamlLoader
{
public:

    bool LoadAppRootXaml(ApplicationRootItem* root, const String& strPath, bool onlyReadAppRes);
    bool LoadElementRootXaml(ElementRootItem* root, const String& strPath);
    bool LoadResourceDicRootXaml(ResourceDicRootItem* root, const String& strPath);

    bool LoadElementRootXamlFromMemory(ElementRootItem* root, const Mulstr& data);
    bool LoadResourceDicRootXamlFromMemory(ResourceDicRootItem* root, const Mulstr& data);
};

#endif
