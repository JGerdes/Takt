#include <pebble.h>
#include "tap_window.h"
#include "config.h"
#include "detector.h"
#include "progress_circle.h"

static TextLayer *s_time_layer;
static BitmapLayer *s_bg_layer;
static Layer *s_arc_layer;

static AppTimer *s_auto_beat_timer = NULL;

void tap_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  s_bg_layer = bitmap_layer_create(bounds);
  s_arc_layer = layer_create(bounds);
  
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorDarkGray);
  text_layer_set_text(s_time_layer, "Tap!");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bg_layer));
  layer_add_child(window_layer, s_arc_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  layer_set_update_proc(s_arc_layer, progress_circle_draw);

}

void tap_window_unload(Window *window) {
  if(s_auto_beat_timer != NULL) {
    app_timer_cancel(s_auto_beat_timer);
  }
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_bg_layer);
}

static void toggle_screen() {
  static bool screen_dark = false;
  if(screen_dark) {
    text_layer_set_text_color(s_time_layer, COLOR_FONT_EVEN);
    bitmap_layer_set_background_color(s_bg_layer, COLOR_BACKGROUND_EVEN);
  } else {
    text_layer_set_text_color(s_time_layer, COLOR_FONT_ODD);
    bitmap_layer_set_background_color(s_bg_layer, COLOR_BACKGROUND_ODD);
  }
  screen_dark = !screen_dark;
  progress_circle_handle_beat();
  layer_mark_dirty(s_arc_layer);
}

static void auto_beat_handler(void *data) {
  uint bpm = detector_get_bpm();
  s_auto_beat_timer = app_timer_register(detector_get_beat_duration(bpm), auto_beat_handler, NULL);
  toggle_screen();
  vibes_short_pulse();
  
}

static void update_bpm_text(unsigned int bpm) {
  static char s_buffer[16];
  snprintf(s_buffer, 16, "%u", bpm);
  text_layer_set_text(s_time_layer, s_buffer);
}
static void beat_happend() {
  detector_beat_happend();
  uint bpm = detector_get_bpm();
  update_bpm_text(bpm);
  
  if(bpm != 0) {
    if(s_auto_beat_timer == NULL) {
      auto_beat_handler(NULL);
    } else {
      app_timer_reschedule(s_auto_beat_timer, detector_get_beat_duration(bpm));
      toggle_screen();
    }
  }
}

void tap_window_select_handler(ClickRecognizerRef recognizer, void *context) {
  beat_happend();
}

void tap_window_up_handler(ClickRecognizerRef recognizer, void *context) {
  //progress_circle_reset_beats();
  uint bpm = detector_get_bpm();
  detector_set_bpm(bpm+1);
  update_bpm_text(bpm+1);
}

void tap_window_down_handler(ClickRecognizerRef recognizer, void *context) {
  uint bpm = detector_get_bpm();
  if(bpm>1) {
    detector_set_bpm(bpm-1);
    update_bpm_text(bpm-1);
  }
  
}

void tap_window_tap_handler(AccelAxisType axis, int32_t direction) {
  //beat_happend();
}