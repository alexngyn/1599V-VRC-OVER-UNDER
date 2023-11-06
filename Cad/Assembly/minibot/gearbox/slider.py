# Written by Brandt Dillon, VexU team NJIT alumni, 2021
# This program maps motor cap colors onto a rainbow scale based on rpm

from tkinter import *
import math

#function to display data to python consol
def show_values():
    print (str(rpm2rgb(x)))

#function to convert rgb tuple to tkinter format
def _from_rgb(rgb):
    return "#%02x%02x%02x" % rgb  

#function to convert input of rpm to rgb scale
def rpm2rgb(x):
    
    x = x + 5    #use for slight color correction shift

    #converts rpm value into x position on rgb rainbow scale
    if(x < 176.078):
        x = 0.04 * x - 4
    elif(x >= 176.078 and x <= 1589.42):
        x = .4301* math.log(x, math.e) + .8191
    elif(x > 1589.42):
        x = 0.001 * x + 2.4

    #convets x position on raiinbow scale into r g b values
    r = int(-21.417*x**3 + 309.21*x**2 - 1386.8*x + 1961.6)
    g = int(-21.333*x**3 + 64.286*x**2 + 84.19*x - 0.0286)
    b = int(97.667*x**3 - 1362*x**2 + 6175.3*x - 8905)

    #keeps values within range
    if(r > 255):r = 255
    if(b > 255):b = 255
    if(g > 255):g = 255
    if(r < 0):r = 0
    if(b < 0):b = 0
    if(g < 0):g = 0

    rgb = (r, g, b)
    return(rgb)

#setting up the interface
master = Tk()
master.title('Motor Cart Color')

w2 = Scale(master, from_=100, to=3600, orient=HORIZONTAL)
w2.set(100)
w2.pack(ipadx=200,ipady=20)

#creates rainbow rpm rainbow scale
canvas = Canvas(master, height = 50)

for i in range (0,500): 
    color = _from_rgb(rpm2rgb(100+int(7*i)))
    canvas.create_rectangle(i, 10, i, 50, outline= color, fill=color)

canvas.pack(ipadx=50,ipady=0)

#adds print button
butt = Button(master, text="Print", command=show_values).pack(ipadx=5,ipady=1)

#adds text box
T = Text(master, height=1, width=4)
T.place(height = 20,width = 40,x=180, y =140)

#adds label
lab = Label(master, text="[R, G, B]")
lab.place(height = 20,width = 90,x=290, y =140)

xold = 0

#task that repeates every 50ms
def task():
    global xold
    global x
    
    #gets value of slider
    x = w2.get()

    #gets text value
    if(T.get("1.0", END).strip().isdigit() == False):
        Text = "100"
    else:
        Text = T.get("1.0", END)

    #gets text and modifies slider position if text is changed 
    if((Text.isspace() == False) and (xold !=  int(Text))):
        xold = int(Text)
        w2.set(xold)

    lab.config(text="RGB: "+str(rpm2rgb(x)))
    w2.configure(background= _from_rgb(rpm2rgb(x)))
    master.after(50, task)

master.after(50, task)
mainloop()
