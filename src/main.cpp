/*
File:   main.cpp
Author: Taylor Robbins
Date:   09\21\2024
Description: 
	** This is the main file that gets compiled into the WASM module.
	** All other files are #included from this one (including cpp files!) to keep compilation simple
*/

#include <math.h>
#define restrict __restrict
#include <stdio.h>

// +--------------------------------------------------------------+
// |                       Library Includes                       |
// +--------------------------------------------------------------+
#include <orca.h>
#include "build_config.h"
#include "my_orca/my_orca.h"

// #define ORCA_COMPILATION
// #include "gylib/gy.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

// +--------------------------------------------------------------+
// |                         Header Files                         |
// +--------------------------------------------------------------+
#include "version.h"
#include "types.h"
#include "main.h"

// +--------------------------------------------------------------+
// |                           Globals                            |
// +--------------------------------------------------------------+
OC_Arena_t mainArena;
AppState_t* app = nullptr;
v2 MousePos = Vec2_Zero_Const;
v2 ScreenSize = Vec2_Zero_Const;
v2i ScreenSizei = Vec2i_Zero_Const;
rec ScreenRec = Rec_Zero_Const;

// +--------------------------------------------------------------+
// |                         Source Files                         |
// +--------------------------------------------------------------+
#include "svg.cpp"

