// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����SettingWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�Sharpui������
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _SETTINGWINDOW_H_
#define _SETTINGWINDOW_H_

#include "ChatWindow.h"
#include <Framework/Controls/Menu.h>

using namespace suic;

class SettingWindow : public OSWindow
{
public:

    SettingWindow();
    ~SettingWindow();

    void OnLoaded(suic::LoadedEventArg* e);

    void OnScrollViewChanged(suic::Element*, suic::ScrollChangedEventArg*);
    void OnLeftSwitchClick(suic::Element* sender, suic::RoutedEventArg* e);

protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

private:

    suic::Array<String, true> _labels;
};

#endif

