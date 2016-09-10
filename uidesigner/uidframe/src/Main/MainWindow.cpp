/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"

#include "MainWindow.h"
#include "DesignPanel.h"
#include "Solution.h"
#include <main/XamlRootItem.h>

#include <shellapi.h>

LockShowHandler* g_lockshow = NULL;

MainWindow::MainWindow()
{
    _uidCore = NULL;
    _eleTree = NULL;
    _startWindow = NULL;
    _ctrlAssetsWnd = NULL;
    _docMana = new SlnTreeManager();

    _loadThread = NULL;
    _loadPrjHandler = NULL;

    _previewWnd = NULL;

    _mainMenu = NULL;
    _menuBtn = NULL;

    _encryWnd = NULL;
}

MainWindow::~MainWindow()
{
    if (NULL != _previewWnd)
    {
        _previewWnd->unref();
    }

    if (NULL != _loadThread)
    {
        _loadThread->unref();
    }

    if (NULL != _loadPrjHandler)
    {
        _loadPrjHandler->unref();
    }

    if (NULL != _startWindow)
    {
        _startWindow->unref();
    }

    if (NULL != _ctrlAssetsWnd)
    {
        _ctrlAssetsWnd->unref();
    }

    if (NULL != _encryWnd)
    {
        _encryWnd->unref();
    }

    delete _docMana;

    if (_uidCore)
    {
        ::FreeLibrary(_uidCore);
    }
}

Project* MainWindow::OpenProject(const suic::String& path)
{
    return _docMana->OpenProject(path);
}

Project* MainWindow::CreateProject(const suic::String& name, const suic::String& path)
{
    return _docMana->CreateProject(name, path);
}

void MainWindow::OnClosing(CancelEventArg* e)
{
    if (_docMana->HasModified() && !Utils::CheckUseTimeout(_U("")))
    {
        int iResult = InfoBox::Show(_U("�ļ��Ѿ��޸ģ��Ƿ񱣴棿"), _U("��ʾ"), InfoBox::IB_YesNoCancel);
        if (iResult == InfoBox::Cancel)
        {
            e->SetCancel(true);
            return;
        }
        else if (iResult == InfoBox::Yes)
        {
            _docMana->SaveAll();
        }
        else if (iResult == InfoBox::No)
        {
            ;
        }
    }
}

void MainWindow::OnAccessKey(suic::AccessKeyEventArg* e)
{
    if (e->GetKey().Equals(_U("z")))
    {
        ExecuteUndo();
    }
    else
    {
        ExecuteRedo();
    }
}

