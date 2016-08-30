// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ElementTreeManager.h
// ��  �ܣ�����Ԫ������
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _ELEMENTTREEMANAGER_H_
# define _ELEMENTTREEMANAGER_H_

#include "ElementTree.h"
#include "ControlWindow.h"
#include "PropertyWindow.h"

class DesignPanel;

class ObjTreeManager : public IElementTreeDoc
{
public:

    ObjTreeManager();
    ~ObjTreeManager();

    // �õ�������
    DesignPanel* GetDesignPanel();
    // ��ȡ�ؼ��ӿ�
    IControlWindow* GetControlWindow();
    // ��ȡ�������ӿ�
    IPropertyWindow* GetPropertyWindow();

    // ��ȡ�ؼ�������
    ElementTree* GetElementTree();

    void SetProject(Project* pPrj);
    Project* GetProject() const;

    /// <summary>
    ///   ��ʼ���༭�Ŀؼ�
    /// </summary>
    /// <param name="mainWnd">�ؼ�����</param>
    /// <returns>��</returns>
    void InitElementTree(FrameworkElement* mainWnd);
    void SetTargetElement(DesignElement* resElem, ElementRootItem* elem);

    void UpdateElementTree();
    void SetSelectedElement(Object* elem);
    void SetMouseOverElement(Object* elem);

    void OnControlNotify(DesignMode mode, const suic::String& name);
    void OnTreeMouseEnter(suic::Element* sender, suic::MouseButtonEventArg* e);

protected:

    void BindTreeEvent();

protected:

    // ��ǰ�༭�Ŀؼ������Ĺ���
    Project* _project;
    ElementTree* _elemTree;
    DesignPanel* _designPanel;

    ControlWindowOp _ctrlWindow;
    PropertyWindowOp _propWindow;
};

inline DesignPanel* ObjTreeManager::GetDesignPanel()
{
    return _designPanel;
}

inline IControlWindow* ObjTreeManager::GetControlWindow()
{
    return &_ctrlWindow;
}

inline IPropertyWindow* ObjTreeManager::GetPropertyWindow()
{
    return &_propWindow;
}

inline ElementTree* ObjTreeManager::GetElementTree()
{
    return _elemTree;
}

inline void ObjTreeManager::SetProject(Project* pPrj)
{
    _project = pPrj;
}

inline Project* ObjTreeManager::GetProject() const
{
    return _project;
}


#endif

