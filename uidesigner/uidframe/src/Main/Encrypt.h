// ���ڿƼ���Ȩ���� 2010-2011
// 
// �ļ�����Encry.h
// ��  �ܣ������㷨�ļ���
// 
// ��  �ߣ�����
// ʱ  �䣺2012-02-02
// 
// ============================================================================

#ifndef _UIENCRYPT_H_
#define _UIENCRYPT_H_

#include <System/Windows/Object.h>
#include <System/Types/String.h>

class UIEncrypt
{
public:

    static int Encrypt(const suic::String& pwd, suic::String path);

protected:

    UIEncrypt();
};

#endif
