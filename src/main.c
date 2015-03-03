#include <pebble.h>

static Window * s_main_window;
static TextLayer * s_time_layer;
static GFont s_time_font;
static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;

static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char buffer[] = "00:00";
	
	if(clock_is_24h_style()) {
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

	text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window * window) {
	//Create layer for clock with space in the middle
	s_time_layer = text_layer_create(GRect(0, 134, 144, 34));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);
	
	//set font
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
	
	//Create bitmap
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DITHERED);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	
	update_time();
		
	//add layers to window
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window * window) {
	fonts_unload_custom_font(s_time_font);
	bitmap_layer_destroy(s_background_layer);
	gbitmap_destroy(s_background_bitmap);
}

static void tick_handler(struct tm *tick_time, TimeUnits unitsChanged) {
	update_time();
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
  .load = main_window_load,
  .unload = main_window_unload
  });
  window_stack_push(s_main_window,true);
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
} 
  
int main() {
  init();
  app_event_loop();
  deinit();
}