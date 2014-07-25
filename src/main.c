#include <pebble.h>
#include "pebble_macro.h"
#include "filters/filters.h"

Window *window;
TextLayer *text_date_layer;
TextLayer *text_time_layer;
Layer *line_layer;

bool mutex_acquire(volatile unsigned char* lock);
bool mutex_release(volatile unsigned char* lock);

uint32_t pedometer_count = 0;

void handle_pedometer_update() {
  static char buff[10];
  pedometer_count++;
  snprintf(buff, 10, "%lu", pedometer_count);
  text_layer_set_text(text_time_layer, buff);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";

  char *time_format;

  if (!tick_time) {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }

  // TODO: Only update the date when it's changed.
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  text_layer_set_text(text_date_layer, date_text);


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
  accel_data_service_unsubscribe();
}

void handle_init(void) {
  init(Window, window, (),
      stack_push,             (true),
      set_background_color,   (GColorBlack),
      set_fullscreen,         (true)
  );
  init(TextLayer, text_date_layer, GRect(8, 68, 144-8, 168-68),
      set_text_color,         (GColorWhite),
      set_background_color,   (GColorClear),
      set_font,               (fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21))
  )

  init(TextLayer, text_time_layer, GRect(7, 92, 144-7, 168-92),
      set_text_color,         (GColorWhite),
      set_background_color,   (GColorClear),
      set_font,               (fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49))
  )

  init(Layer, line_layer, GRect(8, 97, 139, 2),
      set_update_proc_m,      ((Layer *layer, GContext* ctx), {
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
      })
  )

  Layer *window_layer = window_get_root_layer(window);
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
  layer_add_child(window_layer, line_layer);

//  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);
  
  pedometer_data_service_subscribe_m((uint64_t timestamp, uint32_t intensity), {
    int32_t output = iir_5_35_filter(intensity > 0 ? 100 : 0);
    INFO("%lu %d", intensity, (int)output);
    
    if (output >= 40) {
      handle_pedometer_update();
    }
  });

  accel_data_service_subscribe(5, pedometer_data_service_listen);
 
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  
}

int main(void) {
  handle_init();

  app_event_loop();
  
  handle_deinit();
}