// +--------------------------------------------------------------+
// |                   Application Entry Points                   |
// +--------------------------------------------------------------+
// +==============================+
// |          OC_OnInit           |
// +==============================+
EXPORT void OC_OnInit()
{
	OC_Log_I("%s app v%d.%d(%d) is starting...", PROJECT_NAME_STR, APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
	
	OC_ArenaInit(&mainArena);
	app = OC_ArenaPushType(&mainArena, AppState_t);
	OC_Assert(app != nullptr, "Failed to allocate AppState_t structure!!");
	memset(app, 0x00, sizeof(AppState_t)); //TODO: Change to ClearPntr?
	
	OC_ArenaScope_t scratch = OC_ScratchBegin();
	
	#if DEBUG_BUILD
	MyStr_t windowTitle = OC_Str8Pushf(&mainArena, "%s %d.%d(%d)", PROJECT_NAME_STR, APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
	#else
	MyStr_t windowTitle = NewStr(PROJECT_NAME_STR);
	#endif
	OC_WindowSetTitle(windowTitle);
	
	app->renderer = OC_CanvasRendererCreate();
	app->surface = OC_CanvasSurfaceCreate(app->renderer);
	app->canvasContext = OC_CanvasContextCreate();
	
	app->pigTexture = OC_ImageCreateFromPath(app->renderer, NewStr("Image/pig_invalid.png"), false);
	OC_Assert(!OC_ImageIsNil(app->pigTexture), "Failed to load pig_invalid.png!");
	
	MyStr_t svgFilePath = NewStr("Vector/blue_shape.svg");
	OC_File_t svgFile = OC_FileOpen(svgFilePath, OC_FILE_ACCESS_READ, OC_FILE_OPEN_NONE);
	OC_Assert(!OC_FileIsNil(svgFile), "Failed to open %.*s", svgFilePath.length, svgFilePath.chars);
	
	u64 svgFileSize = OC_FileSize(svgFile);
	OC_Assert(svgFileSize > 0, "SVG file failed to open or is empty!");
	// OC_Log_I("svg file is %llu bytes", svgFileSize);
	
	MyStr_t svgFileContents = NewStr(svgFileSize, OC_ArenaPushArray(scratch.arena, char, svgFileSize+1));
	OC_Assert(svgFileContents.chars != nullptr, "Failed to allocate space for %u byte svg file", svgFileSize);
	u64 readResult = OC_FileRead(svgFile, svgFileSize, svgFileContents.chars);
	//TODO: Assert on readResult?
	svgFileContents.chars[svgFileContents.length] = '\0';
	
	app->svgImage = nsvgParse(svgFileContents.chars, "px", 96);
	OC_Assert(app->svgImage != nullptr, "Failed to parse svg image %.*s", svgFilePath.length, svgFilePath.chars);
	OC_FileClose(svgFile);
	
	OC_ScratchEnd(scratch);
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
	OC_SetColorSrgba(NewColorfHex(0xFFCC3B95));
	OC_Clear();
	
	OC_SetColorSrgba(NewColorfHex(0xFFFFFFFF));
	// OC_ImageDraw(app->pigTexture, ScreenRec); //TODO: Color doesn't matter for OC_ImageDraw?
	OC_SetImage(app->pigTexture);
	OC_RectangleFill(ScreenRec);
	
	#if 1
	// OC_Log_I("shapes: %p %fx%f", app->svgImage->shapes, app->svgImage->width, app->svgImage->height);
	OC_SetImage(OC_ImageNil());
	u32 shapeIndex = 0;
	for (NSVGshape *shape = app->svgImage->shapes; shape != nullptr; shape = shape->next)
	{
		// OC_Log_I("Drawing shape[%u]", shapeIndex);
		u32 pathIndex = 0;
		for (NSVGpath *path = shape->paths; path != nullptr; path = path->next)
		{
			if (path->npts > 0)
			{
				// OC_Log_I("\tDrawing path[%u]", pathIndex);
				for (int pIndex = 0; pIndex < path->npts-1; pIndex += 3)
				{
					float* pointsPntr = &path->pts[pIndex*2];
					v2 start = NewVec2(pointsPntr[0], pointsPntr[1]);
					v2 control1 = NewVec2(pointsPntr[2], pointsPntr[3]);
					v2 control2 = NewVec2(pointsPntr[4], pointsPntr[5]);
					v2 end = NewVec2(pointsPntr[6], pointsPntr[7]);
					if (pIndex == 0) { OC_MoveTo(start); }
					if (start == control1)
					{
						// OC_Log_I("\t\tDrawing quadratic1[%u] (%g, %g) (%g, %g) (%g, %g) (%g, %g)",
						// 	pIndex/3,
						// 	start.x, start.y,
						// 	control1.x, control1.y,
						// 	control2.x, control2.y,
						// 	end.x, end.y
						// );
						OC_QuadraticTo(
							control2,
							end
						);
					}
					else if (control2 == end)
					{
						// OC_Log_I("\t\tDrawing quadratic2[%u] (%g, %g) (%g, %g) (%g, %g) (%g, %g)",
						// 	pIndex/3,
						// 	start.x, start.y,
						// 	control1.x, control1.y,
						// 	control2.x, control2.y,
						// 	end.x, end.y
						// );
						OC_QuadraticTo(
							control1,
							end
						);
					}
					else
					{
						// OC_Log_I("\t\tDrawing cubic[%u] (%g, %g) (%g, %g) (%g, %g) (%g, %g)",
						// 	pIndex/3,
						// 	start.x, start.y,
						// 	control1.x, control1.y,
						// 	control2.x, control2.y,
						// 	end.x, end.y
						// );
						OC_CubicTo(
							control1.x, control1.y,
							control2.x, control2.y,
							end.x, end.y
						);
					}
				}
				OC_ClosePath();
				if (shape->fill.type == NSVG_PAINT_COLOR)
				{
					OC_SetColorSrgba(NewColorfSvg(shape->fill.color));
					OC_Fill();
				}
				
				for (int pIndex = 0; pIndex < path->npts-1; pIndex += 3)
				{
					float* pointsPntr = &path->pts[pIndex*2];
					if (pIndex == 0) { OC_MoveTo(pointsPntr[0], pointsPntr[1]); }
					// OC_Log_I("\t\tDrawing curve[%u]", pIndex/3);
					OC_CubicTo(
						pointsPntr[2], pointsPntr[3],
						pointsPntr[4], pointsPntr[5],
						pointsPntr[6], pointsPntr[7]
					);
				}
				OC_ClosePath();
				if (shape->stroke.type == NSVG_PAINT_COLOR)
				{
					OC_SetColorSrgba(NewColorfSvg(shape->stroke.color));
					// OC_Log_I("width: %g", shape->strokeWidth);
					OC_SetWidth(shape->strokeWidth);
					OC_Stroke();
				}
			}
			pathIndex++;
		}
		
		shapeIndex++;
	}
	#endif
	
	OC_SetColorSrgba(NewColorfHex(0xFFF27CB1));
	OC_SetImage(app->pigTexture);
	OC_RoundedRectangleFill(NewRec(MousePos.x, MousePos.y, 100, 200), 25);
	OC_SetImage(OC_ImageNil());
	
	OC_CircleFill(MousePos.x - 50 + 25, MousePos.y - 50 + 25, 50);
	
	OC_SetColorSrgba(NewColorfHex(0xFFF83333));
	OC_SetWidth(20);
	OC_MoveTo(100, 100);
	OC_CubicTo(100, 100, MousePos.x, MousePos.y, 200, 100);
	OC_Stroke();
	
	rec rec1 = Rec_Zero;
	rec rec2 = Rec_Zero;
	OC_ImageDrawRegion(app->pigTexture, rec1, rec2);
	
	// oc_move_to(612.69f, 429.33f);
	// oc_cubic_to(544.713f, 413.23f, 453.48f, 418.597f, 454.375f, 393.553f);
	// oc_quadratic_to(513.407f, 223.609f, 513.407f, 223.609f);
	// oc_cubic_to(569.161f, 218.243f, 624.914f, 212.876f, 680.667f, 207.51f);
	// oc_cubic_to(747.392f, 199.288f, 745.241f, 332.973f, 705.712f, 375.664f);
	// oc_quadratic_to(705.712f, 375.664f, 690.506f, 488.363f);
	// oc_cubic_to(663.971f, 427.84f, 637.436f, 367.316f, 610.901f, 306.792f);
	// oc_cubic_to(589.435f, 320.805f, 567.968f, 334.818f, 546.502f, 348.831f);
	// oc_cubic_to(568.564f, 375.664f, 590.627f, 402.497f, 612.69f, 429.33f);
	// oc_close_path();
	// oc_set_color_srgba(1, 1, 1, 1);
	// oc_set_width(18.8f);
	// oc_stroke();
	
	OC_CanvasRender(app->renderer, app->canvasContext, app->surface);
	OC_CanvasPresent(app->renderer, app->surface);
	
	OC_ScratchEnd(scratch);
}
