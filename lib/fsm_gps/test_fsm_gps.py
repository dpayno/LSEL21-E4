import unittest
from unittest.mock import Mock
from unittest.mock import MagicMock
from unittest.mock import patch
from fsm_gps import FsmGps
from datetime import datetime as dt

class TestStringMethods(unittest.TestCase):

    ###############################################################
    ##                    TRANSITIONS TEST
    ###############################################################

    """TEST INIT ESTATE"""
    def test_fsm_gps_initStateEqualToOff(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGps("mi_fsm", mi_sim868)
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'OFF')

    """TEST TRANSITION OFF/ON"""
    def test_fsm_gps_checkTransitionOffOnIfIsActive(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGps("mi_fsm", mi_sim868)
        mi_fsm.start()
        mi_fsm.flag_active = 1

        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'ON')
    
    """TEST TRANSITION ON/ON"""
    def test_fsm_gps_checkTransitionOnOnTimeoutActive(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGps("mi_fsm", mi_sim868)
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 1
        mi_fsm.timeout_and_active = Mock(return_value = True)
        mi_fsm.send_gps_frame_and_init_timer = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'ON')
        mi_fsm.send_gps_frame_and_init_timer.assert_called()

    """TEST TRANSITION ON/OFF"""
    def test_fsm_gps_checkTransitionOnOffIfAlarmIsNotActive(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGps("mi_fsm", mi_sim868)
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 0
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'OFF')

if __name__ == '__main__':
    unittest.main()
