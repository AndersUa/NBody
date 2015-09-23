#ifndef CLUTIL_H
#define CLUTIL_H

#include <CL\cl.h>
#include <vector>

void PrintDeviceInfo(int deviceNo, cl_device_id deviceId);
void clErrNoAssert(cl_int err);
void GetDeviceIds(std::vector<cl_device_id>& out);


#endif