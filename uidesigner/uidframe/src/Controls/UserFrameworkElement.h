///
/// �ļ�����UserFrameworkElement.h
///
/// ��  �ܣ�ʵ��һ��ģ����û��Զ���ؼ�
///
/// ��  �ߣ�����
///

#ifndef _USERFRAMEWORKELEMENT_H
#define _TEXTBOXRANGE_H

#include <Framework/Controls/RangeBase.h>

class UserFrameworkElement : public suic::FrameworkElement
{
public:

    RTTIOfClass(UserFrameworkElement)

    UserFrameworkElement();
    virtual ~UserFrameworkElement();

protected:

    void OnRender(Drawing* drawing);
};

#endif
