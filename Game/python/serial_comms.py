import sys
import glob
import serial
#import time

baudrates = [110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200]

class serial_comms:

    def __init__(self):
        #self.baudrate = [110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200]
        self.stopbits = serial.STOPBITS_TWO
        self.bytesize = serial.EIGHTBITS
        #self.serial_test = _serial_ports()

def get_port_options():
    """Lists serial ports

    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
        A list of available serial ports
    """
    # For Windows Platform
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    # For Linux Platforms
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    # For OSX Platforms
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')


    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

    #def get_serial_ports(self):
    #    return self.serial_ports()

def get_baud_options():
    return baudrates


#ser = serial.Serial('/dev/tty.usbmodem1431')  # open first serial port
# ser = serial.Serial(
#     port='/dev/tty.usbmodem1431',
#     baudrate=9600,
#     #parity=serial.PARITY_ODD,
#     stopbits=serial.STOPBITS_TWO,
#     bytesize=serial.EIGHTBITS
# )

#ser.open()
# ser.isOpen()
#
# print ('Enter your commands below.\r\nInsert "exit" to leave the application.')
#
# #input=1
# while 1 :
#     # get keyboard input
#     #input = raw_input(">> ")
#     # Python 3 users
#     data = input()
#     #sread(size=1)
#
#     if data == 'exit':
#         ser.close()
#         exit()
#     else:
#         # send the character to the device
#         # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
#         #ser.write(input + '\r\n')
#         ser.write(bytes(data, 'UTF-8'))
#         out = ''
#         # let's wait one second before reading output (let's give device time to answer)
#         time.sleep(1)
#         while ser.inWaiting() > 0:
#             out += ser.read(1)
#
#         if out != '':
#             print (">>" + out)
