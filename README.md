# LSEL21-E4
LSEL 2021 Equipo 4 - Sistema de seguridad para coches


## Configuración de la tarjeta SIM
Es necesario crear un fichero llamado "config.json" cuyos contenido sea:


```
{
      "PIN" : "xxxx",
      "APN" : "apn_of_the_company"
}
```

## Procedimiento de notificación a Device Hive

### Solicitud de token a partir de refresh token

```
POST /api/rest/token/refresh HTTP/1.1
Host: playground.devicehive.com
{"refreshToken": "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjM4NjM3NjUyNDgzLCJ0IjowLCJ1Ijo4NTMwLCJuIjpbIjg0MzEiXSwiZHQiOlsiKiJdfX0.5RA3KAiwb99u1pFY7UUyWZhL3qqCFVz8C8uGw6ic123"}

```
#### Respuesta con el token

```
{"accessToken":"eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjIyOTIxMTg3MjE4LCJ0IjoxLCJ1Ijo4NTMwLCJuIjpbIjg0MzEiXSwiZHQiOlsiKiJdfX0.xxvrdBUadlbUgRKTxhjdVdpGYoI_lDe_y6EVkYwcuTs"}

```

### Envío de notificaciones

```
POST /api/rest/device/7owB9W6exKuJ0S4WMYs6ylqac5E4oeMUwPXo/notification HTTP/1.1
Host: playground.devicehive.com
Authorization: Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjIyOTIxMTg3MjE4LCJ0IjoxLCJ1Ijo4NTMwLCJuIjpbIjg0MzEiXSwiZHQiOlsiKiJdfX0.xxvrdBUadlbUgRKTxhjdVdpGYoI_lDe_y6EVkYwcuTs

{"notification": "7owB9W6exKuJ0S4WMYs6ylqac5E4oeMUwPXo", "parameters": {"param-test": 1}}
