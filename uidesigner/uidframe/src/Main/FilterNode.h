
#ifndef _FILTERNODE_H_
#define _FILTERNODE_H_

#include <Core/ResNode.h>

using namespace suic;

class ElementRootItem;
class Project;

class FilterNode : public ResNode
{
public:

    RTTIOfClass(FilterNode)

    BeginMember(FilterNode, ResNode)
        MemberGetInt(ShowItem)
    EndMember()

    FilterNode();
    FilterNode(const String& name);

    ~FilterNode();

    int GetShowItem()
    {
        return Visibility::Visible;
    }

    bool IsSingleValue();
    suic::String GetResXml(const String& offset);

    virtual Project* GetProject() const;
    
    // �������ƣ��ҵ���һ�������ļ��нڵ�
    FilterNode* FindItemOnChild(const String& name);
    // ��·�������ӽڵ�(images/button/1.png)
    FilterNode* FindItemFromPath(const String& name);

    // �Ƴ�ָ���Ľڵ�
    void RemoveFilterItem(FilterNode* rootElem);
    // ��·���Ƴ��ӽڵ�(images/button/1.png)
    void RemoveItemOnPath(const String& strName);

    bool IsRootNode() const;

    // ��ȡ���ProjectĿ¼�����·��
    String GetRelativePath() const;

    // ��ȡȫ·��
    String GetFullPath() const;

    // ����һ�������ļ���
    void AddSubFilter(const String& strPath);

    void SetName(const String& name);
    String GetName() const;

    String ToString();

private:

    String _name;
};

#endif
