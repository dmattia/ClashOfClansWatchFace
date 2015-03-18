#include <pebble.h>

static Window * s_main_window;
static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;

static void main_window_load(Window * window) {
	//Create bitmap
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLASH);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
		
	//add layers to window
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
}

static void main_window_unload(Window * window) {
	bitmap_layer_destroy(s_background_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
  .load = main_window_load,
  .unload = main_window_unload
  });
  window_stack_push(s_main_window,true);
	//tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
} 
  
int main() {
  init();
  app_event_loop();
  deinit();
}