# LSEL21-E4
LSEL 2021 Equipo 4 - Sistema de seguridad para coches

## FSM_GPS

### Requisitos

1. Estar hecho con FSM
2. Estado inicial 'OFF' -> done

Comprobar transiciones

3. Comprobar transición OFF / ON si está activo
4. Comprobar transición ON / ON si está activo y solicitan la ubicación del coche y no se ha producido un asalto.
5. Comprobar transición ON / ASAULT si está activo y se produce un asalto del coche.
6. Comprobar transición ASAULT / ASAULT si está activo y además se desborda el timer o el usuario solicita la ubicación.
7. Comprobar que solo se puede salir del estado ASAULT si se desactiva la alarma.
8. Comprobar que siempre que se desactive la alarma se vuelve al estado OFF en la siguiente transición.

Comprobar funciones de salida

9.  En la transición OFF / ON solo se produce el cambio de estado (no hay ninguna función de salida)
10. En la transición ON / ON se envía la posición del coche. 
11. En la transición ON / ASAULT se envía la ubicación y se inicializa un timer de 30 segundos. 
12. En la transición ASAULT / ASAULT se envía la ubicación y se inicializa un timer de 30 segundos. 
13. En las transicioes al estado OFF solo se produce el cambio de estado (no hay ninguna función de salida)
