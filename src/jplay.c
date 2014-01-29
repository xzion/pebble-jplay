#include <pebble.h>
#include "jplay.h"
#include "messaging.h"

static Window *window;
static TextLayer *text_layer;
TextLayer *title_layer;
TextLayer *live_layer;
TextLayer *local_layer;





static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  window_set_background_color(window, GColorBlack);

  // Build the title layer
  title_layer = text_layer_create(GRect(0, 0, 144, 40));
  text_layer_set_font(title_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MYRIAD_16)));
  text_layer_set_text(title_layer, "Triple J\nNow Playing");
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(title_layer));

  // Build the live layer
  live_layer = text_layer_create(GRect(0, 42, 144, 60));
  text_layer_set_font(live_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MYRIAD_14)));
  text_layer_set_text(live_layer, "Live Stream\n...\n...");
  text_layer_set_overflow_mode(live_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(live_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(live_layer));

  // Build the local layer
  local_layer = text_layer_create(GRect(0, 104, 144, 60));
  text_layer_set_font(local_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MYRIAD_14)));
  text_layer_set_text(local_layer, "QLD Radio\n...\n...");
  text_layer_set_overflow_mode(live_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(local_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(local_layer));

  app_message_init();

}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  text_layer_destroy(title_layer);
  text_layer_destroy(live_layer);
  text_layer_destroy(local_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
