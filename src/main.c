#include <pebble.h>

static Window * s_main_window;
static TextLayer * s_time_layer;
static TextLayer * s_date_layer;
static TextLayer * s_battery_layer;
static GFont s_time_font;
static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;

static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char buffer[] = "00:00";
	static char battery_percent[] = "+00%";
	
	if(clock_is_24h_style()) {
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

	text_layer_set_text(s_time_layer, buffer);	
	
	BatteryChargeState battery = battery_state_service_peek();
	if (battery.charge_percent < 100) {
		snprintf(battery_percent, sizeof("+00%"), "%i\%c%c", battery.charge_percent, '%', ' ');
		if(battery.is_charging) {
			battery_percent[3] = battery_percent[2];
			battery_percent[2] = battery_percent[1];
			battery_percent[1] = battery_percent[0];
			battery_percent[0] = '+';
		}
	} else {
    snprintf(battery_percent, sizeof("+00%"), "%s", "FULL");
	}
	text_layer_set_text(s_battery_layer,battery_percent);
}

static void update_date() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char weekday[] = "MON JAN 01";
	strftime(weekday, sizeof("MON JAN 01"), "%a %b %d", tick_time);
	text_layer_set_text(s_date_layer, weekday);
}

static void main_window_load(Window * window) {
	//Create layer for clock
	s_time_layer = text_layer_create(GRect(0, 118, 144, 34));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	
	//Create layer for date
	s_date_layer = text_layer_create(GRect(0, 142, 140, 18));
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorWhite);
	
	//Create layer for battery percentage
	s_battery_layer = text_layer_create(GRect(4,4,140,20));
	text_layer_set_background_color(s_battery_layer, GColorClear);
	text_layer_set_text_color(s_battery_layer, GColorWhite);
	
	//set fonts
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
	text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
	
	//Create bitmap
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLASH);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
		
	update_time();
	update_date();
		
	//add layers to window
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
}

static void main_window_unload(Window * window) {
	bitmap_layer_destroy(s_background_layer);
	fonts_unload_custom_font(s_time_font);
	bitmap_layer_destroy(s_background_layer);
	gbitmap_destroy(s_background_bitmap);
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_battery_layer);
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
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
} 
  
int main() {
  init();
  app_event_loop();
  deinit();
}