from tkinter import *
from windows import StartWindow


root = Tk() # Create new root to work with Tkinter
root.title("Alien Invaders")
#root.geometry("1024x624+300+300") # Initial size of window
root.geometry("512x320+300+300") # Initial size of window
#root.geometry("320x240+300+300") # Initial size of window
root.resizable(0,0)

initial = StartWindow(root)
initial.pack(fill=BOTH, expand=YES)

root.mainloop()


# def main():
#
#     root = Tk()
#     root.geometry("250x150+300+300")
#     app = Example(root)
#     root.mainloop()
#
#
# if __name__ == '__main__':
#     main()
