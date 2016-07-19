
#include "stdafx.h"
#include "UserManager.h"

ImplementRTTIOfClass(UseId, NotifyPropChanged)
ImplementRTTIOfClass(UserUseId, UseId)
ImplementRTTIOfClass(GroupUseId, UseId)
ImplementRTTIOfClass(HierarchyGroup, HierarchicalItem)

UseId::UseId()
{

}

UseId::~UseId()
{

}

suic::String UseId::GetUserId() const
{
    return _userNumber;
}

suic::String UseId::GetUserNumber()
{
    return _userNumber;
}

void UseId::SetUserNumber(suic::String val)
{
    _userNumber = val;
}

int UseId::GetUserType() const
{
    return _userType;
}

void UseId::SetUserType(int val)
{
    _userType = val;
}

UserUseId::UserUseId()
{
    _isLogin = false;
    _isOnLine = false;
}

UserUseId::UserUseId(ImageSource* headIcon)
{
    _isLogin = false;
    _isOnLine = false;
    _headIcon = headIcon;
}

UserUseId::~UserUseId()
{

}

String UserUseId::GetUserName()
{
    return _userName;
}

void UserUseId::SetUserName(String userName)
{
    _userName = userName;
}

String UserUseId::GetUserPwd()
{
    return _userPwd;
}

void UserUseId::SetUserPwd(String userPwd)
{
    _userPwd = userPwd;
}

String UserUseId::GetUserMark()
{
    return _userMark;
}

void UserUseId::SetUserMark(String val)
{
    _userMark = val;
}

bool UserUseId::GetEnableGrey()
{
    return !_isOnLine;
}

suic::Object* UserUseId::GetHeadIcon()
{
    return _headIcon.get();
}

void UserUseId::SetHeadIcon(suic::Object* val)
{
    _headIcon = val;
}

GroupUseId::GroupUseId()
{

}

GroupUseId::~GroupUseId()
{

}

HierarchyGroup::HierarchyGroup(suic::String name)
{
    _groupName = name;
}

HierarchyGroup::HierarchyGroup()
{
}

suic::String HierarchyGroup::GetUserGroup()
{
    suic::String strName;
    strName.Format(_U("%s 0/%d"), _groupName.c_str(), GetChildren()->GetCount());
    return strName;
}

void HierarchyGroup::SetUserGroup(suic::String val)
{
    _groupName = val;
}

//===========================================

UserTemplateSelector::UserTemplateSelector(suic::DataTemplate* t1, suic::DataTemplate* t2)
{
    userGrp = t1;
    userId = t2;
}

DataTemplate* UserTemplateSelector::SelectTemplate(Object* item, FrameworkElement* container)
{
    if (NULL == item)
    {
        return NULL;
    }
    else if (item->GetRTTIType() == HierarchyGroup::RTTIType())
    {
        return userGrp;
    }
    else if (item->GetRTTIType() == UserUseId::RTTIType())
    {
        return userId;
    }

    return NULL;
}

ClassifyGroup::ClassifyGroup()
{
}

// ��ʼ����������

void UserManager::InitTestUserGroup(String groupName, int iCount)
{
    int iNum = 0;
    suic::String str;
    suic::Window* mainWnd = suic::Application::Current()->GetMainWindow();

    HierarchyGroup* hierGrp = new HierarchyGroup();
    suic::ItemCollection* items = hierGrp->GetChildren();

    hierGrp->SetUserGroup(groupName);
    _friendGrp->AddItem(hierGrp);

    for (int j = 0; j < iCount; ++j)
    {
        str.Format(_U("Head%d"), iNum + 1);
        suic::Object* headIcon = mainWnd->FindRes(str);

        if (DpProperty::UnsetValue() == headIcon)
        {
            iNum = 0;
            str.Format(_U("Head%d"), iNum + 1);
            headIcon = mainWnd->FindRes(str);
        }

        if (_currUser->GetHeadIcon() == NULL)
        {
            _currUser->SetHeadIcon(headIcon);
        }

        ++iNum;

        bool isOnline = false;
        int iIndex = iNum % 9;
        UserUseId* userItem = NULL;

        userItem = new UserUseId(suic::DynamicCast<suic::ImageSource>(headIcon));

        userItem->SetUserName(String().Format(_U("����ͣ��-%d"), j + 1));
        userItem->SetUserNumber(_U("386666951"));
        userItem->SetUserMark(_userMarks[iIndex]);

        items->AddItem(userItem);
    }
}

void UserManager::InitTestUserMarks()
{
    _userMarks[0] = _U("~������");
    _userMarks[1] = _U("���ˣ�������Ϣ��");
    _userMarks[2] = _U("���º���ƽ����");
    _userMarks[3] = _U("�úñ���...");
    _userMarks[4] = _U("��֪��");
    _userMarks[5] = _U("��������ʡ�");
    _userMarks[6] = _U("�����������ȡ�");
    _userMarks[7] = _U("������������");
    _userMarks[8] = _U("���������������ǹ�����");
    _userMarks[9] = _U("�����кñ�");
}

void UserManager::InitTestUsers()
{
    InitTestUserMarks();

    InitTestUserGroup(_U("�ҵĺ���"), 800);
    InitTestUserGroup(_U("����ͬѧ"), 30);
    InitTestUserGroup(_U("����"), 60);
    InitTestUserGroup(_U("��ѧͬѧ"), 29);
}

UserManager::UserManager()
{
    _currUser = new UserUseId();
    _currUser->ref();

    _friendGrp = new ClassifyGroup();
    _friendGrp->ref();

    InitTestUsers();
}

UserManager::~UserManager()
{
    _currUser->unref();
    _friendGrp->unref();
}

UserManager* UserManager::Ins()
{
    static UserManager _ins;
    return &_ins;
}

UserUseId* UserManager::GetCurrentUser() const
{
    return _currUser;
}

UseId* UserManager::FindUserId(suic::String userId)
{
    UseId* useId = NULL;
    _users.TryGetValue(userId, useId);
    return useId;
}

void UserManager::AddUserId(UseId* userId)
{
    _users.Add(userId->GetUserId(), userId);
}

ClassifyGroup* UserManager::GetFriendGroup()
{
    return _friendGrp;
}

ClassifyGroup* UserManager::GetGroupGroup()
{
    return NULL;
}

ClassifyGroup* UserManager::GetDiscussGroup()
{
    return NULL;
}
