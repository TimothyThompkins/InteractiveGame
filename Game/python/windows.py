from tkinter import *
from tkinter import ttk
import tkinter.messagebox
from PIL import Image, ImageTk
from config import sys_config
from serial_comms import get_port_options
from serial_comms import get_baud_options
from errors import *

serial_error_msg = "Please select a serial port."
baud_error_msg = "Please select a baud rate."

# TMT add help menu with design information

class startWindow(Frame):

    def __init__(self, master, system, w, h, x, y, *pargs):
        Frame.__init__(self, master, *pargs)

        # TMT add below code to load game too
        def _start_new_game():
            new_game = True
            system.set_new_game(new_game)
            user_input_choice = self.input_selection.get()
            system.set_input_method(user_input_choice)

            try:
                user_serial_selection = system.get_serial_port()
                user_baud_selection = system.get_baud_rate()

                if user_serial_selection == None:
                    raise serialPortDefError
                if user_baud_selection == None:
                    raise  baudRateDefError

            except(serialPortDefError, baudRateDefError) as e:

                if (user_serial_selection == None) and (user_baud_selection == None):
                    serial_baud_error = error_window(w, h, x, y, serial_error_msg, baud_error_msg)

                elif (user_serial_selection == None):
                    serial_baud_error = error_window(w, h, x, y, serial_error_msg)

                elif (user_baud_selection == None):
                    serial_baud_error = error_window(w, h, x, y, baud_error_msg)

            else:
                #TMT add error handling here in case user doesn't select baud rate and serial port
                self.master.destroy()

        def _load_old_game():
            new_game = False
            system.set_new_game(new_game)
            user_input_choice = self.input_selection.get()
            system.set_input_method(user_input_choice)
            self.master.destroy()

        serial_ports = get_port_options()
        baud_rates = get_baud_options()

        map_serial_ports = {}
        map_baud_rates = {}


        # Creates a new function for each entry in the tool bar menu options. Each function has the same name & is stored in map_X[]
        for i, name in enumerate(serial_ports):
            def new_command(i=i):
                port_selection = portOptionsMenu.entrycget(i,'label')

                # This if just removes the bullet character if the user reselects the same port
                if (port_selection.find(u"\u2022") == 0):
                    port_selection = port_selection.strip("\u2022")

                # If the port selection contains the word 'Error', we know that there are no ports to be selected
                if (port_selection.find("Error") < 0):

                    for k, name in enumerate(serial_ports):
                        portOptionsMenu.entryconfigure(k, label=serial_ports[k])

                    portOptionsMenu.entryconfigure(i, label=port_selection + u"\u2022")

                    system.set_serial_port(port_selection)# Set system variable


            map_serial_ports[i] = new_command

        for i, name in enumerate(baud_rates):
            def new_command(i=i):
                baud_selection = baudOptionsMenu.entrycget(i,'label')
                baud_selection = str(baud_selection) # Must convert baud_rate to string for menu manipliation

                #This if just removes the bullet character if the user reselects the same baud rate
                if (baud_selection.find(u"\u2022") == 0):
                    baud_selection = baud_selection.strip("\u2022")

                for k, name in enumerate(baud_rates):
                    baudOptionsMenu.entryconfigure(k, label=baud_rates[k])

                baudOptionsMenu.entryconfigure(i, label=baud_selection + u"\u2022")

                system.set_baud_rate(int(baud_selection))# Converts baud_rate back to int for program manipulation and sets system variable


            map_baud_rates[i] = new_command

        # Menu bar construction
        menubar = Menu(master, tearoff=0)
        tools_menu = Menu(master, tearoff=0)
        portOptionsMenu = Menu(master, tearoff=0)
        baudOptionsMenu = Menu(master, tearoff=0)

        menubar.add_cascade(label="Tools", menu=tools_menu)

        tools_menu.add_cascade(label="Ports", menu=portOptionsMenu)
        for i, name in enumerate(serial_ports):
            portOptionsMenu.add_command(label=name, command=map_serial_ports[i])

        tools_menu.add_cascade(label="Baud Rates", menu=baudOptionsMenu)
        for i, name in enumerate(baud_rates):
            baudOptionsMenu.add_command(label=name, command=map_baud_rates[i])

        master.config(menu=menubar)


        # Loads the background image
        self.image = Image.open("Space_Invaders_Background_Image.gif") # Pillow is needed to import a background image
        self.img_copy= self.image.copy()
        self.background_image = ImageTk.PhotoImage(self.image)
        self.background = Label(self, image=self.background_image)
        self.background.place(x=0,y=0,relwidth=1, relheight=1)
        self.background.bind('<Configure>', self._resize_start_window_image) # Automatically resizes the background

        # New Game Button
        self.new_game_button = Button(self, width=25, text='New Game', font=('Helvetica', 10), highlightbackground='#464646', command=_start_new_game)
        self.new_game_button.pack(expand=NO, side=BOTTOM, pady=11, padx=0)

        # Load Game Button
        self.load_game = Button(self, width=25, text='Load Game', font=('Helvetica', 10), highlightbackground='#464646', command=_load_old_game)
        self.load_game.pack(expand=NO, side=BOTTOM, pady=0, padx=0)

        # Select User Input Box
        input_options = system.get_sys_input_options()

        self.input_selection = StringVar(master)
        self.input_selection.set(input_options[0]) # initial value
        #selected_input = ttk.Combobox(self, textvariable=self.input_selection, background='#464646', state='readonly', values=input_options)
        selected_input = OptionMenu(self, self.input_selection, *input_options)
        selected_input.config(width=15, background='#464646', highlightbackground='#464646')
        selected_input.pack(expand=NO, side=BOTTOM, pady=17, padx=0)


    def _resize_start_window_image(self,event):
        new_width = event.width
        new_height = event.height

        self.image = self.img_copy.resize((new_width, new_height))

        self.background_image = ImageTk.PhotoImage(self.image)
        self.background.configure(image =  self.background_image)

