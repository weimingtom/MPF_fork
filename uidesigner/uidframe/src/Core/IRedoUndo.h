///
/// �ļ�����IRedoUndo.h
///
/// ��  �ܣ������UI��Ӧ������
///
/// ��  �ߣ�����
///

#ifndef _IREDOUNDO_H_
#define _IREDOUNDO_H_

class IRedoUndo
{
public:

    virtual ~IRedoUndo() {}

    virtual bool Redo() = 0;
    virtual void Undo() = 0;
};

#endif

