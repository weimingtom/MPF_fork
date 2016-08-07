//////////////////////////////////////////////////////////////////////////////
// Project.cpp

#include "stdafx.h"
#include "Project.h"
#include "SlnTreeManager.h"

#include <main/DesignPanel.h>
#include <System/Tools/CoreHelper.h>

Project* Project::_currentProject = NULL;

ImplementRTTIOfClass(Project, FilterNode)

Project::Project()
    : _docMana(NULL)
    , _appRoot(NULL)
{
    InitEditNodes();
}

Project::Project(SlnTreeManager* docMana)
    : _docMana(docMana)
    , _appRoot(NULL)
{
    InitEditNodes();
}

Project::~Project()
{
    FREEREFOBJ(_appRoot);
}

void Project::InitEditNodes()
{
}

Brush* Project::GetIcon() const
{
    return NULL;
}

SlnTreeManager* Project::GetSlnTreeManager() const
{
    return _docMana;
}

ApplicationRootItem* Project::GetApplicationRootItem() const
{
    return _appRoot;
}

ElementRootItem* Project::GetMainWindow() const
{
    return _appRoot->GetMainRootItem();
}

void Project::UpdateSlnTree(bool bUpdateElementTree)
{
    if (NULL != _docMana)
    {
        _docMana->UpdateSolutionTree();
        if (bUpdateElementTree)
        {
            _docMana->GetObjTreeManager()->UpdateElementTree();
        }
    }
}

void Project::RemoveRootItem(RootItem* rootItem)
{
    if (GetMainWindow() != rootItem && NULL != rootItem)
    {
        XamlNodeItem* pParent = rootItem->GetParent();
        String strPath = rootItem->GetFullPath();

        CheckAndCloseRootElement(rootItem);
        rootItem->Close();
        FileDir::RemoveFile(strPath);
        pParent->RemoveItem(rootItem);
        UpdateSlnTree(true);
    }
}

