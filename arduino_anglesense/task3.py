# Some code to plot the 3D data given by the accelerometer and gyroscope
# on the Arduino NANO 33 BLE. We assume the data is sent over the COM port
# in the format: "accelerometer_angle gyroscope_angle current_angle"

# Task 3 | Plot Gyroscope Data

import serial
import serial.tools.list_ports
import datetime as dt
import re
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def get_COM():
    ports = serial.tools.list_ports.comports()

    for port in ports:
        if "USB Serial Device" in port.description:
            return port.device
    return None  

# This function is called periodically from FuncAnimation
def animate(i, xs, accAngle, gyrAngle, currAngle, ser):
    ser.flushInput()
    data = ser.readline().decode('ascii')
    # data_processed = data.decode("utf-8").strip('\r\n')
    data_processed = re.findall(r"[-+]?\d*\.\d+|\d+", data)
    try:
        acc, gyr, curr = list(map(float, data_processed))

        print("%s\t%s\t%s" % (acc, gyr, curr))
    
        # Add x and y to lists
        xs.append(dt.datetime.now().strftime('%S.%f')[:-3])

        accAngle.append(acc)
        gyrAngle.append(gyr)
        currAngle.append(curr)

        # Limit lists to 20 items
        xs = xs[-20:]
        accAngle = accAngle[-20:]
        gyrAngle = gyrAngle[-20:]
        currAngle = currAngle[-20:]

        # Draw x and other variable lists
        px[0].clear()
        px[0].plot(xs, accAngle)

        px[1].clear()
        px[1].plot(xs, gyrAngle)

        px[2].clear()
        px[2].plot(xs, currAngle)

        # Format plot **
        px[0].set_title("Accelerometer Angle")
        # px[0].legend(loc='upper right')
        px[0].set_ylabel("degrees")
        px[0].set_ylim([-100,100])
        px[1].set_title("Gyroscope Angle")
        # px[1].legend(loc='upper right')
        px[1].set_ylabel("degrees")
        px[1].set_ylim([-100,100])
        px[2].set_title("Filtered Angle")
        px[2].set_ylabel("degrees")
        px[2].set_ylim([-100,100])
        # plt.margins(0.2)
        plt.xticks(rotation='vertical')
    except:
        pass

# Create figure for plotting
fig, px = plt.subplots(3, sharex=True)
# variable arrays
xs = []
accAngle = []
gyrAngle = []
currAngle = []

com = get_COM()
baud = 115200                   # hard-programmed

ser = serial.Serial(com, baud)  
time.sleep(2)                   # arduino serial init time

# Set up plot to call animate() function periodically; might have to change interval increment
ani = animation.FuncAnimation(fig, animate, fargs=(xs, accAngle, gyrAngle, currAngle, ser), interval=10)
plt.show()
plt.close()
ser.close()