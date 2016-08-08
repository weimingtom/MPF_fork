/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"
#include "MainWindow.h"

bool MenuData::CanExecute(Object* target, Object* param)
{
    return true;
}

void MenuData::Execute(Object* target, Object* param)
{
    if (GetName().Equals("ϵͳ����"))
    {
        static SettingWindow* _setWnd;

        if (NULL == _setWnd)
        {
            _setWnd = new SettingWindow();
            _setWnd->ref();
            suic::XamlReader::LoadUri(NULL, _setWnd, "QQ/Layout/SettingWindow.xml");
        }

        if (_setWnd)
        {
            _setWnd->Show();
            _setWnd->CenterWindow();
        }
    }
    else
    {
        InfoBox::Show(GetName(), _U("����˵�"), InfoBox::IB_OK);
    }
}

MainWindow::MainWindow()
{
    _iAniNum = 0;
    _userMenu = NULL;
    _mainMenu = NULL;

    _timer = new AssignerTimer();
    _timer->ref();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::Dispose()
{
    //
    // �Ƿ���Դ
    //
    if (NULL != _userMenu)
    {
        _userMenu->unref();
    }

    if (NULL != _mainMenu)
    {
        _mainMenu->unref();
    }

    _timer->unref();
    _chatSessionMana.RemoveAll();
}

void MainWindow::OnMouseLeftButtonDown(suic::MouseButtonEventArg* e)
{
    suic::Window::OnMouseLeftButtonDown(e);
}

bool MainWindow::OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj)
{
    return false;
}

void MainWindow::OnConnect(suic::IXamlNode* pNode, suic::Object* target)
{
    
}

void MainWindow::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Window::OnLoaded(e);

    InitBusiness();
    UpdateArrange();

    // 
    // ���ھ�����ʾ
    //
    CenterWindow();

    //
    // �����¼���棬������ʹ�����Ƕ����LoginWindow
    //
    _login = suic::XamlReader::LoadUri(NULL, new LoginWindow(), "QQ/Layout/LoginWindow.xml").get();

    if (_login)
    {
        HWND hwnd = HANDLETOHWND(HwndHelper::GetHostHwnd(this));
        //::ShowWindow(hwnd, SW_HIDE);

        //
        // ��ģ̬�Ի�����ʾ
        //
        _login->ShowDialog();

        if (!_login->IsLogin())
        {
            //
            // �����¼���ɹ����رմ��ڣ��ڶ���������true��ʾ�����첽��ʽ�ر�
            //
            HwndHelper::CloseWindow(this, true);
            return;
        }
        //::ShowWindow(hwnd, SW_SHOW);
        //InvalidateVisual();
    }
}

