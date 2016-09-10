// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ThemeEditorWindow.h
// ��  �ܣ���Դ�༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _THEMEEDITORWINDOW_H_
# define _THEMEEDITORWINDOW_H_

#include <Main/FilterNode.h>

#include <Editor/TemplateEditorWindow.h>
#include <Editor/ResSelectorWindow.h>
#include <Editor/PropEditorWindow.h>
#include <Editor/TriggerEditorWindow.h>

#include <Core/ValueEditor.h>
#include <Core/TypeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class EditRootPanel;

struct StackItem
{
    Element* elem;
    String title;
    String returnName;

    StackItem()
        : elem(NULL)
    {
    }
};

// �༭������ϸ��Ϣ
struct ThemeEditorDetail
{
    // ӵ�д�����Ŀؼ�Ԫ��
    // ���������ȫ����Ϊ��
    DesignElement* owner;
};

class EditorStack : public Array<StackItem>
{
public:

    EditorStack()
        : _ownerWnd(NULL)
    {

    }

    void SetOwnerWnd(Window* pWnd)
    {
        _ownerWnd = pWnd;
    }

    void Push(const String& name, const String& title, const String& returnName)
    {
        StackItem item;
        int iCount = GetCount() - 1;
        item.elem = _ownerWnd->FindName(name);
        item.title = title;
        item.returnName = returnName;

        if (iCount >= 0)
        {
            GetItem(iCount).elem->SetVisibility(Visibility::Collapsed);
        }
        Add(item);
        ShowEditorPanel(GetCount() - 1);
    }

    void Pop()
    {
        int iCount = GetCount() - 1;
        if (iCount > 0)
        {
            GetItem(iCount).elem->SetVisibility(Visibility::Collapsed);
            RemoveAt(iCount);
            ShowEditorPanel(iCount - 1);
        }
    }

protected:

    void ShowEditorPanel(int index)
    {
        StackItem& item = GetItem(index);
        _ownerWnd->SetTitle(item.title);
        item.elem->SetVisibility(Visibility::Visible);

        if (item.returnName.Empty())
        {
            return;
        }

        Button* returnBtn = item.elem->FindElem<Button>(item.returnName);

        if (NULL != returnBtn && index > 0)
        {
            String returnName = _U("���� ");
            returnName += GetItem(index - 1).title;
            returnBtn->SetContent(new OString(returnName));
        }
    }

private:

    Window* _ownerWnd;
};

enum ShowEditType
{
    setRes,
    setStyle,
    setTemplate,
};

class ThemeEditorWindow : public suic::Window
{
public:

    ThemeEditorWindow(RootItem* root, ResourceDictionaryNode* resNode);
    ~ThemeEditorWindow();

    void Dispose();

    TemplateEditorWindow* GetTemplateEditorWindow()
    {
        return _tempEditorWnd;
    }

    PropEditorWindow* GetStylePropEditorWindow()
    {
        return _stylePropEditorWnd;
    }

    PropEditorWindow* GetTemplatePropEditorWindow()
    {
        return _templatePropEditorWnd;
    }

    StyleNode* GetStyleNode() const;
    void SetStyleNode(StyleNode* node);

    TemplateRootItem* GetTemplateNode() const;
    void SetTemplateNode(TemplateRootItem* node);

    ResourceDictionaryNode* GetResourceNode()
    {
        return _resourceNode;
    }

    suic::ListBox* GetResListBox();
    Project* GetProject() const;

    EditRootPanel* GetEditRootPanel();
    RootItem* GetRootItem() const;

    DesignElement* GetResourceElement() const;

    // ������Դ��ѯ��Ŀ��Ԫ��
    // ����ģ����˵����������ģ���ģ�常Ԫ�أ�ϵͳ����������ѯ��Դ
    void SetResourceElement(DesignElement* resTarget);
    void SetHideMainReturnButton();

    PropEditorWindow* ShowStylePropEditorWindow();
    PropEditorWindow* ShowTemplatePropEditorWindow();

    void ShowTriggersEditorWindow(TemplateRootItem* tempRoot);
    void SwitchToTemplateEditor(TemplateRootItem* tempNode);

    void ShowPropWindow();
    void SetModified();

    void SetMainReturnEvent(suic::EventHandler h);

    void OnDbClickTree(Element* sender, MouseButtonEventArg* e);

    // ��Դ�����仯��Ӧ�¼�
    void OnResItemSelectedChanged(suic::Element* sender, suic::SelectionChangedEventArg* e);
    void OnxKeyChanged(Element* sender, RoutedEventArg* e);
    void OnxKeyPreDown(Element* sender, suic::KeyboardEventArg* e);

    // ����ǰһ���༭����
    void OnReturnPrePanel(Element* sender, RoutedEventArg* e);
    void OnReturnMainPanel(Element* sender, RoutedEventArg* e);

    void OnAccessKey(suic::AccessKeyEventArg* e);

    // ��Դ����
public:

    String GenerateXKey();
    void AddResourceItem(ResTypeItem* resItem, TypeItem* ctrlItem, bool bFromType);

    void CreateStyleNode(ResTypeItem* resItem, TypeItem* ctrlItem, ResNodePtr& resNode);
    void CreateControlTemplateNode(ResTypeItem* resItem, TypeItem* ctrlItem, ResNodePtr& resNode);

    // ��Ӧ������Դ��ť
    void OnAddResButton(Element* sender, RoutedEventArg* e);
    // ��Ӧɾ�������ơ�������Դ��ť
    void OnOpResButton(Element* sender, RoutedEventArg* e);

protected:

    void InitTreeDoc();
    void RefleshStackShow();
    void RefleshEditPanel(const String& name, const String& title, const String& returnName);

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

    void InitChildEditPanel(const String& strPath, Element* pParent, const String& strName);

protected:

    TemplateEditorWindow* _tempEditorWnd;

    TriggerEditorWindow* _styleTrgEditorWnd;
    TriggerEditorWindow* _tempTrgEditorWnd;

    PropEditorWindow* _stylePropEditorWnd;
    PropEditorWindow* _templatePropEditorWnd;

    EditorControl* _prevEditCtrl;
    EditRootPanel* _editRootPanel;

    // ��ǰ�༭����Դ�ļ����󣨿�������Դ��Ҳ�����ǲ��֣�
    RootItem* _rootItem;
    DesignElement* _resTarget;

    bool _hideMainReturnButton;

    suic::ListBox* _resListBox;
    Project* _project;

    EditorStack _editorStatck;
    suic::EventHandler _mainReturnEvent;

    StyleNode* _styleNode;
    TemplateRootItem* _templateNode;
    ResourceDictionaryNode* _resourceNode;
};

#endif
