/*
File:   main.cpp
Author: Taylor Robbins
Date:   09\21\2024
Description: 
	** This is the main file that gets compiled into the WASM module.
	** All other files are #included from this one (including cpp files!) to keep compilation simple
*/

// +--------------------------------------------------------------+
// |                           Includes                           |
// +--------------------------------------------------------------+
#include "build_config.h"

// +==============================+
// |          Libraries           |
// +==============================+
#define ORCA_COMPILATION
#define GYLIB_USE_ASSERT_FAILURE_FUNC
#define GYLIB_SCRATCH_ARENA_AVAILABLE
#include "gylib/gy.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

// +==============================+
// |   Application Header Files   |
// +==============================+
#include "version.h"
#include "debug.h"
#include "types.h"
#include "main.h"

// +==============================+
// |     Application Globals      |
// +==============================+
AppState_t* app = nullptr;
MemArena_t* stdHeap;
MemArena_t* mainHeap;
v2 MousePos = Vec2_Zero_Const;
v2 ScreenSize = Vec2_Zero_Const;
v2i ScreenSizei = Vec2i_Zero_Const;
rec ScreenRec = Rec_Zero_Const;

// +==============================+
// |   Application Source Files   |
// +==============================+
#include "debug.cpp"
#include "scratch.cpp"
#include "svg.cpp"

