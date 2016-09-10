// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����MainWindow.h
// ��  �ܣ������洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _MAINWINDOW_H_
# define _MAINWINDOW_H_

using namespace suic;

#include "StartWindow.h"
#include "SlnTreeManager.h"
#include <main/EncryResource.h>
#include <Editor/ControlAssetsWindow.h>

#define PREVIEWAPP _U("uidpreviewu.exe")

class LoadPrjThread : public suic::Thread
{
public:

    LoadPrjThread(suic::InvokeProxy* h)
    {
        _handler = h;
        _project = NULL;
    }

    void Run();
    void OnInvoker(Object* sender, InvokerArg* e);

protected:

    Project* _project;
    suic::InvokeProxy* _handler;
};

class LockShowHandler : public suic::AssignerInvoker
{
public:

    LockShowHandler()
    {
        _assigner = NULL;
    }

    ~LockShowHandler()
    {
    }

    void Init(Element* target)
    {
        _assigner = target->GetAssigner();
    }

    void Invoke()
    {
        if (_notifyLocked)
        {
            _target->NotifyLockedChanged();
        }
        else
        {
            _target->NotifyShowChanged();
        }
    }

    void Notify(DesignElement* target, bool notify)
    {
        _target = target;
        _notifyLocked= notify;
        _assigner->BeginInvoke(this);
    }

private:

    bool _notifyLocked;
    DesignElement* _target;
    suic::Assigner* _assigner;
};

class MainWindow : public suic::Window
{
public:

    MainWindow();
    ~MainWindow();

    /// <summary>
    ///   ��ָ��·���Ĺ���
    /// </summary>
    /// <param name="path">����·��</param>
    /// <returns>���̶���</returns>
    Project* OpenProject(const suic::String& path);

    /// <summary>
    ///   ���ݹ������ƺ�·�������µĹ���
    /// </summary>
    /// <param name="name">��������</param>
    /// <param name="path">����·��</param>
    /// <returns>���̶���</returns>
    Project* CreateProject(const suic::String& name, const suic::String& path);

    void ExecutePreview(ElementRootItem* rootElem);

    Project* OpenOrLoadProject();
    void OpenOrLoadProjectSuccess(Project* prj);

    void ShowStartWindow();

    void SwitchToThemeView(suic::Window* themeElem);
    void SwitchToMainView();

protected:

    void OnMainMenuClick(suic::DpObject* sender, suic::RoutedEventArg* e);
    void OnHyperlinkClick(suic::DpObject* sender, suic::RoutedEventArg* e);

protected:

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);
    void OnShowWindow();

    void OnClosing(suic::CancelEventArg* e);
    void OnAccessKey(suic::AccessKeyEventArg* e);

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

    void OnRedoUndoButtonClick(suic::Element* sender, suic::RoutedEventArg* e);
    void OnPreviewButtonClick(suic::Element* sender, suic::RoutedEventArg* e);
    void OnExportResButtonClick(suic::Element* sender, suic::RoutedEventArg* e);
    void OnMoreExpanderClick(suic::Element* sender, suic::RoutedEventArg* e);

    void OnMenuButtonClick(suic::Element* sender, suic::RoutedEventArg* e);

    void OnApplyTemplate();
    void InitTreeDoc();
    void InitToolbarEvents();

    bool StartProcess(wchar_t* path, int timeout);

    void ExecuteRedo();
    void ExecuteUndo();

protected:

    HMODULE _uidCore;

    // Ԫ�ض�����
    ElementTree* _eleTree;
    // ������ͼ�������
    SlnTreeManager* _docMana;

    // �����������˵�
    suic::ContextMenu* _mainMenu;
    suic::CheckBox* _menuBtn;

    // ��ʼ���棬���ڴ򿪣���������
    StartWindow* _startWindow;
    // �ؼ���Դ������
    ControlAssetsWindow* _ctrlAssetsWnd;

    LoadPrjThread* _loadThread;
    suic::Handler* _loadPrjHandler;

    Window* _previewWnd;
    EncryResource* _encryWnd;
};

#endif

