from tkinter import *
from tkinter import ttk
from PIL import Image, ImageTk
from config import sys_config
from serial_comms import get_port_options
from serial_comms import get_baud_options

class startWindow(Frame):

    def __init__(self, master, system, *pargs):
        Frame.__init__(self, master, *pargs)

        def _start_new_game():
            new_game = True
            user_input_choice = self.input_selection.get()
            system.set_game_options(new_game, user_input_choice, user_port_selection, user_baud_selection)
            #system.set_game_options(new_game, user_input_choice, user_baud_selection)
            #TMT add error handling here in case user doesn't select baud rate and serial port
            self.master.destroy()

        def _load_old_game():
            new_game = False
            user_input_choice = self.input_selection.get()
            self.master.destroy()

        #def _set_user_port_selection():

        #def _set_user_baud_selection(user_baud_selection):
        #    user_baud_selection = user_baud_selection

        serial_ports = get_port_options()
        baud_rates = get_baud_options()

        map_serial_ports = {}
        map_baud_rates = {}

        #user_port_selection = None
        #user_baud_selection = None


        # Creates a new function for each entry in the tool bar menu options. Each function has the same name & is stored in map_X[]
        for i, name in enumerate(serial_ports):
            def new_command(i=i):
                port_selection = portOptionsMenu.entrycget(i,'label')

                #This if just removes the bullet character if the user reselects the same port
                if (port_selection.find(u"\u2022") == 0):
                    port_selection = port_selection.strip("\u2022")

                for k, name in enumerate(serial_ports):
                    portOptionsMenu.entryconfigure(k, label=serial_ports[k])

                portOptionsMenu.entryconfigure(i, label=port_selection + u"\u2022")

                user_port_selection = port_selection
                #return user_port_selection

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

                baud_selection = int(baud_selection) # Converts baud_rate back to int for program manipulation
                user_baud_selection = baud_selection

                #_set_user_baud_selection(user_baud_selection)

            map_baud_rates[i] = new_command


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


        # This loads the background image
        self.image = Image.open("Space_Invaders_Background_Image.gif") # Pillow is needed to import a background image
        self.img_copy= self.image.copy()
        self.background_image = ImageTk.PhotoImage(self.image)
        self.background = Label(self, image=self.background_image)
        self.background.place(x=0,y=0,relwidth=1, relheight=1)
        self.background.bind('<Configure>', self._resize_start_window_image)# Automatically resizes the background

        # New Game Button
        self.new_game_button = Button(self, width=25, text='New Game', font=('Helvetica', 10), command=_start_new_game)
        self.new_game_button.pack(expand=NO, side=BOTTOM, pady=11, padx=0)
        #self.new_game_button.bind('<Button-1>', self._start_new_game)

        # Load Game Button
        self.load_game = Button(self, width=25, text='Load Game', font=('Helvetica', 10), command=_load_old_game)
        self.load_game.pack(expand=NO, side=BOTTOM, pady=0, padx=0)
        #self.load_game.bind('<Button-1>', self._load_old_game)

        # Select User Input Box
        input_options = system.get_sys_input_options()

        self.input_selection = StringVar(master)
        self.input_selection.set(input_options[0]) # initial value
        selected_input = ttk.Combobox(self, textvariable=self.input_selection, state='readonly', values=input_options)
        selected_input.current(0)
        selected_input.pack(expand=NO, side=BOTTOM, pady=17, padx=0)


    def _resize_start_window_image(self,event):

        new_width = event.width
        new_height = event.height

        self.image = self.img_copy.resize((new_width, new_height))

        self.background_image = ImageTk.PhotoImage(self.image)
        self.background.configure(image =  self.background_image)

class game_parameters():
        #new_game, user_input_choice, user_port_selection, user_baud_selection
        #TMT make class to hold game parameters maybe?

#class calibrateCameraInput():

#class gameWindow(Frame):

#class spashScreen(Frame):