bool MainWindow::OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj)
{
    suic::String strName = pNode->FindAttri(_U("Name"));
    if (strName.Equals(_U("SlnTreeView")))
    {
        obj = new ElementTree();
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::OnConnect(suic::IXamlNode* pNode, suic::Object* target)
{
}

void MainWindow::OnInitialized(EventArg* e)
{
    suic::Window::OnInitialized(e);
    g_lockshow = new LockShowHandler();
    g_lockshow->ref();
    g_lockshow->Init(this);

    SwitchToMainView();

    AddHandler(suic::MenuItem::ClickEvent, new suic::RoutedEventHandler(this, &MainWindow::OnMainMenuClick));

    // ע���ݼ���ctrl+z��ctrl+y��
    AcceleratorOp::Register(ModifierKey::mkControl, _U("z"), this);
    AcceleratorOp::Register(ModifierKey::mkControl, _U("y"), this);
}

void MainWindow::OnMainMenuClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    suic::MenuItem* pItem = RTTICast<suic::MenuItem>(e->GetOriginalSource());
    if (NULL != pItem && NULL != pItem->GetDataContext())
    {
        suic::String strName = pItem->GetDataContext().GetTarget()->ToString();
        if (strName.Equals(_U("�˳�")))
        {
            suic::Application::Current()->Shutdown(0);
        }
        else if (strName.Equals(_U("�½�")) || strName.Equals(_U("��")))
        {
            ShowStartWindow();
        }
        else if (strName.Equals(_U("����")))
        {
            ObjTreeManager* objMana = _docMana->GetObjTreeManager();
            ElementRootItem* rootItem = objMana->GetDesignPanel()->GetRootElement();
            if (NULL != rootItem)
            {
                rootItem->Save();
            }
        }
        else if (strName.Equals(_U("ȫ������")))
        {
            _docMana->SaveAll();
        }

        e->SetHandled(true);
    }
}

void MainWindow::SwitchToThemeView(suic::Window* themeElem)
{
    suic::Panel* mainView = FindElem<suic::Panel>(_U("mainGridView"));
    suic::Panel* themeView = FindElem<suic::Panel>(_U("mainThemeView"));
    const String strPath = "/mpfuid;/resource/uidesign/layout/ThemeEditor.xaml";

    if (mainView != NULL && themeView != NULL)
    {
        mainView->SetVisibility(suic::Visibility::Collapsed);
        themeView->SetVisibility(suic::Visibility::Visible);

        themeView->GetChildren()->Clear();

        suic::XamlReader xamlRead;
        themeElem->ref();
        xamlRead.LoadUri(NULL, themeElem, strPath);
        suic::FrameworkElement* fe = themeElem->FindElem<suic::FrameworkElement>(_U("editRootPanel"));

        themeView->AddChild(themeElem);

        themeElem->SetBorderThickness(suic::Rect());

        themeElem->SetWidth(-1);
        themeElem->SetHeight(-1);
        
        themeElem->FindName(_U("ThemeCap"))->SetVisibility(suic::Visibility::Collapsed);

        themeElem->unref();
    }
}

void MainWindow::SwitchToMainView()
{
    suic::Panel* mainView = FindElem<suic::Panel>(_U("mainGridView"));
    suic::Panel* themeView = FindElem<suic::Panel>(_U("mainThemeView"));

    if (mainView != NULL && themeView != NULL)
    {
        mainView->SetVisibility(suic::Visibility::Visible);
        themeView->SetVisibility(suic::Visibility::Collapsed);
        themeView->GetChildren()->Clear();
    }
}

Project* MainWindow::OpenOrLoadProject()
{
    Project* prj = NULL;

    if (!_startWindow->GetOpenProjectPath().Empty())
    {
        prj = OpenProject(_startWindow->GetOpenProjectPath());
    }
    else
    {
        String strPrjDir = _startWindow->GetNewProjectPath();
        strPrjDir += _U("\\") + _startWindow->GetProjectName() + _U("\\");
        prj = CreateProject(_startWindow->GetProjectName(), strPrjDir);
    }

    return prj;
}

void MainWindow::OpenOrLoadProjectSuccess(Project* prj)
{
    if (NULL != prj)
    {
        _startWindow->AddRecentlyProject(prj->GetProjectName(), prj->GetProjectPath());
        _startWindow->SaveRecentlyProjects();
        String strTitle;

        strTitle.Format(_U("MPF�������(%s)"), prj->GetProjectPath().c_str());
        SetTitle(strTitle);

        prj->GetSlnTreeManager()->RefleshProject(prj);
    }
}

void LoadPrjThread::Run()
{
    MainWindow* mainWnd = (MainWindow*)_handler->GetTarget();

    try
    {
        _project = mainWnd->OpenOrLoadProject();
    }
    catch (...)
    {
    }

    _handler->PostInvoker(0, NULL);
}

void LoadPrjThread::OnInvoker(Object* sender, InvokerArg* e)
{
    MainWindow* mainWnd = (MainWindow*)_handler->GetTarget();
    mainWnd->OpenOrLoadProjectSuccess(_project);
}

void MainWindow::ShowStartWindow()
{
    if (NULL != _startWindow)
    {
        _startWindow->unref();
        _startWindow = NULL;
    }

    if (NULL == _startWindow)
    {
        Project* prj = NULL;
        _startWindow = new StartWindow();
        _startWindow->ref();
        suic::Builder::BuildUri(_startWindow, "/mpfuid;/resource/uidesign/layout/StartWindow.xaml");
        _startWindow->ShowDialog();

        if (!_startWindow->IsCanContinue())
        {
            AsyncClose();
        }
        else 
        {
            /*_loadPrjHandler = new Handler(this);
            _loadPrjHandler->ref();
            _loadThread = new LoadPrjThread(_loadPrjHandler);
            _loadThread->ref();

            _loadPrjHandler->Invoker += InvokerHandler(_loadThread, &LoadPrjThread::OnInvoker);
            _loadThread->Start();*/

            prj = OpenOrLoadProject();
            OpenOrLoadProjectSuccess(prj);
        }
    }
}

void MainWindow::OnShowWindow()
{
    
}

void MainWindow::OnHyperlinkClick(suic::DpObject* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    suic::Element* pLink = CASTTOE(e->GetOriginalSource());
    if (pLink->GetName().Equals(_U("txtLink")))
    {
        ::ShellExecute(NULL, L"open", L"www.shidian.net.cn", NULL, NULL, SW_SHOWNORMAL);
    }
}

void MainWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);

    InitTreeDoc();

    // ���ھ�����ʾ
    CenterWindow();

    // ��ʼ�����������¼�
    InitToolbarEvents();

    // �ҽ�TextBlock���Hyperlink����¼�
    suic::TextBlock* pLink = FindElem<suic::TextBlock>(_U("myBlk"));
    if (NULL != pLink)
    {
        pLink->AddHandler(suic::Hyperlink::ClickEvent, new suic::RoutedEventHandler(this, &MainWindow::OnHyperlinkClick));
    }

    ShowStartWindow();
}

