// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����PropertyWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _PROPERTYWINDOW_H_
#define _PROPERTYWINDOW_H_

#include <Core/TypeManager.h>
#include "CommonInterface.h"

class DesignPanel;
class SetterEditor;
class EditRootPanel;

class PropertyWindow : public suic::Window
{
public:

    PropertyWindow();
    ~PropertyWindow();

    void OnLoaded(suic::LoadedEventArg* e);

protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);
};

class PropertyWindowOp : public IPropertyWindow
{
public:

    PropertyWindowOp();
    ~PropertyWindowOp();

    void InitEditRootPanel(Element* rootPanel, Element* parent);
    suic::FrameworkElement* GetPropertyWindow(suic::Element* parent);

    void UpdateModified();

    void SetDesignPanel(DesignPanel* design);
    void InitEditPanelInfo(ElementRootItem* rootElem, DesignPanel* design);
    void SetCurrentElement(DesignElement* resElem, DesignElement* delem);

    void UpdateSetter(SetterNode* setter, bool needNotifyChanged);
    void UpdateResource(ResNode* setter);

    void SetSetterNotify(SetterNotifyEventHandler h);

private:

    DesignPanel* _designPanel;
    EditRootPanel* _editPanel;
    PropertyWindow* _propWindow;
};

#endif
