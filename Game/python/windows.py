from tkinter import *
from tkinter import ttk
from PIL import Image, ImageTk
from config import sys_config

class startWindow(Frame):

    def __init__(self, master, system, *pargs):
        Frame.__init__(self, master, *pargs)


        def _start_new_game():
            input_choice = self.var.get()
            self.master.destroy()


        def _load_old_game():
            input_choice = self.var.get()
            self.master.destroy()

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

        self.var = StringVar(master)
        self.var.set(input_options[0]) # initial value
        selected_input = ttk.Combobox(self, textvariable=self.var, state='readonly', values = input_options)
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
