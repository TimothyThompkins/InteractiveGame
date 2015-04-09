from tkinter import *
from windows import startWindow
from config import sys_config

# Create the main screen
def _start(system):

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

    initial_window = startWindow(root, system)
    initial_window.pack(fill=BOTH, expand=YES)

    #root.lift()
    #root.call('wm', 'attributes', '.', '-topmost', True) # Bring window to front, should work

    root.mainloop()

def _game_window(system):

    root = Tk() # Create new root to work with Tkinter
    root.title('Alien Invaders')

    w = 1024
    h = 640

    # get screen width and height
    ws = root.winfo_screenwidth() #This value is the width of the screen
    hs = root.winfo_screenheight() #This is the height of the screen

    # calculate position x, y
    x = (ws/2) - (w/2)
    y = (hs/2) - (h/2)

    #This is responsible for setting the dimensions of the screen and where it is placed
    root.geometry('%dx%d+%d+%d' % (w, h, x, y))
    root.resizable(0,0)

    initial_window = startWindow(root, system)
    initial_window.pack(fill=BOTH, expand=YES)

    #root.lift()
    #root.call('wm', 'attributes', '.', '-topmost', True) # Bring window to front, should work

    root.mainloop()


def main():

    system = sys_config() # Create a new system on which to run
    _start(system)
    print ('here')


if __name__ == '__main__':
    main()
