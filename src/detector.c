#include <pebble.h>
#include "detector.h"

static long last_ms = 0;
static uint last_bpm[10];
static uint last_index = 0;

void detector_beat_happend() {
  time_t temp = time(NULL);
  long ms = time_ms(&temp, NULL);
  ms += temp * 1000;
  long diff = ms - last_ms;
  uint bpm = 60000 / diff;
  if(last_ms != 0) {
    if(last_bpm[0] == 0) {
      for(uint i=0; i<10; i++) {
        last_bpm[i] = bpm;
      }
    } else {
      last_index++;
      last_index = last_index % 10;
      last_bpm[last_index] = bpm;
    }
  }  
  last_ms = ms;
}

uint detector_get_bpm() {
  uint sum = 0;
  for(uint i=0; i<10; i++) {
      sum += last_bpm[i];
    }
  uint bpm = (sum/10) + 0.5;
  return bpm;
}