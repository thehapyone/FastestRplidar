#!/usr/bin/env python3

'''

 - Animates distances and measurment quality
 - Saves scan data to a file
 
 @authur : Ayo Ayibiowu
 @email : charlesayibiowu@hotmail.com
    
'''

from myRplidar import RPlidar
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
import time

# PORT_NAME is not used. The default port (ttyUSB0) is used.
PORT_NAME = '/dev/ttyUSB0'
DMAX = 4000
IMIN = 50
IMAX = 500

# track scans
scan_no = 0
scan_len = 300 # looking for a scan with a threashold above 200 to use as the ground truth data.

x_data = []
y_data = []
save_scan = False

saved = False

def update_line(num, iterator, line):
    scan = next(iterator)
    global save_scan, saved
    
    # enable or disable saving of scan data to file
    if len(scan) >= scan_len:
        # To be able to save scan data to file. Change this to True. i.e save_scan = True
        save_scan = False
        if saved:
            save_scan = False

    offsets2 = np.array([(np.radians(meas[1]), meas[2]) for meas in scan])
    # convert polar to Cartesian
    offsets = np.array([(meas[2]*np.sin(np.radians(meas[1])), meas[2]*np.cos(np.radians(meas[1]))) for meas in scan])

    # saves scan data
    if save_scan:
        if offsets2.shape[0] >= 300:
            f = open("lidar_scan_data.txt", "wb")
            # save to file
            np.savetxt(f, offsets2)
            f.close()
            print ('save scan data')
            saved = True
            exit(4)

    line.set_offsets(offsets)
    intens = np.array([meas[0] for meas in scan])
    line.set_array(intens)
    return line,

def run():
    lidar = RPlidar(PORT_NAME)
    fig = plt.figure()
    #ax = plt.subplot(111, projection='polar')
    ax = plt.subplot(111) # X,Y coordinates

    line = ax.scatter([0, 0], [0, 0], s=10, c=[IMIN, IMAX],
                           cmap=plt.cm.Greys_r, lw=0)
    #ax.set_rmax(DMAX)
    ax.set_xlim(-5000, 5000)
    ax.set_ylim(-5000, 5000)

    ax.grid(True)
    # Connect the robot
    lidar.connect()
    lidar.startmotor()

    # try and plot the orgin
    ax.plot(0,0, 'r*')
    iterator = lidar.iter_scans()
    ani = animation.FuncAnimation(fig, update_line,
        fargs=(iterator, line), interval=50)
    plt.show()
    lidar.stopmotor()

if __name__ == '__main__':
    run()
    #lidar.stopmotor()
