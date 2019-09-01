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

#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header
#include "fastestrplidar.h"

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
		std::string FastestRplidar::fetchscandata()
		{
			
			size_t count = sizeof(nodes)/sizeof(rplidar_response_measurement_node_hq_t);
			
			// Grab a scan frame
			op_result = drv->grabScanDataHq(nodes, count);
						
			 // Initialize String output Array 
			//static std::string scan_result[(int)count];
			static std::string *scan_result = new std::string[(int)count];

			std::string output = "";
			if (IS_OK(op_result)) {
				// readjust the scan data
				drv->ascendScanData(nodes, count);
				//printf("data supposed length: %d \n",(int)count); 
					
				std::string newScan = "1";   
				//int qualitycount = 0;
				for (int pos = 0; pos < (int)count ; ++pos) {
					if ((nodes[pos].quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT) > 0)
					{  
						//qualitycount++;
						double distance = nodes[pos].dist_mm_q2/4.0f;
						double angle = nodes[pos].angle_z_q14 * 90.f / (1 << 14);
						int quality = nodes[pos].quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
						
						//std::string s = std::to_string(number);
						scan_result[pos] = newScan +","+ std::to_string(angle)+"," + std::to_string(distance)+"," + std::to_string(quality)+";";
						
						scan_result[pos] = newScan +","+ std::to_string(quality)+ "," +std::to_string(angle)+"," + std::to_string(distance) + ";";

						// reset newscan to false
						output += scan_result[pos];
						newScan = "0";
					}
					
				}
				//printf("len - %d",qualitycount);

			}
			//return scan_result;
			// remove last semicolon
			output.pop_back();
			return output;
			
		}
		
	

