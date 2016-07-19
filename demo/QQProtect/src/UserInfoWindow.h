// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����UserInfoWindow.h
// ��  �ܣ��û���Ϣ���洰�ڡ�
// 
// ��  �ߣ�Sharpui������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _USERINFOWINDOW_H_
#define _USERINFOWINDOW_H_

#include "UserManager.h"

using namespace suic;

class UserInfoWindow : public suic::Window
{
public:

    UserInfoWindow();
    UserInfoWindow(UserUseId* userInfo);
    ~UserInfoWindow();

    void OnTimer(Object* sender, EventArg* e);
    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

protected:

    void StartHeadImgAnimate();
    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    int _moveTop;
    Point _pos;
    UserUseId* _userInfo;
    AssignerTimer* _timer;
};

#endif

