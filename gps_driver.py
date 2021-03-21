import serial
import os, time

from random import randrange 
import RPi.GPIO as GPIO


# Enable Serial Communication
port = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=1)


class GPS:
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
        self.Power_pin = pin

        self.__power_pin_init()

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
        time.sleep(2) 
    
    def __check_status(self):
        print("check status")
        port.write(str.encode('AT'+'\r\n'))
        rcv = port.read(64)
        print(rcv)
            
        if (str(rcv).find('OK') == -1):
            return 0
        else:
            return 1
           
           
           
new_gps = GPS(4)
#time.sleep(10) #seconds

print("comprobamos")
new_gps.power_on_board()

"""
port.write(str.encode('AT'+'\r\n'))
rcv = port.read(4500)
print (rcv)
time.sleep(5) #seconds

port.write(str.encode('AT+CGNSPWR=1'+'\r\n'))
rcv = port.read(4500)
print (rcv)
time.sleep(5) #seconds

port.write(str.encode('AT+CGNSPWR?'+'\r\n'))
rcv = port.read(4500)
print (rcv)
time.sleep(5) #seconds


for i in range(3):
	port.write(str.encode('AT+CGNSINF'+'\r\n'))
	rcv = port.read(4500)
	print (rcv)
	time.sleep(20) #seconds
"""
print("apagado final \n")
new_gps.power_off_board()

GPIO.cleanup()

