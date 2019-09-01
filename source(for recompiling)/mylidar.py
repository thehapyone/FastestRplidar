import fastestrplidar

lidar = fastestrplidar.FastestRplidar()

lidar.connectlidar()
lidar.startmotor()
result = lidar.fetchscandata()
print ("result - ")
print (result)
lidar.stopmotor()
