# **DIoTCAM**

Es un proyecto para realizar un dispositivo IoT para la alimentación de mascotas, además de poder llevar un control de la temperatura, presión atmosférica y humedad en el ambiente en que esta se encuentre.

El dispositivo posee los siguientes componentes:

- ESP32
- Buzzer
- Sensor BME280
- 5 Neopixeles

## ¿Cómo funciona?

DIoTCAM funciona por medio de un microcontrolador ESP32 con conectividad WiFi y Bluetooth, que puede ser programado en diferentes lenguajes de programación como Arduino, para controlar los distintos sensores integrados y acoplados.

## ¿Qué podemos hacer?

Desplegar la información de los sensores en un dashboard MQTT a través de conexión WiFi, para visualizar las mediciones de temperatura, humedad y presión atmosférica en el ambiente en que se encuentra nuestra mascota gracias al sensor BME280. También puede acoplarse un sensor PIR y un servomotor para construir un dispositivo que sea también capaz de alimentar a nuestra mascota controlado desde el dashboard MQTT.

## Descripción de los pines
### BME 280: Temperatura, humedad y presión atmosférica - Comunicación I2C 0x77
| Dispositivo | GPIO |
|----------|----------|
| VCC | 3V3 |
| GND | Ground |
| SCL / SCK | IO22 |
| SDA / SDI | IO21 |

### Buzzer
| Dispositivo | GPIO |
|----------|----------|
| Buzzer | IO4 |

### Control Servomotor
| Dispositivo | GPIO |
|----------|----------|
| VCC | 5V |
| GND | Ground |
| Señal | IO13 |

### Control Sensor PIR
| Dispositivo | GPIO |
|----------|----------|
| VCC | 5V |
| GND | Ground |
| Señal | IO27 |

### NeoPixeles
| Dispositivo | GPIO |
|----------|----------|
| VCC | 5V |
| GND | Ground |
| Señal | IO14 |

## Licencia

<image src="/img/oshw_facts.jpg">
