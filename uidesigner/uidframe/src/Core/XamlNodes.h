///
/// �ļ�����XamlNodes.h
///
/// ��  �ܣ������UI��Ӧ������
///
/// ��  �ߣ�����
///

#include <Core/XamlSceneNode.h>

class PropertyId : public IPropertyId
{
public:

    PropertyId(const String& name, DpProperty* dp);
    virtual ~PropertyId();

    // ��ȡ�����Զ�Ӧ������
    virtual DpProperty* GetTargetType();
    virtual String GetName();
    virtual String GetUniqueName();

protected:

    suic::String _name;
    suic::DpProperty* _prop;
};

class PropElement
{
public:

    PropElement(RTTIOfInfo* info, RTTIOfInfo* rttiInfo);

    RTTIOfInfo* GetRTTIInfo() const
    {
        return _rttiInfo;
    }

    RTTIOfInfo* GetDesignInfo() const
    {
        return _desingInfo;
    }

    IPropertyId* RegisterProperty(const String& name, DpProperty* dp);

private:

    IPropertyId* Find(const String& name);

    friend class XamlNodes;

private:

    RTTIOfInfo* _rttiInfo;
    RTTIOfInfo* _desingInfo;
    suic::StringDic<IPropertyId*> _props;
};