void MainWindow::InitToolbarEvents()
{
    // ����Ԥ����ť
    suic::Button* previewBtn = FindElem<suic::Button>(_U("PreviewBtn"));
    if (NULL != previewBtn)
    {
        previewBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnPreviewButtonClick));
    }

    // �򿪸���ؼ����水ť
    suic::Button* exportBtn = FindElem<suic::Button>(_U("ExportRes"));
    if (NULL != exportBtn)
    {
        exportBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnExportResButtonClick));
    }

    // ������ť
    suic::Button* redoBtn = FindElem<suic::Button>(_U("RedoBtn"));
    if (NULL != redoBtn)
    {
        redoBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnRedoUndoButtonClick));
    }

    // ������ť
    suic::Button* undoBtn = FindElem<suic::Button>(_U("UndoBtn"));
    if (NULL != undoBtn)
    {
        undoBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnRedoUndoButtonClick));
    }

    // ���浱ǰ���ڱ༭���ݰ�ť
    suic::Button* saveBtn = FindElem<suic::Button>(_U("SaveBtn"));
    if (NULL != saveBtn)
    {
        saveBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnRedoUndoButtonClick));
    }

    // �������������˵���ť
    _menuBtn = FindElem<suic::CheckBox>(_U("menuBtn"));
    if (NULL != _menuBtn)
    {
        _menuBtn->AddClick(new suic::ClickEventHandler(this, &MainWindow::OnMenuButtonClick));
    }
}

void MainWindow::ExecuteRedo()
{
    ObjTreeManager* objMana = _docMana->GetObjTreeManager();
    ElementRootItem* rootItem = objMana->GetDesignPanel()->GetRootElement();

    if (rootItem != NULL && rootItem->GetRootItemData() != NULL)
    {
        rootItem->GetRootItemData()->ruMana.Redo();
    }
}

void MainWindow::ExecuteUndo()
{
    ObjTreeManager* objMana = _docMana->GetObjTreeManager();
    ElementRootItem* rootItem = objMana->GetDesignPanel()->GetRootElement();

    if (rootItem != NULL && rootItem->GetRootItemData() != NULL)
    {
        rootItem->GetRootItemData()->ruMana.Undo();
    }
}

void MainWindow::OnApplyTemplate()
{
}


void MainWindow::OnMoreExpanderClick(suic::Element* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
}

void MainWindow::OnRedoUndoButtonClick(suic::Element* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    ObjTreeManager* objMana = _docMana->GetObjTreeManager();

    if (sender->GetName().Equals(_U("RedoBtn")))
    {
        ExecuteRedo();
    }
    else if (sender->GetName().Equals(_U("SaveBtn")))
    {
        FileRootItem* pRootItem = suic::RTTICast<FileRootItem>(_docMana->GetProjectTree()->GetSelectedItem());
        if (NULL != pRootItem)
        {
            pRootItem->Save();
        }
        /*if (NULL != objMana->GetDesignPanel())
        {
            objMana->GetDesignPanel()->GetRootElement()->Save();
        }*/
    }
    else
    {
        ExecuteUndo();
    }
}

void MainWindow::OnMenuButtonClick(suic::Element* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);
    if (NULL == _mainMenu)
    {
        _mainMenu = new suic::ContextMenu();
        _mainMenu->ref();

        _mainMenu->AddChild(new suic::OString(_U("�½�")));
        _mainMenu->AddChild(new suic::OString(_U("��")));
        _mainMenu->AddChild(new suic::OString(_U("����")));
        _mainMenu->AddChild(new suic::OString(_U("ȫ������")));

        _mainMenu->AddChild(new suic::Separator());

        _mainMenu->AddChild(new suic::OString(_U("�˳�")));
    }

    _mainMenu->SetPlacement(suic::PlacementMode::pmBottom);
    _mainMenu->SetPlacementTarget(sender);
    _mainMenu->TrackContextMenu();

    _menuBtn->SetChecked(Boolean::False);
}

