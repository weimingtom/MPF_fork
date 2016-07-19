
#ifndef _CHARMESSAGE_H_
#define _CHARMESSAGE_H_

using namespace suic;

class InfoItem : public Object
{
public:

};

// ����������Ϣ
class ChatItem : public InfoItem
{
public:

    ChatItem()
        : _sendState(0)
        , _msgPos(0)
        , _sendState(0)
    {
    }

    suic::Byte GetSendState() const
    {
        return _sendState;
    }

    void SetSendState(suic::Byte val)
    {
        _sendState = val;
    }

    suic::Uint32 GetMessagePos() const
    {
        return _msgPos;
    }

    void SetMessagePos(suic::Uint32 val)
    {
        _msgPos = val;
    }

    suic::String GetUserId() const
    {
        return _userId;
    }

    void SetUserId(suic::String val)
    {
        _userId = val;
    }

    suic::Uint32 GetMessageDate() const
    {
        return _msgDate;
    }

    void SetMessageDate(suic::Uint32 val)
    {
        _msgDate = val;
    }

    suic::ByteStream* GetMessageStream()
    {
        return &_message;
    }

private:

    // ��Ϣ����״̬0:�ɹ�
    suic::Byte _sendState;
    // ��Ϣ���������ݿ�ʼλ������
    // ������ÿ�μ�����Ϣ���ݻᶯ̬�ı�
    suic::Uint32 _msgPos;
    // �û�ID
    suic::String _userId;
    // ��Ϣ����ʱ��
    suic::Uint32 _msgDate;
    // ��Ϣ����
    suic::ByteStream _message;
};

class ChatMessage
{
public:

    ChatMessage()
    {
    }

    int GetCount() const
    {
        return _msgCache.GetCount();
    }

    InfoItem* GetItem(int index) const
    {
        return DynamicCast<InfoItem>(_msgCache.GetItem(index));
    }

    void AddItem(InfoItem* item)
    {
        _msgCache.Add(item);
    }

private:

    // ��Ϣ����
    suic::XArray _msgCache;
};

#endif
