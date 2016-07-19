///
/// �ļ�����IRedoUndo.h
///
/// ��  �ܣ������UI��Ӧ������
///
/// ��  �ߣ�����
///

#ifndef _REDOUNDOMANAGER_H_
#define _REDOUNDOMANAGER_H_

#include <Core/IRedoUndo.h>

class RedoUndoManager
{
public:

    RedoUndoManager();
    ~RedoUndoManager();

    void AddRedoUndo(IRedoUndo* pRedoUndo);
    void Redo();
    void Undo();

    void Clear();

private:

    void DoMaxRedoUndos();

private:

    int _maxRedoUndos;
    // ��¼����
    suic::Array<IRedoUndo*> _redos;
    suic::Array<IRedoUndo*> _undos;
};

#endif
