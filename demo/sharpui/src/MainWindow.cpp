/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"
#include "MainWindow.h"
#include <ShellAPI.h>

MainWindow::MainWindow()
{
    _isPendingHome = false;
    _isPendingCtrl = false;
    _currentMode = ViewMode::vmHome;
    _timer = new AssignerTimer();
    _timer->ref();
}

MainWindow::~MainWindow()
{
    _timer->unref();
}

GroupData* MainWindow::NewGroupData(suic::String name)
{
    GroupData* group = new GroupData(name);
    group->GetItems()->AddItem(new suic::OString(_U("Sub Group 1")));
    group->GetItems()->AddItem(new suic::OString(_U("Sub Group 2")));
    group->GetItems()->AddItem(new suic::OString(_U("Sub Group 3")));
    group->GetItems()->AddItem(new suic::OString(_U("Sub Group 4")));
    group->GetItems()->AddItem(new suic::OString(_U("Sub Group 5")));
    return group;
}

void MainWindow::InitGroupListData(suic::ListBox* pListBox)
{
    if (NULL != pListBox)
    {
        GroupData* group = NewGroupData(_U("һ������ 1"));
        GroupData* subGroup = new GroupData(_U("�������� 1-1"));

        subGroup->GetItems()->AddItem(new suic::OString(_U("Sub Group 1-1")));
        subGroup->GetItems()->AddItem(new suic::OString(_U("Sub Group 1-2")));
        subGroup->GetItems()->AddItem(new suic::OString(_U("Sub Group 1-3")));

        group->GetItems()->AddItem(subGroup);

        pListBox->AddChild(new OString(_U("������ʾ")));
        pListBox->AddChild(group);
        pListBox->AddChild(NewGroupData(_U("һ������ 2")));
        pListBox->AddChild(NewGroupData(_U("һ������ 3")));
    }

}

void MainWindow::SwitchToControl(String name)
{
    Panel* fe = FindElem<Panel>(_U("CtrlArea"));
    FrameworkElement* elem = NULL;
    _ctrls.TryGetValue(name, elem);

    if (elem == NULL && fe)
    {
        String strPath;
        strPath.Format(_U("resource/MPF/Controls/%s.xml"), name.c_str());
        ObjectPtr obj = NULL;
        
        obj = XamlReader::LoadUri(NULL, NULL, strPath.c_str());
        elem = RTTICast<FrameworkElement>(obj.get());

        if (NULL != elem)
        {
            _ctrls.Add(name, elem);
        }

        ListView* listView = RTTICast<ListView>(elem);
        if (listView)
        {
            for (int i = 0; i < 50000; ++i)
            {
                NameField* pName = new NameField(i + 1, _U("����ͣ��"));
                pName->SetSexy(_U("��"));
                listView->AddChild(pName);
            }
        }

        if (name.Equals(_U("ListBox")))
        {
            elem->ApplyTemplate();
            suic::TabControl* tabCtrl = elem->FindElem<suic::TabControl>(_U("TabList"));
            if (NULL != tabCtrl && tabCtrl->GetCount() >= 2)
            {
                suic::TabItem* tabItem = RTTICast<suic::TabItem>(tabCtrl->GetItem(1));
                suic::ListBox* pListBox = RTTICast<suic::ListBox>(tabItem->GetContent());
                InitGroupListData(pListBox);
            }
        }
    }

    if (NULL != elem)
    {
        fe->GetChildren()->Clear();
        fe->AddChild(elem);

        Button* pBtn = elem->FindElem<Button>(_U("SetPosBtn"));
        if (pBtn != NULL)
        {
            pBtn->AddClick(new ClickEventHandler(this, &MainWindow::OnSetPosBtnClicked));
        }
        else
        {
            RepeatButton* pRepeatBtn = elem->FindElem<RepeatButton>(_U("RepeatBtn"));
            if (pRepeatBtn)
            {
                pRepeatBtn->AddClick(new ClickEventHandler(this, &MainWindow::OnSetPosBtnClicked));
            }
        }
    }

    UpdateLayout();
}

void MainWindow::OnCtrlAreaStart(Object* sender, EventArg* e)
{
    Element* fe = FindName(_U("CtrlArea"));
    fe->SetVisibility(Visibility::Visible);
}

