/* rptrial.i */
%module fastestrplidar
%{
/* Our headers will be here */
#include <stdio.h>
#include <stdlib.h>
#include "include/rplidar.h"
#include <unistd.h>
#include "include/rptypes.h"
#include <stdint.h>
#include <string>


#include "src/sdkcommon.h"

#include "src/hal/abs_rxtx.h"
#include "src/hal/thread.h"
#include "src/hal/types.h"
#include "src/hal/assert.h"
#include "src/hal/locker.h"
#include "src/hal/socket.h"
#include "src/hal/event.h"
#include "src/rplidar_driver_impl.h"
#include "src/rplidar_driver_serial.h"
#include "src/rplidar_driver_TCP.h"

#include <algorithm>
#include "include/rplidar_driver.h"
#include "fastestrplidar.h"

using namespace rp::standalone::rplidar;

%}

%include <std_vector.i>
%include <std_string.i>
%include "include/rplidar_driver.h"
%include "fastestrplidar.h"

using namespace rp::standalone::rplidar;
