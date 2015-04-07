from tkinter import *
from tkinter import ttk
from PIL import Image, ImageTk

class StartWindow(Frame):

    def __init__(self, master, *pargs):
        Frame.__init__(self, master, *pargs)

        # This loads the background image
        self.image = Image.open("Space_Invaders_Background_Image.gif") # Pillow is needed to import a background image
        self.img_copy= self.image.copy()
        self.background_image = ImageTk.PhotoImage(self.image)
        self.background = Label(self, image=self.background_image)
        self.background.place(x=0,y=0,relwidth=1, relheight=1)
        self.background.bind('<Configure>', self._resize_image)# Automatically resizes the background

        # New Game Button
        new_game = Button(self, width=25, text='New Game', font=('Helvetica', 10))
        new_game.pack(expand=NO, side=BOTTOM, pady=11, padx=0)

        # Load Game Button
        load_game = Button(self, width=25, text='Load Game', font=('Helvetica', 10))
        load_game.pack(expand=NO, side=BOTTOM, pady=0, padx=0)

        # Select User Input Box
        #if platform is pc these are the optoins
        input_options = ['Arrow Keys','Web Cam']
        #else
        #input_options = ['Tilt Sensor']

        self.var = StringVar(master)
        self.var.set(input_options[0]) # initial value
        selected_input = ttk.Combobox(self, textvariable=self.var, state='readonly', values = input_options)
        selected_input.current(0)
        selected_input.pack(expand=NO, side=BOTTOM, pady=17, padx=0)


    def _resize_image(self,event):

        new_width = event.width
        new_height = event.height

        self.image = self.img_copy.resize((new_width, new_height))

        self.background_image = ImageTk.PhotoImage(self.image)
        self.background.configure(image =  self.background_image)