void MainWindow::OnLeftAniEnd(Object* sender, EventArg* e)
{
    Element* feLeft = FindName(_U("DemonPanel"));
    feLeft->SetVisibility(Visibility::Collapsed);
}

void MainWindow::OnTimer(Object* sender, EventArg* e)
{
    ListBox* listObj = FindElem<ListBox>(_U("CtrlPanel"));
    ElementPtr listItem = DynamicCast<Element>(listObj->GetSelectedItem());
    if (listItem.get() != DpProperty::UnsetValue())
    {
        listObj->ScrollIntoView(listItem.get());
    }
    _timer->SetEnabled(false);
}

void MainWindow::OnRightAniEnd(Object* sender, EventArg* e)
{
    if (_currentMode == ViewMode::vmControl)
    {
        /*_timer->SetInterval(1000);
        _timer->SetTick(EventHandler(this, &MainWindow::OnTimer));
        _timer->Start();*/

        // 
        // �ҵ��ؼ���ʾ��������ؼ�
        //
        Element* fe = FindName(_U("CtrlArea"));
        Int32Animation* xAni = new Int32Animation(120, 0, Duration(120), FillBehavior::fbStop);
        DoubleAnimation* opaAni = new DoubleAnimation(0.2f, 1, Duration(120), FillBehavior::fbStop);
        TransformGroup* transGrp = new TransformGroup();
        TranslateTransform* trans = new TranslateTransform();

        //
        // ��ƫ��ת�����ӵ�TransformGroup
        //
        transGrp->AddChild(trans);
        // 
        // ǰ��40%���٣�����60%����ǰ��
        //
        opaAni->SetDecelerationRatio(0.6f);

        //
        // ���ӻص���������״̬�仯������ӻ��Ϊֹͣ��
        //
        opaAni->StateInvalidated += EventHandler(this, &MainWindow::OnCtrlAreaStart);

        //
        // ��ʼ�������Ѷ���Ӧ�õ��ؼ�Ԫ����
        //
        fe->BeginAnimation(OpacityProperty, opaAni);

        //
        // ���ӿؼ�Ԫ�ص���Ⱦ�任����
        // �ؼ�Ԫ����Ⱦ���ʱ��ʹ�ô˱任�������ת��Ȼ������ʾ
        //
        fe->SetRenderTransform(transGrp);
        trans->BeginAnimation(TranslateTransform::YProperty, xAni);
    }

    UpdateLayout();
}

void MainWindow::OnControlMouseEnter(Element* sender, MouseButtonEventArg* e)
{
    ListBoxItem* pItem = DynamicCast<ListBoxItem>(sender);
    if (NULL != pItem)
    {
        //DoubleAnimation* pAni = new DoubleAnimation(0.3, 1, Duration(300), FillBehavior::fbStop);
        //pAni->SetAutoReverse(true);
        //pItem->BeginAnimation(OpacityProperty, pAni);
        //pItem->GetAssigner()->GetTimeManager()->Tick();
    }
}

void MainWindow::OnControlSwitchComplete(Object* sender, EventArg* e)
{
    Clock* clock = DynamicCast<Clock>(sender);
    if (NULL == clock || clock->GetCurrentState() == Clock::ClockState::Active)
    {
        return;
    }
    
    ListBox* listObj = FindElem<ListBox>(_U("CtrlPanel"));
    Object* itemHost = listObj->GetSelectedItem();
    ElementPtr listItem = itemHost;
    if (NULL == listItem.get() || listItem.get() == DpProperty::UnsetValue())
    {
        return;
    }
    String ctrlName = listItem->GetName();

    //
    // �л�ָ������ʾ�ؼ�ҳ��
    //
    SwitchToControl(ctrlName);
    FrameworkElement* elem = NULL;
    _ctrls.TryGetValue(ctrlName, elem);

    if (NULL != elem)
    {
        DoubleAnimation* opaAni = new DoubleAnimation(0.1f, 1, Duration(200), FillBehavior::fbStop);
        elem->BeginAnimation(OpacityProperty, opaAni);

        /*ScaleTransform* trans = new ScaleTransform(1, 1, 0.5, 0.5);
        DoubleAnimation* scaleAni = new DoubleAnimation(0, 1, Duration(200), FillBehavior::fbHoldEnd);

        elem->SetRenderTransform(trans);
        trans->BeginAnimation(ScaleTransform::ScaleXProperty, scaleAni);
        trans->BeginAnimation(ScaleTransform::ScaleYProperty, new DoubleAnimation(0, 1, Duration(200), FillBehavior::fbHoldEnd));*/

        //
        // ���ô˾����������ͬ������״̬���������Ը��ݶ�����ǰʱ��ı�ؼ�������ֵ��
        //
        GetAssigner()->GetTimeManager()->Tick();
    }
}

