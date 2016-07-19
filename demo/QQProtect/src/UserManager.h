
#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include <System/Graphics/Bitmap.h>

class UseId : public suic::NotifyPropChanged
{
public:

    BeginMember(UseId, NotifyPropChanged)
        MemberString(UserNumber)
    EndMember()

    RTTIOfClass(UseId)

    UseId();
    virtual ~UseId();

    suic::String GetUserId() const;

    suic::String GetUserNumber();
    void SetUserNumber(suic::String val);

    int GetUserType() const;
    void SetUserType(int val);

private:
    
    int _userType;
    bool _isContact;
    suic::String _userNumber;
};

class UserUseId : public UseId
{
public:

    BeginMember(UserUseId, UseId)
        MemberString(UserName)
        MemberString(UserPwd)
        MemberGetBool(EnableGrey)
        MemberString(UserMark)
        MemberGetObject(HeadIcon)
    EndMember()

    RTTIOfClass(UserUseId)

    UserUseId();
    UserUseId(ImageSource* headIcon);
    ~UserUseId();

    String GetUserName();
    void SetUserName(String usrName);

    String GetUserPwd();
    void SetUserPwd(String usrPwd);

    String GetUserMark();
    void SetUserMark(String val);

    bool GetEnableGrey();

    suic::Object* GetHeadIcon();
    void SetHeadIcon(suic::Object* val);

    bool IsLogin()
    {
        return _isLogin;
    }

    void SetIsLogin(bool val)
    {
        _isLogin = val;
    }

private:

    // �û���ʾ����
    String _userName;
    // �û�����(MD5����)
    String _userPwd;
    // �û���ע��Ϣ
    String _userMark;

    // �û�Ŀǰʹ�õ������ʶ
    String _userThemeId;

    // �û���Ա����ĿǰΪ1-7����
    int _userVipLevel;

    // �û��Ƿ�װ����ͷ
    bool _hasCamara;
    // �û��Ƿ�����
    bool _isOnLine;
    // �û��Ƿ��¼
    bool _isLogin;

    // �û�ͷ��
    ImageSourcePtr _headIcon;
};

class GroupUseId : public UseId
{
public:

    RTTIOfClass(GroupUseId)

    GroupUseId();
    ~GroupUseId();
};

class HierarchyGroup : public suic::HierarchicalItem
{
public:

    // ������Ҫ�󶨵�ģ�������
    // �������ݻ��Զ��󶨵�����ģ���н�����ʾ
    // ����UserGroup���ģ���е�Binding Path=UserGroup��Ӧ
    BeginMember(HierarchyGroup, HierarchicalItem)
        MemberString(UserGroup)
    EndMember()

    RTTIOfClass(HierarchyGroup)

    HierarchyGroup(suic::String name);
    HierarchyGroup();

    suic::String GetUserGroup();
    void SetUserGroup(suic::String val);

private:

    // ��������
    suic::String _groupName;
};

///
/// <Summary>
///  ������飬����һ���û���Ⱥ���������
/// <Summary/>
///
class ClassifyGroup : public suic::ObservableCollection
{
public:

    ClassifyGroup();
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

    // �û���Ϣģ��
    DataTemplate* userId;
    // �û���������ģ��
    DataTemplate* userGrp;
};

class UserManager
{
public:

    static UserManager* Ins();

    UseId* FindUserId(suic::String userId);
    void AddUserId(UseId* userId);

    UserUseId* GetCurrentUser() const;

    ClassifyGroup* GetFriendGroup();
    ClassifyGroup* GetGroupGroup();
    ClassifyGroup* GetDiscussGroup();

private:

    // ��������
    void InitTestUserMarks();
    void InitTestUsers();
    void InitTestUserGroup(String groupName, int iCount);

private:

    UserManager();
    ~UserManager();

    // ��ǰ��¼�û���Ϣ
    UserUseId* _currUser;
    ClassifyGroup* _friendGrp;

    // �����û���Ϣ���������Ѻ�İ����
    suic::StringAutoDic<UseId> _users;

    // ��������
    String _userMarks[10];
};

#endif

