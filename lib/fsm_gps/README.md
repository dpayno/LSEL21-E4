# LSEL21-E4
LSEL 2021 Equipo 4 - Sistema de seguridad para coches


## CONEXIÓN  RASPBERRY/SIM868

1. Conectar alimentaciones de ambas placas, conectar tierras (recomendado alimentar el SIM868 de forma externa a la Raspberry para evitar quemar la placa)
2. Conectar PIN4 de la Raspberry al pin POWKEY del SIM868
3. Conectar pines UART
4. Conectar 3,3V de la Raspberry a VDD del SIM868

NOTA: no es necesario conectar PWREN

## CONFIGURACIÓN SIM868

Para un correcto funcionamiento del código es necesario activar la opción echo, para ello debemos introducir los siguientes comandos:

1. ATE1
2. AT&W

## FSM_GPS

### Requisitos

1. Estar hecho con FSM
2. Estado inicial 'OFF' -> done

Comprobar transiciones

3. Comprobar transición OFF / ON si está activo -> done 
4. Comprobar transición ON / ON si está activo y solicitan la ubicación del coche y no se ha producido un asalto.  -> done 
5. Comprobar transición ON / ASSAULT si está activo y se produce un asalto del coche. -> done 
6. Comprobar transición ASSAULT / ASSAULT si está activo y además se desborda el timer o el usuario solicita la ubicación. -> done
7. Comprobar que solo se puede salir del estado ASSAULT si se desactiva la alarma. -> done
8. Comprobar que siempre que se desactive la alarma se vuelve al estado OFF en la siguiente transición. -> done

Comprobar funciones de salida

9.  En la transición OFF / ON solo se produce el cambio de estado (no hay ninguna función de salida) 
10. En la transición ON / ON se envía la posición del coche. -> done
11. En la transición ON / ASSAULT se envía la ubicación y se inicializa un timer. -> done

12. En la transición ASSAULT / ASSAULT se envía la ubicación y se inicializa un timer. -> done
13. En la transición ASSAULT / ASSAULT si no se desborda el timer ni se solicita la ubicación del coche no se ejecuta la función. -> done

14. En las transicioes al estado OFF solo se produce el cambio de estado (no hay ninguna función de salida)

Comprobar entrada de datos 

15. Comprobar lectura de los flags de entrada ??????? (uso de mock)