bool Project::EqualCurrentDesignRootElement(RootItem* rootElem)
{
    DesignPanel* designPanel = _docMana->GetObjTreeManager()->GetDesignPanel();
    if (designPanel->GetRootElement() == rootElem)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Project::CheckAndCloseRootElement(RootItem* rootElem)
{
    ObjTreeManager* objTree = _docMana->GetObjTreeManager();
    DesignPanel* designPanel = objTree->GetDesignPanel();

    if (designPanel->GetRootElement() == rootElem)
    {
        objTree->SetTargetElement(NULL);
        return true;
    }
    else
    {
        return false;
    }
}

void Project::SwitchRootElement(ElementRootItem* rootElem)
{
    if (!rootElem->IsLoaded())
    {
        rootElem->Load(false);
    }

    if (NULL != rootElem->GetUIElement())
    {
        ObjTreeManager* objTree = _docMana->GetObjTreeManager();
        ProjectTree* prjTree = _docMana->GetProjectTree();

        prjTree->SetSelectedItem(rootElem);
        prjTree->ScrollIntoView(rootElem, false);
        objTree->SetTargetElement(rootElem);

        UpdateSlnTree(false);
    }
}

String Project::GetProjectName() const
{
    return _prjName;
}

void Project::SetProjectName(String val)
{
    _prjName = val;
}

String Project::GetProjectDir() const
{
    return _prjDir;
}

String Project::GetProjectPath() const
{
    return _prjPath;
}

bool Project::ReadApp(const String& strFilename)
{
    if (NULL == _appRoot)
    {
        _appRoot = new ApplicationRootItem();
        _appRoot->ref();
    }

    _appRoot->SetProject(this);

    // �����ļ���
    _appRoot->SetFileName(strFilename);
    
    if (_appRoot->Load(false))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Project::CopyFilesUnderDir(const String& oriDir, const String& destDir, const String& fileExt, bool bFailIfExist)
{
    FileFinder fileFinder;
    if (!fileFinder.Open(oriDir, _U("*.*")))
    {
        return;
    }

    bool bSearch = true;
    while (bSearch)
    {
        if (fileFinder.FindNext())
        {
            if (fileFinder.IsDot())
            {
                continue;
            }

            String strOriNew;
            String strDestNew;

            strOriNew.Format(_U("%s\\%s"), oriDir.c_str(), fileFinder.GetFileName().c_str());
            strDestNew.Format(_U("%s\\%s"), destDir.c_str(), fileFinder.GetFileName().c_str());

            // Ŀ¼
            if (fileFinder.IsDir())
            {
                strDestNew += _U("\\");

                strOriNew.Replace(_U("\\\\"), _U("\\"));
                strDestNew.Replace(_U("\\\\"), _U("\\"));

                FileDir::DupCreateDir(strDestNew);

                // ��ȡ��Ŀ¼
                CopyFilesUnderDir(strOriNew, strDestNew, fileExt, bFailIfExist);
            }
            else
            {
                String strFilename(fileFinder.GetFileName());
                strFilename.Trim();
                int iPos = strFilename.IndexOf(_U("."));
                if (iPos != -1)
                {
                    String strExt = strFilename.Substring(iPos);
                    strExt.ToLower();

                    if (fileExt.IndexOf(strExt) != -1)
                    {
                        FileDir::CopyFileTo(strOriNew, strDestNew, bFailIfExist);
                    }
                }
            }
        }
        else
        {
            if (GetLastError() == ERROR_NO_MORE_FILES)
            {
                bSearch = false;
            }
            else 
            {
                return;
            }
        }
    }
}

bool Project::ReadFilterFromDir(const String& strDir, const String& strExt, FilterNode* parent)
{
    FileFinder fileFinder;
    if (!fileFinder.Open(strDir, _U("*.*")))
    {
        return false;
    }

    bool bSearch = true;
    while (bSearch)
    {
        if (fileFinder.FindNext())
        {
            if (fileFinder.IsDot())
            {
                continue;
            }

            // Ŀ¼
            if (fileFinder.IsDir())
            {
                TCHAR RelativePathNewDirFound[MAX_PATH] = {0};
                _tcscat(RelativePathNewDirFound, fileFinder.GetFilePath().c_str());

                String strFilename(fileFinder.GetFileName());
                strFilename.Trim();

                {
                    FilterNode* newNode = new FilterNode();
                    parent->AddItem(newNode);
                    newNode->SetName(strFilename);

                    // ��ȡ��Ŀ¼
                    ReadFilterFromDir(RelativePathNewDirFound, strExt, newNode);
                }
            }
            else
            {
                String strFilename(fileFinder.GetFileName());
                strFilename.Trim();
                int iPos = strFilename.IndexOf(_U("."));
                if (iPos != -1)
                {
                    RootItem* newNode = NULL;
                    String fileExt = strFilename.Substring(iPos);

                    fileExt.ToLower();

                    if (fileExt.Equals(_U(".xaml")))
                    {
                        String strFullPath = parent->GetFullPath() + strFilename;
                        String strNodeName = Utils::CheckUIXmlRoot(strFullPath);

                        if (strNodeName.Equals(_U("Application")))
                        {
                            _appRoot = new ApplicationRootItem();
                            _appRoot->ref();
                            newNode = _appRoot;
                        }
                        else if (strNodeName.Equals(_U("ResourceDictionary")))
                        {
                            newNode = new ResourceDicRootItem();
                        }
                        else
                        {
                            newNode = new ElementRootItem(new DesignElement());
                        }

                        newNode->SetFileName(strFilename);
                    }
                    else if (strExt.IndexOf(fileExt) != -1)
                    {
                        newNode = new ImageRootItem();
                        String strFilepath = parent->GetRelativePath() + strFilename;
                        newNode->SetFileName(strFilename);
                    }

                    if (NULL != newNode)
                    {
                        parent->AddItem(newNode);
                        //newNode->SetProject(this);
                    }
                }
            }
        }
        else
        {
            if (GetLastError() == ERROR_NO_MORE_FILES)
            {
                bSearch = false;
            }
            else 
            {
                return false;
            }
        }
    }

    return true;
}

Project* Project::OpenPrj(SlnTreeManager* docMana, const suic::String& path)
{
    Project* pPrj = new Project(docMana);
    pPrj->ref();

    if (!pPrj->LoadProject(path))
    {
        pPrj->unref();
        pPrj = NULL;
    }

    return pPrj;
}

bool Project::InternalLoadPrj()
{
    String strPath;

    ReadFilterFromDir(GetProjectDir(), RES_FILTER, this);

    if (_appRoot == NULL)
    {
        return false;
    }

    strPath = _appRoot->GetFullPath();

    if (ReadApp(strPath))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Project::Reload()
{
    if (_appRoot != NULL)
    {
        return true;
    }

    XmlDoc xmlDoc;
    XmlNode* root = NULL;

    try
    {
        suic::Mulstr strPath(_prjPath.c_str());
        suic::Mulstr buff;

        Utils::ReadResFromFile(Mulstr(strPath.c_str()).c_str(), "rb", buff);
        buff = buff.FromUtf8(buff.c_str(), buff.Length());

        xmlDoc.parse<0>(buff.c_str());
        root = xmlDoc.first_node();

        if (NULL == root)
        {
            return false;
        }

        XmlNode* childNode = root->first_node();
        while (childNode != NULL)
        {
            String strName(childNode->name());
            if (strName.Equals(_U("Name")))
            {
                XmlAttr* attr = childNode->first_attribute("Value");

                if (NULL != attr)
                {
                    SetProjectName(attr->value());
                }
            }
            else if (strName.Equals(_U("Application")))
            {
                ;
            }
            else if (strName.Equals(_U("MainWindow")))
            {
                ;
            }

            childNode = childNode->next_sibling();
        }
    }
    catch (char* err)
    {
        ;
    }
    catch (...)
    {
    }

    return InternalLoadPrj();
}

void Project::Close()
{
    ObjTreeManager* objTree = _docMana->GetObjTreeManager();
    DesignPanel* designPanel = objTree->GetDesignPanel();
    ElementRootItem* rootItem = designPanel->GetRootElement();

    if (rootItem != NULL && rootItem->GetProject() == this)
    {
        objTree->SetTargetElement(NULL);
    }

    if (_appRoot != NULL)
    {
        _appRoot->unref();
        _appRoot = NULL;
    }

    Clear();
}

bool Project::LoadProject(const String& path)
{
    _prjPath = path;

    for (int i = path.Length() - 1; i >= 0; --i)
    {
        if (path[i] == _U('\\') || path[i] == _U('/'))
        {
            _prjDir = path.Substring(0, i + 1);
            break;
        }
    }

    return Reload();
}

Project* Project::CreatePrj(SlnTreeManager* docMana, const suic::String& name, const suic::String& path)
{
    Project* pPrj = new Project(docMana);

    pPrj->SetProjectName(name);
    pPrj->_prjDir = path;
    pPrj->ref();

    pPrj->_prjDir.Trim();
    if (pPrj->_prjDir[pPrj->_prjDir.Length() - 1] != _U('\\'))
    {
        pPrj->_prjDir += _U("\\");
    }

    FileDir::DupCreateDir(pPrj->_prjDir);

    if (!pPrj->InitDefaultProject())
    {
        pPrj->unref();
        pPrj = NULL;
    }

    return pPrj;
}

bool Project::CopyTemplateFile(const String& tempPath, const String& destPath)
{
    Mulstr buff;
    Mulstr strPath;
    String strFlag;

    strPath = tempPath.c_str();
    strFlag.Format(_U("%s"), GetProjectName().c_str());

    Utils::ReadResFromFile(strPath.c_str(), "rb", buff);
    buff = buff.Replace("@template_flag@", strFlag.c_str());

    if (buff.Length() < 16)
    {
        return false;
    }

    FileDir::DupCreateDir(destPath);
    strPath = destPath.c_str();

    FILE* f = fopen(strPath.c_str(), "wb");
    if (NULL != f)
    {
        fwrite(buff.c_str(), 1, buff.Length(), f);
        fclose(f);
        return true;
    }
    else
    {
        return false;
    }
}

bool Project::InitDefaultProject()
{
    String tempFile;
    String destFile;

    String strPath;

    String strFilename = _U("Application.xaml");

    _prjPath.Format(_U("%s%s.uiproj"), _prjDir.c_str(), _prjName.c_str());

    // ����Application.xml�ļ�
    tempFile = FileDir::CalculatePath(_U("resource\\uidesign\\Template\\Application.xaml"));
    destFile.Format(_U("%sApplication.xaml"), _prjDir.c_str());
    if (!CopyTemplateFile(tempFile, destFile))
    {
        return false;
    }

    // ����Window.xml�ļ�
    tempFile = FileDir::CalculatePath(_U("resource\\uidesign\\Template\\Window.xaml"));
    destFile.Format(_U("%sMainWindow.xaml"), _prjDir.c_str());
    if (!CopyTemplateFile(tempFile, destFile))
    {
        return false;
    }

    // ����default.xml�ļ�
    tempFile = FileDir::CalculatePath(_U("resource\\uidesign\\Template\\theme.xaml"));
    destFile.Format(_U("%stheme\\default.xaml"), _prjDir.c_str());
    if (!CopyTemplateFile(tempFile, destFile))
    {
        return false;
    }

    // ����imagesĿ¼
    tempFile = FileDir::CalculatePath(_U("resource\\uidesign\\Template\\images"));
    destFile.Format(_U("%simages\\"), _prjDir.c_str());
    FileDir::DupCreateDir(destFile);
    CopyFilesUnderDir(tempFile, destFile, RES_FILTER, true);

    if (InternalLoadPrj())
    {
        SaveProject();
        return true;
    }
    else
    {
        return false;
    }
}

String Project::ToString()
{
    return _prjName;
}

void Project::SaveProject()
{
    int i = 0;
    suic::String strXml = _U("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

    strXml += _U("<UIProject>\n");

    strXml += ResNode::OFFSET1 + _U("<Name Value=\"");
    strXml += _prjName;
    strXml += _U("\"/>\n");
    strXml += ResNode::OFFSET1 + _U("<Author Value=\"Created by MPF\"/>\n");
    strXml += ResNode::OFFSET1 + _U("<Version Value=\"0.1.0.0\"/>\n");

    strXml += _U("</UIProject>\n");

    FileWriter fs;
    String strFile = GetProjectPath();

    if (fs.Open(strFile))
    {
        fs.Write(strXml);
        fs.Close();
    }
}

bool Project::Zip(const String& strZip, const String& pwd)
{
    String strPath = GetProjectDir();
    String zipExt = RES_FILTER;

    return Utils::ZipFolder(strZip, strPath, zipExt, pwd);
}

void Project::SaveAll()
{
    SaveProject();
    SaveFilterNode(this);
}

void Project::Reflesh(bool onlyReadAppRes)
{
    if (NULL != _appRoot)
    {
        SlnTreeManager* slnMana = GetSlnTreeManager();
        ObjTreeManager* objTree = slnMana->GetObjTreeManager();
        DesignPanel* pDesignPanel = objTree->GetDesignPanel();
        ElementRootItem* rootItem = pDesignPanel->GetRootElement();

        if (onlyReadAppRes)
        {
            _appRoot->ClearOnlyRes();
        }
        else
        {
            _appRoot->Close();
        }
        _appRoot->Load(onlyReadAppRes);
        //objTree->SetTargetElement(NULL);
        RefleshFilterNode(this, rootItem);

        //DesignElement* focused = rootItem->GetFocusElement();

        //pDesignPanel->SwitchRootElement(rootItem);
        //SwitchRootElement(rootItem);
        //pDesignPanel->SetFocusedElement(focused, true);
    }
}

void Project::RefleshFilterNode(FilterNode* pNode, RootItem* currShow)
{
    for (int i = 0; i < pNode->GetCount(); ++i)
    {
        ElementRootItem* rootElem = RTTICast<ElementRootItem>(pNode->GetItem(i));

        if (NULL != rootElem)
        {
            if (rootElem->IsLoaded())
            {
                /*if (rootElem->IsModified())
                {
                    rootElem->Save();
                }
                rootElem->Reset();
                rootElem->Load(false);*/
                if (rootElem == currShow)
                {
                    rootElem->SetRefreshFlag(true);
                    rootElem->Refresh();
                }
                else
                {
                    rootElem->SetRefreshFlag(true);
                }
            }

            continue;
        }

        FilterNode* subNode = RTTICast<FilterNode>(pNode->GetItem(i));
        if (NULL != subNode)
        {
            RefleshFilterNode(subNode, currShow);
        }
    }
}

void Project::SaveFilterNode(FilterNode* pNode)
{
    for (int i = 0; i < pNode->GetCount(); ++i)
    {
        RootItem* rootElem = RTTICast<RootItem>(pNode->GetItem(i));

        if (NULL != rootElem)
        {
            if (rootElem->IsModified())
            {
                rootElem->Save();
            }

            continue;
        }

        FilterNode* subNode = RTTICast<FilterNode>(pNode->GetItem(i));
        if (NULL != subNode)
        {
            SaveFilterNode(subNode);
        }
    }
}

bool Project::HasModified()
{
    if (HasModified(this))
    {
        return true;
    }

    return false;
}

bool Project::HasModified(FilterNode* pNode)
{
    for (int i = 0; i < pNode->GetCount(); ++i)
    {
        RootItem* rootElem = RTTICast<RootItem>(pNode->GetItem(i));

        if (NULL != rootElem && rootElem->IsModified())
        {
            return true;
        }

        FilterNode* subNode = RTTICast<FilterNode>(pNode->GetItem(i));
        if (NULL != subNode)
        {
            if (HasModified(subNode))
            {
                return true;
            }
        }
    }

    return false;
}

String Project::GetRootItemsAsString(const String& offset, FilterNode* pNode)
{
    String strXml;

    for (int i = 0; i < pNode->GetCount(); ++i)
    {
        RootItem* rootElem = RTTICast<RootItem>(pNode->GetItem(i));
        if (NULL != rootElem)
        {
            if (rootElem != _appRoot && rootElem != _appRoot->GetMainRootItem())
            {
                strXml += offset + _U("<File Name=\"");
                strXml += rootElem->GetRelativePath();
                strXml += _U("\" />\n");
            }
        }
        else
        {
            FilterNode* subNode = RTTICast<FilterNode>(pNode->GetItem(i));
            if (NULL != subNode)
            {
                String subXml = GetRootItemsAsString(offset + ResNode::OFFSET1, subNode);
                if (subXml.Empty())
                {
                    strXml += offset + _U("<Path Name=\"");
                    strXml += subNode->GetName();
                    strXml += _U("\" />\n");
                }
                else
                {
                    strXml += offset + _U("<Path Name=\"");
                    strXml += subNode->GetName();
                    strXml += _U("\" >\n");
                    strXml += subXml;
                    strXml += offset + _U("</Path>\n");
                }
            }
        }
    }

    return strXml;
}

String Project::ComputeWindowPath(const String& shortPath)
{
    // �����½����ڵ�·��
    String strWndPath;
    int iWndIndex = 1;

    for (;;)
    {
        strWndPath.Format(_U("%sWindow%d.xaml"), shortPath.c_str(), iWndIndex);

        String strPath = GetProjectDir() + strWndPath;

        if (FileDir::GetFileProp(strPath.c_str()) == FileDir::fpNone)
        {
            break;
        }
        ++iWndIndex;
    }

    return strWndPath;
}

Project* Project::GetCurrentProject()
{
    return _currentProject;
}

void Project::SwitchToCurrent()
{
    ApplicationRootItem* appItem = NULL;
    if (_currentProject != this)
    {
        _currentProject = this;
        appItem = _currentProject->GetApplicationRootItem();
        if (NULL != appItem)
        {
            DesignHelper::SetDesignApplication(appItem->GetApplicationNode()->GetApplication());
        }
    }
}

bool Project::OpenRootElement(ElementRootItem* mainElem)
{
    String strWndPath = GetProjectDir() + mainElem->GetFileName();
    XamlLoader xamlLoader;

    if (NULL != mainElem->GetRootElement())
    {
        return true;
    }

    //mainElem->SetProject(this);
    xamlLoader.LoadElementRootXaml(mainElem, strWndPath);

    if (mainElem->GetRootElement() != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ResNode* Project::FindResItem(const String& strName)
{
    ResNode* resNode = NULL;
    if (NULL != _appRoot)
    {
        resNode = _appRoot->GetApplicationNode()->GetResourceDictionary()->SearchResItem(strName);
    }
    return resNode;
}

RootItem* Project::FindRootItem(const String& strPath)
{
    String strFilename;
    RootItem* rootItem = NULL;
    FilterNode* filterNode = CreateFilterByPath(strPath, strFilename);

    if (NULL != filterNode)
    {
        FilterNode* newNode = filterNode->FindItemOnChild(strFilename);
        if (NULL != newNode)
        {
            rootItem = RTTICast<RootItem>(newNode);
        }
        return rootItem;
    }
    else
    {
        return NULL;
    }
}

/*XamlRootItem* Project::FindAndCreateRootItem(const String& strPath)
{
    String strFilename;
    XamlRootItem* rootItem = NULL;
    FilterNode* filterNode = CreateFilterByPath(strPath, strFilename);

    if (NULL != filterNode)
    {
        FilterNode* newNode = filterNode->FindItemOnChild(strFilename);
        if (NULL == newNode)
        {
            rootItem = new XamlRootItem();
            rootItem->SetFileName(strFilename);
            filterNode->AddItem(rootItem);
        }
        else
        {
            rootItem = RTTICast<XamlRootItem>(newNode);
        }
        return rootItem;
    }
    else
    {
        return NULL;
    }
}*/

ElementRootItem* Project::AddRootElement(FilterNode* pParent, const String& fileName)
{
    String strName = fileName + _U(".xaml");
    String strPath = pParent->GetFullPath() + strName;
    String strOriPath = FileDir::CalculatePath(_U("resource\\uidesign\\Template\\Window.xaml"));

    FileDir::CopyFileTo(strOriPath, strPath, true);

    ElementRootItem* rootItem = new ElementRootItem(new DesignElement());

    rootItem->SetFileName(strName);
    //rootItem->SetProject(this);

    pParent->AddItem(rootItem);

    return rootItem;
}

FilterNode* Project::CreateFilterByPath(const String& strUri, String& strName)
{
    FilterNode* addNode = NULL;
    String strPath = ResourceUri(strUri).GetResourcePath();
    strPath.Replace(_U("\\"), _U("/"));
    StringArray vec;

    vec.SplitString(strPath, _U("/"));

    // Ϊ1��ʾ���ӵ����ڵ���
    if (vec.GetCount() == 1)
    {
        addNode = this;
        strName = vec[0];
    }
    else
    {
        addNode = this;
        strName = vec[vec.GetCount() - 1];

        for (int i = 0; i < vec.GetCount() - 1; ++i)
        {
            String strSubDir = vec[i];
            FilterNode* newNode = NULL;
            strSubDir.Trim();

            if (!strSubDir.Empty())
            {
                newNode = addNode->FindItemOnChild(strSubDir);
                if (NULL == newNode)
                {
                    newNode = new FilterNode();
                    newNode->SetName(strSubDir);
                    addNode->AddItem(newNode);
                }
                addNode = newNode;
            }
        }
    }

    return addNode;
}

void Project::RemoveRootElement(ElementRootItem* pItem)
{
    RemoveRootItem(pItem);
}

