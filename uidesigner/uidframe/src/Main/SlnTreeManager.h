// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����DocTreeManager.h
// ��  �ܣ������ĵ�����
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

#ifndef _DOCTREEMANAGER_H_
#define _DOCTREEMANAGER_H_

#include "ElementTreeManager.h"

class Project;

class SlnTreeManager
{
public:

    SlnTreeManager();
    ~SlnTreeManager();

    ProjectTree* GetProjectTree();
    ObjTreeManager* GetObjTreeManager();

    bool HasModified();
    void SaveAll();

    void RefleshProject(Project* pPrj);

    void ReloadProject(Project* pPrj);
    bool CloseProject(Project* pPrj, bool bTipIfChanged);
    bool RemoveProject(Project* pPrj, bool bTipIfChanged);

    int GetProjectCount();
    Project* GetProject(int index);

    Project* OpenProject(const suic::String& path);
    Project* CreateProject(const suic::String& name, const suic::String& path);

    bool ExistProject(const suic::String& prjDir);

    void InitSolutionTree(FrameworkElement* mainWnd);
    void UpdateSolutionTree();
    void UpdateProjectTree(Project* pPrj);

    void SwitchToRootElement(ElementRootItem* rootElem);

protected:

    Solution* _solution;
    ProjectTree* _slnTree;
    ObjTreeManager* _elemTreeMana;
};

inline ObjTreeManager* SlnTreeManager::GetObjTreeManager()
{
    return _elemTreeMana;
}

inline ProjectTree* SlnTreeManager::GetProjectTree()
{
    return _slnTree;
}


#endif

