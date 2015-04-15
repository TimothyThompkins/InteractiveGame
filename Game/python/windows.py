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
            user_input_choice = self.input_selection.get()
            self.master.destroy()

        def _load_old_game():
            user_input_choice = self.input_selection.get()
            self.master.destroy()

# TMT Left off here. Trying to get user clicked menu option. Also trying to change that option to display the one you clicked
        def _select_port(menu):
            #port_selection = self.text.get()
            menu.entryconfigure(1, label="Clicked!")
            #print(port_selection)
            print (u"\u2022")

        # Implement update method. postcommand=update
        serial_ports = get_port_options()
        baudrates = get_baud_options()

        menubar = Menu(master)
        tools_menu = Menu(master, tearoff=0)
        portOptionsMenu = Menu(master, tearoff=0)
        baudOptionsMenu = Menu(master, tearoff=0)

        menubar.add_cascade(label="Tools", menu=tools_menu)

        tools_menu.add_cascade(label="Ports", menu=portOptionsMenu)
        for name in (serial_ports):
            portOptionsMenu.add_command(label=name, command=_select_port(portOptionsMenu))

        tools_menu.add_cascade(label="Baudrate", menu=baudOptionsMenu)
        for name in (baudrates):
            baudOptionsMenu.add_command(label=name)

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

#class calibrateCameraInput():

#class gameWindow(Frame):

#class spashScreen(Frame):
