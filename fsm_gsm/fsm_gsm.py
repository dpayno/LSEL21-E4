from transitions.extensions import GraphMachine as Machine
from datetime import datetime as dt
from gsm_driver import GsmDriver as gsm_driver
from fifo_driver import FifoDriver as fifo_driver

""" This class is the implementation of the GSM FSM behaviour.
"""
class FsmGsm(object):

	""" FSM states
	"""
	states = ['init', 'GSM_IDLE', 'GSM_CHECK', 'GSM_READ']

	request_list = ["GSM_ACTIVE_REQUEST", "GSM_DEACTIVE_REQUEST", "GSM_POSITION_REQUEST"]

	def __init__(self, name):

		self.name = name
		self.T_CHECK = 5
		self.flag_data_available = 0
		self.flag_active = 0
		self.flag_find_car = 0
		self.gsm_data = None

		""" Timeout GSM
		"""
		self.timeout_gsm = int(dt.now().timestamp()) + self.T_CHECK

		""" Initialize the state machine
		"""
		self.machine = Machine(model=self, states=FsmGsm.states, initial='init')

		""" Add transitions
		"""
		self.machine.add_transition(trigger='start', source='init', dest='GSM_IDLE')
		self.machine.add_transition('fire', 'GSM_IDLE', 'GSM_CHECK', conditions=['check_timeout'], after='update_timeout')
		self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_READ', conditions=['check_gsm_data_available'], after='gsm_read')
		self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_IDLE', conditions=['gsm_not_data_available_and_fifo_is_empty'])
		self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_IDLE', conditions=['gsm_not_data_available_and_fifo_is_not_empty'], after='read_fifo_and_gsm_send')
		self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['active_request'], after='set_alarm')
		self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['deactive_request'], after='reset_alarm')
		self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['pos_request'], after='find_car')
		self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['invalid_request'])

	def check_timeout(self):
		return int(dt.now().timestamp()) > self.timeout_gsm

	def check_gsm_data_available(self):
		return self.flag_data_available

	def gsm_not_data_available_and_fifo_is_empty(self):
		return (not self.flag_data_available) and (fifo_driver.fifo_is_empty())

	def gsm_not_data_available_and_fifo_is_not_empty(self):
		return (not self.flag_data_available) and (not fifo_driver.fifo_is_empty())

	def active_request(self):
		return (self.gsm_data == "GSM_ACTIVE_REQUEST")

	def deactive_request(self):
		return (self.gsm_data == "GSM_DEACTIVE_REQUEST")

	def pos_request(self):
		return (self.gsm_data == "GSM_POSITION_REQUEST")

	def invalid_request(self):
		return (self.gsm_data not in self.request_list)

	def update_timeout(self):
		self.timeout_gsm = self.timeout_gsm + self.T_CHECK

	def gsm_read(self):
		self.gsm_data = gsm_driver.gsm_read_data()
		print("Gsm read!")

	def read_fifo_and_gsm_send(self):
		data = fifo_driver.fifo_read()
		gsm_driver.gsm_send(data)
		print("Read Fifo and Gsm Send")

	def set_alarm(self):
		self.flag_active = 1
		print("Set alarm!")

	def reset_alarm(self):
		self.flag_active = 0
		print("Reset alarm!")

	def find_car(self):
		self.flag_find_car = 1
		print("Find car!")
