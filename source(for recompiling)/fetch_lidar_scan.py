from fastestrplidar import FastestRplidar

if __name__ == '__main__':
    # Initialize the library and creates and object
    lidar = FastestRplidar()

    # connects the lidar using the default port (tty/USB0)
    lidar.connectlidar()
    # Starts the lidar motor
    lidar.startmotor(my_scanmode=0)
    # fetches the lidar scan data
    result = lidar.fetchscandata()
    print ("result - ")
    print (result)

    # done. Stops the motor
    lidar.stopmotor()