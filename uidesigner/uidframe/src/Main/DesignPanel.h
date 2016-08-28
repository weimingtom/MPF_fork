
#ifndef _DESIGNPANEL_H_
#define _DESIGNPANEL_H_

#include "DragMetaTool.h"
#include "CommonInterface.h"
#include "ProjectTreeMenu.h"

#include <Main/XamlRootItem.h>
#include <Framework/Presenter/ScrollContentPresenter.h>

class DragBox
{
public:

    void UpdateDragRect(suic::Rect rect);
    void DrawDrag(suic::Drawing* drawing);
    bool HitTest(suic::Point pt) const;
    suic::Rect GetRect() const;

private:

    suic::Rect _rect;
};

enum eMouseAction
{
    eMouseMove,
    eMouseLeftButtonDown,
    eMouseLeftButtonUp,
};

class DesignPanel : public IDesignWindow
{
public:

    DesignPanel();
    virtual ~DesignPanel();

    void UpdateDElement();

    void SetTreeDoc(IElementTreeDoc* docMana);
    IElementTreeDoc* GetTreeDoc() const;

    IPropertyWindow* GetPropWindow() const;

    void ResetDesignPanel(bool bAsNull);

    void InitEditPanelInfo(ElementRootItem* rootElem);
    void SwitchRootElement(ElementRootItem* root);

    ElementRootItem* GetRootElement() const;
    void UpdateModified();

    void RefleshDesignPanel();
    DesignElement* HitTestElement(Point pt);

    // ���õ�ǰ����Ԫ������
    void SetDesignMode(DesignMode mode, const String& elemType);
    void RemoveElement(DesignElement* dfocus);

    void SetFocusedElement(DesignElement* elem, bool bFromDesign);
    void SetMouseOverElement(DesignElement* elem, bool bFromDesign);

    void SetElementWidth(DesignElement* elem, int wid);
    void SetElementHeight(DesignElement* elem, int hei);
    void SetElementMargin(DesignElement* elem, Rect rect);

    void UpdateElementSetter(DesignElement* elem, SetterNode* pSetter);

public:

    // �϶����
    void OnDragLeft(DragMeta& meta, int iAction);

    // �϶����Ͻ�
    void OnDragLeftTop(DragMeta& meta, int iAction);

    // �϶��ϱ�
    void OnDragTop(DragMeta& meta, int iAction);

    // �϶����Ͻ�
    void OnDragRightTop(DragMeta& meta, int iAction);

    // �϶��ұ�
    void OnDragRight(DragMeta& meta, int iAction);

    // �϶����½�
    void OnDragRightBottom(DragMeta& meta, int iAction);

    // �϶��±�
    void OnDragBottom(DragMeta& meta, int iAction);

    // �϶����½�
    void OnDragLeftBottom(DragMeta& meta, int iAction);

    void OnDragBox(DragMeta& meta, int iAction);

    // �ƶ�
    void OnDragMove(DragMeta& meta, int iAction);

protected:

    void OnScrollChanged(suic::ScrollChangedEventArg& e);

    int HitTestFilter();
    bool IsAncestor(DesignElement* child, DesignElement* ancestor);

    int GetVisualChildrenCount();
    Element* GetVisualChild(int index);

    Size OnMeasure(const suic::Size& constraitSize);
    void OnArrange(const Size& arrangeSize);

    void DispatchRender(Drawing * drawing);
    void OnLoaded(suic::LoadedEventArg* e);
    void OnRender(suic::Drawing* drawing);

    void RenderChildren(suic::Drawing* drawing);
    void RenderDesignMeta(suic::Drawing* drawing);

    void OnMouseEnter(suic::MouseButtonEventArg* e);
    void OnMouseLeave(suic::MouseButtonEventArg* e);
    void OnMouseMove(suic::MouseButtonEventArg* e);

    void OnSetCursor(suic::CursorEventArg* e);
    void OnPreviewSetCursor(CursorEventArg* e);

    void OnPreviewMouseDown(suic::MouseButtonEventArg* e);
    void OnPreviewMouseUp(suic::MouseButtonEventArg* e);

    void OnMouseLeftButtonDown(suic::MouseButtonEventArg* e);
    void OnMouseLeftButtonUp(suic::MouseButtonEventArg* e);
    void OnMouseRightButtonDown(suic::MouseButtonEventArg* e);
    void OnTextInput(suic::KeyboardEventArg* e);
    void OnKeyDown(suic::KeyboardEventArg* e);

    void OnContextMenu(ContextMenuEventArg* e);

    void OnDrawDesign(suic::Drawing* drawing);
    void OnDrawBkgnd(suic::Drawing* drawing);

protected:

    bool IsMain() const;
    void LinkDragMetaEvent();
    DragMeta* HittestDragMeta(suic::Point pt);

    Rect GetRelativeRect(Element* Other);

    void DrawRectOnly(suic::Drawing* drawing, Rect rect, suic::Color clr, int iThickness);

    void InitDragMeta(suic::Rect rect);
    void HandleDragRect(suic::Rect& rect);
    suic::Rect CalcElementRect(DesignElement* elem);

    void DrawDElement(Drawing* drawing, DesignElement* delem, int t, Color clr);
    void DrawDElement(Drawing * drawing, FrameworkElement* fe, int t, Color clr);
    void InternalHandleDrag(suic::Rect& rect, int* pNori, int* pVert);

    void DragMoveFinish(DesignElement* dfocus, DesignElement* dparent);
    bool GetDragingRect(suic::Rect& rcMgr);

    void RenderInSelectMode(suic::Drawing * drawing);
    void RenderInMoveMode(suic::Drawing * drawing);
    void RenderInAddMode(suic::Drawing * drawing);

    void MouseMoveInSelectMode(suic::MouseButtonEventArg* e);
    void MouseMoveInAddMode(suic::MouseButtonEventArg* e);

    void MLDownInSelectMode(suic::MouseButtonEventArg* e);
    void MLDownInAddMode(suic::MouseButtonEventArg* e);
    void CompleteAddElement();

    bool CheckCanAddElement(DesignElement* delem);
    Rect GetInsertElemMargin(DesignElement* parent, Point curPt);
    int CalcStackPanelPos(StackPanel* parent, Point pt, Size& size);

    void HandleCopyElement(DesignElement* dfocus, suic::IAddChild* mvPanel);
    DesignElement* GetSameParentElement(DesignElement* dfocus, DesignElement* dMouse);

protected:

    DragMeta _dragMetas[11];

    // ��ǰ������϶���(���ѹ����Ч)
    DragMeta* _dragMeta;

    int _dragMetaCount;

    // ��ǰ���ӵĸ�Ԫ��
    DesignElement* _addParent;
    // ��ǰ��Ƶĸ�Ԫ��
    ElementRootItem* _root;

    int _designMode;
    bool _needMeasure;
    bool _bIsDown;
    // ����Ԫ���Ƿ�ɹ�
    bool _isAddSucc;

    String _elemType;

    // ����������Ԫ��
    suic::Rect _mouseOverRc;
    // ��ʼ�϶�ʱ�ĵ�
    suic::Point _startDragPt;
    // ��ʼ�϶�ʱԪ�������С
    suic::Rect _startDragRc;
    suic::Rect _startDragMgr;
    suic::Rect _startMoveMgr;

    DragBox _dragBox;

    DesignElement* _dragMouseOver;
    // ��ǰ����
    suic::Point _trackDragPt;
    suic::Rect _margin;

    IElementTreeDoc* _docTree;
    ElemTreeMenu _elemMenu;
};

#endif
