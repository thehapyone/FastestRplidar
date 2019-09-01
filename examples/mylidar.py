#!/usr/bin/env python3

'''
This code snippet demonstrate the basic use of the FastestRplidar Library. 

How it Works - 
 : Initialize the Fastestrplidar library
 : Create an instance of the Class
 : Connects to the Lidar (Uses the default port - tty/USB0)
 : Start the lidar motor
 : Fetchs a full 360 degree scan as a string. (Currently use the 8000 Samples option)
 : Stops the motor
 
 @authur : Ayo Ayibiowu
 @email : charlesayibiowu@hotmail.com
 
 Version: v1
 '''

from fastestrplidar import FastestRplidar

if __name__ == '__main__':
    # Initialize the library and creates and object
    lidar = FastestRplidar()

    # connects the lidar using the default port (tty/USB0)
    lidar.connectlidar()
    # Starts the lidar motor
    lidar.startmotor()
    # fetches the lidar scan data
    result = lidar.fetchscandata()
    print ("result - ")
    print (result)

    # done. Stops the motor
    lidar.stopmotor()
