from transitions.extensions import GraphMachine as Machine
from datetime import datetime as dt
import json
import re

""" This class is the implementation of the GSM FSM behaviour.
"""
class FsmGsm(object):

    """ FSM states
    """
    states = ['init', 'GSM_IDLE', 'GSM_CHECK', 'GSM_READ']

    request_list = ["GSM_ACTIVE_REQUEST", "GSM_DEACTIVE_REQUEST", "GSM_POSITION_REQUEST"]

    def __init__(self, name, mi_sim868, url_post_token, url_post_notification, refresh_token, url_get, device_id):

        self.name = name
        self.T_CHECK = 30
        self.flag_data_available = 0
        self.flag_active = 0
        self.__new_active = 0
        self.flag_find_car = 0
        self.gsm_data = None

        ''' SIM868
        '''
        self.sim868   = mi_sim868
        self.url_post_token = url_post_token
        self.url_post_notification = url_post_notification
        self.refresh_token = refresh_token
        self.url_get  = url_get
        self.token = 0
        self.device_id = device_id

        ''' Send data
        '''
        self.last_data = {}

        """ Timeout GSM
        """
        self.timeout_gsm = int(dt.now().timestamp()) + self.T_CHECK

        """ Initialize the state machine
        """
        self.machine = Machine(model=self, states=FsmGsm.states, initial='init')

        """ Add transitions
        """
        self.machine.add_transition(trigger='start', source='init', dest='GSM_IDLE')
        self.machine.add_transition('fire', 'GSM_IDLE', 'GSM_CHECK', conditions=['check_timeout'], after='gsm_get_and_update_timeout')
        self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_READ', conditions=['gsm_new_data_available'], after = 'reset_flag_new_data_available')
        self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_IDLE', conditions=['gsm_not_new_data_available_and_active'], after='read_data_and_gsm_send')
        self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_IDLE', conditions=['gsm_not_new_data_available_and_not_active'])
        self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['active_request'], after='set_alarm')
        self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['deactive_request'], after='reset_alarm')
        self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['invalid_request'])

        self.__n = 0

    # Funciones de guarda
    def check_timeout(self):
        return int(dt.now().timestamp()) > self.timeout_gsm

    def gsm_new_data_available(self):
        return self.flag_data_available

    def gsm_not_new_data_available_and_active(self):
        return (not self.flag_data_available and self.flag_active)

    def gsm_not_new_data_available_and_not_active(self):
        return (not self.flag_data_available and not self.flag_active)

    def active_request(self):
        return (self.__new_active == 1)

    def deactive_request(self):
        return (self.__new_active == 0)

    def invalid_request(self):
        print("Dato no valido")
        return (self.__new_active != 1 and self.__new_active != 0)


    # Funciones salida

    def gsm_get_and_update_timeout(self):
        self.get_token()
        headers = f"Authorization: Bearer {self.token}"
        rcv = (self.sim868.gsm_get(self.url_get, headers))
        try:
            json_text = re.search("({.*})",  rcv)[0]
            rcv_dict = json.loads(json_text)
            self.__new_active = int(rcv_dict["parameters"]["active"])
        except:
            pass

        print(f"new_active = {self.__new_active}; active: {self.flag_active}")

        if self.flag_active != self.__new_active:
            self.flag_data_available = 1
            print("flag_data_available = 1")
        self.timeout_gsm = self.timeout_gsm + self.T_CHECK

        self.__n += 1



    def read_data_and_gsm_send(self):
        print("------ GSM SEND ------")
        self.get_token();
        '''Send notification'''
        body_dict = {"notification": self.device_id, "parameters": self.last_data}
        body = json.dumps(body_dict)
        headers = f"Authorization: Bearer {self.token}"
        self.sim868.gsm_post(url = self.url_post_notification, headers = headers, body = body)

    def get_token(self):
        '''Get token'''
        body = {"refreshToken": self.refresh_token}
        response = self.sim868.gsm_post(url = self.url_post_token, body = json.dumps(body))
        json_text = re.search("({.*})",  response)[0]
        token_dict = json.loads(json_text)
        self.token = token_dict["accessToken"]

    def reset_flag_new_data_available(self):
        print("Reset flag new data available")
        self.flag_data_available = 0

    def set_alarm(self):
        self.flag_active = 1
        print("------Set alarm!------")

    def reset_alarm(self):
        self.flag_active = 0
        print("------Reset alarm!------")
