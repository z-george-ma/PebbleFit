#ifndef __PEBBLE_MACROS_H__
#define __PEBBLE_MACROS_H__

#include "macro.h"

// Window
#define DSL_INIT_Window(ref, params) \
  ref = window_create()

#define DSL_TYPEPREFIX_Window window_

// Layer
#define DSL_INIT_Layer(ref, params) \
  ref = layer_create params

#define DSL_TYPEPREFIX_Layer layer_

// Text Layer
#define DSL_INIT_TextLayer(ref, params) \
  ref = text_layer_create params

#define DSL_TYPEPREFIX_TextLayer text_layer_

#define PEBBLE_LayerSetUpdateProcTemplate(callback, layer) \
  layer_set_update_proc(layer, callback)

#define layer_set_update_proc_m(layer, callback_params, body) \
  DSL_CALLBACK_OUTPUT(DSL_UNIQUE_NAME(LayerSetUpdateProc), void, callback_params, body, PEBBLE_LayerSetUpdateProcTemplate, (layer))

// Action Bar Layer 
#define PEBBLE_ActionBarLayerSetClickConfigProviderTemplate(callback, action_bar) \
  action_bar_layer_set_click_config_provider(action_bar, callback)
  
#define action_bar_layer_set_click_config_provider_m(action_bar, callback_params, body) \
  DSL_CALLBACK_OUTPUT(DSL_UNIQUE_NAME(ANY_UNIQUE_NAME), void, callback_params, body, PEBBLE_ActionBarLayerSetClickConfigProviderTemplate, (action_bar))

// Accelerometer Service
#define PEBBLE_AccelDataServiceSubscribeTemplate(callback, sampling_rate) \
  accel_data_service_subscribe(sampling_rate, callback)

#define accel_data_service_subscribe_m(sampling_rate, callback_params, body) \
  DSL_CALLBACK_OUTPUT(DSL_UNIQUE_NAME(AccelDataServiceSubscribe), void, callback_params, body, PEBBLE_AccelDataServiceSubscribeTemplate, (sampling_rate))
	
#define PEBBLE_AccelTapServiceSubscribeTemplate(callback, _) \
  accel_tap_service_subscribe(callback)

#define accel_tap_service_subscribe_m(callback_params, body) \
  DSL_CALLBACK_OUTPUT(DSL_UNIQUE_NAME(AccelTapServiceSubscribe), void, callback_params, body, PEBBLE_AccelTapServiceSubscribeTemplate, ())
	
// Tick Timer Service
#define PEBBLE_TickTimerServiceSubscribeTemplate(callback, units) \
  tick_timer_service_subscribe(units, callback)

#define tick_timer_service_subscribe_m(units, callback_params, body) \
  DSL_CALLBACK_OUTPUT(DSL_UNIQUE_NAME(TickTimerServiceSubscribe), void, callback_params, body, PEBBLE_TickTimerServiceSubscribeTemplate, (units))
	
// Logging
#define VERBOSE(...) PEBBLE_LOG(__FILE__, __LINE__, APP_LOG_LEVEL_DEBUG_VERBOSE, __VA_ARGS__)
#define DEBUG(...) PEBBLE_LOG(__FILE__, __LINE__, APP_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define INFO(...) PEBBLE_LOG(__FILE__, __LINE__, APP_LOG_LEVEL_INFO, __VA_ARGS__)
#define WARN(...) PEBBLE_LOG(__FILE__, __LINE__, APP_LOG_LEVEL_WARNING, __VA_ARGS__)
#define ERROR(...) PEBBLE_LOG(__FILE__, __LINE__, APP_LOG_LEVEL_ERROR, __VA_ARGS__)

#define PEBBLE_LOG(file, line, level, ...) app_log(level, file, line, __VA_ARGS__)

// Pedometer Service
#define PEBBLE_PedometerDataServiceSubscribeTemplate(callback, _) \
  pedometer_data_service_subscribe(callback)

#define pedometer_data_service_subscribe_m(callback_params, body) \
  DSL_CALLBACK_OUTPUT(DSL_UNIQUE_NAME(PedometerDataServiceSubscribe), void, callback_params, body, PEBBLE_PedometerDataServiceSubscribeTemplate, ())

#endif
