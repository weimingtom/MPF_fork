
#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include "Project.h"

using namespace suic;

class SlnTreeManager;

class Solution : public XamlNodeItem
{
public:

    Solution(SlnTreeManager* docMana);
    Solution(SlnTreeManager* docMana, String name);

    virtual ~Solution();

    suic::ItemCollection* GetProjects();
    Brush* GetIcon() const;

    String GetName() const;
    void SetName(String val);

    String ToString();
    int GetProjectCount();

    void Close();
    void SaveAll();

protected:

    SlnTreeManager* _docMana;
    // ��������·��
    String _slnDir;
    // ��������
    String _slnName;
};

#endif
