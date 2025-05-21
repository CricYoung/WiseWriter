
// simple c++ wxWidgets app
// use defined colors exp: CRed CGreen...
// draw rectangles with each defined colors

//include
#include <string>
#include <wx/wx.h>
#include "KWxColor.hpp"

// use VDefaultAlpha to adjust alpha value(opacity) default value is 255 for all predefined colors
int VDefaultAlpha=255;

using namespace std;
struct KWxColor
{
  string name;
  wxColor color;
};

// colors
KWxColor ColorGrp[] = {
    {"CRed", CRed}, {"CGreen", CGreen}, {"CBlue", CBlue}, {"CMagenta", CMagenta}, {"CCyan", CCyan},
    {"CBlack", CBlack}, {"CWhite", CWhite}, {"CGray", CMGray}, {"CLGray", CLGray}, {"CDGray", CDGray}, 
    {"CDGray1", CDGray1} 
    };

#if YccInSingleFileTestKWxColor == 1
// wxColor KWxColors[KWxColorCount] = { CRed, CGreen, CBlue, CMagenta, CCyan, CBlack, CWhite, CMGray, CLGray, CDGray, CDGray1 };
// main frame class
class MyFrame : public wxFrame
{
public:
  MyFrame() : wxFrame(NULL, wxID_ANY, "KWxColor"){
    this->Bind(wxEVT_PAINT, &MyFrame::OnPaint,this);
  };
  bool DrawRect(wxDC &dc, wxColor color, int x, int y, int w, int h)
  {
    dc.SetBrush(color);
    dc.DrawRectangle(x, y, w, h);
    return true;
  };
  bool DrawAllColors()
  {
    wxWindow *win = wxTheApp->GetTopWindow();
    wxWindowDC dc = wxWindowDC(win);
    int x = 10;
    int y = 10;
    int w = 100;
    int h = 100;
    int tCount=sizeof(ColorGrp)/sizeof(KWxColor);
    for (int i = 0; i < tCount; i++){
      //draw rectangles
      DrawRect(dc, ColorGrp[i].color, x, y, w, h);
      //draw text
      wxColor tCurColor=ColorGrp[i].color;
      if(tCurColor == CBlack || tCurColor == CBlue || tCurColor == CDGray) dc.SetTextForeground(CWhite);
      else dc.SetTextForeground(CBlack);
      dc.DrawText(ColorGrp[i].name, x, y);
      //next
      x += 110;
      if (x > 600)
      {
        x = 10;
        y += 110;
        if (y > 600)
          break;
      }
    }
    return true;
  };
  virtual void OnPaint(wxPaintEvent& ){
    DrawAllColors();
  }
};

class  MyApp : public wxApp
{
public:
  virtual bool OnInit(){
    MyFrame *frame = new MyFrame();
    
    return frame->Show(true);
  }
  
  virtual int OnExit() { return 0; };
};


wxIMPLEMENT_APP(MyApp);

#endif