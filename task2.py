from serial.tools import list_ports
import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Obtain COM we use
#ports = list_ports.comports()
#for port in ports: print(port)

serialcom = serial.Serial("COM3",115200) #change COM if run into error

nData = 50
class AnimationPlot:

    def animate(self, i, ax_dataList,ay_dataList,az_dataList, ser):
        for n in range(nData):
            try:
                readBytes = serialcom.readline()

                decodedBytes = readBytes.decode("utf-8").strip('\r\n')
                axDataPt, ayDataPt, azDataPt = map(float, decodedBytes.split())
                print(axDataPt)
                
                ax_dataList.append(axDataPt)              # Add to the list holding the fixed number of points to animate
                ay_dataList.append(ayDataPt)              # Add to the list holding the fixed number of points to animate
                az_dataList.append(azDataPt)              # Add to the list holding the fixed number of points to animate

            except:                                             # Pass if data point is bad                               
                pass

            ax_dataList = ax_dataList[-50:]                           # Fix the list size so that the animation plot 'window' is x number of points
            ay_dataList = ay_dataList[-50:]                           # Fix the list size so that the animation plot 'window' is x number of points
            az_dataList = az_dataList[-50:]                           # Fix the list size so that the animation plot 'window' is x number of points

            ax.clear()                                          # Clear last data frame

            self.getPlotFormat()
            ax.plot(ax_dataList, label ="ax")                                   # Plot new data frame
            ax.plot(ay_dataList, label ="ay")                                   # Plot new data frame
            ax.plot(az_dataList, label ="az")                                   # Plot new data frame
            plt.legend(loc="upper left")

    def getPlotFormat(self):
        #ax.set_ylim([0, 1200])                              # Set Y axis limit of plot
        ax.set_title("Accelorometer Readings")              # Set title of figure
        ax.set_ylabel("Value [m/s^2]")                              # Set title of y axis

dataListX = []                                           # Create empty list variable for later use
dataListY = []                                           # Create empty list variable for later use
dataListZ = []                                           # Create empty list variable for later use
                                                        
fig = plt.figure()                                      # Create Matplotlib plots fig is the 'higher level' plot window
ax = fig.add_subplot(111)                               # Add subplot to main fig window

realTimePlot = AnimationPlot()

#ser =  serial.Serial("COM3",115200)                        # Establish Serial object with COM port and BAUD rate to match Arduino Port/rate
#time.sleep(2)                                           # Time delay for Arduino Serial initialization 

                                                        # Matplotlib Animation Fuction that takes takes care of real time plot.
                                                        # Note that 'fargs' parameter is where we pass in our dataList and Serial object. 
ani = animation.FuncAnimation(fig, realTimePlot.animate, frames=100, fargs=(dataListX,dataListY,dataListZ, serialcom), interval=100) 

plt.show()                                              # Keep Matplotlib plot persistent on screen until it is closed
serialcom.close()                                             # Close Serial connection when plot is closed
#def pollData():
#    while True:
#        try:
#            readByets = serialcom.readline()
#
#            decodedBytes = readByets.decode("utf-8").strip('\r\n')
#            print(decodedBytes)
#        except:
#            print("no work")

