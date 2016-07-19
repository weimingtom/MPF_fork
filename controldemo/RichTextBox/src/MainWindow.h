// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����MainWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�Sharpui������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

class MainWindow : public suic::Window
{
public:

    MainWindow();
    ~MainWindow();

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

    // ���������Ϣ���ػ�س���������Ϣ
    void OnPreviewTextInput(suic::KeyboardEventArg* e);
    void OnPreviewKeyDown(suic::KeyboardEventArg* e);
    void OnSendInfoClick(suic::Element* sender, suic::RoutedEventArg* e);
    void OnInsertImageClick(suic::Element* sender, suic::RoutedEventArg* e);

private:

    void RegisterButtonEvent();
    void InsertAniImage();

    void SendInfo();

    suic::ImagePlayEmbbed* InsertOneImage(suic::String strDir, int count, int delay);
};

#endif

