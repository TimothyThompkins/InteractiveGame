from tkinter import *
from windows import startWindow, gameWindow
from config import sys_config
from serial_comms import serial_comms

import struct

# Create the main screen
def _start_screen(system):

    root = Tk() # Create new root to work with Tkinter
    root.title('Alien Invaders')

    w, h = system.get_start_window_dim()

    # get screen width and height
    ws = root.winfo_screenwidth() #This value is the width of the screen
    hs = root.winfo_screenheight() #This is the height of the screen

    # calculate position x, y
    x = (ws/2) - (w/2)
    y = (hs/2) - (h/2)

    #This is responsible for setting the dimensions of the screen and where it is placed
    root.geometry('%dx%d+%d+%d' % (w, h, x, y))
    root.resizable(0,0)

    start_window = startWindow(root, system, w, h, x, y)
    start_window.pack(fill=BOTH, expand=YES)

    #root.lift()
    #root.call('wm', 'attributes', '.', '-topmost', True) # Bring window to front, should work
    root.focus()

    root.mainloop()

def _play_game(system):

    root = Tk() # Create new root to work with Tkinter
    root.title('Alien Invaders')

    w, h = system.get_game_window_dim()

    # get screen width and height
    ws = root.winfo_screenwidth() #This value is the width of the screen
    hs = root.winfo_screenheight() #This is the height of the screen

    # calculate position x, y
    x = (ws/2) - (w/2)
    y = (hs/2) - (h/2)

    #This is responsible for setting the dimensions of the screen and where it is placed
    root.geometry('%dx%d+%d+%d' % (w, h, x, y))

    game_window = gameWindow(root, system)
    game_window.pack(fill=BOTH, expand=YES)

    #root.lift()
    #root.call('wm', 'attributes', '.', '-topmost', True) # Bring window to front, should work
    root.focus()

    root.mainloop()


def main():

    init_game_sequence = bytes([1]); # This is the flag to the controller to init its game sequence #TMT

    system = sys_config() # Create a new system on which to run
    _start_screen(system)

    # This just checks if we actually clicked start / load game. Otherwise the user closed the window
    if system.get_game_init() is True:
        # TMT Create new serial object, test serial connectivity, pass to _play_game
        #_play_game(system)

        # TMT Left off here. Need to
        # 2.) Figure out how to sent an array to the uC with the init sequence data
        serial_port = serial_comms(system.get_serial_port(), system.get_baud_rate())
        serial_port.send_data(init_game_sequence)

        system.print_data()
        print ('Execution Ended')


if __name__ == '__main__':
    main()
