
import unittest
from fsm_gps import FsmGps

class TestStringMethods(unittest.TestCase):

	def test_init_stateEqualToOff(self):
		mi_fsm = FsmGps("mi_fsm")
		mi_fsm.start()
		self.assertEqual(mi_fsm.state, 'OFF')



if __name__ == '__main__':
    unittest.main()