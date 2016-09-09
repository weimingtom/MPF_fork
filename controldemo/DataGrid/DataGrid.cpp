// DataGrid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/MainWindow.h"
#include "src/SimpleDataGrid.h"

static int columns = 10;
static int _count = 0;
static int _count2 = 0;
static int _maxCount = 5000;

bool Test(suic::Object* sender)
{
	
	FrameworkCore::SimpleDataGrid* _drg=suic::DynamicCast<FrameworkCore::SimpleDataGrid>(sender);
	if(_drg != NULL)
	{
		for (_count; _count < _maxCount; _count++)
		{
			FrameworkCore::SimpleDataGridRow* row=new FrameworkCore::SimpleDataGridRow();
			row->SetFlag(_count);
			row->SetProperty("ID",new suic::OString(_count));
			row->SetProperty("����",new suic::OString("W"));
			row->SetProperty("�Ա�",new suic::OString("��"));
			row->SetProperty("����",new suic::OString("20"));
			row->SetProperty("�绰",new suic::OString("1234567890"));

            for (int i = 0; i < columns; ++i)
            {
                suic::String strName;
                strName.Format(_U("name_%d"), i + 1);
                row->SetProperty(strName, new suic::OString(suic::String().Format(_U("1234567890_%d"), _count + 1)));
            }

			_drg->AddRow(row);
			_count2++;
			if(_count2 >= 100)
			{
				_count2=0;
				break;;
			}
		}
	}

    return (_count < _maxCount);
}

void Run(suic::Window* pWnd)
{	
	if (pWnd != NULL)
	{	
		_count=0;
		_count2=0;
		//pWnd->SetWidth(500);
		//pWnd->SetHeight(500);
		pWnd->SetTitle("�򿪼򵥱�");
		FrameworkCore::SimpleDataGrid* _drg=new FrameworkCore::SimpleDataGrid();
		_drg->AddColumn("ID");
		_drg->AddColumn("����");
		_drg->AddColumn("�Ա�");
		_drg->AddColumn("����");
		_drg->AddColumn("�绰");
        for (int i = 0; i < columns; ++i)
            {
                suic::String strName;
                strName.Format(_U("name_%d"), i + 1);
                _drg->AddColumn(strName);
            }

        //Test(_drg);
		_drg->AddInvoker(FrameworkCore::DelegateFunC(Test));
		
        suic::Grid* pGrid = pWnd->FindElem<suic::Grid>(_U("mainContent"));
        if (NULL != pGrid)
        {
            pGrid->AddChild(_drg);
        }
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    suic::InitUIWgx();

    suic::WindowPtr wnd(new MainWindow());

    try
    {
        // ������Դ�ļ�
        suic::ThemeOp::SetSystemTheme("default.sres", "wr");
        suic::String strApp = _T("ControlDemo/DataGrid/Application.xml");
        suic::Builder::BuildUri(wnd.get(), strApp.c_str());

        Run(wnd.get());

        // ���벼����Դ������ϵͳ
        suic::Application::Current()->Run(wnd.get());
        wnd->Dispose();
    }
    catch(suic::Exception& e)
    {
        suic::String err = e.GetErrorMsg();
    }

    suic::ExitUIWgx();

    return 0;
}
