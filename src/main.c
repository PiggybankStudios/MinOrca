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

#include "build_config.h"

oc_surface surface;
oc_canvas_renderer renderer;
oc_canvas_context canvasContext;
oc_vec2 mousePos;

ORCA_EXPORT void oc_on_init()
{
	oc_log_info("oc_on_init!");
	
	oc_window_set_title(OC_STR8(PROJECT_NAME_STR));
	
    renderer = oc_canvas_renderer_create();
    surface = oc_canvas_surface_create(renderer);
    canvasContext = oc_canvas_context_create();
}

ORCA_EXPORT void oc_on_terminate()
{
	oc_log_info("oc_on_terminate!");
	//TODO: Implement me!
}

ORCA_EXPORT void oc_on_resize(u32 width, u32 height)
{
	oc_log_info("oc_on_resize(%d, %d)!", width, height);
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

ORCA_EXPORT void oc_on_mouse_move(float x, float y, float dx, float dy)
{
    mousePos.x = x;
    mousePos.y = y;
}

ORCA_EXPORT void oc_on_frame_refresh()
{
	oc_arena_scope scratch = oc_scratch_begin();
	
	oc_canvas_context_select(canvasContext);
    oc_set_color_rgba(0xCC/255.0f, 0x3B/255.0f, 0x95/255.0f, 1); //FFCC3B95
    oc_clear();
    
    oc_set_color_rgba(0xF2/255.0f, 0x7C/255.0f, 0xB1/255.0f, 1); //FFF27CB1
    oc_rounded_rectangle_fill(mousePos.x, mousePos.y, 100, 200, 15);
    
    oc_circle_fill(mousePos.x - 50 + 15, mousePos.y - 50 + 15, 50);
    
    oc_set_color_rgba(0xF8/255.0f, 0x33/255.0f, 0x33/255.0f, 1); //0xFFF83333
    oc_set_width(20);
    oc_move_to(100, 100);
    oc_cubic_to(100, 100, mousePos.x, mousePos.y, 200, 100);
    oc_stroke();
    
    oc_canvas_render(renderer, canvasContext, surface);
    oc_canvas_present(renderer, surface);
    
    oc_scratch_end(scratch);
}

