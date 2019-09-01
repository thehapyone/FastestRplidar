###################################################################################################
#
#
#
####################################################################################################
# This is an helper code that will be used to communicate with the c++ lidar interface
import fastestrplidar
import signal


# defualt port for the lidar

# some possible arguments to pass to the c++ interface
# start
# gethealth
# stop
# my_scan = a set of scan data
# kill

port = "/dev/ttyUSB0"

#p1 = subprocess.Popen(['sudo ./myrplidarc ', port], bufsize=-1, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)

class RPlidar(object):
    # wrapper class for taking to c++

    def __init__(self, port="/dev/ttyUSB0"):
        self._port = port
        # import the libray used
        self.lidar = fastestrplidar.FastestRplidar()

        #self.connect()

    def connect(self):
        # now will initiatie a subprocess connection to cmd
        try:
            self.lidar.connectlidar()
        except Exception:
            print (" Can't connect to port ")

    def _disconnect(self):
        # disconnect motor
        self.lidar.stopmotor()

    def startmotor(self):
        # ensure interrupt does happen not here, so have some mutex for this code
        self.lidar.startmotor()

    def stopmotor(self):
        # stop the motor
        self._disconnect()

    def gethealth(self):
        # get heath status
        return self.lidar.checkhealth()


    def _process_scan(self):
        # extracts out the results from the sensor
        output = self.lidarpipe.stdout.readline()
        # output should be fltered
        #print (output)
        output = output.split(',',4)
        return float(output[0]), float(output[3]), float(output[1]), float(output[2])


    def iter_measurments(self):
        '''Iterate over measurments. Note that consumer must be fast enough,
        otherwise data will be accumulated inside buffer and consumer will get
        data with increaing lag
        '''
        while True:
            # also check for the buffer thing, interrupts, and threads
            data = self.lidar.fetchscandata()
            full_scan = data.split(';')
            for i in range(len(full_scan)):
                output_pre = full_scan[i].split(',', 4)
                output_post = int(output_pre[0]), int(output_pre[1]), float(output_pre[2]), float(output_pre[3])
                yield output_post

    def iter_scans(self, min_len=5):
        '''Iterate over scans. Note that consumer must be fast enough,
        otherwise data will be accumulated inside buffer and consumer will get
        data with increasing lag.
        '''
        scan = []
        # I should check if it's a new scan
        #newScan = True

        iterator = self.iter_measurments()
        for newScan, quality, angle, distance in iterator:
            if newScan == 1:
                if len(scan) > min_len:
                    yield scan
                scan = []
            scan.append((quality, angle, distance))
            #print ("dada - ", newScan)
