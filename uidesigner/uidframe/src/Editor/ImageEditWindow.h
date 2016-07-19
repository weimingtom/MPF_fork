// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����ImageEditWindow.h
// ��  �ܣ��󶨱༭���档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _IMAGEEDITWINDOW_H_
# define _IMAGEEDITWINDOW_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

#include <Editor/ImageEditPanel.h>

using namespace suic;

class ImageEditWindow : public suic::Window
{
public:

    ImageEditWindow(Element* hostElem);
    ~ImageEditWindow();

    void OnButtonClick(Element* sender, RoutedEventArg* e);

    void SetEditBitmap(suic::ImageBrush* bmp);

    void NotifyViewboxChanged(suic::fRect rect);
    void NotifyViewcornerChanged(suic::Rect rect);

    bool IsCommit() const;

protected:

    void UpdateImagePreview();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

    void OnViewboxChanged(Object* sender, EventArg* e);
    void OnViewcornerChanged(Object* sender, EventArg* e);

private:

    bool _isCommit;

    Element* _hostElem;
    CheckBox* _viewboxUnits;
    RectEditor* _viewBox;
    RectEditor* _viewCorner;

    ImageEditPanel* _imgPanel;
};

#endif
