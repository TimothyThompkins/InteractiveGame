import sys
import glob
import serial
#import time

baudrates = [110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200]
error_msg = "Error: No Serial ports available"

class serial_comms:

    def __init__(self, serial_port, baud_rate):

        #ser = serial.Serial(
        self.port = serial_port,
        self.baudrate = baud_rate,
        self.stopbits = serial.STOPBITS_TWO,
        self.bytesize = serial.EIGHTBITS
        #)

    # TMT This needs work, these are just placeholders
    def send_data(self):
        self.ser.isOpen()
        delim = "\x00"
        ser.write(msg+delim)

    def recv_data(self):
        self.ser.isOpen()
        delim = "\x00"
        recvd = "".join(iter(lambda:ser.read(1),delim))
        return recvd

    def _a2s(arr):
        """ Array of integer byte values --> binary string
        """
        return ''.join(chr(b) for b in arr)

    #def test_comms(self):


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

    if len(result) == 0:
        result.append(error_msg)

    return result

def get_baud_options():
    return baudrates
