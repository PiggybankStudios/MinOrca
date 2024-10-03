/*
File:   svg.cpp
Author: Taylor Robbins
Date:   09\29\2024
Description: 
	** Holds the functions that help us load a .svg file into a VectorImg_t struct
	** using nanosvg to do the parsing work
*/

colf NewColorfSvg(u32 svgPackedColor)
{
	return NewColorfBytes(
		(u8)((svgPackedColor >> 0) & 0xFF),
		(u8)((svgPackedColor >> 8) & 0xFF),
		(u8)((svgPackedColor >> 16) & 0xFF),
		(u8)((svgPackedColor >> 24) & 0xFF)
	);
}

//TODO: Turn the assertions into false returns!
bool TryLoadVectorImgFromPath(MyStr_t filePath, OC_Arena_t* arena, VectorImg_t* imageOut)
{
	OC_Assert(arena != nullptr, "Passed nullptr for arena");
	OC_Assert(imageOut != nullptr, "Passed nullptr for imageOut");
	OC_ArenaScope_t scratch = OC_ScratchBegin();
	NSVGimage* nsvg = nullptr;
	
	// Open, read and parse the file
	{
		OC_File_t svgFile = OC_FileOpen(filePath, OC_FILE_ACCESS_READ, OC_FILE_OPEN_NONE);
		OC_Assert(!OC_FileIsNil(svgFile), "Failed to open %.*s", filePath.length, filePath.chars);
		
		u64 svgFileSize = OC_FileSize(svgFile);
		OC_Assert(svgFileSize > 0, "SVG file failed to open or is empty!");
		// OC_Log_I("svg file is %llu bytes", svgFileSize);
		
		MyStr_t svgFileContents = NewStr(svgFileSize, OC_ArenaPushArray(scratch.arena, char, svgFileSize+1));
		OC_Assert(svgFileContents.chars != nullptr, "Failed to allocate space for %u byte svg file", svgFileSize);
		u64 readResult = OC_FileRead(svgFile, svgFileSize, svgFileContents.chars);
		//TODO: Assert on readResult?
		svgFileContents.chars[svgFileContents.length] = '\0';
		
		nsvg = nsvgParse(svgFileContents.chars, "px", 96);
		OC_Assert(nsvg != nullptr, "Failed to parse svg image %.*s", filePath.length, filePath.chars);
		OC_FileClose(svgFile);
	}
	
	u32 memorySize = 0;
	u8* memoryPntr = nullptr;
	for (u8 pass = 0; pass < 2; pass++)
	{
		u32 byteIndex = 0;
		
		u32 numShapes = 0;
		NSVGshape* nshape = nsvg->shapes;
		while (nshape != nullptr) { numShapes++; nshape = nshape->next; }
		
		VectorShape_t* shapes = (pass == 1) ? &memoryPntr[byteIndex] : nullptr;
		byteIndex += sizeof(VectorShape_t) * numShapes;
		
		nshape = nsvg->shapes;
		while (nshape != nullptr)
		{
			OC_Assert(BufferIsNullTerminated(ArrayCount(nshape->id), &nshape->id[0]));
			MyStr_t idStr = NewStr(&nshape->id[0]);
			
			shape = shape->next;
		}
		
		if (pass == 0)
		{
			if (byteIndex == 0) { break; }
			memorySize = byteIndex;
			memoryPntr = 
		}
	}
	
	OC_ScratchEnd(scratch);
}
