// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����StartWindow.h
// ��  �ܣ��������洰�ڡ�
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _STARTWINDOW_H_
# define _STARTWINDOW_H_

using namespace suic;

class RecentlyPrj : public Object
{
public:

    String name;
    String path;

    RecentlyPrj()
    {

    }

    RecentlyPrj(const String& n, const String& p)
    {
        name = n;
        path = p;
    }

    String ToString()
    {
        return name;
    }
};

class StartWindow : public suic::Window
{
public:

    StartWindow();
    ~StartWindow();

    suic::String GetProjectName() const
    {
        return _prjName;
    }

    suic::String GetNewProjectPath() const
    {
        return _newPrjPath;
    }

    suic::String GetOpenProjectPath() const
    {
        return _openPrjPath;
    }

    bool IsCanContinue() const
    {
        return _bCanContinue;
    }

    void SaveRecentlyProjects();
    void AddRecentlyProject(const String& name, const String& path);

    void OnNewProject(suic::Element* sender, suic::RoutedEventArg* e);
    void OnOpenProjectDir(suic::Element* sender, suic::RoutedEventArg* e);

    void OnOpenProject(suic::Element* sender, suic::RoutedEventArg* e);
    void OnCloseStartWindow(suic::Element* sender, suic::RoutedEventArg* e);

    void OnDbclkOnItem(suic::Element* sender, suic::MouseButtonEventArg* e);

protected:

    void ReadRecentlyProjects();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    // �����Լ��Ŀؼ�����
    bool OnBuild(suic::IXamlNode* pNode, suic::ObjectPtr& obj);
    void OnConnect(suic::IXamlNode* pNode, suic::Object* target);

protected:

    suic::String _prjName;
    suic::String _openPrjPath;
    suic::String _newPrjPath;

    bool _bCanContinue;

    suic::ObservableCollection _recentlyPrjs;
};

#endif

