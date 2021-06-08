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
                
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')

    def test_fsm_gsm_timeoutUpdatesWhenInitFsm(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        self.assertTrue(mi_fsm.timeout_gsm > int(dt.now().timestamp()))

    def test_fsm_gsm_checkTimeoutReturnsTrueIfNowGreaterThanTimeout(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        mi_fsm.timeout_gsm = 0
        self.assertTrue(mi_fsm.check_timeout())
        mi_fsm.timeout_gsm = int(dt.now().timestamp()) + 10
        self.assertFalse(mi_fsm.check_timeout())

    def test_fsm_gsm_checkTransitionFromIdle2CheckWhenTimeout(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.timeout_gsm = 0
        mi_fsm.gsm_get_and_update_timeout = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_CHECK')
        mi_fsm.gsm_get_and_update_timeout.assert_called()

    def test_fsm_gsm_checkStaysIdleWhenNotTimeout(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.check_timeout = Mock(return_value = False)
        mi_fsm.gsm_get_and_update_timeout = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.gsm_get_and_update_timeout.assert_not_called()

    def test_fsm_gsm_GsmNewDataAvailableReturnsTrueWhenDataAvailable(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        mi_fsm.flag_data_available = 1
        self.assertTrue(mi_fsm.gsm_new_data_available())
        mi_fsm.flag_data_available = 0
        self.assertFalse(mi_fsm.gsm_new_data_available())
    
    def test_fsm_gsm_GsmNotNewDataAvailableReturnsTrueWhenDataNotAvailable(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        mi_fsm.flag_active = 1
        mi_fsm.flag_data_available = 1
        self.assertFalse(mi_fsm.gsm_not_new_data_available_and_active())
        mi_fsm.flag_data_available = 0
        self.assertTrue(mi_fsm.gsm_not_new_data_available_and_active())

    def test_fsm_gsm_checkTransitionFromGsmCheckToGsmReadAndCallResetFlagNewDataAvailable(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        mi_fsm.state = 'GSM_CHECK'
        mi_fsm.gsm_new_data_available = Mock(return_value = True)
        mi_fsm.reset_flag_new_data_available = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.reset_flag_new_data_available.assert_called()
        
    def test_fsm_gsm_checkTransitionFromGsmReadToGsmIdleWhenActiveRequestAndSetAlarmCalled(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        mi_fsm.active_request = Mock(return_value = True)
        mi_fsm.set_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.set_alarm.assert_called()
        
    def test_fsm_gsm_checkTransitionFromGsmReadToGsmIdleWhenDeactiveRequestAndResetAlarmCalled(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        mi_fsm.deactive_request = Mock(return_value = True)
        mi_fsm.reset_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.reset_alarm.assert_called()
        
    def test_fsm_gsm_checkTransitionFromGsmReadToGsmIdleWhenInvalidDataAndNotActiveModification(self):
        mi_sim868 = Mock(return_value = 1)
        mi_fsm = FsmGsm("mi_fsm", mi_sim868, "url_post_token", "url_post_notification", "refresh_token", "url_get", "device_id")
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

''' TEST ANTIGUOS

    ###############################################################
    ##                    TRANSITIONS TEST
    ###############################################################
    """TEST INIT ESTATE"""
    def test_fsm_gsm_initStateEqualToGsmIdle(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')

    def test_fsm_gsm_timeoutUpdatesWhenInitFsm(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        self.assertTrue(mi_fsm.timeout_gsm > int(dt.now().timestamp()))

    def test_fsm_gsm_checkTimeoutReturnsTrueIfNowGreaterThanTimeout(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.timeout_gsm = 0
        self.assertTrue(mi_fsm.check_timeout())
        mi_fsm.timeout_gsm = int(dt.now().timestamp()) + 10
        self.assertFalse(mi_fsm.check_timeout())

    def test_fsm_gsm_checkTransitionFromIdle2CheckWhenTimeout(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.check_timeout = Mock(return_value = True)
        mi_fsm.timeout_gsm = 10
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_CHECK')
        self.assertEqual(mi_fsm.timeout_gsm, 15)


    def test_fsm_gsm_checkStaysIdleWhenNotTimeout(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.check_timeout = Mock(return_value = False)
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')

    def test_fsm_gsm_GsmDataAvailableReturnsTrueWhenDataAvailable(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.flag_data_available = 1
        self.assertTrue(mi_fsm.check_gsm_data_available())
        mi_fsm.flag_data_available = 0
        self.assertFalse(mi_fsm.check_gsm_data_available())

    def test_fsm_gsm_GsmNotDataAvailableAndFifoIsEmptyReturnsTrueWhenDataIsNotAvailableAndFifoIsEmpty(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.flag_data_available = 0
        fifo_driver.fifo_is_empty = Mock(return_value = False)
        self.assertFalse(mi_fsm.gsm_not_data_available_and_fifo_is_empty())
        mi_fsm.flag_data_available = 0
        fifo_driver.fifo_is_empty = Mock(return_value = True)
        self.assertTrue(mi_fsm.gsm_not_data_available_and_fifo_is_empty())
        mi_fsm.flag_data_available = 1
        fifo_driver.fifo_is_empty = Mock(return_value = False)
        self.assertFalse(mi_fsm.gsm_not_data_available_and_fifo_is_empty())
        mi_fsm.flag_data_available = 1
        fifo_driver.fifo_is_empty = Mock(return_value = True)
        self.assertFalse(mi_fsm.gsm_not_data_available_and_fifo_is_empty())

    def test_fsm_gsm_GsmNotDataAvailableAndFifoIsNotEmptyReturnsTrueWhenDataIsNotAvailableAndFifoIsNotEmptyAndFifoAndGsmSendIsCalled(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()

        mi_fsm.flag_data_available = 0
        fifo_driver.fifo_is_empty = Mock(return_value = False)
        self.assertTrue(mi_fsm.gsm_not_data_available_and_fifo_is_not_empty())
        mi_fsm.flag_data_available = 0
        fifo_driver.fifo_is_empty = Mock(return_value = True)
        self.assertFalse(mi_fsm.gsm_not_data_available_and_fifo_is_not_empty())
        mi_fsm.flag_data_available = 1
        fifo_driver.fifo_is_empty = Mock(return_value = False)
        self.assertFalse(mi_fsm.gsm_not_data_available_and_fifo_is_not_empty())
        mi_fsm.flag_data_available = 1
        fifo_driver.fifo_is_empty = Mock(return_value = True)
        self.assertFalse(mi_fsm.gsm_not_data_available_and_fifo_is_not_empty())

    def test_fsm_gsm_checkTransitionFromFsmCheck2GsmIdleWhenDataNotAvailableAndFifoIsEmpty(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_CHECK'
        mi_fsm.gsm_not_data_available_and_fifo_is_empty = Mock(return_value = True)
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')

    def test_fsm_gsm_checkTransitionFromFsmCheck2GsmIdleWhenDataNotAvailableAndFifoIsNotEmptyAndReadFifoAndSendDataCalled(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_CHECK'
        mi_fsm.gsm_not_data_available_and_fifo_is_empty = Mock(return_value = False)
        mi_fsm.gsm_not_data_available_and_fifo_is_not_empty = Mock(return_value = True)
        mi_fsm.read_fifo_and_gsm_send = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.read_fifo_and_gsm_send.assert_called()

    def test_fsm_gsm_checkTransitionFromFsmCheck2FsmReadWhenDataAvailableAndReadIsCalled(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_CHECK'
        self.assertEqual(mi_fsm.state, 'GSM_CHECK')
        mi_fsm.check_gsm_data_available = Mock(return_value = True)
        mi_fsm.gsm_read = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.gsm_read.assert_called()

    def test_fsm_gsm_checkTransitionFromGsmRead2GsmIdleWhenActiveRequestAndSetAlarmIsCalled(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.active_request = Mock(return_value = True)
        mi_fsm.set_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.set_alarm.assert_called()

    def test_fsm_gsm_checkTransitionFromGsmRead2GsmIdleWhenDeactiveRequestAndResetAlarmIsCalled(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.deactive_request = Mock(return_value = True)
        mi_fsm.reset_alarm = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.reset_alarm.assert_called()

    def test_fsm_gsm_checkTransitionFromGsmRead2GsmIdleWhenPositionRequestAndFindCarIsCalled(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.pos_request = Mock(return_value = True)
        mi_fsm.find_car = MagicMock()
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')
        mi_fsm.find_car.assert_called()

    def test_fsm_gsm_checkTransitionFromGsmRead2GsmIdleWhenInvalidRequirement(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.state = 'GSM_READ'
        self.assertEqual(mi_fsm.state, 'GSM_READ')
        mi_fsm.invalid_request = Mock(return_value = True)
        mi_fsm.fire()
        self.assertEqual(mi_fsm.state, 'GSM_IDLE')

    def test_fsm_gsm_checkActiveRequestReturnsTrueWhenDataIsActiveRequest(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.gsm_data = "GSM_ACTIVE_REQUEST"
        self.assertTrue(mi_fsm.active_request())
        mi_fsm.gsm_data = -1
        self.assertFalse(mi_fsm.active_request())

    def test_fsm_gsm_checkDeactiveRequestReturnsTrueWhenDataIsDeactiveRequest(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.gsm_data = "GSM_DEACTIVE_REQUEST"
        self.assertTrue(mi_fsm.deactive_request())
        mi_fsm.gsm_data = -1
        self.assertFalse(mi_fsm.deactive_request())

    def test_fsm_gsm_checkPositionRequestReturnsTrueWhenDataIsPositionRequest(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.gsm_data = "GSM_POSITION_REQUEST"
        self.assertTrue(mi_fsm.pos_request())
        mi_fsm.gsm_data = -1
        self.assertFalse(mi_fsm.pos_request())

    def test_fsm_gsm_checkInvalidRequestReturnsTrueWhenDataIsInvalidRequest(self):
        mi_fsm = FsmGsm("mi_fsm")
        mi_fsm.start()
        mi_fsm.gsm_data = -1
        self.assertTrue(mi_fsm.invalid_request())
        mi_fsm.gsm_data = "GSM_POSITION_REQUEST"
        self.assertFalse(mi_fsm.invalid_request())
        '''
