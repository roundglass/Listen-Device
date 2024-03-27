from random import sample
import sys
import threading
from tkinter import Widget
from turtle import update
from typing import ItemsView
from unittest import result

from wsgiref.validate import validator
from PyQt6.QtCore import *
from PyQt6.QtGui import *
from PyQt6.QtWidgets import *
from PyQt6.QtTest import *

from GUI import Ui_GUI
from dataChecking import DataChecking
from portDetect import Port



class window(QWidget):

    def __init__(self):
        super().__init__()
        self.ui  = Ui_GUI()
        self.ui.setupUi(self)

        self.setFixedSize(QSize(380,650))
        
        if(Ports.device_port):
            for ports in Ports.device_port:
                self.ui.comboBox.addItem(ports[1])
        else:
            self.ui.comboBox.addItem("")

       

        self.micMode = 'stereo'
        self.gain = 'Medium'
        self.mode = 'Interval Mode'
        
        regexp = QRegularExpression('[a-zA-Z0-9_]\\w{0,13}')
        validator = QRegularExpressionValidator(regexp)
        self.ui.DeviceID.setValidator(validator)



        self.ui.recordDuration.setInputMask('9999')

        self.ui.sleepDuration.setInputMask('9999')

        self.ui.startRecord.setInputMask('9999')
        self.ui.endRecord.setInputMask('9999')

        self.ui.recordDuration.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.ui.sleepDuration.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.ui.startRecord.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.ui.endRecord.setAlignment(Qt.AlignmentFlag.AlignCenter)
        
        self.ui.startRecord.clear()
        self.ui.endRecord.clear()

        self.ui.server.setEnabled(False)
        self.ui.server.setStyleSheet("background:rgb(234, 234, 234)")
        self.ui.user.setEnabled(False)
        self.ui.user.setStyleSheet("background:rgb(234, 234, 234)")
        self.ui.Pass.setEnabled(False)
        self.ui.Pass.setStyleSheet("background:rgb(234, 234, 234)")

        self.set_radioButton()
        self.ui.pushButton.clicked.connect(lambda:self.addRecordingPeriod())
        self.ui.pushButton_2.clicked.connect(lambda:self.removeRecordingPeriod())
        self.ui.pushButton_3.clicked.connect(lambda:self.checkdata())
                

    def set_radioButton(self):
        self.ui.monoL.toggled.connect(lambda:self.onClicked())
        self.ui.monoR.toggled.connect(lambda:self.onClicked())
        self.ui.stereo.toggled.connect(lambda:self.onClicked())
        self.ui.Low.toggled.connect(lambda:self.onClicked())
        self.ui.Medium.toggled.connect(lambda:self.onClicked())
        self.ui.High.toggled.connect(lambda:self.onClicked())
        self.ui.Interval.toggled.connect(lambda:self.onClicked())
        self.ui.Continuous.toggled.connect(lambda:self.onClicked())
        self.ui.uploadData.stateChanged.connect(lambda:self.onClicked())


    def onClicked(self):
        command = ''
        radioBtn = self.sender()
        checkUpload = self.ui.uploadData.isChecked()
        if radioBtn.isChecked():
            command = radioBtn.text()   
        if(command=='Mono L'):
            self.micMode = 'Mono L'
        elif(command=='Mono R'):
            self.micMode = 'Mono R'
        elif(command=='Stereo'):
            self.micMode = 'Stereo'
        elif(command=='High'):
            self.gain = 'High'
        elif(command=='Medium'):
            self.gain = 'Medium'
        elif(command=='Low'):
            self.gain = 'Low'
        elif(command=='Interval Mode'):
            self.mode = 'Interval Mode'
            self.ui.sleepDuration.setEnabled(True)
            self.ui.sleepDuration.setStyleSheet("background:rgb(255, 255, 255)")
            self.ui.startRecord.setEnabled(True)
            self.ui.startRecord.setStyleSheet("background:rgb(255, 255, 255)")
            self.ui.endRecord.setEnabled(True)
            self.ui.endRecord.setStyleSheet("background:rgb(255, 255, 255)")
        elif(command=='Continuous Mode'):
            self.mode = 'Continuous Mode'
            self.ui.sleepDuration.setEnabled(False)
            self.ui.sleepDuration.setStyleSheet("background:rgb(234, 234, 234)")
            self.ui.startRecord.setEnabled(False)
            self.ui.startRecord.setStyleSheet("background:rgb(234, 234, 234)")
            self.ui.endRecord.setEnabled(False)
            self.ui.endRecord.setStyleSheet("background:rgb(234, 234, 234)")

        if(checkUpload):
            self.ui.server.setEnabled(True)
            self.ui.server.setStyleSheet("background:rgb(255, 255, 255)")
            self.ui.user.setEnabled(True)
            self.ui.user.setStyleSheet("background:rgb(255, 255, 255)")
            self.ui.Pass.setEnabled(True)
            self.ui.Pass.setStyleSheet("background:rgb(255, 255, 255)")
        else:
            self.ui.server.setEnabled(False)
            self.ui.server.setStyleSheet("background:rgb(234, 234, 234)")
            self.ui.user.setEnabled(False)
            self.ui.user.setStyleSheet("background:rgb(234, 234, 234)")
            self.ui.Pass.setEnabled(False)
            self.ui.Pass.setStyleSheet("background:rgb(234, 234, 234)")


        print('MicMode: '+self.micMode+'\n'+'Gain: '+self.gain+'\n'+'Mode: '+self.mode+'\n')


    def addRecordingPeriod(self):
        startRecord = self.ui.startRecord.text()
        endRecord = self.ui.endRecord.text()
        listitem = startRecord +" - "+ endRecord
        if(startRecord!="" and endRecord!=""):   
            self.ui.listWidget.addItem(listitem)


    def removeRecordingPeriod(self):
        Item = self.ui.listWidget.currentRow()
        if(self.ui.listWidget.count()==0):
            print('list is 0')
        self.ui.listWidget.takeItem(Item)
    

    def checkdata(self):
        checkUpload = self.ui.uploadData.isChecked()
        RecordMode = self.mode
        RecordDuration = self.ui.recordDuration.text()
        SleepDuration = self.ui.sleepDuration.text()
        
        RecordPeriods = []

        for i in range(0,self.ui.listWidget.count()):
            Period = self.ui.listWidget.item(i)
            RecordPeriods.append(Period.text())
        
    
        Result = datacheck.CheckData(RecordMode,RecordDuration,SleepDuration,RecordPeriods)

        if(Result):
            self.errorPrint(Result)
        else:
            
            comPort = self.ui.comboBox.currentIndex()
            print(comPort)
            if(comPort >= 0):
                deviceID = self.ui.DeviceID.text()
                Ports.connectPort(comPort,115200)
                print(deviceID)
                # send Data function to be filled
                #send Data by send 0
                
                Ports.sendConfig(self.ui.DeviceID.text(),RecordMode,RecordDuration,SleepDuration,RecordPeriods,self.gain,self.micMode,self.ui.uploadData.isChecked(),self.ui.server.text(),self.ui.user.text(),self.ui.Pass.text())
                print("sent data by send0")

                Ports.close()
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Icon.Information)
                msg.setText("Configuration Send")
                msg.setInformativeText("")
                msg.setWindowTitle("Configuration Complete")
                msg.exec()
            else:
                #Ports.sendConfig(self.ui.DeviceID.text(),RecordMode,RecordDuration,SleepDuration,RecordPeriods,self.gain,self.micMode,self.ui.uploadData.isChecked(),self.ui.server.text(),self.ui.user.text(),self.ui.Pass.text())
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Icon.Critical)
                msg.setText("Warning")
                msg.setInformativeText("Listen device not found")
                msg.setWindowTitle("Device not found")
                msg.exec()

            
            
            

    def errorPrint(self,Result):
        if (Result == 101):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Enter Sleep Duration.")
            msg.setWindowTitle("Sleep Duration not set")
            msg.exec()
        
        elif(Result == 102):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Enter Record Duration.")
            msg.setWindowTitle("Record Duration not set")
            msg.exec()
        
        elif(Result == 103):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Enter Record and Sleep Duration.")
            msg.setWindowTitle("Durations not set")
            msg.exec()
        
        elif(Result == 104):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Record/Sleep Duration should between 0 and 3600")
            msg.setWindowTitle("Wrong Record/Sleep Duration")
            msg.exec()
        
        elif(Result == 106):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Start and End Recording can't be Same")
            msg.setWindowTitle("Same Start and End duration")
            msg.exec()
        
        elif(Result == 105):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Start and End Duration should not be greater than 2400.")
            msg.setWindowTitle("Wrong Start and End Duration")
            msg.exec()
        
        elif(Result == 201):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Record Duration should be between 0 and 3600")
            msg.setWindowTitle("Wrong Record Duration")
            msg.exec()
        
        elif(Result == 203):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Warning)
            msg.setText("Error")
            msg.setInformativeText("Enter Record Duration.")
            msg.setWindowTitle("Record Duration not set")
            msg.exec()


    def update(self):
        self.ui.comboBox.clear()
        for ports in Ports.device_port:
            self.ui.comboBox.addItem(ports[1])
        


def update():
    while True:
        comport = Ports.device_port
        Ports.device_port = Ports.getPort()
        if(comport != Ports.device_port):
            gui.update()
        QTest.qWait(2000)

def main(): 
   app = QApplication(sys.argv)
   global gui
   gui = window()
   gui.show()
   threading.Thread(target=update, daemon = True).start()

   try:
      sys.exit(app.exec())
   except SystemExit:
      print('Closing Window...')     

if __name__=='__main__':
    Ports = Port()
    datacheck = DataChecking()
    main()



    