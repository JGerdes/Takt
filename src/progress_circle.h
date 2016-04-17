#pragma once
#include <pebble.h>

void progress_circle_draw(struct Layer *layer, GContext *ctx);

void progress_circle_handle_beat();
void progress_circle_reset_beats();