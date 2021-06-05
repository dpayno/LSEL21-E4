import unittest
from unittest.mock import Mock
from unittest.mock import MagicMock
from unittest.mock import patch
from fsm_gsm import FsmGsm
from datetime import datetime as dt


class TestStringMethods(unittest.TestCase):

    ###############################################################
    ##                    TRANSITIONS TEST
    ###############################################################
    """TEST INIT ESTATE"""
    def test_fsm_gsm_initStateEqualToGsmIdle(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')

    def test_fsm_gsm_timeoutUpdatesWhenInitFsm(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        self.assertTrue(mi_fsm.timeout_gsm > int(dt.now().timestamp()))

    def test_fsm_gsm_checkTimeoutReturnsTrueIfNowGreaterThanTimeout(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.timeout_gsm = 0
        self.assertTrue(mi_fsm.check_timeout())
        mi_fsm.timeout_gsm = int(dt.now().timestamp()) + 10
        self.assertFalse(mi_fsm.check_timeout())

    def test_fsm_gsm_checkTransitionFromIdle2CheckWhenTimeout(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.timeout_gsm = 0
        mi_fsm.gsm_get_and_update_timeout = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_CHECK')
        mi_fsm.gsm_get_and_update_timeout.assert_called()

    def test_fsm_gsm_checkStaysIdleWhenNotTimeout(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.check_timeout = Mock(return_value = False)
        mi_fsm.gsm_get_and_update_timeout = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.gsm_get_and_update_timeout.assert_not_called()

    def test_fsm_gsm_GsmNewDataAvailableReturnsTrueWhenDataAvailable(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.flag_data_available = 1
        self.assertTrue(mi_fsm.gsm_new_data_available())
        mi_fsm.flag_data_available = 0
        self.assertFalse(mi_fsm.gsm_new_data_available())
    
    def test_fsm_gsm_GsmNotNewDataAvailableReturnsTrueWhenDataNotAvailable(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.flag_data_available = 1
        self.assertFalse(mi_fsm.gsm_not_new_data_available())
        mi_fsm.flag_data_available = 0
        self.assertTrue(mi_fsm.gsm_not_new_data_available())

    def test_fsm_gsm_checkTransitionFromGsmCheckToGsmReadAndCallResetFlagNewDataAvailable(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.state = 'GSM_CHECK'
        mi_fsm.gsm_new_data_available = Mock(return_value = True)
        mi_fsm.reset_flag_new_data_available = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.reset_flag_new_data_available.assert_called()
        
    def test_fsm_gsm_checkTransitionFromGsmReadToGsmIdleWhenActiveRequestAndSetAlarmCalled(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        mi_fsm.active_request = Mock(return_value = True)
        mi_fsm.set_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.set_alarm.assert_called()
        
    def test_fsm_gsm_checkTransitionFromGsmReadToGsmIdleWhenDeactiveRequestAndResetAlarmCalled(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        mi_fsm.deactive_request = Mock(return_value = True)
        mi_fsm.reset_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.reset_alarm.assert_called()
        
    def test_fsm_gsm_checkTransitionFromGsmReadToGsmIdleWhenInvalidDataAndNotActiveModification(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post", "url_get")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        mi_fsm.invalid_request = Mock(return_value = True)
        mi_fsm.deactive_request = Mock(return_value = False) # tiene que ser un dato erroneo
        mi_fsm.reset_alarm = MagicMock()
        mi_fsm.set_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.reset_alarm.assert_not_called()
        mi_fsm.set_alarm.assert_not_called()
        

if __name__ == '__main__':
    unittest.main()
