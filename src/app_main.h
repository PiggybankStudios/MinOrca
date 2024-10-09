/*
File:   main.h
Author: Taylor Robbins
Date:   09\22\2024
*/

#ifndef _MAIN_H
#define _MAIN_H

struct AppState_t
{
	OC_Image_t pigTexture;
	VectorImg_t testVector;
	RandomSeries_t rand;
	
	r64 renderTimeLastFrame;
	r64 presentTimeLastFrame;
	
	r32 sliderValue;
};

#endif //  _MAIN_H
