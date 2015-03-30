import serial
import time
#ser = serial.Serial('/dev/tty.usbmodem1431')  # open first serial port
ser = serial.Serial(
    port='/dev/tty.usbmodem1431',
    baudrate=9600,
    #parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)

#ser.open()
ser.isOpen()

print ('Enter your commands below.\r\nInsert "exit" to leave the application.')

#input=1
while 1 :
    # get keyboard input
    #input = raw_input(">> ")
    # Python 3 users
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
        out = ''
        # let's wait one second before reading output (let's give device time to answer)
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)

        if out != '':
            print (">>" + out)
