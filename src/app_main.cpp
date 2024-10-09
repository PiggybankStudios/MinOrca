/*
File:   main.cpp
Author: Taylor Robbins
Date:   09\21\2024
Description: 
	** This is the main file that is #included by oc_main.cpp and it includes all
	** other application specific files
*/

// +--------------------------------------------------------------+
// |                   Application Header Files                   |
// +--------------------------------------------------------------+
#include "version.h"
#include "app_main.h"

// +--------------------------------------------------------------+
// |                     Application Globals                      |
// +--------------------------------------------------------------+
AppState_t* app = nullptr;

// +--------------------------------------------------------------+
// |                   Application Source Files                   |
// +--------------------------------------------------------------+
//TODO: Any files we want to include?

// +--------------------------------------------------------------+
// |                           AppInit                            |
// +--------------------------------------------------------------+
void AppInit()
{
	app->pigTexture = OC_ImageCreateFromPath(platform->renderer, NewStr("Image/pig_invalid.png"), false);
	OC_Assert(!OC_ImageIsNil(app->pigTexture), "Failed to load pig_invalid.png!");
	
	MyStr_t svgFilePath = NewStr("Vector/blue_shape.svg");
	if (TryLoadVectorImgFromPath(svgFilePath, mainHeap, &app->testVector))
	{
		PrintLine_I("Loaded SVG with %llu shape%s:", app->testVector.shapes.length, Plural(app->testVector.shapes.length, "s"));
		DebugPrintVectorImg(&app->testVector, DbgLevel_Debug);
	}
	else { AssertMsg(false, "Failed to load and parse SVG file!"); }
}

// +--------------------------------------------------------------+
// |                      AppUpdateAndRender                      |
// +--------------------------------------------------------------+
void AppUpdateAndRender()
{
	r64 renderStartTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	MemArena_t* scratch = GetScratchArena();
	
	OC_UiSetContext(&platform->ui);
	OC_CanvasContextSelect(platform->canvasContext);
	OC_SetColor(NewColor(0xFFCC3B95));
	OC_Clear();
	
	#if 1
	OC_SetColor(NewColor(0xFFFFFFFF));
	// OC_ImageDraw(app->pigTexture, ScreenRec); //TODO: Color doesn't matter for OC_ImageDraw?
	OC_SetImage(app->pigTexture);
	OC_RectangleFill(ScreenRec);
	#endif
	
	#if 1
	OC_UiStyle_t uiStyle = {};
	uiStyle.font = platform->debugFont;
	OC_UiFrame(ScreenSize, &uiStyle, OC_UI_STYLE_FONT)
	{
		OC_UiMenuBar("menu_bar")
		{
			OC_UiMenu("Test")
			{
				if (OC_UiMenuButton("Quit").pressed)
				{
					OC_RequestQuit();
				}
			}
		}
		
		OC_UiSetNextWidth(200, OC_UI_SIZE_PIXELS);
		OC_UiSlider("test_slider", &app->sliderValue);
		
		if (OC_UiButton("Commit!").pressed)
		{
			WriteLine_I("Clicked commit!");
		}
	}
	#endif
	
	RenderVectorImg(&app->testVector, NewVec2(0, 0), NewVec2(1, 1), White);
	
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
	OC_SetFont(platform->debugFont);
	OC_SetFontSize(18);
	OC_SetColor(Black);
	OC_TextFill(10,  25, PrintInArenaStr(scratch, "   Monotonic:    %f", OC_ClockTime(OC_CLOCK_MONOTONIC)));
	OC_TextFill(10,  50, PrintInArenaStr(scratch, "      Uptime:    %f", OC_ClockTime(OC_CLOCK_UPTIME)));
	OC_TextFill(10,  75, PrintInArenaStr(scratch, "        Date: %f", OC_ClockTime(OC_CLOCK_DATE)));
	OC_TextFill(10, 100, PrintInArenaStr(scratch, "Orca Version: %s", ORCA_VERSION));
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
	
	OC_UiDraw();
	
	r64 renderEndTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	r64 presentStartTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	OC_CanvasRender(platform->renderer, platform->canvasContext, platform->surface);
	OC_CanvasPresent(platform->renderer, platform->surface);
	r64 presentEndTime = OC_ClockTime(OC_CLOCK_MONOTONIC);
	
	FreeScratchArena(scratch);
	
	app->renderTimeLastFrame = renderEndTime - renderStartTime;
	app->presentTimeLastFrame = presentEndTime - presentStartTime;
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
