# CoBot Gripper — ESP32-C3 SuperMini

Control de gripper electronico para brazo colaborativo, basado en ESP32-C3 SuperMini.

## Hardware

| Componente | GPIO |
|---|---|
| Motor IN1 | GPIO2 |
| Motor IN2 | GPIO3 |
| DI OPEN (optoacoplador) | GPIO4 |
| DI CLOSE (optoacoplador) | GPIO5 |
| LED RGB NeoPixel (x12) | GPIO10 |

## Dependencias

- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)

## Comportamiento

- **Al energizar:** abre completamente durante `TIEMPO_ABRIR` ms sin importar la posición previa
- **Pulso OPEN:** gira hasta abrir completamente (`TIEMPO_ABRIR` ms)
- **Pulso CLOSE:** gira hasta cerrar completamente (`TIEMPO_CERRAR` ms)
- Los botones funcionan por flanco — basta un pulso corto para completar el ciclo

## LEDs

| Color | Estado |
|---|---|
| Morado girando | Abriendo |
| Azul cyan girando | Cerrando |
| Verde neón lento | Abierto o cerrado (listo) |

## Ajuste de tiempos

```cpp
#define TIEMPO_ABRIR  3400  // ms
#define TIEMPO_CERRAR 3400  // ms
```

## Programa Cobot (Elite Robot)

Ver `cobot_programa.jbi` — programa de pruebas de repetibilidad.

| Salida | Función |
|---|---|
| OT#(48) | Abrir gripper |
| OT#(49) | Cerrar gripper |

Los timers del cobot están configurados con margen de seguridad:
- Apertura: 3.5s (gripper abre en 3.4s)
- Cierre: 5.0s (gripper cierra en 3.4s)
