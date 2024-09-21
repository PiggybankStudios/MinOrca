/*
File:   main.c
Author: Taylor Robbins
Date:   09\21\2024
Description: 
	** This is the main file that gets compiled into the WASM module.
	** All other files are #included from this one (including cpp files!) to keep compilation simple
*/

#include <math.h>

#include <orca.h>

ORCA_EXPORT void oc_on_init()
{
	oc_log_info("oc_on_init!");
	//TODO: Implement me!
}

ORCA_EXPORT void oc_on_terminate()
{
	oc_log_info("oc_on_terminate!");
	//TODO: Implement me!
}

ORCA_EXPORT void oc_on_resize(u32 width, u32 height)
{
	//TODO: Implement me!
}

ORCA_EXPORT void oc_on_key_down(oc_scan_code scan, oc_key_code key)
{
	//TODO: Implement me!
}

ORCA_EXPORT void oc_on_key_up(oc_scan_code scan, oc_key_code key)
{
	//TODO: Implement me!
}

ORCA_EXPORT void oc_on_frame_refresh()
{
	//TODO: Implement me!
}

