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

    def __init__(self, name, mi_sim868, ulr_post, url_get):
        
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
        self.post_url = ulr_post
        self.get_url  = url_get
        
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
        self.machine.add_transition('fire', 'GSM_CHECK', 'GSM_IDLE', conditions=['gsm_not_new_data_available'], after='read_data_and_gsm_send')
        self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['active_request'], after='set_alarm')
        self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['deactive_request'], after='reset_alarm')
        self.machine.add_transition('fire', 'GSM_READ', 'GSM_IDLE', conditions=['invalid_request'])
        
        self.__n = 0

    # Funciones de guarda
    def check_timeout(self):
        return int(dt.now().timestamp()) > self.timeout_gsm

    def gsm_new_data_available(self):
        return self.flag_data_available

    def gsm_not_new_data_available(self):
        return (not self.flag_data_available)

    def active_request(self):
        return (self.__new_active == 1)

    def deactive_request(self):
        return (self.__new_active == 0)

    def invalid_request(self):
        print("Dato no valido")
        return (self.__new_active != 1 and self.__new_active != 0)


    # Funciones salida
    
    def gsm_get_and_update_timeout(self):
        
        if (self.__n >= 4):
            rcv = (self.sim868.gsm_get("postman-echo.com/get?active=0"))
        else:
            rcv = (self.sim868.gsm_get(self.get_url))
        
        json_text = re.search("({.*})",  rcv)[0]
        rcv_dict = json.loads(json_text)
        self.__new_active = int(rcv_dict["args"]["active"])
        
        print(f"new_active = {self.__new_active}; active: {self.flag_active}")
        
        if self.flag_active != self.__new_active:
            self.flag_data_available = 1
            print("flag_data_available = 1")
        self.timeout_gsm = self.timeout_gsm + self.T_CHECK
        
        self.__n += 1


    def read_data_and_gsm_send(self):
        print("------ GSM SEND ------")
        headers = "Prueba_Header"
        #body = str(self.sim868.gps_data)
        self.last_data = self.sim868.gps_data
        body = json.dumps(self.last_data)
        self.sim868.gsm_post(url = self.post_url, headers = headers, body = body)
        
    def reset_flag_new_data_available(self):
        print("Reset flag new data available")
        self.flag_data_available = 0

    def set_alarm(self):
        self.flag_active = 1
        print("------Set alarm!------")

    def reset_alarm(self):
        self.flag_active = 0
        print("------Reset alarm!------")

    
