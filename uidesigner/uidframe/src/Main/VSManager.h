// ���ڿƼ���Ȩ���� 2010-2020
// 
// �ļ�����VSManager.h
// ��  �ܣ���
// 
// ��  �ߣ�����
// ʱ  �䣺2013-01-02
// 
// ============================================================================

# ifndef _VSMANAGER_H_
# define _VSMANAGER_H_

using namespace suic;

class VSManager
{
public:

	static void UnzipTo(const suic::Mulstr& data, const suic::String& strDir);
    static bool CreateVSProject(const suic::String& name, const suic::String& strDir);
};

#endif

