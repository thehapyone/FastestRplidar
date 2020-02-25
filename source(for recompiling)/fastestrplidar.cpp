/*
 *  RPLIDAR
 *  FastestRplidar C++ Simple Data Grabber App
 *
 *  Copyright (c) 2019 by Ayo Ayibiowu
 *  charlesayibiowu@hotmail.com
 *  https://github.com/thehapyone
 *
 */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

#include "include/rplidar.h" //RPLIDAR standard sdk, all-in-one header
#include "fastestrplidar.h"

constexpr double deg2rad = M_PI/180.0;

//using namespace rp::standalone::rplidar;

/* Function to get device info
 * Future updates should provide support for this. 
 * This is just a dummy code
 * 
 
string getInfo()
{
	// print out the device serial number, firmware and hardware version number..
    printf("RPLIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }
     

    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);

}
*/



// Creating a class to manage everything

// Create the constructor: Here the driver will be created.
FastestRplidar::FastestRplidar(char * my_port)
{
    // create/update the driver instance
    drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
    // Update the port
    com_port = my_port;
}
FastestRplidar::~FastestRplidar()
{
    RPlidarDriver::DisposeDriver(drv);
    drv = NULL;
}

// Setup connection to the rplidar.
// Connect Lidar
void FastestRplidar::connectlidar (void)
{
    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    // make connection...
    size_t baudRateArraySize = (sizeof(baudrateArray))/ (sizeof(baudrateArray[0]));
    for(size_t i = 0; i < baudRateArraySize; ++i)
    {
        if(!drv)
            drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
        if(IS_OK(drv->connect(com_port, baudrateArray[i])))
        {
            op_result = drv->getDeviceInfo(devinfo);

            if (IS_OK(op_result))
            {
                connectSuccess = true;
                break;
            }
            else
            {
                delete drv;
                drv = NULL;
            }
        }
    }

    if (!connectSuccess) {

        fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
            , com_port);
        // since failed. It should destroy the created driver
        destroydriver();
    }

}

// destroy driver if error exist
void FastestRplidar::destroydriver(void)
{
    RPlidarDriver::DisposeDriver(drv);
    drv = NULL;
}

// A wrapper code for the checkheatlh status
bool FastestRplidar::checkhealth()
{
    return checkRPLIDARHealth(drv);
}

// for stopping the motor
void FastestRplidar::stopmotor()
{
    drv->stop();
    drv->stopMotor();
    destroydriver();

}


// This function starts the motor and also begins scan
// It requires the scanmode to use: 3 scan modes are supported: 1,2,3
// Default mode is 2
void FastestRplidar::startmotor(int my_scanmode)
{
    // Starts the motor.
    drv->startMotor();
    //Fetching supported scan modes
    //RplidarScanMode scanMode;
    drv->getAllSupportedScanModes(myscanModes);

    // start scan...
    //drv->startScan(0,1);
    drv->startScanExpress(false, myscanModes[my_scanmode].id);
}


/*
 * This function will be used in fetching the scan data
 * The output is a formatted string.
 * Output: "newscan,angle,distance,quality"
 * */
std::vector<lidar_sample> FastestRplidar::get_scan_as_lidar_samples(bool filter_quality)
{
    int quality; // holder for quality of sample
    lidar_sample new_point;
    std::vector<lidar_sample> output;  // vector of vectors for converted data

    size_t count = sizeof(nodes)/sizeof(rplidar_response_measurement_node_hq_t);

    // Grab a scan frame
    op_result = drv->grabScanDataHq(nodes, count);

    if (IS_OK(op_result)) {
        // readjust the scan data
        drv->ascendScanData(nodes, count);
        output.reserve(count); // allocate space for data

        for (int pos = 0; pos < (int)count ; pos++) {
            quality = nodes[pos].quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
            if (quality > 0 || !filter_quality)
            {
                new_point.distance = nodes[pos].dist_mm_q2/4.0f;
                new_point.angle = nodes[pos].angle_z_q14 * 90.f / (1 << 14);
                new_point.quality = quality;
                output.push_back(new_point);
            }
        }
    }
    return output;
}

std::vector<std::vector<double>> FastestRplidar::get_scan_as_vectors(bool filter_quality)
{
    int quality; // holder for quality of sample
    std::vector<double> sample(3);
    std::vector<std::vector<double>> output;  // vector of vectors for converted data

    size_t count = sizeof(nodes)/sizeof(rplidar_response_measurement_node_hq_t);

    // Grab a scan frame
    op_result = drv->grabScanDataHq(nodes, count);
    output.reserve(count); // allocate space for data

    if (IS_OK(op_result)) {
        // readjust the scan data
        drv->ascendScanData(nodes, count);

        for (int pos = 0; pos < (int)count ; pos++) {
            quality = nodes[pos].quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
            if (quality > 0 || !filter_quality)
            {
                sample.at(0) = nodes[pos].angle_z_q14 * 90.f / (1 << 14);
                sample.at(1) = nodes[pos].dist_mm_q2/4.0f;
                sample.at(2) = quality;
                output.push_back(sample);
            }
        }
    }
    return output;
}

std::vector<std::vector<double>> FastestRplidar::get_scan_as_xy(bool filter_quality)
{
    std::vector<std::vector<double>> points = get_scan_as_vectors(filter_quality);

    // allocate output size
    std::vector<std::vector<double>> output(points.size(), std::vector<double>(2));

    for (unsigned int i = 0; i<points.size(); i++)
    {
        output.at(i).at(0) = std::cos(deg2rad*points.at(i).at(0)) * points.at(i).at(1);
        output.at(i).at(1) = std::sin(deg2rad*points.at(i).at(0)) * points.at(i).at(1);
    }
    return output;
}

	

