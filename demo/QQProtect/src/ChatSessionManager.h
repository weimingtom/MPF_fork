// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ChatSessionManager.h
// ��  �ܣ�����Ự����
// 
// ��  �ߣ�Sharpui������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _CHATSESSIONMANAGER_H_
#define _CHATSESSIONMANAGER_H_

#include "UserManager.h"
#include "ChatSession.h"
#include "ChatWindow.h"

class ChatSessionItem
{
public:

    ChatWindow* chatWindow;
    ChatSession* chatSession;

    ChatSessionItem();
    ~ChatSessionItem();
};

class ChatSessionManager : public Object
{
public:

    ChatSessionManager();
    ~ChatSessionManager();

    bool OpenChatSession(UserUseId* pUserInfo);

    void RemoveAll();
    void RemoveChatWindow(ChatWindow* pWindow);

    void OnClosingChatWindow(Object* sender, EventArg* e);

private:

    bool _operPending;
    suic::Array<ChatSessionItem*> _chatSessions;
};

#endif
