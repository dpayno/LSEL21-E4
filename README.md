# LSEL21-E4
LSEL 2021 Equipo 4 - Sistema de seguridad para coches

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
10. En la transición ON / ON se envía la posición del coche. 
11. En la transición ON / ASSAULT se envía la ubicación y se inicializa un timer de 30 segundos. 
12. En la transición ASSAULT / ASSAULT se envía la ubicación y se inicializa un timer de 30 segundos. 
13. En las transicioes al estado OFF solo se produce el cambio de estado (no hay ninguna función de salida)
