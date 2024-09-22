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

#include "build_config.h"

OC_Surface_t surface;
OC_CanvasRenderer_t renderer;
OC_CanvasContext_t canvasContext;
v2 mousePos;

ORCA_EXPORT void OC_OnInit()
{
	OC_Log_I("OC_OnInit!");
	OC_Log_I("sizeof(size_t): %d", sizeof(size_t));
	OC_Log_I("sizeof(oc_str8): %d", sizeof(oc_str8));
	OC_Log_I("sizeof(MyStr_t): %d", sizeof(MyStr_t));
	
	OC_WindowSetTitle(NewStr(PROJECT_NAME_STR));
	
	renderer = OC_CanvasRendererCreate();
	surface = OC_CanvasSurfaceCreate(renderer);
	canvasContext = OC_CanvasContextCreate();
}

ORCA_EXPORT void OC_OnTerminate()
{
	OC_Log_I("OC_OnTerminate!");
	//TODO: Implement me!
}

ORCA_EXPORT void OC_OnResize(u32 width, u32 height)
{
	OC_Log_I("oc_on_resize(%d, %d)!", width, height);
	//TODO: Implement me!
}

ORCA_EXPORT void OC_OnKeyDown(oc_scan_code scan, oc_key_code key)
{
	//TODO: Implement me!
}

ORCA_EXPORT void OC_OnKeyUp(oc_scan_code scan, oc_key_code key)
{
	//TODO: Implement me!
}

ORCA_EXPORT void OC_OnMouseMove(float x, float y, float dx, float dy)
{
	mousePos.x = x;
	mousePos.y = y;
}

ORCA_EXPORT void OC_OnFrameRefresh()
{
	OC_ArenaScope_t scratch = OC_ScratchBegin();
	
	OC_CanvasContextSelect(canvasContext);
	OC_SetColorRgba(NewColorfHex(0xFFCC3B95));
	OC_Clear();
	
	OC_SetColorRgba(NewColorfHex(0xFFF27CB1));
	OC_RoundedRectangleFill(mousePos.x, mousePos.y, 100, 200, 15);
	
	OC_CircleFill(mousePos.x - 50 + 15, mousePos.y - 50 + 15, 50);
	
	OC_SetColorRgba(NewColorfHex(0xFFF83333));
	OC_SetWidth(20);
	OC_MoveTo(100, 100);
	OC_CubicTo(100, 100, mousePos.x, mousePos.y, 200, 100);
	OC_Stroke();
	
	OC_CanvasRender(renderer, canvasContext, surface);
	OC_CanvasPresent(renderer, surface);
	
	OC_ScratchEnd(scratch);
}

