#pragma once
#include <pebble.h>

void tap_window_load(Window *window);
void tap_window_unload(Window *window);
void tap_window_select_handler(ClickRecognizerRef recognizer, void *context);
void tap_window_tap_handler(AccelAxisType axis, int32_t direction);