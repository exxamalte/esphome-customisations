#include "esphome.h"
#include <Wire.h>
#include "Adafruit_VL6180X.h"

#define TAG "vl6180x"

class VL6180XSensor : public PollingComponent {

  public:
    Adafruit_VL6180X vl;
    Sensor *luminance_sensor = new Sensor();
    Sensor *distance_sensor = new Sensor();

    VL6180XSensor() : PollingComponent(15000) { }

    void setup() override {
      ESP_LOGCONFIG(TAG, "Setting up VL6180XSensor...");
      vl.begin();
      LOG_SENSOR("  ", "Luminance", this->luminance_sensor);
      LOG_SENSOR("  ", "Distance", this->distance_sensor);
    }

    void update() override {
      float luminance = vl.readLux(VL6180X_ALS_GAIN_5);
      ESP_LOGD(TAG, "Luminance measured: %f", luminance);
      luminance_sensor->publish_state(luminance);

      uint8_t range = vl.readRange();
      uint8_t status = vl.readRangeStatus();

      if (status == VL6180X_ERROR_NONE) {
        ESP_LOGD(TAG, "Range measured: %i", range);
        distance_sensor->publish_state(range);
      }

      // Log any errors.
      if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
        ESP_LOGE(TAG, "System error");
      }
      else if (status == VL6180X_ERROR_ECEFAIL) {
        ESP_LOGW(TAG, "ECE failure");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_NOCONVERGE) {
        ESP_LOGW(TAG, "No convergence");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_RANGEIGNORE) {
        ESP_LOGW(TAG, "Ignoring range");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_SNR) {
        ESP_LOGW(TAG, "Signal/Noise error");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_RAWUFLOW) {
        ESP_LOGW(TAG, "Raw reading underflow");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_RAWOFLOW) {
        ESP_LOGW(TAG, "Raw reading overflow");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_RANGEUFLOW) {
        ESP_LOGW(TAG, "Range reading underflow");
        distance_sensor->publish_state(NAN);
      }
      else if (status == VL6180X_ERROR_RANGEOFLOW) {
        ESP_LOGW(TAG, "Range reading overflow");
        distance_sensor->publish_state(NAN);
      }
    }
};
