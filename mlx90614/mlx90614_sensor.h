#include "esphome.h"
#include <Wire.h>
#include "Adafruit_MLX90614.h"

#define TAG "mlx90614"

class MLX90614Sensor : public PollingComponent {

  public:
    Adafruit_MLX90614 mlx;

    Sensor *ambient_temperature_sensor = new Sensor();
    Sensor *object_temperature_sensor = new Sensor();

    MLX90614Sensor() : PollingComponent(15000) { }

    void setup() override {
      ESP_LOGCONFIG(TAG, "Setting up MLX90614Sensor...");
      mlx.begin();
      LOG_SENSOR("  ", "Ambient temperature", this->ambient_temperature_sensor);
      LOG_SENSOR("  ", "Object temperature", this->object_temperature_sensor);
    }

    void update() override {
      // Ambient temperature range: -40°C ... +125˚C
      double ambient_temperature = mlx.readAmbientTempC();
      if ((-40.0 <= ambient_temperature) && (ambient_temperature <=125.0)) {
        ESP_LOGD(TAG, "Ambient temperature measured: %f", ambient_temperature);
        ambient_temperature_sensor->publish_state(ambient_temperature);
      } else {
        ESP_LOGW(TAG, "Ambient temperature out of range: %f", ambient_temperature);
      }

      // Object temperature range: -70°C ... +380˚C
      double object_temperature = mlx.readObjectTempC();
      if ((-70.0 <= object_temperature) && (object_temperature <=380.0)) {
        ESP_LOGD(TAG, "Object temperature measured: %f", object_temperature);
        object_temperature_sensor->publish_state(object_temperature);
      } else {
        ESP_LOGW(TAG, "Object temperature out of range: %f", object_temperature);
      }
    }
};
