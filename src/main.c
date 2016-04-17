#include <pebble.h>
#include "tap_window.h"

static Window *tap_window;

static void click_config_provider(void *context) {
  ButtonId id = BUTTON_ID_SELECT;
  window_single_click_subscribe(id, tap_window_select_handler);
  id = BUTTON_ID_UP;
  window_single_click_subscribe(id, tap_window_up_handler);
  id = BUTTON_ID_DOWN;
  window_single_click_subscribe(id, tap_window_down_handler);

}

static void init() {
  tap_window = window_create();
  
  window_set_window_handlers(tap_window, (WindowHandlers) {
    .load = tap_window_load,
    .unload = tap_window_unload
  });
  
  window_stack_push(tap_window, true);
  window_set_click_config_provider(tap_window, click_config_provider);
  accel_tap_service_subscribe(tap_window_tap_handler);
}

static void deinit() {
  accel_tap_service_unsubscribe();
  window_destroy(tap_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}