from fastestrplidar import FastestRplidar

if __name__ == '__main__':
    # Initialize the library and creates and object
    lidar = FastestRplidar()

    # connects the lidar using the default port (tty/USB0)
    lidar.connectlidar()
    # Starts the lidar motor
    lidar.startmotor(my_scanmode=0)
    # fetches the lidar scan data
    try:
        while 1:
            result = lidar.get_scan_as_vectors(filter_quality=True)
            string_result = '\n'.join(["{:10.1f}".format(x[0])+',' + "{:10.1f}".format(x[1]) for x in result])
            print(string_result)
    except:
        pass
    # done. Stops the motor
    lidar.stopmotor()