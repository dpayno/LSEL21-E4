# LSEL21-E4
LSEL 2021 Equipo 4 - Sistema de seguridad para coches

## Secuencia de comandos HTTP: POST Y GET

### Configuración inicial. 

1. AT+CPIN? -> Si la respuesta es distinta a READY -> AT+CPIN = xxxx (donde xxxx es el pin de la tarjeta SIM insertada). 
2. AT + SAPBR=3,1,"CONTYPE","GPRS" -> Indica que la conexión a realizar es del tipo GPRS
3. AT + SAPBR=3,1,"APN","apn_de_la_compañía" -> introducimos el APN correspondiente a la compañía. P.ej para Orange es "orangeworld"
4. AT + SAPBR=1,1 -> Activamos la conexión -> con AT + SAPBR=2,1 podemos ver la dirección IP asignada

NOTA: en algunos casos esta configuración es insuficiente y es necesario introducir el usuario y contraseña de la compañía:
5. AT + SAPBR=3,1,"USER","user_de_la_compañía" -> en el caso de Orange el usuario es "orange"
6. AT + SAPBR=3,1,"PWD","contraseña_de_la_compañía" -> en el caso de Orange el usuario es "orange"


### HTTP POST

Tras realizar la configuración anterior:

1. AT+HTTPINIT 
2. AT+HTTPPARA = "CID", 1 
3. AT+HTTPPARA = "URL", "ptsv2.com/t/9pvw5-1619460668/post" -> la URL es un ejemplo, en la dirección https://ptsv2.com/t/9pvw5-1619460668 debemos ver el post realizado
4. AT+DATA = Size_Bytes, max_latency -> aparecerá un download, ahí debemos enviar por uart los datos que queramos enviar

Tras recibir el OK:
5. AT+HTTPACTION = 1

NOTA: si tras el paso 4 no esperamos al OK, el comando AT que mandemos se añadirá a la trama a enviar

### HTTP GET

Tras realizar la configuración anterior:

1. AT+HTTPINIT 
2. AT+HTTPPARA = "CID", 1 
3. AT+HTTPPARA = "URL", "http://my-json-server.typicode.com/typicode/demo/posts" -> la URL es un ejemplo, debe descargar un JSON
4. AT+HTTPACTION = 0
5. AT+ HTTPREAD
