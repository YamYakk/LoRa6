#pragma once

#include "driver/temperature_sensor.h"

// ——— TSENS helper for ESP32-S3 on-die temp ———

static temperature_sensor_handle_t tsens_handle = NULL;

inline void tsens_init() {
  if (tsens_handle == NULL) {
    temperature_sensor_config_t cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-40, 125);
    if (temperature_sensor_install(&cfg, &tsens_handle) == ESP_OK) {
      temperature_sensor_enable(tsens_handle);
    }
  }
}

inline float tsens_read() {
  if (tsens_handle == NULL) {
    return NAN;
  }
  float t = NAN;
  if (temperature_sensor_get_celsius(tsens_handle, &t) == ESP_OK) {
    return t;
  }
  return NAN;
}
