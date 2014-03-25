#ifndef _GUI_COLOURS_H_
#define _GUI_COLOURS_H_

#include "common_irrlicht.h"

#define GUI_BG_TOP		video::SColor(240,50,50,70)
#define GUI_BG_BTM		video::SColor(240,30,30,50)
#define GUI_BORDER		video::SColor(245,60,60,80)

#define GUI_BG_DEATH		video::SColor(180,50,0,0)

#define GUI_BG_TT		video::SColor(140,30,30,50)
#define GUI_TT_TEXT		video::SColor(255,255,255,255)

#define GUI_3D_HIGH_LIGHT	video::SColor(255,0,0,0)
#define GUI_HIGH_LIGHT		video::SColor(255,70,100,50)
#define GUI_HIGH_LIGHT_TEXT	video::SColor(255,255,255,255)
#define GUI_BUTTON_TEXT		video::SColor(255,255,255,255)

#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
// Irrlicht 1.8 input colours
#define GUI_3D_FACE		video::SColor(255,60,60,80)
#define GUI_3D_SHADOW		video::SColor(255,60,60,80)
#define GUI_3D_DARK_SHADOW	video::SColor(255,30,30,50)
#define GUI_EDITABLE		video::SColor(255,60,60,80)
#define GUI_FOCUSED_EDITABLE	video::SColor(255,60,60,80)

#else

#define GUI_3D_SHADOW		video::SColor(255,0,0,0)

#endif

#endif
