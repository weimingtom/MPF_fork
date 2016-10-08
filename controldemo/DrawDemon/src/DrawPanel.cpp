/////////////////////////////////////////////////////
// MainWindow.cpp

#include "stdafx.h"
#include "DrawPanel.h"

ImplementRTTIOfClass(DrawPanel, suic::Panel)

DrawPanel::DrawPanel()
    : _drawing(NULL)
{
}

DrawPanel::~DrawPanel()
{
}

void DrawPanel::OnInitialized(suic::EventArg* e)
{
    suic::Panel::OnInitialized(e);

    ResetRenderContext();

    suic::ButtonBase* pBtn = NULL;

    pBtn = GetParent()->FindElem<suic::ButtonBase>("saveShot");
    if (NULL != pBtn)
    {
        pBtn->AddClick(new suic::ClickEventHandler(this, &DrawPanel::OnButtonClicked));
    }
}

void DrawPanel::OnLoaded(suic::LoadedEventArg* e)
{
    suic::Panel::OnLoaded(e);
}

void DrawPanel::ResetRenderContext()
{
    RenderOpen(&_renderCtx);
    _drawing = _renderCtx.drawing;
}

void DrawPanel::UpdateDrawArea()
{
    if (_drawing != NULL)
    {
        suic::fRect rect(suic::fPoint(), GetRenderSize().TofSize());

        suic::Pen pen;
        suic::Drawing* drawing = _drawing;

        // ���Ʊ���
        drawing->FillRect(suic::Colors::Black, rect);

        // ��������
        pen.SetBrush(suic::SolidColorBrush::Red);
        pen.SetThickness(1);

        // ˮƽ�߲������
        pen.SetAntiAlias(false);
        drawing->DrawLine(&pen, suic::fPoint(8, 8), suic::fPoint(100, 8));

        pen.SetAntiAlias(true);
        drawing->DrawLine(&pen, suic::fPoint(2, 5), suic::fPoint(100, 100));

        // ����Բ��
        drawing->DrawCircle(NULL, &pen, suic::fPoint(100, 100), 50);

        suic::fRect fRect = suic::fRect(260,150,360,200);
        drawing->DrawRect(suic::SolidColorBrush::Green, &pen, &fRect);

        // �������ɫ
        pen.SetThickness(6);
        pen.SetBrush(new suic::SolidColorBrush(0xFFFF0000));
        drawing->DrawCircle(suic::SolidColorBrush::Green, &pen, suic::fPoint(200, 200), 40);

        drawing->DrawCircle(suic::SolidColorBrush::Green, NULL, suic::fPoint(60, 200), 20);

        // ����·��
        suic::PathGeometry path;
        path.MoveTo(200, 10);
        path.LineTo(180, 80);
        path.LineTo(230, 30);
        path.Close();

        //drawing->Save();
        //drawing->ClipGeometry(&path, suic::ClipOp::OpIntersect, false);

        path.AddOval(suic::fRect(250, 15, 300, 50));
        pen.SetThickness(3);

        drawing->DrawGeometry(suic::SolidColorBrush::Green, &pen, &path);
        //drawing->Restore();

        // ���ƻ���
        // ������brush����仭ˢ��pen���߿�ˢ��oval������������
        //       starta����ʼ�Ƕȣ�sweepa��˳ʱ�뷽�򾭹��ĽǶȣ�usecenter���Ƿ�ʹ��oval���ĵ�
        drawing->DrawArc(suic::SolidColorBrush::Green, &pen, suic::fRect(260, 200, 320, 280), 10, 90, true);

        suic::fRect txtRect(10,250,100,280);
        suic::FormattedText formattedText;

        formattedText.SetColor(0xff00ff00);

        drawing->DrawString(&formattedText, _U("980"), 3, &txtRect);
    }
}

void DrawPanel::OnRenderSizeChanged(suic::SizeChangedInfo& e)
{
    suic::Panel::OnRenderSizeChanged(e);

    ResetRenderContext();
    UpdateDrawArea();
}

void DrawPanel::OnRender(suic::Drawing* dr)
{
    suic::Panel::OnRender(dr);

    if (_renderCtx.bitmap != NULL)
    {
        suic::PathGeometry path;
        path.setAutoDelete(false);

        path.MoveTo(200, 10);
        path.LineTo(180, 80);
        path.LineTo(230, 10);
        path.Close();

        //dr->ClipGeometry(&path, suic::ClipOp::OpDifference, false);

        suic::fRect rcdc(0, 0, GetRenderSize().cx, GetRenderSize().cy);
        suic::fRect rcimg(0, 0, _renderCtx.bitmap->Width(), _renderCtx.bitmap->Height());

        //dr->DrawImage(_renderCtx.bitmap, &rcdc, &rcimg, 255);

        suic::BitmapTool::FillFrom(dr->GetBitmap(), dr->GetOffset().x, dr->GetOffset().y,
            _renderCtx.bitmap, 0, 0, _renderCtx.bitmap->Width(), _renderCtx.bitmap->Height());
    }
}

void DrawPanel::OnButtonClicked(suic::Element* sender, suic::RoutedEventArg* e)
{
    if (_renderCtx.bitmap != NULL)
    {
        _renderCtx.bitmap->Save(_U("d:\\DrawPanel.png"), suic::Bitmap::kPNG);
        suic::InfoBox::Show(_U("���浽 \"d:\\DrawPanel.png\""), _U("��ʾ"));
    }
}
