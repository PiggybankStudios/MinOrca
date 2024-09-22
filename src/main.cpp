/*
File:   main.cpp
Author: Taylor Robbins
Date:   09\21\2024
Description: 
	** This is the main file that gets compiled into the WASM module.
	** All other files are #included from this one (including cpp files!) to keep compilation simple
*/

#include <math.h>
#include <orca.h>

#include "orca_aliases.h"
#include "orca_addons.h"
#include "main.h"

#include "build_config.h"

OC_Arena_t mainArena;
AppState_t* app = nullptr;
v2 MousePos = Vec2_Zero_Const;
v2 ScreenSize = Vec2_Zero_Const;
v2i ScreenSizei = Vec2i_Zero_Const;
rec ScreenRec = Rec_Zero_Const;

// +==============================+
// |          OC_OnInit           |
// +==============================+
EXPORT void OC_OnInit()
{
	OC_Log_I("OC_OnInit!");
	
	OC_ArenaInit(&mainArena);
	
	OC_WindowSetTitle(NewStr(PROJECT_NAME_STR));
	app = OC_ArenaPushType(&mainArena, AppState_t);
	OC_Assert(app != nullptr, "Failed to allocate AppState_t structure!!");
	memset(app, 0x00, sizeof(AppState_t)); //TODO: Change to ClearPntr?
	
	app->renderer = OC_CanvasRendererCreate();
	app->surface = OC_CanvasSurfaceCreate(app->renderer);
	app->canvasContext = OC_CanvasContextCreate();
	
	app->pigTexture = OC_ImageCreateFromPath(app->renderer, NewStr("Image/pig_invalid.png"), false);
	OC_Assert(!OC_ImageIsNil(app->pigTexture), "Failed to load pig_invalid.png!");
}

//TODO: Should we call OC_ArenaCleanup on mainArena when the application is closing?

// +==============================+
// |         OC_OnResize          |
// +==============================+
EXPORT void OC_OnResize(u32 width, u32 height)
{
	// OC_Log_I("OC_OnResize(%d, %d)!", width, height);
	ScreenSize = NewVec2((r32)width, (r32)height);
	ScreenSizei = NewVec2i((i32)width, (i32)height);
	ScreenRec = NewRec(0, 0, (r32)width, (r32)height);
}

// +==============================+
// |         OC_OnKeyDown         |
// +==============================+
EXPORT void OC_OnKeyDown(OC_ScanCode_t scan, OC_KeyCode_t key)
{
	//TODO: Implement me!
}

// +==============================+
// |          OC_OnKeyUp          |
// +==============================+
EXPORT void OC_OnKeyUp(OC_ScanCode_t scan, OC_KeyCode_t key)
{
	//TODO: Implement me!
}

// +==============================+
// |        OC_OnMouseMove        |
// +==============================+
EXPORT void OC_OnMouseMove(float x, float y, float dx, float dy)
{
	MousePos.x = x;
	MousePos.y = y;
}

// +==============================+
// |      OC_OnFrameRefresh       |
// +==============================+
EXPORT void OC_OnFrameRefresh()
{
	OC_ArenaScope_t scratch = OC_ScratchBegin();
	
	OC_CanvasContextSelect(app->canvasContext);
	OC_SetColorRgba(NewColorfHex(0xFFCC3B95));
	OC_Clear();
	
	OC_SetColorRgba(NewColorfHex(0xFF222222));
	// OC_ImageDraw(app->pigTexture, ScreenRec); //TODO: Color doesn't matter for OC_ImageDraw?
	OC_SetImage(app->pigTexture);
	OC_RectangleFill(ScreenRec);
	
	OC_SetColorRgba(NewColorfHex(0xFFF27CB1));
	OC_SetImage(app->pigTexture);
	OC_RoundedRectangleFill(NewRec(MousePos.x, MousePos.y, 100, 200), 25);
	OC_SetImage(OC_ImageNil());
	
	OC_CircleFill(MousePos.x - 50 + 25, MousePos.y - 50 + 25, 50);
	
	OC_SetColorRgba(NewColorfHex(0xFFF83333));
	OC_SetWidth(20);
	OC_MoveTo(100, 100);
	OC_CubicTo(100, 100, MousePos.x, MousePos.y, 200, 100);
	OC_Stroke();
	
	OC_CanvasRender(app->renderer, app->canvasContext, app->surface);
	OC_CanvasPresent(app->renderer, app->surface);
	
	OC_ScratchEnd(scratch);
}
