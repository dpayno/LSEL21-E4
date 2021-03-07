
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
    TIMEOUT_DEFAULT = 300

    def __init__(self, name):
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

        """ Add transitions
        """
        self.machine.add_transition(trigger='start', source='init', dest='OFF')
        self.machine.add_transition('fire', 'OFF', 'ON', conditions=['is_active'])
        self.machine.add_transition('fire', 'ON', 'OFF', conditions=['is_not_active'])
        self.machine.add_transition('fire', 'ON', 'ON', conditions=['find_car'], after='send_gps_frame')
        self.machine.add_transition('fire', 'ON', 'ASSAULT', conditions=['assault_and_not_find_car'], after='init_timer_gps')
        self.machine.add_transition('fire', 'ASSAULT', 'ON', conditions=['not_assault'])
        self.machine.add_transition('fire', 'ASSAULT', 'ASSAULT', conditions=['timeout_or_find_car'], after='send_gps_frame_and_init_timer')
        self.machine.add_transition('fire', 'ASSAULT', 'OFF', conditions=['is_not_active'])

    """ Guard functions
    """ 
    def is_active(self):
        return self.flag_active

    def is_not_active(self):
        return not self.flag_active

    def find_car(self):
        return self.flag_find_car

    def assault_and_not_find_car(self):
        return self.flag_init_gps_record and (not flag_find_car)

    def not_assault(self):
        return not self.flag_init_gps_record

    def timeout_or_find_car(self):
        return (int(dt.now().timestamp()) > self.timeout_gps) or self.flag_find_car

    """ Output functions
    TODO: Modify GPS frame sending with GPS drivers
    """ 
    def send_gps_frame(self):
        print("Se ha enviado un nuevo frame GPS")

    def init_timer_gps(self):
        print("Se ha reiniciado el timer GPS")
        self.timeout_gps = int(dt.now().timestamp()) + TIMEOUT_DEFAULT

    def send_gps_frame_and_init_timer(self):
        print("Se ha enviado un nuevo frame GPS y se ha iniciado el timer")
        self.timeout_gps = int(dt.now().timestamp()) + TIMEOUT_DEFAULT