void MainWindow::OnAppSelected(Element* sender, MouseButtonEventArg* e)
{
    //
    // ��ȡSharpui��������ʾ�����б���󣨱���QQDemon��SuiDemon�ȣ�
    //
    ListBox* listApp = FindElem<ListBox>(_U("AppList"));
    if (listApp)
    {
        //
        // ��ȡ��ǰѡ�е�Ӧ����Ŀ������ListBox�ǵ�ѡ��
        //
        ElementPtr item = listApp->GetSelectedItem();
        if (item.get())
        {
            String strPath;
            strPath.Format(_U("%s.exe"), item->GetName().c_str());
            strPath = FileDir::CalculatePath(strPath);
            ShellExecute(NULL, _U("open"), strPath.c_str(), NULL, NULL, SW_SHOW);
        }
        e->SetHandled(true);
    }
}

void MainWindow::OnControlSelected(Element* sender, SelectionChangedEventArg* e)
{
    if (_isPendingHome && _currentMode == ViewMode::vmHome)
    {
        return;
    }

    ElementPtr item;

    //
    // ��ʾģʽ������ҳģʽ
    //
    if (_currentMode == ViewMode::vmHome)
    {
        _currentMode = ViewMode::vmControl;
        if (e->GetAddedItems()->GetCount() == 1)
        {
            item = e->GetAddedItems()->GetItem(0);
            LeftPanelAnimate(false);
            RightPanelAnimate(false);
            SwitchToControl(item->GetName());
            UpdateLayout();
        }
    }
    else
    {
        //
        // ����������ҳ��ť������Ҫ�л�״̬��vmHome
        // ��������ǵ���˿ؼ��б����������л���Ҫ��ʾ�Ŀؼ���
        //
        if (_isPendingHome)
        {
            _currentMode = ViewMode::vmHome;
        }
        if (e->GetRemovedItems()->GetCount() == 1)
        {
            ElementPtr oitem = e->GetRemovedItems()->GetItem(0);
            FrameworkElement* elem = NULL;
            _ctrls.TryGetValue(oitem->GetName(), elem);

            if (NULL != elem)
            {
                //ScaleTransform* trans = new ScaleTransform(1, 1, 0.5, 0.5);
                DoubleAnimation* opaAni = new DoubleAnimation(1, 0, Duration(200), FillBehavior::fbStop);
                opaAni->Completed += EventHandler(this, &MainWindow::OnControlSwitchComplete);

                //elem->SetRenderTransform(trans);
                //elem->BeginAnimation(OpacityProperty, opaAni);
                //trans->BeginAnimation(ScaleTransform::ScaleXProperty, opaAni);
                //trans->BeginAnimation(ScaleTransform::ScaleYProperty, new DoubleAnimation(1, 0, Duration(200), FillBehavior::fbStop));
                elem->BeginAnimation(OpacityProperty, opaAni);
            }
            else if (e->GetAddedItems()->GetCount() == 1)
            {
                item = e->GetAddedItems()->GetItem(0);
                SwitchToControl(item->GetName());
            }
            UpdateLayout();
        }
    }

    e->SetHandled(true);
}