void MainWindow::InitBusiness()
{
    //
    // ��ʼ�������û��б�
    //
    InitUserIds();
    InitUserInfo();

    //
    // ��ͷ��ؼ�ע����������뿪�¼�����������û���Ϣ��ʾ���ڹر�
    //
    Element* userInfo = FindElem<Element>(_U("HeadImg"));
    if (NULL != userInfo)
    {
        userInfo->AddMouseEnter(new MouseButtonEventHandler(this, &MainWindow::OnMouseEnterUserInfo), true);
        userInfo->AddMouseLeave(new MouseButtonEventHandler(this, &MainWindow::OnMouseLeaveUserInfo), true);
    }

    //
    // �ҽӻ�����ť�¼�������󵯳���������
    //
    Button* themeBtn = FindElem<Button>(_U("ThemeBtn"));
    if (NULL != themeBtn)
    {
        themeBtn->AddClick(new ClickEventHandler(this, &MainWindow::OnThemeClick));
    }

    //
    // ���մ������а�ť����¼�����������˵��ϵİ�ť���
    // ����Ҳ����ֱ���ڲ˵�������ע��
    //
    Button* menuBtn = FindElem<Button>(_U("MainMenuBtn"));
    if (NULL != menuBtn)
    {
        menuBtn->AddClick(new ClickEventHandler(this, &MainWindow::OnMainMenuClick));
    }

    //
    // ���һ�����ð�ťʱ���������˵�
    // ������ʾ�˲˵����û����ݰ󶨹���
    //
    ButtonBase* userSet = FindElem<ButtonBase>(_U("UserSet"));
    if (NULL != userSet)
    {
        userSet->AddClick(new ClickEventHandler(this, &MainWindow::OnUserSet));
        _userMenu = new suic::ContextMenu();
        _userMenu->ref();

        _userMenu->SetPlacementTarget(userSet);
        _userMenu->SetPlacement(PlacementMode::pmBottom);

        _userMenu->AddChild(new MenuData("��������", FindRes("imonline_icon")));
        _userMenu->AddChild(new MenuData("Q�Ұ�", FindRes("Qme_icon")));
        _userMenu->AddChild(new MenuData("�뿪", FindRes("away_icon")));
        _userMenu->AddChild(new MenuData("æµ", FindRes("busy_icon")));
        _userMenu->AddChild(new MenuData("�������", FindRes("mute_icon")));
        _userMenu->AddChild(new MenuData("����", FindRes("invisible_icon")));
        _userMenu->AddChild(new MenuData("����", FindRes("imoffline_icon")));
        _userMenu->AddChild(new MenuData("���״̬��Ϣ", NULL));

        _userMenu->AddChild(new suic::Separator());

        _userMenu->AddChild(new MenuData("�ر�����", NULL));
        _userMenu->AddChild(new MenuData("�ر�ͷ������", NULL));

        _userMenu->AddChild(new suic::Separator());

        _userMenu->AddChild(new MenuData("����QQ CTRL + ALT + L ", FindRes("lock20_icon")));

        _userMenu->AddChild(new suic::Separator());

        _userMenu->AddChild(new MenuData("ϵͳ����", NULL));
        _userMenu->AddChild(new MenuData("�ҵ�����", NULL));
        _userMenu->AddChild(new MenuData("�ҵ�QQ����", NULL));
    }

    // 
    // ͳһע��Button��·���¼�������Խ������ؼ������еİ�ť����¼�
    //
    TreeView* tv = DynamicCast<TreeView>(FindName(_U("UserIds")));
    if (NULL != tv)
    {
        tv->AddHandler(Button::ClickEvent, new ClickEventHandler(this, &MainWindow::OnButtonClicked));
    }

    //
    // ������ʾͷ��Ķ������Ŵ���С��͸���Ƚ��䡢��ת��϶�����
    //
    Element* fe = FindElem<Element>(_U("HeadImg"));
    StartHeadImgAnimate(fe);
}

void MainWindow::StartHeadImgAnimate(Element* fe, bool bScale)
{
    return;
    if (fe)
    {
        if (fe->GetRenderTransform())
        {
            fe->GetRenderTransform()->BeginAnimation(RotateTransform::AngleProperty, NULL);
        }

        TransformGroup* transGrp = new TransformGroup();
        RotateTransform* trans = new RotateTransform();
        RepeatBehavior rb;

        rb.type = RepeatBehavior::Type::Forever;

        if (bScale)
        {
            ScaleTransform* transScale = new ScaleTransform();
            DoubleAnimation* dAniX = new DoubleAnimation(0.5, 1, Duration(2000), FillBehavior::fbHoldEnd);
            DoubleAnimation* dAniY = new DoubleAnimation(0.5, 1, Duration(2000), FillBehavior::fbHoldEnd);

            transGrp->Add(transScale);

            dAniX->SetRepeatBehavior(rb);
            dAniX->SetAutoReverse(true);
            
            dAniY->SetRepeatBehavior(rb);
            dAniY->SetAutoReverse(true);

            transScale->BeginAnimation(ScaleTransform::ScaleXProperty, dAniX);
            transScale->BeginAnimation(ScaleTransform::ScaleYProperty, dAniY);
        }

        transGrp->Add(trans);
        
        fe->SetRenderTransform(transGrp);
        fe->SetRenderTransformOrigin(fPoint(0.5f, 0.5f));
        DoubleAnimation* dAni = new DoubleAnimation(0, 360, Duration(2000), FillBehavior::fbHoldEnd);
        
        // ���ö�����Ϊ
        dAni->SetRepeatBehavior(rb);
        // ��������
        trans->BeginAnimation(RotateTransform::AngleProperty, dAni);

        DoubleAnimation* dhAni = new DoubleAnimation(0.1f, 1.0f, Duration(2000), FillBehavior::fbHoldEnd);

        dhAni->SetRepeatBehavior(rb);
        dhAni->SetAutoReverse(true);
        dhAni->SetAccelerationRatio(0.8f);
        fe->BeginAnimation(OpacityProperty, dhAni);
    }
}

