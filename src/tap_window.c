#include <pebble.h>
#include "tap_window.h"
#include "detector.h"

static TextLayer *s_time_layer;
static BitmapLayer *s_arc_layer;

static bool s_screen_dark = false;

void tap_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  s_arc_layer = bitmap_layer_create(layer_get_bounds(window_layer));
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorDarkGray);
  text_layer_set_text(s_time_layer, "Tap!");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_arc_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

void tap_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_arc_layer);
}

static void beat_happend() {
  detector_beat_happend();
  uint bpm = detector_get_bpm();
  static char s_buffer[16];
  snprintf(s_buffer, 16, "%u", bpm);
  text_layer_set_text(s_time_layer, s_buffer);
  
  if(s_screen_dark) {
    text_layer_set_text_color(s_time_layer, GColorChromeYellow);
    bitmap_layer_set_background_color(s_arc_layer, GColorWhite);
  } else {
    text_layer_set_text_color(s_time_layer, GColorPictonBlue);
    bitmap_layer_set_background_color(s_arc_layer, GColorBlack);
  }
  s_screen_dark = !s_screen_dark;
}

void tap_window_select_handler(ClickRecognizerRef recognizer, void *context) {
  beat_happend();
}

void tap_window_tap_handler(AccelAxisType axis, int32_t direction) {
  beat_happend();
}