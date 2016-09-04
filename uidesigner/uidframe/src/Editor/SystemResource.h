// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����SystemResource.h
// ��  �ܣ�ϵͳĬ����Դ����
// 
// ��  �ߣ�MPF������
// ʱ  �䣺2011-08-02
// 
// ============================================================================

#ifndef _UISYSTEMRESOURCE_H_
#define _UISYSTEMRESOURCE_H_

#include <core/StyleNode.h>
#include <main/XamlRootItem.h>

class SystemResource
{
public:

    static SystemResource* Ins();

    ResourceDicRootItem* GetDefaultResDic();

    ResNode* FindResNode(const suic::String& strName);
    TemplateRootItem* FindControlTemplate(const suic::String& strType);
    StyleNode* FindStyle(const suic::String& strType);

    bool InitDefaultResDic();

private:

    SystemResource();
    ~SystemResource();

    ResourceDicRootItem* _resRootItem;
};

#endif

