///
/// �ļ�����XamlSceneNode.h
///
/// ��  �ܣ�������Դ�ڵ����
///
/// ��  �ߣ�����
///

#ifndef _XAMLSCENENODE_H
#define _XAMLSCENENODE_H

class IPropertyId
{
public:

    virtual ~IPropertyId() {}

    // ��ȡ�����Զ�Ӧ������
    virtual DpProperty* GetTargetType() = 0;
    virtual String GetName() = 0;
    virtual String GetUniqueName() = 0;
};

class XamlSceneNode : public suic::HierarchicalItem
{
public:

    RTTIOfClass(XamlSceneNode)

    XamlSceneNode();
    virtual ~XamlSceneNode();

    template<typename T>
    static T* Create()
    {
        return (T*)(T::RTTIType()->Create());
    }
};

class XamlNodeItem : public XamlSceneNode
{
public:

    BeginMember(XamlNodeItem, XamlSceneNode)
        MemberGetString(DisplayName)
    EndMember()

    RTTIOfAbstractClass(XamlNodeItem)

    XamlNodeItem();
    virtual ~XamlNodeItem();

    void Dispose();

    String GetDisplayName();
    void UpdateDisplayName();

    ItemCollection* GetItemColl();

    int GetCount();
    XamlNodeItem* GetItem(int index);

    void AddItem(XamlNodeItem* val);
    void InsertItem(int index, XamlNodeItem* child);
    void RemoveItem(XamlNodeItem* val);
    void RemoveItemAt(int index);

    virtual void Clear();

    virtual bool IsLocked() const;
    void SetLocked(bool val);

    bool GetLockedSelf() const;

    XamlNodeItem* GetParent() const;
    void SetParent(XamlNodeItem* val);

protected:

    virtual void OnAddItem(XamlNodeItem* val);

protected:

    bool _isLocked;
    WeakRef _parent;
};

#endif
