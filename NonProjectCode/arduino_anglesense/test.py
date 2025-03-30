# Some code to plot the 3D data given by the accelerometer and gyroscope
# on the Arduino NANO 33 BLE. We assume the data is sent over the COM port
# in the format: "pidOutput angle"

# Plots the PID output and angle

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
def animate(i, xs, pidOutput, angleRead, ser):
    ser.flushInput()
    data = ser.readline().decode('ascii')
    # data_processed = data.decode("utf-8").strip('\r\n')
    data_processed = re.findall(r"[-+]?\d*\.\d+|\d+", data)
    try:
        pid, angle = list(map(float, data_processed))

        # print("%s\t%s" % (pid, angle))
    
        # Add x and y to lists
        xs.append(dt.datetime.now().strftime('%S.%f')[:-3])

        pidOutput.append(pid)
        angleRead.append(angle)
        

        # Limit lists to 20 items
        xs = xs[-20:]
        pidOutput = pidOutput[-20:]
        angleRead = angleRead[-20:]
        

        # Draw x and other variable lists
        px[0].clear()
        px[0].plot(xs, pidOutput)

        px[1].clear()
        px[1].plot(xs, angleRead)

    

        # Format plot **
        px[0].set_title("PID Output")
        # px[0].legend(loc='upper right')
        px[0].set_ylabel("u0")
        px[0].set_ylim([-1.1,1.1])
        px[1].set_title("Angle")
        # px[1].legend(loc='upper right')
        px[1].set_ylabel("degrees")
        px[1].set_ylim([-45,45])

        # plt.margins(0.2)
        plt.xticks(rotation='vertical')
    except:
        pass

# Create figure for plotting
fig, px = plt.subplots(2, sharex=True)
# variable arrays
xs = []
pidOutput = []
angleRead = []


com = get_COM()
baud = 115200                   # hard-programmed

ser = serial.Serial(com, baud)  
time.sleep(2)                   # arduino serial init time

# Set up plot to call animate() function periodically; might have to change interval increment
ani = animation.FuncAnimation(
    fig, animate, fargs=(xs, pidOutput, angleRead, ser), interval=10, cache_frame_data=False
)

plt.show()
plt.close()
ser.close()