// +--------------------------------------------------------------+
// |                   Application Entry Points                   |
// +--------------------------------------------------------------+
// +==============================+
// |          OC_OnInit           |
// +==============================+
ORCA_EXPORT void OC_OnInit()
{
	// +==============================+
	// |   Initialize Memory Arenas   |
	// +==============================+
	{
		MemArena_t stdHeapLocal = {};
		InitMemArena_StdHeap(&stdHeapLocal);
		app = AllocStruct(&stdHeapLocal, AppState_t);
		NotNull(app);
		ClearPointer(app);
		MyMemCopy(&app->stdHeap, &stdHeapLocal, sizeof(MemArena_t));
		stdHeap = &app->stdHeap;
		InitMemArena_PagedHeapArena(&app->mainHeap, MAIN_HEAP_PAGE_SIZE, stdHeap);
		mainHeap = &app->mainHeap;
		InitScratchArenas(stdHeap, SCRATCH_ARENAS_PAGE_SIZE, SCRATCH_ARENAS_MAX_MARKS);
	}
	
	MemArena_t* scratch = GetScratchArena();
	PrintLine_I("%s app v%d.%d(%d) is starting...", PROJECT_NAME_STR, APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
	
	#if DEBUG_BUILD
	MyStr_t windowTitle = PrintInArenaStr(scratch, "%s %d.%d(%d)", PROJECT_NAME_STR, APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
	#else
	MyStr_t windowTitle = NewStr(PROJECT_NAME_STR);
	#endif
	OC_WindowSetTitle(windowTitle);
	
	app->renderer = OC_CanvasRendererCreate();
	app->surface = OC_CanvasSurfaceCreate(app->renderer);
	app->canvasContext = OC_CanvasContextCreate();
	OC_UiInit(&app->ui);
	OC_UiSetContext(&app->ui);
	
	app->pigTexture = OC_ImageCreateFromPath(app->renderer, NewStr("Image/pig_invalid.png"), false);
	OC_Assert(!OC_ImageIsNil(app->pigTexture), "Failed to load pig_invalid.png!");
	
    OC_UnicodeRange_t fontRanges[] = {
        OC_UNICODE_BASIC_LATIN,
        OC_UNICODE_C1_CONTROLS_AND_LATIN_1_SUPPLEMENT,
        OC_UNICODE_LATIN_EXTENDED_A,
        OC_UNICODE_LATIN_EXTENDED_B,
        OC_UNICODE_SPECIALS
    };
	app->debugFont = OC_FontCreateFromPath(NewStr("Font/consolab.ttf"), ArrayCount(fontRanges), fontRanges);
	
	MyStr_t svgFilePath = NewStr("Vector/blue_shape.svg");
	OC_File_t svgFile = OC_FileOpen(svgFilePath, OC_FILE_ACCESS_READ, OC_FILE_OPEN_NONE);
	OC_Assert(!OC_FileIsNil(svgFile), "Failed to open %.*s", svgFilePath.length, svgFilePath.chars);
	
	u64 svgFileSize = OC_FileSize(svgFile);
	OC_Assert(svgFileSize > 0, "SVG file failed to open or is empty!");
	// PrintLine_I("svg file is %llu bytes", svgFileSize);
	
	MyStr_t svgFileContents = NewStr(svgFileSize, AllocArray(scratch, char, svgFileSize+1));
	OC_Assert(svgFileContents.chars != nullptr, "Failed to allocate space for %u byte svg file", svgFileSize);
	u64 readResult = OC_FileRead(svgFile, svgFileSize, svgFileContents.chars);
	//TODO: Assert on readResult?
	svgFileContents.chars[svgFileContents.length] = '\0';
	
	app->svgImage = nsvgParse(svgFileContents.chars, "px", 96);
	OC_Assert(app->svgImage != nullptr, "Failed to parse svg image %.*s", svgFilePath.length, svgFilePath.chars);
	OC_FileClose(svgFile);
	
	if (TryLoadVectorImgFromPath(svgFilePath, mainHeap, &app->testVector))
	{
		PrintLine_I("Loaded SVG with %llu shape%s:", app->testVector.shapes.length, Plural(app->testVector.shapes.length, "s"));
		VarArrayLoop(&app->testVector.shapes, sIndex)
		{
			VarArrayLoopGet(VectorShape_t, shape, &app->testVector.shapes, sIndex);
			PrintLine_I("\tShape[%llu]: \"%s\" %llu path%s", sIndex, shape->name.chars, shape->paths.length, Plural(shape->paths.length, "s"));
			VarArrayLoop(&shape->paths, pIndex)
			{
				VarArrayLoopGet(VectorPath_t, path, &shape->paths, pIndex);
				PrintLine_I("\t\tPath[%llu]: %llu edge%s", pIndex, path->edges.length, Plural(path->edges.length, "s"));
				VarArrayLoop(&path->edges, eIndex)
				{
					VarArrayLoopGet(VectorEdge_t, edge, &path->edges, eIndex);
					PrintLine_I("\t\t\tEdge[%llu]: (%g,%g) (%g,%g) (%g,%g) (%g,%g)",
						pIndex,
						edge->start.x, edge->start.y,
						edge->control1.x, edge->control1.y,
						edge->control2.x, edge->control2.y,
						edge->end.x, edge->end.y
					);
					
				}
			}
		}
	}
	else { AssertMsg(false, "Failed to load and parse SVG file!"); }
	
	FreeScratchArena(scratch);
}

//TODO: Should we free any memory when the application closes?

ORCA_EXPORT void OC_OnRawEvent(OC_Event_t* event)
{
	OC_UiSetContext(&app->ui);
	OC_UiProcessEvent(event);
}

// +==============================+
// |         OC_OnResize          |
// +==============================+
ORCA_EXPORT void OC_OnResize(u32 width, u32 height)
{
	// PrintLine_I("OC_OnResize(%d, %d)!", width, height);
	ScreenSize = NewVec2((r32)width, (r32)height);
	ScreenSizei = NewVec2i((i32)width, (i32)height);
	ScreenRec = NewRec(0, 0, (r32)width, (r32)height);
}

// +==============================+
// |         OC_OnKeyDown         |
// +==============================+
ORCA_EXPORT void OC_OnKeyDown(OC_ScanCode_t scan, OC_KeyCode_t key)
{
	//TODO: Implement me!
}

// +==============================+
// |          OC_OnKeyUp          |
// +==============================+
ORCA_EXPORT void OC_OnKeyUp(OC_ScanCode_t scan, OC_KeyCode_t key)
{
	if (key == OC_KEY_ENTER)
	{
		MyStr_t testStr = NewStr("Is this thing on?");
		PrintLine_I("Testing 123 \"%.*s\"", StrPrint(testStr));
		PrintLine_I("Testing 234 \"%.*s\"", testStr.length, testStr.chars);
		PrintLine_I("Testing 345 \"%.*s\"", (u32)testStr.length, testStr.chars);
		PrintLine_I("Testing 456 %u", sizeof(testStr.length));
	}
	//TODO: Implement me!
}

// +==============================+
// |        OC_OnMouseMove        |
// +==============================+
ORCA_EXPORT void OC_OnMouseMove(r32 x, r32 y, r32 dx, r32 dy)
{
	MousePos.x = x;
	MousePos.y = y;
}

// +==============================+
// |      OC_OnFrameRefresh       |
// +==============================+
ORCA_EXPORT void OC_OnFrameRefresh()
{
	// r64 renderStartTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	MemArena_t* scratch = GetScratchArena();
	
	OC_UiSetContext(&app->ui);
	OC_CanvasContextSelect(app->canvasContext);
	OC_SetColor(NewColor(0xFFCC3B95));
	OC_Clear();
	
	#if 1
	OC_SetColor(NewColor(0xFFFFFFFF));
	// OC_ImageDraw(app->pigTexture, ScreenRec); //TODO: Color doesn't matter for OC_ImageDraw?
	OC_SetImage(app->pigTexture);
	OC_RectangleFill(ScreenRec);
	#endif
	
	// OC_UiSetTheme(&OC_UI_DARK_THEME);
	// oc_ui_box_size size;
	// oc_ui_layout layout;
	// oc_ui_box_floating floating;
	// oc_vec2 floatTarget;
	// oc_color color;
	// oc_color bgColor;
	// oc_color borderColor;
	// oc_font font;
	// f32 fontSize;
	// f32 borderSize;
	// f32 roundness;
	// f32 animationTime;
	// oc_ui_style_mask animationMask;
	#if 0
	OC_UiStyle_t uiStyle = {};
	uiStyle.font = app->debugFont;
	OC_UiFrame(ScreenSize, &uiStyle, OC_UI_STYLE_FONT)
	{
		OC_UiMenuBar("menu_bar")
		{
			OC_UiMenu("Test")
			{
				OC_UiSetNextWidth(200, OC_UI_SIZE_PIXELS);
				OC_UiSlider("test_slider", &app->testValue);
				if (OC_UiMenuButton("Quit").pressed)
				{
					OC_RequestQuit();
				}
			}
		}
	}
	#endif
	
	#if 0
	// PrintLine_I("shapes: %p %fx%f", app->svgImage->shapes, app->svgImage->width, app->svgImage->height);
	OC_SetImage(OC_ImageNil());
	u32 shapeIndex = 0;
	for (NSVGshape *shape = app->svgImage->shapes; shape != nullptr; shape = shape->next)
	{
		// PrintLine_I("Drawing shape[%u]", shapeIndex);
		u32 pathIndex = 0;
		for (NSVGpath *path = shape->paths; path != nullptr; path = path->next)
		{
			if (path->npts > 0)
			{
				// PrintLine_I("\tDrawing path[%u]", pathIndex);
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
						// PrintLine_I("\t\tDrawing quadratic1[%u] (%g, %g) (%g, %g) (%g, %g) (%g, %g)",
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
						// PrintLine_I("\t\tDrawing quadratic2[%u] (%g, %g) (%g, %g) (%g, %g) (%g, %g)",
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
						// PrintLine_I("\t\tDrawing cubic[%u] (%g, %g) (%g, %g) (%g, %g) (%g, %g)",
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
					OC_SetColor(NewColorSvg(shape->fill.color));
					OC_Fill();
				}
				
				for (int pIndex = 0; pIndex < path->npts-1; pIndex += 3)
				{
					float* pointsPntr = &path->pts[pIndex*2];
					if (pIndex == 0) { OC_MoveTo(pointsPntr[0], pointsPntr[1]); }
					// PrintLine_I("\t\tDrawing curve[%u]", pIndex/3);
					OC_CubicTo(
						pointsPntr[2], pointsPntr[3],
						pointsPntr[4], pointsPntr[5],
						pointsPntr[6], pointsPntr[7]
					);
				}
				OC_ClosePath();
				if (shape->stroke.type == NSVG_PAINT_COLOR)
				{
					OC_SetColor(NewColorSvg(shape->stroke.color));
					// PrintLine_I("width: %g", shape->strokeWidth);
					OC_SetWidth(shape->strokeWidth);
					OC_Stroke();
				}
			}
			pathIndex++;
		}
		
		shapeIndex++;
	}
	#endif
	
	#if 1
	OC_SetColor(NewColor(0xFFF27CB1));
	OC_SetImage(app->pigTexture);
	OC_RoundedRectangleFill(NewRec(MousePos.x, MousePos.y, 100, 200), 25);
	OC_SetImage(OC_ImageNil());
	
	OC_CircleFill(MousePos.x - 50 + 25, MousePos.y - 50 + 25, 50);
	
	OC_SetColor(NewColor(0xFFF83333));
	OC_SetWidth(20);
	OC_MoveTo(100, 100);
	OC_CubicTo(100, 100, MousePos.x, MousePos.y, 200, 100);
	OC_Stroke();
	#endif
	
	#if 1
	OC_SetFont(app->debugFont);
	OC_SetFontSize(18);
	OC_SetColor(Black);
	OC_TextFill(10,  25, PrintInArenaStr(scratch, "   Monotonic:    %f", OC_ClockTime(OC_CLOCK_MONOTONIC)));
	OC_TextFill(10,  50, PrintInArenaStr(scratch, "      Uptime:    %f", OC_ClockTime(OC_CLOCK_UPTIME)));
	OC_TextFill(10,  75, PrintInArenaStr(scratch, "        Date: %f", OC_ClockTime(OC_CLOCK_DATE)));
	OC_TextFill(10, 125, PrintInArenaStr(scratch, " Render Time: %.2fms", app->renderTimeLastFrame * 1000));
	OC_TextFill(10, 150, PrintInArenaStr(scratch, "Present Time: %.2fms", app->presentTimeLastFrame * 1000));
	OC_TextFill(10, 200, PrintInArenaStr(scratch, "Mouse: (%.2f, %.2f)", MousePos.x, MousePos.y));
	#endif
	
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
	
	// OC_UiDraw();
	
	// r64 renderEndTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	// r64 presentStartTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	OC_CanvasRender(app->renderer, app->canvasContext, app->surface);
	OC_CanvasPresent(app->renderer, app->surface);
	// r64 presentEndTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	
	FreeScratchArena(scratch);
	
	// app->renderTimeLastFrame = renderEndTime - renderStartTime;
	// app->presentTimeLastFrame = presentEndTime - presentStartTime;
}

void GyLibAssertFailure(const char* filePath, int lineNumber, const char* funcName, const char* expressionStr, const char* messageStr)
{
	//TODO: Note we don't REALLY need this print out, since the Orca level assertion also does a log,
	//but this will serve as a placeholder until we decide more about error handling and log storage/routing
	if (messageStr != nullptr && messageStr[0] != '\0')
	{
		PrintLine_E("Assertion Failed! %s", messageStr);
		PrintLine_E("\tin %s:%d in function %s!", filePath, lineNumber, funcName);
	}
	else
	{
		PrintLine_E("Assertion Failed! (%s) is not true", expressionStr);
		PrintLine_E("\tin %s:%d in function %s!", filePath, lineNumber, funcName);
	}
}