void MainWindow::OnThemeClick(Element* sender, RoutedEventArg* e)
{
    if (_themeWnd->IsClosed())
    {
        _themeWnd = XamlReader::LoadUri(NULL, new ThemeWindow(), "QQ/Layout/ThemeWindow.xml").get();

        if (_themeWnd)
        {
            _themeWnd->Show(this);
        }
    }
}

void MainWindow::OnMenuButtonClick(Element* sender, RoutedEventArg* e)
{
    suic::ContextMenu::CloseContextMenu();
    suic::Element* pBtn = DynamicCast<suic::Element>(e->GetOriginalSource());
    if (pBtn->GetName().Equals("SwitchUser"))
    {
        if (_login)
        {
            _login->ShowDialog();
        }
        e->SetHandled(true);
    }
    else if (pBtn->GetName().Equals("EditPwd"))
    {
        if (_login)
        {
            Hide();
            _login->ShowDialog();
            Show();
        }
        e->SetHandled(true);
    }
    else if (pBtn->GetName().Equals("ExitApp"))
    {
        HwndHelper::CloseWindow(this, true);
    }
}

void MainWindow::OnMainMenuClick(Element* sender, RoutedEventArg* e)
{
    if (NULL ==_mainMenu)
    {
        suic::ControlTemplate* menuTemp = DynamicCast<suic::ControlTemplate>(FindRes("MainMenuTemp"));
        if (menuTemp)
        {
            suic::AutoObj root = menuTemp->LoadContent(NULL);
            if (root.get() != NULL)
            {
                _mainMenu = DynamicCast<suic::ContextMenu>(root.get());
                if (NULL != _mainMenu)
                {
                    _mainMenu->ref();
                    AddHandler(suic::ButtonBase::ClickEvent, new suic::ClickEventHandler(this, &MainWindow::OnMenuButtonClick));

                    _mainMenu->SetPlacementTarget(sender);
                    _mainMenu->SetPlacement(PlacementMode::pmTop);
                }
            }
        }
    }
    
    if (NULL != _mainMenu)
    {
        _mainMenu->TrackContextMenu();
        _mainMenu->unref();
        _mainMenu = NULL;
    }
}

void MainWindow::OnUserSet(Element* sender, RoutedEventArg* e)
{
    CheckBox* checkBox = RTTICast<CheckBox>(sender);
    if (NULL != checkBox)
    {
        if (NULL != _userMenu)
        {
            _userMenu->TrackContextMenu();
        }
        checkBox->SetChecked(Boolean::False);
    }
}

void MainWindow::OnButtonClicked(Element* sender, RoutedEventArg* e)
{
    Element* btn = DynamicCast<Element>(e->GetOriginalSource());
    if (btn && btn->GetName().Equals(_U("_tv_qq_group")))
    {
        TreeView* tv = DynamicCast<TreeView>(FindName(_U("UserIds")));
        TreeViewItem* tvItem = DynamicCast<TreeViewItem>(btn->GetUserTag(0));
        if (NULL != tvItem && NULL != tv)
        {
            tvItem->SetIsExpanded(false);
            tv->UpdateLayout();
        }
        e->SetHandled(true);
    }
}

void MainWindow::OnTreeKeydown(Element* sender, KeyboardEventArg* e)
{
    TreeView* tv = DynamicCast<TreeView>(sender);
    if (tv && e->GetKey() == Key::kSpace)
    {
        UserUseId* obj = DynamicCast<UserUseId>(tv->GetSelectedItem());

        if (obj)
        {
            // �����Ự
            _chatSessionMana.OpenChatSession(obj);
        }

        e->SetHandled(true);
    }
}

void MainWindow::OnDblTreeClick(Element* sender, MouseButtonEventArg* e)
{
    //
    // ���˫���¼�
    //
    Element* elem = DynamicCast<Element>(e->GetOriginalSource());
    UserUseId* obj = DynamicCast<UserUseId>(elem->GetDataContext());

    if (obj)
    {
        // �����Ự
        _chatSessionMana.OpenChatSession(obj);
    }

    e->SetHandled(true);
}

