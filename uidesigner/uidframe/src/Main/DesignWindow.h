// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����DesignWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _DESIGNWINDOW_H_
#define _DESIGNWINDOW_H_

class DesignWindow : public suic::Window
{
public:

    DesignWindow();
    ~DesignWindow();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

protected:

    // �����Լ��Ŀؼ�����
    suic::ObjectPtr OnBuild(suic::IXamlNode* pNode);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);
};

class DesignWindowOp
{
public:

    suic::FrameworkElement* GetDesignWindow();
};

#endif
