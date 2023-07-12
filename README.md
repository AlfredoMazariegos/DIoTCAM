# **DIoTCAM**

Es un proyecto para realizar un dispositivo IoT para la alimentación de mascotas por via remota, además de poder llevar un control de la temperatura y humedad en el ambiente en que esta se encuentre.

El dispositivo posee los siguientes componentes:

- ESP32
- Buzzer
- Sensor BME280
- 5 Neopixeles
- Conexión para integrar un servomotor y un sensor infrarrojo.

## ¿Cómo funciona?

DIoTCAM funciona por medio de un microcontrolador ESP32 con conectividad WiFi y Bluetooth, que puede ser programado en diferentes lenguajes de programación como Arduino, para controlar los distintos sensores integrados y acoplados de forma remota.

## ¿Qué podemos hacer?

Desplegar la información del sensor en un dashboard MQTT a través de conexión WiFi, para visualizar las mediciones de temperatura y humedad en el ambiente en que se encuentra nuestra mascota. También puede acoplarse un sensor PIR y un servomotor para construir un dispositivo que sea también capaz de alimentar a nuestra mascota controlado por medio de un bot de Telegram como forma de activación remota.

## Descripción de los pines
### BME 280: Temperatura, humedad y presión atmosférica - Comunicación I2C 0x77 (En revisión)
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

Diseñado, Ensamblado y Programado por Alfredo José Mazariegos
Guatemala 2023
