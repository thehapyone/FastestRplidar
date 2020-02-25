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
%template(vector_double) std::vector<double>;
%template(vector_lidar_sample) std::vector<lidar_sample>;
%template(vector_vector_double) std::vector<std::vector<double> >;


// Output typemap converts from C++object to Python object.
// Note error checking not shown for brevity.
// $1 is the C++ object, $result is the Python result.
//
%typemap(out) std::vector<std::vector<double> >* %{
    $result = PyList_New($1->size()); // Create outer Python list of correct size
    for(size_t i = 0; i < $1->size(); ++i)
    {
        auto t = PyList_New((*$1)[i].size()); // Create inner Python list of correct size for this element.
        for(size_t j = 0; j < (*$1)[i].size(); ++j) {
            PyList_SET_ITEM(t,j,PyFloat_FromDouble((*$1)[i][j]));
        }
        PyList_SET_ITEM($result,i,t);
    }
%}