#include <pebble.h>
#include "progress_circle.h"
#include "detector.h"

static char beat_count = 0;

static bool fill_states[] = {true, true};

void draw_progress_circle(GContext *ctx, GRect bounds, uint level, float value) {
  bounds = grect_inset(bounds, GEdgeInsets(9 * level ));
  uint start, end;
  if(fill_states[level]) {
    start = 0;
    end = value * 360;
  } else {
    start = value * 360;
    end = 360;
  }
  graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, 10, DEG_TO_TRIGANGLE(start),  DEG_TO_TRIGANGLE(end));
}

void progress_circle_draw(struct Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  
  float value = (beat_count % 4) / 4.0f;
  draw_progress_circle(ctx, bounds, 0, value);
  
  value = beat_count / 16.0f;
  draw_progress_circle(ctx, bounds, 1, value);
}

void progress_circle_handle_beat() {
  beat_count = (beat_count + 1) % 16;
  if(beat_count % 4 == 0){
    fill_states[0] = !fill_states[0];
  }
  if(beat_count % 16 == 0){
    fill_states[1] = !fill_states[1];
  }
}

void progress_circle_reset_beats() {
  beat_count = 0;
  for(uint i = 0; i < ARRAY_LENGTH(fill_states); i++) {
    fill_states[i] = true;
  }
}