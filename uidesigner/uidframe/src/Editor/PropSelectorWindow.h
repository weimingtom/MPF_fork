// 华勤科技版权所有 2010-2011
// 
// 文件名：PropSelectorWindow.h
// 功  能：属性选择界面。
// 
// 作  者：汪荣
// 时  间：2013-01-02
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
