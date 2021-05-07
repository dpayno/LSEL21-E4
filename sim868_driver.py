import serial
import os, time
import json
import re

from random import randrange 
import RPi.GPIO as GPIO

class SIM868:
    EarthRadius = 6371e3         # meters

    @staticmethod
    def CalculateDeltaP(Position1, Position2):
        phi1 = Position1.Latitude * math.pi / 180.0
        phi2 = Position2.Latitude * math.pi / 180.0
        deltaPhi = (Position2.Latitude - Position1.Latitude) * math.pi / 180.0
        deltaLambda = (Position2.Longitude - Position1.Longitude) * math.pi / 180.0

        a = math.sin(deltaPhi / 2) * math.sin(deltaPhi / 2) + math.cos(phi1) * math.cos(phi2) * math.sin(deltaLambda / 2) * math.sin(deltaLambda / 2)
        c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
        d = GPS.EarthRadius * c    # in meters

        return d

    
    def __init__(self, pin):
        
        # On/Off
        self.Power_pin = pin
        
        # GPS
        self.GNSS_status = 0
        self.Fix_status = 0
        self.UTC = ''           # yyyyMMddhhmmss.sss
        self.Latitude = 0.0     # ±dd.dddddd            [-90.000000,90.000000]
        self.Longitude = 0.0    # ±ddd.dddddd           [-180.000000,180.000000]
        self.Altitude = 0.0     # in meters
        self.Speed = 0.0        # km/h [0,999.99]
        self.Course = 0.0       # degrees [0,360.00]
        self.HDOP = 0.0         # [0,99.9]
        self.PDOP = 0.0         # [0,99.9]
        self.VDOP = 0.0         # [0,99.9]
        self.GPS_satellites = 0 # [0,99]
        self.GNSS_satellites = 0    # [0,99]
        self.Signal = 0.0         # %      max = 55 dBHz
        self.gps_data = 0
        
        # Imself.__port config data
        with open('config.json') as file:
            config_dict = json.load(file)
        
        #GSM
        self.__PIN = config_dict["PIN"]
        self.__APN = config_dict["APN"]


        # Enable Serial Communication
        self.__port = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=1)

        # Setup
        self.__power_pin_init()
        self.power_on_board()
        self.__init_gsm()
        self.__init_gps()


    ##############################################################
    # FUNCIONES CONFIGURACION
    ##############################################################
    
    def __power_pin_init(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.Power_pin, GPIO.OUT)
        GPIO.output(self.Power_pin, True)

    def power_on_board(self):
        if ( not self.__check_status() ):
            self.__switch_power()
            self.power_on_board()
        

    def power_off_board(self):
        if ( self.__check_status() ):
            self.__switch_power()
            self.power_off_board()
        
    def __switch_power(self):
        GPIO.output(self.Power_pin, False)
        time.sleep(1.5) #seconds
        GPIO.output(self.Power_pin, True)
        time.sleep(5) 
    
    def __check_status(self):
        print("check status")
        self.__port.write(str.encode('AT'+'\r\n'))
        time.sleep(0.1)
        rcv = self.__port.read(64)
        print(rcv)
            
        if (str(rcv).find('OK') == -1):
            return 0
        else:
            return 1
        
    ##############################################################
    # FUNCIONES INIT GSM
    ##############################################################
    
    def __init_gsm(self):
        
        print("---------encendemos GSM---------")
        self.__port.write(str.encode('AT+CPIN=' + str(self.__PIN) + '\r\n'))  
        time.sleep(10) #seconds
        rcv = self.__port.read(256)
        print (rcv)

        self.__port.write(str.encode('AT+SAPBR=3,1,"Contype","GPRS"'+'\r\n'))  
        time.sleep(1) #seconds
        rcv = self.__port.read(256)
        print (rcv)
        

        self.__port.write(str.encode('AT+SAPBR=3,1,"APN","' + str(self.__APN) + '"' + '\r\n')) 
        time.sleep(1) #seconds
        rcv = self.__port.read(256)
        print (rcv)
        

        self.__port.write(str.encode('AT+SAPBR=1,1'+'\r\n'))  
        time.sleep(1) #seconds
        rcv = self.__port.read(256)
        print (rcv)
        

        self.__port.write(str.encode('AT+HTTPINIT'+'\r\n'))  
        time.sleep(1) #seconds
        rcv = self.__port.read(256)
        print (rcv)
        
    ##############################################################
    # FUNCIONES INIT GPS
    ##############################################################
    
    def __init_gps(self):
        print("---------encendemos GPS---------")
        self.__port.write(str.encode('AT+CGNSPWR=1'+'\r\n'))
        time.sleep(2) #seconds
        rcv = self.__port.read(4500)
        print (rcv)
        
    
    ##############################################################
    # FUNCIONES PUBLICAS
    ##############################################################    
    
    
    def gsm_read(self, url = None):
        
        self.__port.write(str.encode('AT+HTTPPARA=\"URL\",\"'+str(url)+'"'+'\r\n'))  # Select Message format as Text mode
        time.sleep(1) #seconds
        rcv = self.__port.read(256)
        print (rcv)

        self.__port.write(str.encode('AT+HTTPACTION=0'+'\r\n'))  # Select Message format as Text mode
        time.sleep(5) #seconds
        rcv = self.__port.read(1024)
        print (rcv)
        
        self.__port.write(str.encode('AT+HTTPREAD'+'\r\n'))  # Select Message format as Text mode
        time.sleep(5) #seconds
        rcv = self.__port.read(4500)
        print (rcv)
        return rcv
    
    
    def gsm_post(self, url = None, headers = None, body = None):
        
        self.__port.write(str.encode('AT+HTTPPARA=\"URL\",\"'+ url +'"'+'\r\n'))  # Select Message format as Text mode
        time.sleep(1) #seconds
        rcv = self.__port.read(128)
        print (rcv)
        
        self.__port.write(str.encode('AT+HTTPPARA=\"CONTENT\",\"' + str(headers) + '\"'+'\r\n'))  # Select Message format as Text mode
        time.sleep(1) #seconds
        rcv = self.__port.read(128)
        print (rcv)
        

        self.__port.write(str.encode('AT+HTTPDATA=2000,2000'+'\r\n'))  # Select Message format as Text mode
        time.sleep(0.5) #seconds
        rcv = self.__port.read(128)
        print (rcv)

        self.__port.write(str.encode(str(body) + '\r\n'))  # Select Message format as Text mode
        time.sleep(1.5) #seconds
        rcv = self.__port.read(128)
        print (rcv)
        
        self.__port.write(str.encode('AT+HTTPACTION=1'+'\r\n'))  # Select Message format as Text mode
        time.sleep(1) #seconds
        rcv = self.__port.read(128)
        print (rcv)

    # GPS
    def get_gps_data(self):
        
        self.__port.write(str.encode('AT+CGNSINF'+'\r\n'))  # Select Message format as Text mode
        time.sleep(5) #seconds
        rcv = self.__port.read(256)
        print (rcv)
        self.gps_data = rcv
        return rcv
        
# Iniciamos sim868   
new_sim868 = SIM868(4)

#encendemos sim868
new_sim868.power_on_board()

# PRUEBAS GPS
new_sim868.get_gps_data()


# PRUEBAS GSM
'''
#leemos
new_sim868.gsm_read('httpbin.org/get')
'''
#enviamos
new_sim868.gsm_post('ptsv2.com/t/fn719-1620149096/post', "PRUEBA_HEADER", new_sim868.gps_data)

'''
#apagamos
new_sim868.power_off_board()
GPIO.cleanup()
'''



