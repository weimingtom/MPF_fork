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

/// <Summary>
///  �û����ݽṹʵ��ICommand�ӿڣ����Խ������簴ť֮��ĵ�������¼�
///  �¼�����Ϊ��ť�������¼��ǿ�·�ɵģ�����������ListBox��TreeView��
///  �����Զ�������ģ���ж���İ�ť�ĵ���¼�
/// <Summary/>
class NameField : public suic::NotifyPropChanged
{
public:

    BeginMember(NameField, suic::NotifyPropChanged)
        MemberInt(Number)
        MemberString(Name)
        MemberString(Tel)
        MemberString(Sexy)
        MemberObject(Checked)
        MemberAttributes(Attris)
    EndMember()

    RTTIOfClass(NameField);

    NameField()
    {
        _checked = suic::Boolean::True;
    }

    NameField(int num, suic::String name)
    {
        _number = num;
        _propName = name;
        _addr = _U("�й��㶫����");
        _tel = _U("13500013800");
        _personId = _U("52020298888081231");
        _checked = suic::Boolean::True;
    }

    int GetNumber()
    {
        return _number;
    }

    void SetNumber(int val)
    {
        _number = val;
    }

    suic::String GetName()
    {
        return _propName;
    }

    void SetName(suic::String name)
    {
        _propName = name;
    }

    suic::String GetAddr()
    {
        return _addr;
    }

    void SetAddr(suic::String val)
    {
        _addr = val;
    }

    suic::String GetTel()
    {
        return _tel;
    }

    void SetTel(suic::String val)
    {
        _tel = val;
    }

    suic::String GetSexy()
    {
        return _sexy;
    }

    void SetSexy(suic::String val)
    {
        _sexy = val;
        NotifyChanged(_U("Sexy"));
    }

    suic::String GetPersonId()
    {
        return _personId;
    }

    void SetPersonId(suic::String val)
    {
        _personId = val;
    }

    suic::Object* GetChecked()
    {
        return _checked;
    }

    void SetChecked(suic::Object* obj)
    {
        _checked = suic::DynamicCast<suic::Boolean>(obj);
    }

    bool SetAttris(const suic::String& name, suic::Object* obj)
    {
        if (name.Equals(_U("PersonId")))
        {
            return true;
        }
        else if (name.Equals(_U("Addr")))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool GetAttris(const suic::String& name, suic::ObjectPtr& obj)
    {
        if (name.Equals(_U("PersonId")))
        {
            obj = new suic::OString(_personId);
            return true;
        }
        else if (name.Equals(_U("Addr")))
        {
            obj = new suic::OString(_addr);
            return true;
        }
        else
        {
            return false;
        }
    }

    suic::String ToString()
    {
        suic::String strFmt;
        strFmt.Format(_U("Data Binding: %d->%s"), _number, _propName.c_str());
        return strFmt;
    }

private:

    int _number;
    suic::Boolean* _checked;
    suic::String _propName;
    suic::String _addr;
    suic::String _tel;
    suic::String _sexy;
    suic::String _personId;
};

class MainWindow : public suic::Window
{
public:

    MainWindow();
    ~MainWindow();

    void OnInitialized(suic::EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnBtnClicked(suic::Element* sender, suic::RoutedEventArg* e);
    void OnItemDbClck(suic::Element* sender, suic::MouseButtonEventArg* e);
    void OnClipListView(suic::Element* sender, suic::RoutedEventArg* e);

    void OnItemSelectionChanged(suic::Element* sender, suic::SelectionChangedEventArg* e);

protected:

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

    void OnTimer(suic::Object* sender, suic::EventArg* e);

private:

    void RegisterButtonEvent();

private:

    suic::AssignerTimer* _timer;
};

#endif
