//Created by Wang.xp
//Created Time :2015/09/17
#ifndef _BindingItem_H_
#define _BindingItem_H_

#include <Framework\Controls\Menu.h>

namespace FrameworkCore
{


typedef delegate<bool(suic::Object*)> DelegateFunC;

/*
	���ڰ���֧�ֶ�̬�����м�ֵ�仯�¼������Ŀؼ�
*/
class PropertyDescriptors : public suic::NotifyPropChanged
{
public:

	BeginMember(PropertyDescriptors, suic::NotifyPropChanged)
		MemberString(Tip)
		MemberObject(IsChecked)
		MemberObject(IsReadOnly)
		// �����������ڶ�̬�󶨶����ʹ��
		// ���Ը��ݰ����ƶ�̬��ȡ�����ð�ֵ
		MemberAttributes(Property)
	EndMember()

	PropertyDescriptors();
	~PropertyDescriptors();

	virtual	suic::Object* GetIsChecked();
	virtual	void SetIsChecked(suic::Object* val);

	virtual	suic::Object* GetIsReadOnly();
	virtual	void SetIsReadOnly(suic::Object* val);

	virtual	void SetTip(suic::String tip);
	virtual	suic::String GetTip();

	virtual	bool GetProperty(const suic::String& name, suic::ObjectPtr& val);
	virtual bool SetProperty(const suic::String& name, suic::Object* val);
	
	suic::String ToString();

	void SetTag(suic::Object* tag);
	suic::Object* GetTag();

	void SetFlag(int flag);
	//Ĭ��Ϊ-1
	int GetFlag();

	suic::StringAutoDic<suic::Object> &Properties();

protected:

	int _flag;
	suic::String _tip;
	suic::StringAutoDic<suic::Object> _propertys;
	suic::Boolean* _isChecked;
	suic::Boolean* _isReadOnly;
	suic::Object* _bindingTag;
	void OnPropertyChanged(suic::String pro);
};

}
#endif
