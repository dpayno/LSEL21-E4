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



''' test antiguos: 

    ###############################################################
    ##                    TRANSITIONS TEST
    ###############################################################

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


    """TEST TRANSITION ON/ASSAULT (sin location request)"""
    def test_fsm_gps_checkTransitionOnAssaultIfIsActiveAndAssaultAndNoLocationRequest(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 0
        mi_fsm.flag_init_gps_record = 1

        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'ASSAULT')

        """TEST TRANSITION ON/ASAULT (con location request)"""
    def test_fsm_gps_checkTransitionOnAsaultIfIsActiveAndAsaultAndLocationRequest(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 1
        mi_fsm.flag_init_gps_record = 1

        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'ASSAULT')


        """TEST TRANSITION ON/OFF"""
    def test_fsm_gps_checkTransitionOnOffWhenNotActive(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 0
        mi_fsm.flag_find_car = 1
        mi_fsm.flag_init_gps_record = 1

        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'OFF') # Comprobamos que si se desactiva flag_init_gps_record por error, seguimos en assault


    """TEST TRANSITION ASAULT/OFF"""
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


    """TEST TRANSITION ASAULT/ASSAULT (SI ASK POSITION)"""
    def test_fsm_gps_checkTransitionAssaultAssaultIfActiveAndAskPosition(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ASSAULT'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 1
        mi_fsm.flag_init_gps_record = 0

        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'ASSAULT')

    """TEST TRANSITION ASAULT/ASSAULT (SI TIMEOUT)"""
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

    ###############################################################
    ##                    OUTPUT FUNCTION TEST
    ###############################################################


    """CHECK FUNCIÓN GPSFRAME() EN TRANSICIÓN ON/ON"""
    def test_fsm_gps_checkOutputFunctionSendGpsFrame(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 1
        mi_fsm.flag_init_gps_record = 0
        mi_fsm.send_gps_frame = MagicMock()

        mi_fsm.fire()
        mi_fsm.send_gps_frame.assert_called()


    """CHECK FUNCIÓN SEND_GPS_FRAME_AND_INIT_TIMER SI ES LLAMADA (MOCK)"""
    def test_fsm_gps_checkOutputFunctionSendGpsFrameAndInitTimer1(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 0
        mi_fsm.flag_init_gps_record = 1
        mi_fsm.send_gps_frame_and_init_timer = MagicMock()

        mi_fsm.fire()
        mi_fsm.send_gps_frame_and_init_timer.assert_called()

    """CHECK FUNCIÓN SEND_GPS_FRAME_AND_INIT_TIMER INICIALIZACIÓN TIMER"""
    def test_fsm_gps_checkOutputFunctionSendGpsFrameAndInitTimer2(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ON'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 0
        mi_fsm.flag_init_gps_record = 1
        mi_fsm.timeout_gps = 0

        mi_fsm.fire()
        self.assertNotEqual(mi_fsm.timeout_gps, 0)


    """CHECK NO LLAMADA A FUNCIÓN EN ESTADO ASSAULT"""
    def test_fsm_gps_checkOutputFunctionAssaultNoFunctionCalled(self):
        mi_fsm = FsmGps("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'ASSAULT'
        mi_fsm.flag_active = 1
        mi_fsm.flag_find_car = 0
        mi_fsm.flag_init_gps_record = 1
        mi_fsm.timeout_gps = 6000000000
        mi_fsm.send_gps_frame_and_init_timer = MagicMock()

        mi_fsm.fire()
        mi_fsm.send_gps_frame_and_init_timer.assert_not_called()

'''