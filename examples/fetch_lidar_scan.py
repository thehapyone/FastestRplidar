#!/usr/bin/env python3

'''
This code snippet demonstrate how to use the myRplidar library for 
existing codes that depends on the RPLidar Python library (https://github.com/SkoltechRobotics/rplidar) or 
just want to get started using this Library.


 
 @authur : Ayo Ayibiowu
 @email : charlesayibiowu@hotmail.com
 
 Version: v1
 '''

from myRplidar import RPlidar

# uses the default port - /dev/ttyUSB0
lidar = RPlidar()

if __name__ == '__main__':

    # connects the lidar using the default port (/dev/ttyUSB0)
    lidar.connect()
    
    # Gets the state of the lidar health
    health = lidar.get_health()
    print("Health - ", health)
    
    # connects the lidar using the default port (tty/USB0)
    lidar.connect()
    # Starts the lidar motor
    lidar.startmotor()
    
    for i, scan in enumerate(lidar.iter_scans()):
        print('%d: Got %d measurments' % (i, len(scan)))
        print (scan)
        if i > 10:
            break

    # done. Stops the motor
    lidar.stopmotor()
    
    print ("done")
