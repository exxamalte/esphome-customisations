#include "esphome.h"
#include <Wire.h>
#include "Adafruit_VEML6075.h"

#define TAG_VEML6075 "veml6075"

class VEML6075Sensor : public PollingComponent {

  public:
    Adafruit_VEML6075 uv = Adafruit_VEML6075();

    Sensor *uva_sensor = new Sensor();
    Sensor *uvb_sensor = new Sensor();
    Sensor *uvi_sensor = new Sensor();

    VEML6075Sensor() : PollingComponent(15000) {}

    void setup() override {
      ESP_LOGCONFIG(TAG_VEML6075, "Setting up VEML6070Sensor...");
      uv.begin();
      LOG_SENSOR("  ", "UVA", this->uva_sensor);
      LOG_SENSOR("  ", "UVB", this->uvb_sensor);
      LOG_SENSOR("  ", "UVI", this->uvi_sensor);
    }

    void update() override {
      float uva = uv.readUVA();
      float uvb = uv.readUVB();
      float uvi = uv.readUVI();
      ESP_LOGD(TAG_VEML6075, "UVA measured: %.0f", uva);
      ESP_LOGD(TAG_VEML6075, "UVB measured: %.0f", uvb);
      ESP_LOGD(TAG_VEML6075, "UV Index measured: %.0f", uvi);

      uva_sensor->publish_state(uva);
      uvb_sensor->publish_state(uvb);
      uvi_sensor->publish_state(uvi);
    }
};
