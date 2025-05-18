
/*
The format of the color is:
wxColor(RedValue, GreenValue, BlueValue, AlphaValue)
where:
The values range of RedValue, GreenValue, BlueValue, AlphaValue are integers from 0 to 255,
where 0 is no intensity and 255 is full intensity.
You can also define your own colors by specifying values for the alpha channel (opacity),
which ranges from 0 (fully transparent) to 255 (fully opaque),
using the `wxColor(int red, int green, int blue, int alpha)` constructor.

Besides:
wxWidgets also have a wxColorDataBase class that provides some predefined colors.
see this address for more information:
 https://docs.wxwidgets.org/3.0/classwx_colour_database.html#a66269122cb725da348b3a1c396002e41
*/

#ifndef YccKWxColorH
#define YccKWxColorH

#include <wx/colour.h>

extern int VDefaultAlpha;  
#define CRed wxColor(255, 0, 0, VDefaultAlpha)       
#define CGreen wxColor(0, 255, 0, VDefaultAlpha) 
#define CBlue wxColor(0, 0, 255, VDefaultAlpha)
#define CYellow wxColor(255, 255, 0, VDefaultAlpha)  
#define CMagenta wxColor(255, 0, 255, VDefaultAlpha) 
#define CCyan wxColor(0, 255, 255, VDefaultAlpha)    
#define CBlack wxColor(0, 0, 0, VDefaultAlpha)       
#define CWhite wxColor(255, 255, 255, VDefaultAlpha) 
#define CMGray wxColor(128, 128, 128, VDefaultAlpha) 
#define CLGray wxColor(192, 192, 192, VDefaultAlpha) 
#define CDGray wxColor(64, 64, 64, VDefaultAlpha)    
#define CDGray1 wxColor(125,125,0,VDefaultAlpha)

#endif
