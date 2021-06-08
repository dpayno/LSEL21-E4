from lib.fsm_gps.fsm_gps import FsmGps
from lib.fsm_gsm.fsm_gsm import FsmGsm
from lib.data_manager.data_manager import DataManager
from lib.sim868_driver.sim868_driver import SIM868

CLIENT_NAME = "gateway_client"
MQTT_PORT = 8883
MQTT_BROKER = "192.168.0.110"
MQTT_PUBLISH_TOPIC = "alarm_status"
MQTT_SUSCRIBE_TOPIC = "sensor_data"

#HTTP_POST_TOKEN = "ptsv2.com/t/fn719-1620149096/post"
DEVICE_ID = '7owB9W6exKuJ0S4WMYs6ylqac5E4oeMUwPXo'
HTTP_POST_TOKEN = "http://playground.devicehive.com/api/rest/token/refresh"
HTTP_POST_NOTIFICATION = f"http://playground.devicehive.com/api/rest/device/{DEVICE_ID}/notification"
# HTTP_GET_URL = "postman-echo.com/get?active=1"
HTTP_GET_URL = f"http://playground.devicehive.com/api/rest/device/{DEVICE_ID}/command"
REFRESH_TOKEN = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjM4NjM3NjUyNDgzLCJ0IjowLCJ1Ijo4NTMwLCJuIjpbIjg0MzEiXSwiZHQiOlsiKiJdfX0.5RA3KAiwb99u1pFY7UUyWZhL3qqCFVz8C8uGw6ic9K8"



""" Main function. Creates and inits a Gps FSM
"""
def main():

    # Create SIM868
    mi_sim868 = SIM868(4)
     # Create FSMs
    fsm_gps = FsmGps("fsm_gps", mi_sim868)
    fsm_gsm = FsmGsm("fsm_gsm", mi_sim868, HTTP_POST_TOKEN,
            HTTP_POST_NOTIFICATION, REFRESH_TOKEN, HTTP_GET_URL, DEVICE_ID)
    # Create Data Manager
    data_manager = DataManager(fsm_gps, fsm_gsm, CLIENT_NAME, MQTT_PORT,
            MQTT_BROKER, MQTT_PUBLISH_TOPIC, MQTT_SUSCRIBE_TOPIC)

    # Init FSMs
    fsm_gps.start()
    fsm_gsm.start()
    # Init Data Manager
    data_manager.start()

    # Infinite loop
    while True:
        fsm_gps.fire()
        fsm_gsm.fire()
        data_manager.fire()

if __name__ == "__main__":
    main()

