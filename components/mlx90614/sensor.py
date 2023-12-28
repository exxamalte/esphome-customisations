import esphome.codegen as cg
from esphome.components import i2c

from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS
)

DEPENDENCIES = ["i2c"]

mlx90614_ns = cg.esphome_ns.namespace("mlx90614")

MLX90614Component = mlx90614_ns.class_(
    "MLX90614Component", cg.PollingComponent, i2c.I2CDevice
)

CONF_AMBIENT_TEMPERATURE = "ambient_temperature"
CONF_OBJECT_TEMPERATURE = "object_temperature"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BME280Component),
            cv.Optional(CONF_AMBIENT_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OBJECT_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x5A))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if ambient_temperature_config := config.get(CONF_AMBIENT_TEMPERATURE):
        sens = await sensor.new_sensor(ambient_temperature_config)
        cg.add(var.set_ambient_temperature_sensor(sens))
    if object_temperature_config := config.get(CONF_OBJECT_TEMPERATURE):
        sens = await sensor.new_sensor(object_temperature_config)
        cg.add(var.set_object_temperature_sensor(sens))

    cg.add_library("adafruit/Adafruit BusIO", "1.9.6")
    cg.add_library("Adafruit MLX90614 Library", None)
