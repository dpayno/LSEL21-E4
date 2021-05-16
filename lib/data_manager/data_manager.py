import paho.mqtt.client as mqtt
import json
from fsm_gsm import FsmGsm
from fsm_gps import FsmGps

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
	""" Esta clase representa un data manager. Se encarga de enviar la
	información del estado de la alarma a los diferentes nodos y de recibir
	los datos de los sensores de los nodos, actualizando una trama de datos.

	:param client_name: MQTT client name
	:param mqtt_port: MQTT port
	:param mqtt_broker: MQTT broker IP address
	:param mqtt_publish_topic: MQTT publication topic
	:param mqtt_subscribe_topic: MQTT subscription topic
	:param fsm_gps: FSM GPS
	:param fsm_gsm: FSM GSM
	"""
	def __init__(self, client_name, mqtt_port, mqtt_broker, mqtt_publish_topic,
			mqtt_subscribe_topic, fsm_gps, fsm_gsm, sim868):
		self.mqtt_client = MqttClient(client_name, mqtt_port, mqtt_broker)
		self.mqtt_publish_topic = mqtt_publish_topic
		self.mqtt_subscribe_topic = mqtt_subscribe_topic
		self.fsm_gps = fsm_gps
		self.fsm_gsm = fsm_gsm
		self.flag_active = 0
		self.last_data = {}

	def _message_handler(self, client, userdata, message):
		topic = str(message.topic)
		data = str(message.payload.decode("utf-8"))
		try:
			data_dict = json.loads(data)
			if data_dict["macaddress"] not in self.last_data:
				self.last_data[data_dict["macaddress"]] = data_dict
			else:
				self.last_data[data_dict["mac_address"]].update(data_dict)
		except:
			print("Not a JSON!")

	def start(self):
		self.mqtt_client.connect()
		self.mqtt_client.subscribe(self.mqtt_subscribe_topic, _message_handler)
		self.mqtt_client.start()

	def get_data(self):
		return self.last_data

	def fire(self):
		if self.fsm_gsm.flag_active != self.flag_active:
			self.flag_active = self.fsm_gsm.flag_active
			alarm_status = {self.mqtt_publish_topic: self.flag_active}
			alarm_status_json = json.dumps(alarm_status)
			self.mqtt_client.client.publish(self.mqtt_publish_topic,
					alarm_status_json)
