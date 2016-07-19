// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����CommonInterface.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _COMMONINTERFACE_H_
#define _COMMONINTERFACE_H_

class SetterNode;
class ResNode;
class DesignPanel;
class DesignElement;

class Project;
class ElementRootItem;

typedef delegate<void ()> SetterNotifyEventHandler;

class IPropertyWindow : public suic::Object
{
public:

    virtual ~IPropertyWindow() {}

    virtual void InitEditRootPanel(Element* rootPanel, Element* parent) = 0;
    virtual void SetDesignPanel(DesignPanel* design) = 0;
    virtual void InitEditPanelInfo(ElementRootItem* rootElem, DesignPanel* design) = 0;

    virtual suic::FrameworkElement* GetPropertyWindow(suic::Element* parent) = 0;
    virtual void SetCurrentElement(DesignElement* delem) = 0;

    virtual void UpdateModified() = 0;
    
    virtual void UpdateSetter(SetterNode* setter, bool needNotifyChanged) = 0;
    virtual void UpdateResource(ResNode* setter) = 0;

    virtual void SetSetterNotify(SetterNotifyEventHandler h) = 0;
};

enum DesignMode {NoneMode, SelectMode, MoveMode, AddMode, DragMode};

typedef delegate<void (DesignMode, const suic::String&)> SelectNotifyEventHandler;

class IControlWindow : public suic::Object
{
public:

    virtual ~IControlWindow() {}

    virtual suic::FrameworkElement* GetControlWindow(suic::Element* parent) = 0;
    // ���ݿؼ������������ɿؼ�Ĭ�ϲ���
    virtual suic::FrameworkElementPtr GetControlSample(suic::Object* parent, String strType) = 0;

    virtual void RestoreSelectMode() = 0;
    virtual void SetSelectNotify(SelectNotifyEventHandler selEvent) = 0;

    virtual suic::RadioButton* GetMoreElement() = 0;
    virtual void UpdateMoreElement(const String& name, bool setChecked) = 0;
};

class IElementTreeDoc : public suic::Object
{
public:

    virtual ~IElementTreeDoc() {}

    virtual void UpdateElementTree() = 0;
    virtual void SetSelectedElement(Object* elem) = 0;
    virtual void SetMouseOverElement(Object* elem) = 0;

    virtual IControlWindow* GetControlWindow() = 0;
    virtual IPropertyWindow* GetPropertyWindow() = 0;
};

class IDesignWindow : public suic::FrameworkElement
{
public:

    virtual ~IDesignWindow() {}

    virtual void SetTreeDoc(IElementTreeDoc* pControl) = 0;
    virtual void SwitchRootElement(ElementRootItem* root) = 0;
    virtual ElementRootItem* GetRootElement() const = 0;
};

#endif
