
#ifndef _PROJECT_H_
#define _PROJECT_H_

#include "FilterNode.h"

#include <tools/Utils.h>
#include <Core/FileFinder.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class SlnTreeManager;

class Project : public FilterNode
{
public:

    RTTIOfClass(Project)

    Project();
    Project(SlnTreeManager* docMana);
    virtual ~Project();

    bool Reload();
    void Close();

    bool LoadProject(const String& path);

    Brush* GetIcon() const;
    SlnTreeManager* GetSlnTreeManager() const;

    ApplicationRootItem* GetApplicationRootItem() const;
    ElementRootItem* GetMainWindow() const;

    void UpdateSlnTree(bool bUpdateElementTree);
    void RemoveRootItem(RootItem* rootItem);

    void SwitchRootElement(DesignElement* resElem, ElementRootItem* rootElem);
    bool EqualCurrentDesignRootElement(RootItem* rootElem);
    bool CheckAndCloseRootElement(RootItem* rootElem);

    String GetProjectName() const;
    void SetProjectName(String val);

    String GetProjectDir() const;
    String GetProjectPath() const;

    bool HasModified();
    bool OpenRootElement(ElementRootItem* mainElem);

    static Project* GetCurrentProject();
    static Project* OpenPrj(SlnTreeManager* docMana, const suic::String& path);
    static Project* CreatePrj(SlnTreeManager* docMana, const suic::String& name, const suic::String& path);

    String ToString();

    void SaveAll();
    void SaveProject();

    void Reflesh(bool onlyReadAppRes);

    bool Zip(const String& strZip, const String& pwd);

    void SwitchToCurrent();
    void SetMainWindowTitle();

    void SetMainWindowTitle(suic::String strTitle);

    void FindAllResourceDicRootItems(suic::Array<ResourceDicRootItem*>& resRootItems);

    ElementRootItem* AddRootElement(FilterNode* pParent, const String& fileName);
    ResourceDicRootItem* AddResourceDicRootElement(FilterNode* pParent, const String& fileName);

    void RemoveRootElement(ElementRootItem* dmain);

    //ResNode* FindResItem(const String& strName);
    RootItem* FindRootItem(const String& strPath);
    // �����ļ�·�����ض�Ӧ�ڵ㣬���û�оʹ���
    //XamlRootItem* FindAndCreateRootItem(const String& strPath);

public:

    void BackupRootItem(RootItem* rootItem);

    bool HasModified(FilterNode* pNode);
    void SaveFilterNode(FilterNode* pNode);
    String GetRootItemsAsString(const String& offset, FilterNode* pNode);

protected:

    void CreateBackup();
    suic::String GetBackupDir();
    suic::String GeDefaultResPath();

    void InitEditNodes();
    bool InternalLoadPrj();
    bool ReadApp(const String& strFilename);
    bool InitDefaultProject();

    void RefleshFilterNode(FilterNode* pNode, RootItem* currShow);

    bool CopyTemplateFile(const String& tempPath, const String& shortPath);

    void ReadThemeNodeFromDir();

    void CopyFilesUnderDir(const String& oriDir, const String& destDir, const String& fileExt, bool bFailIfExist);
    void FindAllResourceDicRootItems(FilterNode* pParent, suic::Array<ResourceDicRootItem*>& resRootItems);

    bool ReadFilterFromDir(const String& strDir, const String& strExt, FilterNode* parent);

    // ���ݴ����path����FilterNode
    FilterNode* CreateFilterByPath(const String& strUri, String& strName);

    String ComputeWindowPath(const String& shortPath);

protected:

    SlnTreeManager* _docMana;
    // ��������·��
    String _prjDir;
    // ��������
    String _prjName;
    String _prjPath;

    ApplicationRootItem* _appRoot;

    static Project* _currentProject;
};

#endif
