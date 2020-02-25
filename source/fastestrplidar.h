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

#include "include/rplidar.h" //RPLIDAR standard sdk, all-in-one header

using namespace rp::standalone::rplidar;

typedef struct lidar_sample
{
    double angle;
    double distance;
    int quality;
}lidar_sample;

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
class FastestRplidar {
	protected:
		// For setting scanmodes
		std::vector<RplidarScanMode> myscanModes;
		// some parameters to be set
		_u32         baudrateArray[2] = {115200, 256000};
		_u32         opt_Linecom_baudrate = 0;
		u_result     op_result;
		RPlidarDriver * drv = NULL;
		
		// Creates a device info variable
		rplidar_response_device_info_t devinfo;
		bool connectSuccess = false;

		char * com_port=NULL;
		
		// create a buffer to hold the scanned data
		rplidar_response_measurement_node_hq_t nodes[8192];
		
		/* This function is for checking the device health. 
		 * It returns true if the health is OK and false 
		 * if otherwise
		 * */
		 
		bool checkRPLIDARHealth(RPlidarDriver * drv)
		{
			u_result     op_result;
			rplidar_response_device_health_t healthinfo;

			op_result = drv->getHealth(healthinfo);
			if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
				printf("%d\n", healthinfo.status);
				if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
					 //printf("Error1\n");
					// enable the following code if you want rplidar to be reboot by software
					// drv->reset();
					return false;
				} else {
					return true;
				}

			} else {
				//fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
				//printf("Error2\n");
				return false;
			}
		}

	
	public:
	// Create the constructor: Here the driver will be created.
		FastestRplidar(char * my_port="/dev/ttyUSB0");
		~FastestRplidar();
		// Setup connection to the rplidar.
		// Connect Lidar
		void connectlidar(void);
		// destroy driver
		void destroydriver(void);
		// A wrapper code for the checkhealth status
		bool checkhealth(void);
		// stopping the motor
		void stopmotor(void);
		// starts the motor
		void startmotor(int my_scanmode = 2);
		/*
		 * This function will be used in fetching the scan data
		 * The output is a vector of lidar_samples.
		 * */
		std::vector<lidar_sample> get_scan_as_lidar_samples(bool filter_quality=false);

		/*
		 * This function will be used in fetching the scan data
		 * The output is a vector of vectors.
		 * */
		std::vector<std::vector<double>> get_scan_as_vectors(bool filter_quality=false);

		/*
		 * This function will be used in fetching the scan data
		 * The output is a vector of vectors.
		 * */
		std::vector<std::vector<double>> get_scan_as_xy(bool filter_quality=false);

};

