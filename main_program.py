from lib.fsm_gps.fsm_gps import FsmGps
from lib.sim868_driver.sim868_driver import SIM868

CLIENT_NAME = "gateway_client_test"
MQTT_PORT = 8883
MQTT_BROKER = "192.168.0.110"
MQTT_PUBLISH_TOPIC = "alarm_status"
MQTT_SUSCRIBE_TOPIC = "sensor_data"

""" Main function. Creates and inits a Gps FSM
"""
def main():

    # Start SIM868
    mi_sim868 = SIM868(4)
     # Create FSMs
    fsm_gps = FsmGps("fsm_gps", mi_sim868)
    fsm_fsm = FsmGps("fsm_gsm")
    # Create Data Manager
    data_manager = DataManager(fsm_gps, fsm_gsm, CLIENT_NAME, MQTT_PORT,
    		MQTT_BROKER, MQTT_SUSCRIBE_TOPIC)

    # Init state
    fsm_gps.start()
    fsm_gsm.start()

    while True:
        fsm_gps.fire()
        fsm_gsm.fire()
        data_manager.fire()

if __name__ == "__main__":
    main()
