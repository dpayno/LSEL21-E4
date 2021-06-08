# SIM868
En este documento se explicará el funcionamiento básico del SIM868.

## Alimentación

Siguiendo las especificaciones de diseño indicadas en "sim868_series_hardware_design_v1.07.pdf" (apartado 5.2 Reference Circuit of GSM Power Supply, página 25), el SIM868 necesitará tener una alimentación de 4.1V así como una corriente puntual de 2A para proporcionar un correcto funcionamiento del módulo GSM en todo momento. En caso de no obtener dichos valores el módulo se reiniciará. 

Por lo tanto, en caso de utilizar el SIM868 HAT, es recomendable no alimentar únicamente con los GPIO. Se recomienda alimentar por el conector USB la placa conectando un cargador que proporcione 5V y 2A. Como se puede ver en el datasheet del sim868 HAT "GSM-GPRS-GNSS-HAT-Schematic_.pdf", el módulo tiene una etapa de adaptación de la tensión a 4,1V. 

Nota: no siempre necesitará 2A, solo en situaciones concretas, como podría ser una situación de baja cobertura. 

## PWRKEY
Este pin del SIM868 permite encender/apagar el módulo. Se peude accionar manualmente mediante el botón (correcto para la etapa de desarrollo) pero también existe la opción de utilizar el pin PWRKEY para encenderlo y apagarlo desde el microcontrolador (mirar en el datasheet correspondiente al módulo comprado para conocer dicho pin, en el HAT tiene la etiqueta "P4" y se corresponde con el GPIO 7).

## PIN STATUS

El led "Net" indicará con sus parpadeos el estado de conexión con la red:

|        STATUS       |              NETLIGHT             |
|:-------------------:|:---------------------------------:|
|         Off         |            Powered off            |
|  64ms On/ 800ms Off |     Not registered the network    |
| 64ms On/ 3000ms Off |     Registered to the network     |
|  64ms On/ 300ms Off | GPRS communication is established |

## Comandos AT

Los comandos AT nos permiten comunicarnos con el SIM868. Estos comandos se envían por UART y tienen la siguiente estructura:

`AT+COMMAND\r\n`

Existe una gran variedad de comandos AT, ver documento "sim800_series_at_command_manual_v1.10.pdf" para los comandos AT relacionados con el GSM y "SIM868_GNSS_Application Note_V1.00.pdf" para ver los comandos relacionados con el GNSS. 

### Secuencia de comandos "AT" para realizar un test del funcionamiento del GPS

A continuación se indicarán los pasos a seguir para realizar una prueba del funcionamiento del módulo GPS. Para realizar el envío de estos comandos de forma cómoda se recomienda instalar y utilizar "minicom".
Para ello (en un entorno Linux):
1. sudo apt-get install minicom
2. sudo minicom -D /dev/ttyS0 (realizando la comunicación a través de los GPIO y no del USB)

Para explicar la secuencia se seguirá la siguiente estructura:
Comando -> respuesta -> explicación de que hace el comando.

Una vez tengamos comunicación con el SIM868 seguiremos los siguientes pasos:
1.  Encender el SIM868: puede realizarse mediante el botón PWRKEY o, en caso de tenerlo implementado, desde el microcontrollador.
2.  `AT` -> OK -> comando que nos permite saber si el SIM868 está activado
3.  `AT+CGNSPWR=1` -> OK; +CGNSPWR: 1 -> encender módulo GNS (se podría hacer `AT+CGPSPWR = 1` pero es más recomendable GNS).
4.  `AT+CGNSINF` -> +CGNSINF: <GNSS run status>,<Fix status>, <UTC date & Time>,<Latitude>,<Longitude>...; OK -> comando que solicita la información del GPS. Para ver el significado de cada campo mirar la documentación de los comandos AT del GNS. 

Llamando varias veces al comando 4 veremos el funcionamiento del GNS. Puede tardar un tiempo en recibir la señal. 