void MainWindow::OnButtonClicked(Element* sender, RoutedEventArg* e)
{
    ElementPtr elem = e->GetOriginalSource();
    if (elem)
    {
        if (_currentMode == ViewMode::vmHome && elem->GetName().Equals(_U("CtrlBtn")))
        {
            _isPendingCtrl = true;
            ListBox* listObj = FindElem<ListBox>(_U("CtrlPanel"));
            listObj->SetSelectedItem(listObj->GetItem(0));
            _isPendingCtrl = false;
        }
        else if (_currentMode == ViewMode::vmControl && elem->GetName().Equals(_U("HomeBtn")))
        {
            _isPendingHome = true;
            ListBox* listObj = FindElem<ListBox>(_U("CtrlPanel"));

            //
            // �ص���ҳҳ����Ҫȡ���ؼ�ѡ�������ͬһ���ؼ�ʱ�����л����ؼ���ʾҳ�棩
            // ����ᴥ��OnControlSelected�ص���״̬�ɴ˺����ı�
            //
            listObj->UnselectAll();

            LeftPanelAnimate(true);
            RightPanelAnimate(true);

            Element* fe = FindName(_U("CtrlArea"));
            //
            // ���ؿؼ���ʾ����
            //
            fe->SetVisibility(Visibility::Collapsed);
            _isPendingHome = false;
        }

        UpdateLayout();
        e->SetHandled(true);
    }
}

void MainWindow::OnSetPosBtnClicked(Element* sender, RoutedEventArg* e)
{
    ElementPtr elem = e->GetOriginalSource();
    if (elem->GetName().Equals(_U("RepeatBtn")))
    {
        suic::TextBlock* pText = FindElem<suic::TextBlock>(_U("RepeatText"));
        if (pText)
        {
            static int num = 0;
            String str;
            str.Format(_U("%d"), ++num);
            pText->SetText(str);
        }
        return;
    }

    ProgressBar* pBar = FindElem<ProgressBar>(_U("ProgressBar03"));
    if (pBar)
    {
        Int32Animation* barAni = new Int32Animation(25, 60, Duration(200), FillBehavior::fbStop);
        pBar->BeginAnimation(ProgressBar::ValueProperty, barAni);
        UpdateLayout();
    }
}

bool MainWindow::OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj)
{
    return false;
}

void MainWindow::OnConnect(suic::IXamlNode* pNode, suic::Object* target)
{
    
}

void MainWindow::OnInitialized(EventArg* e)
{
    Window::OnInitialized(e);
}

void MainWindow::OnLoaded(suic::LoadedEventArg* e)
{
    Window::OnLoaded(e);

    // ���ھ�����ʾ
    CenterWindow();

    AddHandler(Button::ClickEvent, new ClickEventHandler(this, &MainWindow::OnButtonClicked), true);

    ListBox* listObj = FindElem<ListBox>(_U("CtrlPanel"));
    if (listObj)
    {
        listObj->AddSelectionChanged(new SelectionChangedEventHandler(this, &MainWindow::OnControlSelected));
    }

    ListBox* listApp = FindElem<ListBox>(_U("AppList"));
    if (listApp)
    {
        //listApp->AddMouseLeftButtonUp(new MouseEventHandler(this, &MainWindow::OnAppSelected), true);
    }
}

void MainWindow::LeftPanelAnimate(bool bHome)
{
    Element* feLeft = FindName(_U("DemonPanel"));
    Int32Animation* widAni = NULL;
    DoubleAnimation* opaAni = NULL;

    if (bHome)
    {
        widAni = new Int32Animation(0, 183, Duration(160), FillBehavior::fbHoldEnd);
    }
    else
    {
        widAni = new Int32Animation(183, 0, Duration(160), FillBehavior::fbHoldEnd);
    }

    //
    // ���ӿ�ȱ仯����
    //
    feLeft->BeginAnimation(WidthProperty, widAni);
}

void MainWindow::RightPanelAnimate(bool bHome)
{
    Element* feRight = FindName(_U("CtrlPanel"));
    Int32Animation* widAni = NULL;
    
    if (bHome)
    {
        widAni = new Int32Animation(150, 610, Duration(100), FillBehavior::fbHoldEnd);
    }
    else
    {
        widAni = new Int32Animation(610, 150, Duration(100), FillBehavior::fbHoldEnd);
    }

    //
    // �ؼ��б�������ʱ��������Ҫ�����ؼ���ʾ�������ʾ����
    // ��ˣ��������Ӷ�����ɵĻص�
    //
    widAni->Completed += EventHandler(this, &MainWindow::OnRightAniEnd);

    widAni->SetAccelerationRatio(0.3f);
    feRight->BeginAnimation(WidthProperty, widAni);
}
