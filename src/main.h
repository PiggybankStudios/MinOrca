/*
File:   main.h
Author: Taylor Robbins
Date:   09\22\2024
*/

#ifndef _MAIN_H
#define _MAIN_H

struct AppState_t
{
	MemArena_t stdHeap;
	MemArena_t mainHeap;
	
	OC_Surface_t surface;
	OC_CanvasRenderer_t renderer;
	OC_CanvasContext_t canvasContext;
	OC_UiContext_t ui;
	
	OC_Image_t pigTexture;
	OC_Font_t debugFont;
	NSVGimage* svgImage;
	VectorImg_t testVector;
	
	r64 renderTimeLastFrame;
	r64 presentTimeLastFrame;
	
	r32 testValue;
};

#endif //  _MAIN_H
