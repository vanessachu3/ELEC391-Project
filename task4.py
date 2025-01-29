# Some code to plot the 3D data given by the accelerometer and gyroscope
# on the Arduino NANO 33 BLE. We assume the data is sent over the COM port
# in the format: "ax\t ay\t az\t gx\t gy\t gz\t"

# Task 4 | Complementary filter 
# Group B5

import serial
import serial.tools.list_ports
import datetime as dt
import re
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import math

def get_COM():
    ports = serial.tools.list_ports.comports()

    for port in ports:
        if "USB Serial Device" in port.description:
            return port.device
    return None  

# This function is called periodically from FuncAnimation
def animate(i, xs, accx, accy, accz, gyx, gyy, gyz, theta, ser):
    ser.write(b'g')
    data = ser.readline().decode('ascii')
    # data_processed = data.decode("utf-8").strip('\r\n')
    data_processed = re.findall(r"[-+]?\d*\.\d+|\d+", data)
    try:
        ax, ay, az, gx, gy, gz, ghz = list(map(float, data_processed))

        k = 0.8

        # Add x and y to lists
        xs.append(dt.datetime.now().strftime('%S.%f')[:-3])

        accx.append(ax)
        accy.append(ay)
        accz.append(az)

        gyx.append(gx)
        gyy.append(gy)
        gyz.append(gz)

        ta = math.atan(ay/az)*180/np.pi
        

        if len(theta) <= 1:
            theta.append(ta)
        else:
            tg = 0+gx*1/ghz
            theta.append(k*(theta[-1]+tg) + (1-k)*ta)

        # Limit lists to 20 items
        xs = xs[-20:]

        accx = accx[-20:]
        accy = accy[-20:]
        accz = accz[-20:]

        gyx = gyx[-20:]
        gyy = gyy[-20:]
        gyz = gyz[-20:]

        theta = theta[-20:]

        print(theta[-1])

        # Draw x and other variable lists
        px[0].clear()
        px[0].plot(xs, accx)
        px[0].plot(xs, accy)
        px[0].plot(xs, accz)

        px[1].clear()
        px[1].plot(xs, theta)

        # Format plot **
        px[0].set_title("Accelerometer")
        px[0].legend(loc='upper right')
        px[0].set_ylabel("ms^-2")
        px[1].set_title("Angle")
        px[1].legend(loc='upper right')
        px[1].set_ylabel("degrees")
        px[1].set_ylim([-100,100])
        # plt.subplots_adjust()
        # plt.margins(0.2)
        plt.xticks(rotation='vertical')

    except:
        pass

# Create figure for plotting
fig, px = plt.subplots(2, sharex=True)
# X-Axis
xs = []
# Accelerometer values
accx = []
accy = []
accz = []
# Gyroscope values
gyx = []
gyy = []
gyz = []
# Theta values
theta = []

com = get_COM()
baud = 115200                   # hard-programmed

ser = serial.Serial(com, baud)  
time.sleep(2)                   # arduino serial init 

# Set up plot to call animate() function periodically; might have to change interval increment
ani = animation.FuncAnimation(fig, animate, fargs=(xs, accx, accy, accz, gyx, gyy, gyz, theta, ser), interval=10)
plt.show()
plt.close()