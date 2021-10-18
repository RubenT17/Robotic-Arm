# Bazo Robótico con interfaz Serial

Este proyecto va destinado a ser un código para recrear el movimiento de un brazo robótico con tres grados de libertad y accionado con servomotores.


### Pre-requisitos 📋

Los requisitos para ejecutar este proyecto son:
- IDE de Arduino.
- Un microcontrolador AVR Atmel.


## Despliegue 📦

Cada grado de libertad se moverá con su joystick.

Instrucciones de Serial:
- "info\n": Para ver la información del brazo robótico.
- "reposo\n": Para poner/quitar el brazo robótico en modo reposo.

Conexión de pines:

    DIGITAL:
    - Pin 0: Vacío.
    - Pin 1: Vacío.
    - Pin 2: Pulsador Reposo.
    - Pin 3 (PWM): PWM Brazo
    - Pin 4: Vacío.
    - Pin 5 (PWM): PWM Antebrazo
    - Pin 6 (PWM): PWM Pinza
    - Pin 7: Vacío.                         
    - Pin 8: Vacío.                      
    - Pin 9 (PWM): PWM Giro            
    - Pin 10 (PWM):                    
    - Pin 11 (PWM): Cerrar Pinza
    - Pin 12: Abrir Pinza
    - Pin 13: Vacío.
    
    ANALÓGICO:
    - A0: Eje X.1
    - A1: Eje Y.1
    - A2: Eje Y.2
    - A3: Vacío.
    - A4: Vacío.
    - A5: Vacío.

## Construido con 🛠️

* [Arduino IDE](https://www.arduino.cc/en/software) - IDE Arduino.


## Contribuyendo 🖇️

Por favor lee el [CONTRIBUTING.md](https://github.com/RubenT17/Prueba/blob/main/CONTRIBUTING.md) para detalles de nuestro código de conducta, y el proceso para enviarnos pull requests.


## Autores ✒️

* **Rubén Torres Bermúdez** - *Proyecto* - [RubenT17](https://github.com/RubenT17)


## Licencia 📄

Este proyecto está bajo la Licencia GNU - mira el archivo [LICENSE](https://github.com/RubenT17/Prueba/blob/main/LICENSE) para más detalles.


## Expresiones de Gratitud 🎁
* Comenta a otros sobre este proyecto 📢
* Da las gracias públicamente 🤓.
* etc.
