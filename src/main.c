#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_arc_layer;

static long last_ms = 0;
static uint last_bpm[10];
static uint last_index = 0;

static void update(){
  time_t temp = time(NULL);
  long ms = time_ms(&temp, NULL);
  ms += temp * 1000;
  long diff = ms - last_ms;
  uint bpm = 60000 / diff;
  if(last_ms != 0){
    if(last_bpm[0] == 0){
      for(uint i=0; i<10; i++){
        last_bpm[i] = bpm;
      }
    }else{
      last_index++;
      last_index = last_index % 10;
      last_bpm[last_index] = bpm;
    }
  }
  
  uint sum = 0;
  for(uint i=0; i<10; i++){
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "bpm on %d is %d", i, last_bpm[i]);
      sum += last_bpm[i];
    }
  bpm = (sum/10) + 0.5;
  
  last_ms = ms;
  
  static char s_buffer[16];
  snprintf(s_buffer, 16, "%u", bpm);
  text_layer_set_text(s_time_layer, s_buffer);
  
  if(last_index % 2 == 1){
    text_layer_set_text_color(s_time_layer, GColorChromeYellow);
    window_set_background_color(s_main_window, GColorWhite);
  }else{
    text_layer_set_text_color(s_time_layer, GColorPictonBlue);
    window_set_background_color(s_main_window, GColorBlack);
  }
  
  
}

//static void layer_update_proc(GContext *ctx, Layer *layer){
  
//}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  update();
}

static void on_tab_handler(AccelAxisType axis, int32_t direction) {
  update();
}

static void click_config_provider(void *context) {
  ButtonId id = BUTTON_ID_SELECT;
  window_single_click_subscribe(id, select_click_handler);

}


static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  s_arc_layer = bitmap_layer_create(GRect(0, 0, 58, 58));
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorDarkGray);
  text_layer_set_text(s_time_layer, "Tap!");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_arc_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_arc_layer);
}

static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  window_set_click_config_provider(s_main_window, click_config_provider);
  accel_tap_service_subscribe(on_tab_handler);
  //layer_set_update_proc(s_arc_layer, layer_update_proc);
}

static void deinit() {
  accel_tap_service_unsubscribe();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}