import serial
import time
#ser = serial.Serial('/dev/tty.usbmodem1431')  # open first serial port
ser = serial.Serial(
    # port = '/dev/tty.usbmodem1431', # For Macbook pro running OSX
    port = '/dev/ttyACM0', # For Ubuntu 14.04. Middle right port
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 5
)

#ser.open()
try:
    ser.isOpen()
    print ('Connected.')
except:
    print ('Connection Failure')

#input=1
while 1 :
    # get keyboard input
    #input = raw_input(">> ")
    # Python 3 users
    print ('Enter your commands below. Insert "exit" to close the application.')
    data = input()
    #sread(size=1)

    if data == 'exit':
        ser.close()
        exit()
    else:
        # send the character to the device
        # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
        #ser.write(input + '\r\n')
        ser.write(bytes(data, 'UTF-8'))
        buffer = ''
        # let's wait one second before reading output (let's give device time to answer)
        # time.sleep(1)

        # x = ser.read()          # read one byte
        # s = ser.read(10)        # read up to ten bytes (timeout)
        # line = ser.readline()   # read a '\n' terminated line}


        # rcv_data = 0;
        # while rcv_data == 0:
        #     # out += ser.readline()
        #
        #     # out = ser.readline()
        #     buffer = ''
        #     while ser.inWaiting() > 0:
        #         buffer += ser.readline().decode('UTF-8')
        #         print (buffer)
        #
        #     if buffer != '':
        #         rcv_data = 1;
        #         print (">>" + buffer)


    while True:
        bytesToRead = ser.inWaiting()
        ser.read(bytesToRead)

    print (bytesToRead)
