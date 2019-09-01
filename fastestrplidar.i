/* rptrial.i */
%module fastestrplidar
%{
/* Our headers will be here */
#include <stdio.h>
#include <stdlib.h>
#include "rplidar.h"
#include <unistd.h>
#include "rptypes.h"
#include <stdint.h>
#include <string>


#include "sdkcommon.h"

#include "hal/abs_rxtx.h"
#include "hal/thread.h"
#include "hal/types.h"
#include "hal/assert.h"
#include "hal/locker.h"
#include "hal/socket.h"
#include "hal/event.h"
#include "rplidar_driver_impl.h"
#include "rplidar_driver_serial.h"
#include "rplidar_driver_TCP.h"

#include <algorithm>
#include "rplidar_driver.h"
#include "fastestrplidar.h"

using namespace rp::standalone::rplidar;

%}

%include <std_vector.i>
%include <std_string.i>
%include "rplidar_driver.h"
%include "fastestrplidar.h"

using namespace rp::standalone::rplidar;
