
from transitions.extensions import GraphMachine as Machine
from datetime import datetime as dt

""" This class is the implementation of the GPS FSM behaviour.
"""
class FsmGps(object):
    """ FSM states
    """
    states = ['init', 'OFF', 'ON', 'ASSAULT']

    """ Default timeout for GPS frame sending
    """

    def __init__(self, name, mi_sim868):
        self.TIMEOUT_DEFAULT = 30
        self.name = name

        """ Input shared variables 
        """
        self.flag_active = 0 # Flag find car
        self.flag_find_car = 0 # Flag find car
        self.flag_init_gps_record = 0 # Flag init GPS record

        """ Timeout GPS
        """
        self.timeout_gps = 0

        """ Initialize the state machine
        """
        self.machine = Machine(model=self, states=FsmGps.states, initial='init')
        self.sim868 = mi_sim868

        """ Add transitions
        """
        self.machine.add_transition(trigger='start', source='init', dest='OFF')
        self.machine.add_transition('fire', 'OFF', 'ON', conditions=['is_active'])
        self.machine.add_transition('fire', 'ON', 'OFF', conditions=['is_not_active'])
        self.machine.add_transition('fire', 'ON', 'ON', conditions=['find_car_and_not_assault_and_active'], after='send_gps_frame')
        self.machine.add_transition('fire', 'ON', 'ASSAULT', conditions=['assault_and_active'], after='send_gps_frame_and_init_timer')
        self.machine.add_transition('fire', 'ASSAULT', 'ASSAULT', conditions=['timeout_or_find_car_and_active'], after='send_gps_frame_and_init_timer')
        self.machine.add_transition('fire', 'ASSAULT', 'OFF', conditions=['is_not_active'])

    """ Guard functions
    """ 
    def is_active(self):
        return self.flag_active

    def is_not_active(self):
        return not self.flag_active

    def find_car_and_not_assault_and_active(self):
        return (self.flag_find_car and (not self.flag_init_gps_record) and self.flag_active)

    def assault_and_active(self):
        return self.flag_init_gps_record and self.flag_active

    def timeout_or_find_car_and_active(self):
        return ((int(dt.now().timestamp()) > self.timeout_gps) or self.flag_find_car) and self.flag_active

    """ Output functions
    TODO: Modify GPS frame sending with GPS drivers
    """ 
    def send_gps_frame(self):
        msg = self.__get_gps()
        self.sim868.gsm_post('ptsv2.com/t/fn719-1620149096/post', "PRUEBA_HEADER", str(msg))
        print(f"Se ha enviado un nuevo frame GPS: {msg}")

    def send_gps_frame_and_init_timer(self):
        msg = self.__get_gps()
        self.sim868.gsm_post('ptsv2.com/t/fn719-1620149096/post', "PRUEBA_HEADER", str(msg))
        print("Se ha enviado un nuevo frame GPS y se ha iniciado el timer: : {msg}")
        self.timeout_gps = int(dt.now().timestamp()) + self.TIMEOUT_DEFAULT
        
    def __get_gps(self):
        gps_data = self.sim868.get_gps_data()
        return "'UTC': {}, 'Latitude': {}, 'Longitude': {}, 'Altitude': {}, 'Speed': {}".format(gps_data["UTC"], gps_data["Latitude"], gps_data["Longitude"],gps_data["Altitude"],gps_data["Speed"])
        