
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
	def test_fsm_gps_checkTransitionOnOnIfIsActiveAndLocationRequestAndNoAssault(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ON'
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 1
		mi_fsm.flag_init_gps_record = 0
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ON')


	'''TEST TRANSITION ON/ASSAULT (sin location request)'''
	def test_fsm_gps_checkTransitionOnAssaultIfIsActiveAndAssaultAndNoLocationRequest(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ON'
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 0
		mi_fsm.flag_init_gps_record = 1
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ASSAULT')

		'''TEST TRANSITION ON/ASAULT (con location request)'''
	def test_fsm_gps_checkTransitionOnAsaultIfIsActiveAndAsaultAndLocationRequest(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ON'
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 1
		mi_fsm.flag_init_gps_record = 1
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ASSAULT')


		'''TEST TRANSITION ON/OFF'''
	def test_fsm_gps_checkTransitionOnOffWhenNotActive(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ON'
		mi_fsm.flag_active = 0
		mi_fsm.flag_find_car = 1
		mi_fsm.flag_init_gps_record = 1
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'OFF') # Comprobamos que si se desactiva flag_init_gps_record por error, seguimos en assault


	'''TEST TRANSITION ASAULT/OFF'''
	def test_fsm_gps_checkTransitionAssaultOffOnlyWhenNotActive(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ASSAULT'
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 0
		mi_fsm.flag_init_gps_record = 0
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ASSAULT') # Comprobamos que si se desactiva flag_init_gps_record por error, seguimos en assault

		mi_fsm.state = 'ASSAULT'
		mi_fsm.flag_active = 0
		mi_fsm.flag_find_car = 1
		mi_fsm.flag_init_gps_record = 0
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'OFF') # Comprobamos que si se desactiva active pasa a OFF (aunque siga activo el flag de gps_record)

		mi_fsm.state = 'ASSAULT'
		mi_fsm.flag_active = 0
		mi_fsm.flag_find_car = 0
		mi_fsm.flag_init_gps_record = 1
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'OFF') # Comprobamos que si se desactiva active pasa a OFF (aunque siga activo el flag de gps_record)

	
	'''TEST TRANSITION ASAULT/ASSAULT (SI ASK POSITION)'''
	def test_fsm_gps_checkTransitionAssaultAssaultIfActiveAndAskPosition(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ASSAULT'
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 1
		mi_fsm.flag_init_gps_record = 0
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ASSAULT')

	'''TEST TRANSITION ASAULT/ASSAULT (SI TIMEOUT)'''
	def test_fsm_gps_checkTransitionAssaultAssaultIfActiveAndTimeout(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		mi_fsm.state = 'ASSAULT'
		mi_fsm.timeout_gps = 0
		mi_fsm.flag_active = 1
		mi_fsm.flag_find_car = 0
		mi_fsm.flag_init_gps_record = 0
		mi_fsm.fire()
		self.assertEqual(mi_fsm.state, 'ASSAULT')


if __name__ == '__main__':
    unittest.main()