class error_window(Frame):

    # Can print up to 3 errors TMT add exception for this
    # TMT add custom menu bar to error window
    def __init__(self, w, h, x, y, *args):

        def _release_window():
            error_window.grab_release()
            error_window.destroy()

        #error_window = Tk() # Create new error_window to work with Tkinter
        error_window = Toplevel()

        error_window.grab_set()

        # Adds blank menu bar
        menubar = Menu(error_window, tearoff=0)
        error_window.config(menu=menubar)

        error_window.title('Error')
        error_window.configure(background='#E9E9E9')

        # This is responsible for setting the dimensions of the window and where it is placed
        error_window.geometry('%dx%d+%d+%d' % (w/2, h/3, (x+(w/4)), (y+(y/3))))
        error_window.resizable(0,0)

        error_window.close = Button(error_window, width=5, text='Close', font=('Helvetica', 10), highlightbackground='#E9E9E9', command=_release_window)
        error_window.close.pack(expand=NO, side=BOTTOM, pady=0, padx=0)

        image = Image.open("error_image.gif")
        #resize = image.resize((64, 64), Image.ANTIALIAS)
        error_image = ImageTk.PhotoImage(image) # Keep a reference, prevent GC
        error_image_label = Label(error_window, image=error_image, background='#E9E9E9', highlightbackground='#E9E9E9')
        error_image_label.pack(side=LEFT, padx=6)

        blank_space = Label(error_window, background='#E9E9E9', highlightbackground='#E9E9E9')
        blank_space.pack(side=TOP)

        for msg, error_msg in enumerate(args):
            current_msg = Label(error_window, text=str(msg+1) + ". " + error_msg, font=('Helvetica', 12), background='#E9E9E9', highlightbackground='#E9E9E9', foreground='black', justify='left')
            current_msg.pack(side=TOP)

        error_window.mainloop()

#class game_parameters():
        #new_game, user_input_choice, user_port_selection, user_baud_selection
        #TMT make class to hold game parameters maybe?

#class calibrateCameraInput():

#class gameWindow(Frame):

#class spashScreen(Frame):
