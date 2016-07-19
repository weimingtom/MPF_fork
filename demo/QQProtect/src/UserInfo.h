
#ifndef _USERINFO_H_
#define _USERINFO_H_

#include <Extend/RichText/TextBox.h>

using namespace suic;

// ����̳���NotifyPropChanged�������û������ҵ�������Ա仯ʱ
// �ͻ�ӵ��֪ͨ����ı������
// �̳�ICommand�ӿڣ�����ı䣨���簴ť��ʱӵ������ִ������
class UserInfo : public ICommand, public NotifyPropChanged
{
public:

    BeginMember(UserInfo, NotifyPropChanged)
        MemberString(UserName)
        MemberString(UserPwd)
        MemberBool(EnableGrey)
        MemberString(UserNumber)
        MemberGetBool(IsHidden)
        MemberString(UserMark)
        MemberGetObject(HeadIcon)
    EndMember()

    RTTIOfClass(UserInfo)

    UserInfo()
    {
        _isHidden = true;
        _isLogin = false;
    }

    UserInfo(String usrName, String usrPwd);

    UserInfo(String usrName, String strNumber, String userMark, suic::Object* val)
    {
        _usrName = usrName;
        _userNumber = strNumber;
        _userMark = userMark;
        _headIcon = val;
        _isHidden = true;
        _isLogin = false;
    }

    bool CanExecute(Object* target, Object* param);
    void Execute(Object* target, Object* param);

    String GetUserName();
    void SetUserName(String usrName);

    String GetUserPwd();
    void SetUserPwd(String usrPwd);

    String GetUserNumber()
    {
        return _userNumber;
    }

    void SetUserNumber(String val)
    {
        _userNumber = val;
    }

    String GetUserMark()
    {
        return _userMark;
    }

    void SetUserMark(String val)
    {
        _userMark = val;
    }

    bool GetIsHidden()
    {
        return _isHidden;
    }

    void SetIsHidden(bool val)
    {
        _isHidden = val;
    }

    bool GetEnableGrey()
    {
        return !_isLogin;
    }

    void SetEnableGrey(bool val)
    {
    }

    suic::Object* GetHeadIcon()
    {
        return _headIcon.get();
    }

    void SetHeadIcon(suic::Object* val)
    {
        _headIcon = val;
    }

    bool GetIsLogin() const
    {
        return _isLogin;
    }

    void SetIsLogin(bool val)
    {
        _isLogin = val;
    }

    void SetBubbleBk(Brush* brush)
    {
        _bubbleBk = brush;
    }

    Brush* GetBubbleBk()
    {
        return _bubbleBk.get();
    }

    suic::String UserInfo::ToString()
    {
        return L"��  ¼";
    }

private:

    // �û���ʾ����
    String _usrName;
    // �û�����(MD5����)
    String _usrPwd;
    // �û���ע��Ϣ
    String _userMark;
    // �û�QQ����
    String _userNumber;

    // �û�Ŀǰʹ�õ������ʶ
    String _userThemeId;

    // �û���Ա����ĿǰΪ1-7����
    int _userVipLevel;
    
    // �û��Ƿ�װ����ͷ
    bool _hasCamara;
    // �û��Ƿ��¼
    bool _isLogin;
    // �û��Ƿ�����
    bool _isHidden;
    
    // �û�ͷ��
    BitmapPtr _headIcon;
    // �û�QQ��ͷ��
    BitmapPtr _qqShowHead;
    BrushPtr _bubbleBk;
};

#endif
