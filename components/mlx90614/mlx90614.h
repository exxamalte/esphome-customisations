#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include "esphome.h"
#include <Wire.h>
#include "Adafruit_MLX90614.h"

namespace esphome {
namespace mlx90614 {

class MLX90614Component : public PollingComponent, public i2c::I2CDevice {

  public:
    void setup() override;
    void update() override;

    void set_ambient_temperature_sensor(sensor::Sensor *temperature_sensor) { ambient_temperature_sensor_ = temperature_sensor; }
    void set_object_temperature_sensor(sensor::Sensor *temperature_sensor) { object_temperature_sensor_ = temperature_sensor; }

  protected:
    sensor::Sensor *ambient_temperature_sensor_{nullptr};
    sensor::Sensor *object_temperature_sensor_{nullptr};

    Adafruit_MLX90614 mlx;

};

}  // namespace mlx90614
}  // namespace esphome
