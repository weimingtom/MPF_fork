// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����PropSelectorWindow.h
// ��  �ܣ�����ѡ����档
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _PROPSELECTORWINDOW_H_
# define _PROPSELECTORWINDOW_H_

#include <Main/FilterNode.h>
#include <Main/SlnTreeManager.h>
#include <Core/TypeControls.h>
#include <Main/XamlRootItem.h>

using namespace suic;

class TargetTypeItem : public Object
{
public:

    TargetTypeItem(RTTIOfInfo* targetType)
    {
        _targetType = targetType;
    }

    String ToString()
    {
        return _targetType->typeName;
    }

    RTTIOfInfo* GetTargetType() const
    {
        return _targetType;
    }

private:

    RTTIOfInfo* _targetType;
};

class PropSelectorWindow : public suic::Window
{
public:

    PropSelectorWindow();
    ~PropSelectorWindow();

    void SetQueryDpCond(const QueryDpCond& cond);

    String GetTargetName();
    void SetTargetElements(suic::ItemCollection* itemColl);

    DpItem* GetSelectedItem();
    void OnDbListClick(Element* sender, MouseButtonEventArg* e);

protected:

    void InitTreeDoc();

    void OnInitialized(EventArg* e);
    void OnLoaded(suic::LoadedEventArg* e);

private:

    QueryDpCond _cond;
    suic::ItemCollection* _targetElements;
};

#endif
