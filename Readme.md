# Semáforo Vehicular y Peatonal con Máquina de Estados Finitos

## Descripción

Esta práctica tiene como propósito implementar el control de un semáforo vehicular y peatonal mediante una máquina de estados finitos (FSM) programada en Arduino UNO R4 WiFi.

El sistema cuenta con luces vehiculares de color rojo, amarillo y verde, luces peatonales de color rojo y verde, y un botón para solicitar el cruce peatonal. La temporización del sistema se realiza mediante `millis()`, sin utilizar `delay()`, permitiendo que el programa responda a eventos externos mientras mantiene el funcionamiento del semáforo.

## Objetivos

* Comprender el funcionamiento de una máquina de estados finitos (FSM).
* Implementar un sistema secuencial mediante diferentes estados.
* Controlar las luces de un semáforo vehicular y peatonal.
* Utilizar `millis()` para realizar una temporización no bloqueante.
* Implementar un botón para solicitar el cruce peatonal.
* Utilizar antirrebote por software para evitar activaciones falsas.
* Establecer reglas de prioridad para atender la solicitud peatonal de manera segura.

## Herramientas y material utilizado

* Arduino UNO R4 WiFi.
* Arduino IDE.
* Protoboard.
* LED rojo vehicular.
* LED amarillo vehicular.
* LED verde vehicular.
* LED rojo peatonal.
* LED verde peatonal.
* Resistencias limitadoras de corriente.
* Pulsador para la solicitud peatonal.
* Cables de conexión (jumpers).

## Diagrama

El diagrama muestra las conexiones utilizadas para implementar el semáforo vehicular y peatonal.

![Diagrama del circuito](diagrama/diagrama-sema.png)

![Diagrama del circuito Fisico](diagrama/semaforo-fisico.jpg)




## Código

El programa implementa la máquina de estados finitos encargada de controlar las luces vehiculares y peatonales, así como la solicitud de cruce mediante el botón.

La temporización se realiza mediante `millis()` para evitar bloquear la ejecución del programa.

[Ver código](codigo/semaforo.aia.ino)

## Reporte

El reporte contiene la explicación del funcionamiento del sistema, la metodología utilizada, el análisis de los resultados y las conclusiones obtenidas durante la práctica.

[Ver Reporte](reporte/Reporte_Semaforo.pdf)

## Resultados

Durante las pruebas, el semáforo presentó el comportamiento esperado de acuerdo con la máquina de estados finitos implementada. El ciclo vehicular de verde, amarillo y rojo se ejecutó con las duraciones programadas mediante una temporización no bloqueante.

El botón permitió realizar la solicitud de cruce peatonal durante los estados correspondientes. La solicitud fue atendida de manera segura cuando el semáforo vehicular llegó al estado rojo, permitiendo el cambio de las luces peatonales.

También se comprobó el funcionamiento del antirrebote por software, evitando activaciones falsas o múltiples debido a una sola pulsación del botón. Cuando no existía una solicitud peatonal, el ciclo del semáforo continuaba funcionando de manera automática.

Los resultados permitieron comprobar el funcionamiento tanto del circuito armado en protoboard como de la lógica de control implementada mediante la máquina de estados finitos.

## Video

El video muestra el funcionamiento del semáforo vehicular y peatonal, incluyendo la respuesta del sistema ante la solicitud de cruce mediante el botón.

[Ver video](https://youtube.com/shorts/mciZtEBKvcg?feature=share)

[Ver carpeta Video](video)

## Conclusiones

La práctica permitió aplicar el concepto de máquina de estados finitos a un sistema de control secuencial, utilizando diferentes estados para representar el comportamiento del semáforo vehicular y peatonal.

El uso de `millis()` permitió realizar la temporización sin bloquear la ejecución del programa, mientras que el botón permitió incorporar un evento externo al funcionamiento del sistema. También se comprendió la importancia del antirrebote y de establecer reglas de prioridad para atender la solicitud peatonal de forma segura.

En conjunto, la práctica permitió relacionar la programación de una máquina de estados con el control de componentes físicos, comprobando su funcionamiento mediante el circuito armado y las pruebas realizadas.
