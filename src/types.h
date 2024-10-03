/*
File:   types.h
Author: Taylor Robbins
Date:   09\29\2024
*/

#ifndef _TYPES_H
#define _TYPES_H

struct VectorPath_t
{
	
};

enum VectorFillType_t
{
	VectorFillType_None,
	VectorFillType_Solid,
	VectorFillType_NumTypes,
};
const char* GetVectorFillTypeStr(VectorFillType_t enumValue)
{
	switch (enumValue)
	{
		case VectorFillType_None:  return "None";
		case VectorFillType_Solid: return "Solid";
		default: return "Unknown";
	}
}
enum VectorStrokeType_t
{
	VectorStrokeType_None,
	VectorStrokeType_Solid,
	VectorStrokeType_NumTypes,
};
const char* GetVectorStrokeTypeStr(VectorStrokeType_t enumValue)
{
	switch (enumValue)
	{
		case VectorStrokeType_None:  return "None";
		case VectorStrokeType_Solid: return "Solid";
		default: return "Unknown";
	}
}

// NSVGshape has the following information we may want to add here:
//      char id[64];				// Optional 'id' attr of the shape or its group
//      float strokeDashOffset;		// Stroke dash offset (scaled).
//      float strokeDashArray[8];	// Stroke dash array (scaled).
//      char strokeDashCount;		// Number of dash values in dash array.
//      char strokeLineJoin;		// Stroke join type.
//      char strokeLineCap;			// Stroke cap type.
//      char fillGradient[64];		// Optional 'id' of fill gradient
//      char strokeGradient[64];	// Optional 'id' of stroke gradient
//      float xform[6];				// Root transformation for fill/stroke gradient
struct VectorShape_t
{
	MyStr_t name;
	bool visible;
	r32 opacity; //IsFlagSet(flags, NSVG_FLAGS_VISIBLE)
	rec bounds;
	
	VectorFillType_t fillType;
	bool fillOddSections; //fillRule == NSVG_FILLRULE_NONZERO
	colf fillColor;
	VectorStrokeType_t strokeType;
	r32 strokeWidth;
	colf strokeColor;
	float miterLimit;
	
	u32 numPaths;
	VectorPath_t* paths;
};

struct VectorImg_t
{
	OC_Arena_t* allocArena;
	u32 memorySize;
	void* memoryPntr;
	
	u32 numShapes;
	VectorShape_t* shapes;
};

#endif //  _TYPES_H
