
import unittest
from fsm_gps import FsmGps

class TestStringMethods(unittest.TestCase):

	"""TEST INIT ESTATE"""
	def test_fsm_gps_initStateEqualToOff(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		self.assertEqual(mi_fsm.state, 'OFF')


	"""TEST TRANSITION OFF/ON"""
	def test_fsm_gps_checkTransitionOffOnIfIsActive(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.flag_active = 1
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ON')
		

	"""TEST TRANSITION ON/ON"""
	def test_fsm_gps_checkTransitionOnOnIfIsActiveAndLocationRequestAndNoAsault(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 1
		mi_fsm.flag_init_gps_record = 0
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ON')



if __name__ == '__main__':
    unittest.main()