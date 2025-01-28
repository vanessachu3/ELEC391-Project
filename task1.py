# Some code to plot the 3D data given by the accelerometer and gyroscope
# on the Arduino NANO 33 BLE. We assume the data is sent over the COM port
# in the format: "ax\t ay\t az\t gx\t gy\t gz\t"

import serial
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Create figure for plotting
fig, ax = plt.subplots(2)
xs = []

accx = []
accy = []
accz = []

gyx = []
gyy = []
gyz = []

ports = serial.tools.list_ports.comports()

if "Arduino Nano 33 BLE" in ports:
    # extract COM port and assign to 'com' variable
    com = 14
    baud = 115200

ser = serial.Serial(com, baud)    

# This function is called periodically from FuncAnimation
def animate(xs, accx, accy, accz, gyx, gyy, gyz):

    ser.flush()
    data = ser.readline()
    data_processed = data.decode("utf-8").strip('\r\n')
    ax, ay, az, gx, gy, gz = map(float, data_processed.split())

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%S.%f')[:-3])

    accx.append(ax)
    accy.append(ay)
    accz.append(az)

    gyx.append(gx)
    gyy.append(gy)
    gyz.append(gz)

    # Limit x and y lists to 20 items
    xs = xs[-20:]

    accx = accx[-20:]
    accy = accy[-20:]
    accz = accz[-20:]

    gyx = gyx[-20:]
    gyy = gyy[-20:]
    gyz = gyz[-20:]

    # Draw x and other variable lists
    ax[0].clear()
    ax[0].plot(xs, accx)
    ax[0].plot(xs, accy)
    ax[0].plot(xs, accz)

    ax[1].clear()
    ax[1].plot(xs, gyx)
    ax[1].plot(xs, gyy)
    ax[1].plot(xs, gyz)

    # Format plot ** needs to be changed to reflect 2 plots
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Accelerometer')
    plt.ylabel('ms^-2')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, accx, accy, accz, gyx, gyy, gyz),
                               interval=10)
plt.show()
plt.close()