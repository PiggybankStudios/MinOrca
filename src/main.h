/*
File:   main.h
Author: Taylor Robbins
Date:   09\22\2024
*/

#ifndef _MAIN_H
#define _MAIN_H

#define MAIN_HEAP_PAGE_SIZE Megabytes(16)

struct AppState_t
{
	MemArena_t stdHeap;
	MemArena_t mainHeap;
	
	OC_Surface_t surface;
	OC_CanvasRenderer_t renderer;
	OC_CanvasContext_t canvasContext;
	OC_Image_t pigTexture;
	NSVGimage* svgImage;
	VectorImg_t testVector;
};

#endif //  _MAIN_H
