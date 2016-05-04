#pragma once
#include <pebble.h>


void detector_beat_happend();
uint detector_get_bpm();
void detector_set_bpm(uint bpm);
uint detector_get_beat_duration(uint bpm);

char detector_get_beat_in_bar();