/*
File:   main.h
Author: Taylor Robbins
Date:   09\22\2024
*/

#ifndef _MAIN_H
#define _MAIN_H

struct AppState_t
{
	OC_Surface_t surface;
	OC_CanvasRenderer_t renderer;
	OC_CanvasContext_t canvasContext;
	OC_Image_t pigTexture;
	NSVGimage* svgImage;
};

#endif //  _MAIN_H
