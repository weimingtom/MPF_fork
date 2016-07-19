
#ifndef _USERGROUP_H_
#define _USERGROUP_H_

#include "UserInfo.h"

using namespace suic;

class UserGroup : public NotifyPropChanged
{
public:

    // ������Ҫ�󶨵�ģ�������
    // �������ݻ��Զ��󶨵�����ģ���н�����ʾ
    // ����UserGroup���ģ���е�Binding Path=UserGroup��Ӧ
    BeginMember(UserGroup, NotifyPropChanged)
        MemberString(UserGroup)
        MemberGetObject(ItemsSource) // ItemsSource�ǹ̶��ģ�����ӵ�������������
    EndMember()

    RTTIOfClass(UserGroup)

    UserGroup(suic::String grpName)
    {
        _groupName = grpName;
        _userIds.setAutoDelete(false);
    }

    UserGroup()
    {

    }

    suic::String GetUserGroup()
    {
        return _groupName;
    }

    void SetUserGroup(suic::String val)
    {
        _groupName = val;
    }

    suic::Object* GetItemsSource()
    {
        return &_userIds;
    }

    void AddUserId(UserInfo* userId)
    {
        _userIds.AddItem(userId);
    }

private:

    ObservableCollection _userIds;
    // �û���������
    String _groupName;
};

///
/// <Summary>
///  ������ģ��ѡ��������Ϊ�û�������û���۲�һ������Ҫ�󶨲�ͬ��ģ�棩
/// </Summary>
///
class UserTemplateSelector : public DataTemplateSelector
{
public:

    UserTemplateSelector(DataTemplate* t1, DataTemplate* t2);
    DataTemplate* SelectTemplate(Object* item, FrameworkElement* container);

private:

    // �û���������ģ��
    DataTemplate* userGrp;
    // �û���Ϣģ��
    DataTemplate* userId;
};

#endif