import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# variable declaration
accelX = []
ay = []
az = []

gx = []
gy = []
gz = []

initX = [0]
initY = [0]
# open arduino serial port
arduinoData = serial.Serial('COM3', 115200) # update with com port

#plt.ion() # plot interactive mode
fig, ax = plt.subplots()
graph = ax.plot()

def plotAccel(frame):
    arduinoString = arduinoData.readline()
    data = arduinoString.split()
    accelX.append(float(data[0]))

    ax.set_title('Acceleration Angle Measurement vs. Time')
    #plt.ylim(-360, 360)
    ax.set_ylabel('Acceleration Angle Measurement (Hz)')

    ax.set_xlabel('Time (s)')

    ax.plot(accelX, 'b-', label='x-Axis Angle (Hz)')
    #ax.plot(ay, 'g-', label='y-Axis Angle (Hz)')
    #ax.plot(az, 'r-', label='z-Axis Angle (Hz)')

    #ax.set_legend(loc='upper right')

while True:
    arduinoString = arduinoData.readline()
    data = arduinoString.split()
    accelX.append(float(data[0]))
    ay.append(float(data[1]))
    az.append(float(data[2]))

    ax.clear()
    plotAccel(accelX)
    fig.canvas.draw()    
    #plt.pause(0.25)
    plt.show()