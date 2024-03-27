from itertools import count
from multiprocessing.reduction import send_handle
import time
import serial
from serial.tools import list_ports

class Port:
    def __init__(self):
        self.device_port = self.getPort()
    
    def getPort(self):
        ports_avaiable = list(list_ports.comports())
        return ports_avaiable

    def connectPort(self,portIndex,baudrate):
        comPort = self.device_port[portIndex][0]
        try:
            self.device = serial.Serial(comPort, baudrate)
            self.device.isOpen()
        except IOError:
            self.device.close()
            self.device.open()
        return(comPort)
    

    def sendConfig(self,deviceId,RecordMode, RecordDuration, SleepDuration, RecordPeriods,gain,micMode,uploadcheck,Server,User,Pass):
        print("Send Data")
        deviceId = deviceId + '\n'
        Mode = RecordMode
        RecordMode = RecordMode + '\n'
        RecordDuration = RecordDuration + '\n'
        SleepDuration = SleepDuration +'\n'
        gain = gain + '\n'
        micMode = micMode + '\n'
        count = len(RecordPeriods)
        count1 = count
        count = str(count)
        count = count + '\n'
        Server = Server+'\n'
        User = User + '\n'
        Pass = Pass + '\n'
        uploadcheck = str(uploadcheck) + '\n'
        t = str(int(time.time()))
        t = t+'\n'

        self.device.write(t.encode())
        self.device.write(deviceId.encode())
        self.device.write(gain.encode())
        self.device.write(micMode.encode())
        self.device.write(RecordMode.encode())
        self.device.write(RecordDuration.encode())
        if(Mode == 'Interval Mode'):
            self.device.write(SleepDuration.encode())
            self.device.write(count.encode())
            if(count1>0):
                for Period in RecordPeriods:
                    Period = Period.split(' - ')
                    Period[0] = Period[0] + '\n'
                    Period[1] = Period[1] + '\n'
                    self.device.write(Period[0].encode())
                    self.device.write(Period[1].encode())
        self.device.write(uploadcheck.encode())
        if(uploadcheck == 'True\n'):
            self.device.write(Server.encode())
            self.device.write(User.encode())
            self.device.write(Pass.encode())
       
        

    def close(self):
        if self.device.isOpen():
            self.device.close()

