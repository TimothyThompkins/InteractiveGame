from tkinter import *
from PIL import Image, ImageTk

root = Tk()
root.title("Title")
root.geometry("800x600")

class Example(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.grid(sticky=N+S+E+W)
        self.image = Image.open("courbe.gif")
        self.img_copy= self.image.copy()
        self.background_image = ImageTk.PhotoImage(self.image)

        self.background = Label(self, image=self.background_image)
        self.background.grid(row =0, column =0,sticky="nsew")
        self.background.grid_rowconfigure(0, weight=1)
        self.background.grid_columnconfigure(0, weight=1)

        self.background.bind('<Configure>', self._resize_image)

    def _resize_image(self,event):
        new_width = event.width
        new_height = event.height

        self.image = self.img_copy.resize((new_width, new_height))

        self.background_image = ImageTk.PhotoImage(self.image)
        self.background.configure(image =  self.background_image)

e = Example(root)
e.grid(row =0, column =0,sticky="nsew")
e.grid_rowconfigure(0, weight=1)
e.grid_columnconfigure(0, weight=1)

root.mainloop()
