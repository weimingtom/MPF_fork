///
/// �ļ�����ResourceDictionaryNode.h
///
/// ��  �ܣ�����ResourceDictionary��Դ�ڵ�
///
/// ��  �ߣ�����
///

#ifndef _RESOURCEDICTIONARYNODE_H
#define _RESOURCEDICTIONARYNODE_H

#include <Core/ResourceCollectionNode.h>
#include <Core/SetterCollectionNode.h>

class ElementRootItem;
class ResourceDicRootItem;
class MergedDictionariesNode;

class ResourceDictionaryNode : public XamlNodeItem
{
public:

    RTTIOfClass(ResourceDictionaryNode)

    ResourceDictionaryNode();
    ~ResourceDictionaryNode();

    void CloneNode(ResNodePtr& obj);
    void CloneResourceDic(ResourceDictionaryNode* pDicNode);

    String GetResXml(const String& offset);

    bool IsOnlyResItem();
    String GetResItemsXml(const String& offset);

    ResourceUri* GetResPath();

    void ClearRes();
    void Clear();
    int GetCount() const;

    ItemCollection* GetResourceCollection() const;
    ResourceCollectionNode* GetResourceCollectionNode() const;

    void SetResourceSourceUri(const String& strSource);
    ResourceUri GetResourceSourceUri() const;

    void SetResourceDictionary(ResourceDictionary* res);
    ResourceDictionary* GetResourceDictionary() const;

    void SetSourceResource(ResourceDicRootItem* resSource);
    ResourceDicRootItem* GetSourceResource() const;

    void AddResItem(const String& key, Object* resItem);
    void AddMergedDictionary(ResourceDictionaryNode* resDic);

    ResNode* FindResItem(const String& key);
    void AddInternalresItem(ResNode* pResNode);
    ResNode* SearchResItem(const String& key);

    void FillResByResType(ItemCollection* itemColl, RTTIOfInfo* targetType, ResNode* breakNode);

    static bool ResInItemCollection(ItemCollection* itemColl, ResNode* resItem);

private:

    ResourceUri _resSourceUri;
    ResourceDictionary* _value;

    MergedDictionariesNode* _mergedDics;

    ResourceDicRootItem* _resSource;
    ResourceCollectionNode* _resItems;
};

class MergedDictionariesNode : public ResNode
{
public:

    RTTIOfClass(MergedDictionariesNode)

    MergedDictionariesNode();
    ~MergedDictionariesNode();

    virtual void CloneNode(ResNodePtr& obj);

    virtual bool IsSingleValue();
    virtual void SetValue(suic::Object* val);
    virtual suic::Object* GetValue();
    virtual suic::String GetSingleXml();

    String GetResXml(const String& offset);
    void AddMergedResource(ResourceDictionaryNode* mergedRes);
    ResNode* SearchResItem(const String& key);

    void FillResByResType(ItemCollection* itemColl, RTTIOfInfo* targetType, ResNode* breakNode);
};

class ApplicationNode : public XamlNodeItem
{
public:

    RTTIOfClass(ApplicationNode)

    ApplicationNode();
    ~ApplicationNode();

    String GetResXml(const String& resName, const String& offset);
    
    void SetApplication(Application* pApp);
    Application* GetApplication() const;

    void SetStartupUri(const String& strUri);
    ResourceUri GetStartupUri() const;

    void SetMainRootItem(ElementRootItem* rootItem);
    ElementRootItem* GetMainRootItem() const;

    SetterCollectionNode* GetSetterCollection();
    ResourceDictionaryNode* GetResourceDictionary();

    void Close();
    void Clear();

private:

    Application* _value;
    ElementRootItem* _mainWnd;
    ResourceUri _startupUri;
    // Ԫ�ر�������ԣ�localֵ��
    SetterCollectionNode* _setterColl;
    // Ԫ����Դ
    ResourceDictionaryNode* _resDic;
};

#endif
