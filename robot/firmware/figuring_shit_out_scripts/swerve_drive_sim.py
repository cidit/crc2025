
#Importing all the modules
import turtle
from turtle import *
import math

#Creating the screen
screen=Screen()

#Creating the turtle
vector1=Turtle("classic")
vector1.speed(-1)
vector1.penup()

#Declaring all the needed variables
Vx=0
Vy=0
V=0
A=0

#Starting the while loop
while True:

    #Updating the screen for better preformence
    screen.update()

    #Carculating the velocity
    V=math.sqrt(Vx**2+Vy**2)

    #Carculating the angle
    if Vx!=0:
        A=math.degrees(math.atan(Vy/Vx))
        if Vy<0 and Vx<0 or Vy>0 and Vx<0:
            A=A-180
    elif Vy <0:
        A=270
    else:
        A=90

#Moving the turtle    
    vector1.seth(A)
    vector1.fd(V)

#Changing the values of the velocities
    Vy-=0.5
    if Vx>0:
        Vx-=0.2
    elif Vx<0:
        Vx+=0.2
