import paho.mqtt.client as mqtt
import time
import atexit

CLIENT_NAME = "gateway_client_test_1"
MQTT_PORT = 8883
MQTT_BROKER = "192.168.0.18"
MQTT_SUSCRIBE_TOPIC = "alarm_status"

def message_handler(client, userdata, message):
	print("Hey")
	topic = str(message.topic)
	message = str(message.payload.decode("utf-8"))
	print(message)

class MqttClient:
	""" Esta clase representa un cliente MQTT.

	:param client_name: MQTT client name
	:param mqtt_port: MQTT port
	:param mqtt_broker: MQTT broker IP address
	"""
	def __init__(self, client_name, mqtt_port, mqtt_broker):
		self.client_name = client_name
		self.mqtt_port = mqtt_port
		self.mqtt_broker = mqtt_broker
		self.client = mqtt.Client(self.client_name)

	def connect(self):
		self.client.connect(self.mqtt_broker, self.mqtt_port)

	def subscribe(self, mqtt_topic, message_function):
		self.client.subscribe(mqtt_topic)
		self.client.on_message = message_function
	
	def start(self):
		self.client.loop_start()

	def stop(self):
		self.client.loop_stop(force=True)


class DataManager:

	def __init__(self, client_name, mqtt_port, mqtt_broker, mqtt_sus_topic):
		self.mqtt_client = MqttClient(client_name, mqtt_port, mqtt_broker)
		self.mqtt_sus_topic = mqtt_sus_topic

	def run(self):
		self.mqtt_client.connect()
		self.mqtt_client.subscribe(self.mqtt_sus_topic, message_handler)
		self.mqtt_client.start()


last_data = {}

#data_manager = DataManager(CLIENT_NAME, MQTT_PORT, MQTT_BROKER, MQTT_SUSCRIBE_TOPIC)
#data_manager.run()

def test_handler(client, userdata, message):
	topic = str(message.topic)
	message = str(message.payload.decode("utf-8"))
	print(message)

client = mqtt.Client(CLIENT_NAME)
client.connect(MQTT_BROKER, MQTT_PORT)
client.subscribe(MQTT_SUSCRIBE_TOPIC)
client.on_message = test_handler
client.loop_start()

while(1):
	time.sleep(3)