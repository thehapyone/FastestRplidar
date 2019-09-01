#!/usr/bin/env python3

'''Animates distances and measurment quality'''
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
    if len(scan) >= scan_len:
        save_scan = False
        if saved:
            save_scan = False

    #for meas in scan:
        #print (means[0], means[1], means[2])
        #print (meas[2]*np.cos(np.radians(meas[1])), meas[2]*np.sin(np.radians(meas[1])))
    offsets2 = np.array([(np.radians(meas[1]), meas[2]) for meas in scan])
    # convert polar to Cartesian
    offsets = np.array([(meas[2]*np.sin(np.radians(meas[1])), meas[2]*np.cos(np.radians(meas[1]))) for meas in scan])

    if save_scan:
        if offsets2.shape[0] >= 300:
            f = open("scan9.txt", "wb")
            #print (offsets2)
            #print (offsets2.shape)
            #print (offsets2.dtype)
            #np.savetxt(f, offsets, delimiter=';', fmt='%s', newline='\n')
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

def run2():
    lidar = RPlidar(PORT_NAME)
    # Connect the robot
    lidar.connect()
    lidar.startmotor()
    iterator = lidar.iter_scans()
    while True:
        old = time.time()
        scan = next(iterator)
        new = time.time()
        delta = new - old
        print('%.2f Hz, %.2f RPM' % (1 / delta, 60 / delta))
        #for means in scan:
            #print (means[0], means[1], means[2])


if __name__ == '__main__':
    run()
    #lidar.stopmotor()