void MainWindow::OnExportResButtonClick(suic::Element* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);

    if (Utils::CheckUseTimeout(_U("����Beta�汾,����������!")))
    {
        return;
    }

    if (_docMana->HasModified())
    {
        int iResult = InfoBox::Show(_U("�ļ��Ѿ��޸ģ��Ƿ����������"), _U("��ʾ"), InfoBox::IB_YesNo);
        if (iResult == InfoBox::No)
        {
            return;
        }
    }

    if (_docMana->GetProjectCount() > 0)
    {
        if (NULL == _encryWnd)
        {
            suic::String strPath = "/mpfuid;/resource/uidesign/layout/EncryWindow.xaml";
            _encryWnd = new EncryResource(_docMana->GetProject(0));
            _encryWnd->ref();
            suic::Builder::BuildUri(_encryWnd, strPath);
        }
        else
        {
            _encryWnd->SetProject(_docMana->GetProject(0));
        }
        
        _encryWnd->ShowDialog();
    }
}

void MainWindow::OnPreviewButtonClick(suic::Element* sender, suic::RoutedEventArg* e)
{
    e->SetHandled(true);

    ObjTreeManager* objMana = _docMana->GetObjTreeManager();
    
    ElementRootItem* rootElem = objMana->GetDesignPanel()->GetRootElement();
    if (rootElem->IsModified())
    {
        rootElem->Save();
    }
    ExecutePreview(rootElem);
}

bool MainWindow::StartProcess(wchar_t* path, int timeout)
{
    wchar_t appCmd[512];
    DWORD dwRet = WAIT_OBJECT_0;
    PROCESS_INFORMATION pi = {0}; 
    STARTUPINFO si = {0}; 
    si.cb = sizeof(STARTUPINFO); 

    appCmd[0] = L'\0';
    swprintf_s(appCmd, 511, path);

    BOOL bRet = ::CreateProcess(NULL, appCmd, NULL, NULL, FALSE, 
        0, NULL, NULL, &si, &pi);

    if (bRet)
    {
        dwRet = ::WaitForSingleObject(pi.hProcess, timeout);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        return (dwRet == WAIT_OBJECT_0);
    }
    else
    {
        return false;
    }
}

void MainWindow::ExecutePreview(ElementRootItem* rootElem)
{
    if (NULL == rootElem)
    {
        return ;
    }

    /*suic::String strWnd;
    Project* pPrj = rootElem->GetProject();
    suic::String strPrjName = pPrj->GetProjectName();
    suic::Mulstr strXml = rootElem->GetResXml(_U("")).c_str();

    if (NULL != _previewWnd)
    {
        _previewWnd->Close();
        _previewWnd = NULL;
    }

    _previewWnd = new suic::Window();
    _previewWnd->ref();

    suic::DesignHelper::SetInDesignMode(_previewWnd, true);
    suic::Builder::BuildXml(_previewWnd, strXml.c_str());
    
    _previewWnd->Show();*/

    suic::String strPreviewPath = suic::FileDir::CalculatePath(PREVIEWAPP);
    suic::String strCmd;
    suic::String strApp;
    suic::String strWnd;

    Project* pPrj = rootElem->GetProject();
    suic::String strPrjName = pPrj->GetProjectName();
    
    strApp.Format(_U("/%s;/application.xaml"), strPrjName.c_str());
    strWnd.Format(_U("/%s;/%s"), strPrjName.c_str(), rootElem->GetFileName().c_str());

    strCmd.Format(_U("\"%s\" %s=%s=%s=%s"), strPreviewPath.c_str()
        , strPrjName.c_str(), pPrj->GetProjectDir().c_str()
        , strApp.c_str(), strWnd.c_str());

    StartProcess(strCmd.c_str(), 0);
}

void MainWindow::InitTreeDoc()
{
    suic::IAddChild* pAddChild = NULL;
    // ��ʼ������༭���
    ObjTreeManager* objTree = _docMana->GetObjTreeManager();
    ScrollViewer* pViewer = FindElem<ScrollViewer>(_U("DesignViewer"));

    if (NULL != pViewer)
    {
        _docMana->InitSolutionTree(this);

        DesignPanel* designWnd = objTree->GetDesignPanel();
        suic::Element* ctrlWnd = FindName(_U("CtrlToolbar"));
        
        pAddChild = suic::DynamicCast<suic::IAddChild>(ctrlWnd);

        pViewer->SetContent(designWnd);

        if (NULL != pAddChild)
        {
            pAddChild->AddChild(objTree->GetControlWindow()->GetControlWindow(ctrlWnd));
        }
    }

    suic::Element* propWnd = FindName(_U("editRootPanel"));
    pAddChild = suic::DynamicCast<suic::IAddChild>(propWnd);

    if (NULL != pAddChild)
    {
        suic::FrameworkElement* fe = objTree->GetPropertyWindow()->GetPropertyWindow(propWnd);
        if (NULL != fe)
        {
            pAddChild->AddChild(fe);
            objTree->GetPropertyWindow()->InitEditRootPanel(this, fe);
        }
    }
}