void MainWindow::OnTreeScrollChanged(Element* sender, ScrollChangedEventArg* e)
{
    TreeView* tv = DynamicCast<TreeView>(sender);
    if (tv)
    {
        ButtonBase* grpElem = DynamicCast<ButtonBase>(tv->GetTemplateChild(_U("_tv_qq_group")));
        Element* visualElem = tv->GetItemsHost()->GetVisibleChild(0);
        TreeViewItem* tvItem = DynamicCast<TreeViewItem>(visualElem);

        if (tvItem->GetVisualOffset().y <= -16 && tvItem->IsExpanded())
        {
            grpElem->SetVisibility(Visibility::Visible);
            grpElem->SetContent(tvItem->GetDataContext());
            grpElem->SetUserTag(0, tvItem);
        }
        else if (!tvItem->IsExpanded() || tvItem->GetVisualOffset().y >= 0)
        {
            grpElem->SetVisibility(Visibility::Collapsed);
            grpElem->SetContent(NULL);
            grpElem->SetTag(NULL);
        }
    }
}

void MainWindow::OnCheckUserWndTimer(Object* sender, EventArg* e)
{
    Element* userInfo = FindElem<Element>(_U("HeadImg"));

    if (!_userWnd || !_userWnd->IsValid() || 
        (!PointTransform::PointInElement(_userWnd.get(), NULL) && 
        !PointTransform::PointInElement(userInfo, NULL)))
    {
        _timer->SetEnabled(false);
        _userWnd->Close();
        _userWnd = NULL;
    }
}

void MainWindow::OnMouseEnterUserInfo(Element* sender, MouseButtonEventArg* e)
{
    if (_userWnd.get() == NULL)
    {
        _userWnd = XamlReader::LoadUri(NULL, new UserInfoWindow(UserManager::Ins()->GetCurrentUser()), "QQ/Layout/UserInfoWindow.xml").get();
        if (_userWnd)
        {
            Size bounds = Environment::GetScreenBound();
            Point pt = PointTransform::GetRootScreenPos(this);
            Element* userInfo = FindElem<Element>(_U("HeadImg"));

            int left = pt.x + GetWidth();
            int top = pt.y + userInfo->PointToScreen(Point()).y;

            _userWnd->SetPos(left, top);
            _userWnd->Show(this);

            //
            // ������ʱ���������뿪ʱ�ر��û���Ϣ�Ի���
            //
            _timer->SetInterval(500);
            _timer->SetTick(EventHandler(this, &MainWindow::OnCheckUserWndTimer));
            _timer->Start();
        }
    }
    e->SetHandled(true);
}

void MainWindow::OnMouseLeaveUserInfo(Element* sender, MouseButtonEventArg* e)
{
    e->SetHandled(true);
}

void MainWindow::InitUserInfo()
{

}

void MainWindow::InitUserIds()
{
    suic::TreeView* tree = FindElem<suic::TreeView>(_U("UserIds"));
    if (tree != NULL)
    {
        //
        // ������Դ�ж�����û���������ģ�� 
        //
        suic::DataTemplate* userGrp = DynamicCast<suic::DataTemplate>(tree->FindRes(_U("UserGroupTemp")));

        //
        // ������Դ�ж�����û���Ϣ����ģ��
        //
        suic::DataTemplate* userIdTemp = DynamicCast<suic::DataTemplate>(tree->FindRes(_U("UserIdTemp")));

        //
        // ��������ģ��ѡ������ϵͳ�����û�������û���Ϣѡ���Ӧ������ģ�棩
        //
        tree->SetItemTemplateSelector(new UserTemplateSelector(userGrp, userIdTemp));

        //
        // ע������¼�
        //
        tree->AddHandler(suic::ScrollViewer::ScrollChangedEvent, new ScrollChangedEventHandler(this, &MainWindow::OnTreeScrollChanged));
        
        //
        // ע�����Ԥ��ʱ�䣨��Ҫ�����������˫���¼���
        //
        tree->AddMouseDoubleClick(new MouseButtonEventHandler(this, &MainWindow::OnDblTreeClick));

        //
        // ע������¼�����
        //
        tree->AddKeyDown(new KeyboardEventHandler(this, &MainWindow::OnTreeKeydown));

        // ���ú�����Ϣ����Դ
        tree->SetItemsSource(UserManager::Ins()->GetFriendGroup());
    }
}