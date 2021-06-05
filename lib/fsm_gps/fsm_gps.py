
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
        self.flag_active = 1

        """ Timeout GPS
        """
        self.timeout_gps = 0
        
        ''' GPS dictionary
        '''
        self.gps_param = {
            
            'GNSS_status': 0,
            'UTC': '',            # yyyyMMddhhmmss.sss
            'Latitude': 0.0,      # ±dd.dddddd            [-90.000000,90.000000]
            'Longitude': 0.0,     # ±ddd.dddddd           [-180.000000,180.000000]
            'Altitude': 0.0,      # in meters
            'Speed': 0.0,         # km/h [0,999.99]
        }
        

        """ Initialize the state machine
        """
        self.machine = Machine(model=self, states=FsmGps.states, initial='init')
        self.sim868 = mi_sim868

        """ Add transitions
        """
        self.machine.add_transition(trigger='start', source='init', dest='OFF')
        self.machine.add_transition('fire', 'OFF', 'ON', conditions=['is_active'])
        self.machine.add_transition('fire', 'ON', 'OFF', conditions=['is_not_active'])
        self.machine.add_transition('fire', 'ON', 'ON', conditions=['timeout_and_active'], after='send_gps_frame_and_init_timer')

    """ Guard functions
    """ 
    def is_active(self):
        return self.flag_active
        

    def is_not_active(self):
        return not self.flag_active

    def timeout_and_active(self):
        return ((int(dt.now().timestamp()) > self.timeout_gps) and self.flag_active)

    """ Output functions
    TODO: Modify GPS frame sending with GPS drivers
    """ 

    def send_gps_frame_and_init_timer(self):
        gps_data = self.sim868.get_gps_data()
        self.gps_param['UTC'] = gps_data["UTC"]
        self.gps_param['Latitude'] = gps_data["Latitude"]
        self.gps_param['Longitude'] = gps_data["Longitude"]
        self.gps_param['Altitude'] = gps_data["Altitude"]
        self.gps_param['Speed'] = gps_data["Speed"]
        
        print( "'UTC': {}, 'Latitude': {}, 'Longitude': {}, 'Altitude': {}, 'Speed': {}".format(self.gps_param["UTC"], self.gps_param["Latitude"], self.gps_param["Longitude"],self.gps_param["Altitude"],self.gps_param["Speed"]))
        self.timeout_gps = int(dt.now().timestamp())+self.TIMEOUT_DEFAULT 