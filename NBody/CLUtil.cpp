#include "CLUtil.h"
#include <stdio.h>
#include <stdlib.h>

void PrintDeviceInfo(int deviceNo, cl_device_id deviceId)
{
	size_t valueSize = 128;
	char* value = new char[valueSize];

	cl_uint maxComputeUnits;

	printf("Device # %d\n", deviceNo);
	clGetDeviceInfo(deviceId, CL_DEVICE_NAME, valueSize, value, nullptr);
	printf("Device: %s\n", value);

	clGetDeviceInfo(deviceId, CL_DEVICE_VERSION, valueSize, value, nullptr);
	printf("Hardware version: %s\n", value);

	clGetDeviceInfo(deviceId, CL_DRIVER_VERSION, valueSize, value, nullptr);
	printf("Software version: %s\n", value);

	clGetDeviceInfo(deviceId, CL_DEVICE_OPENCL_C_VERSION, valueSize, value, nullptr);
	printf("OpenCL C version: %s\n", value);
	cl_device_type device_type;
	clGetDeviceInfo(deviceId, CL_DEVICE_TYPE, valueSize, &device_type, nullptr);
	switch (device_type)
	{
	case CL_DEVICE_TYPE_GPU:
		printf("OpenCL dev type: GPU\n");
		break;
	case CL_DEVICE_TYPE_CPU:
		printf("OpenCL dev type: CPU\n");
		break;
	case CL_DEVICE_TYPE_ACCELERATOR:
		printf("OpenCL dev type: ACCELERATOR\n");
		break;
	}

	clGetDeviceInfo(deviceId, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, nullptr);
	printf("Parallel compute units: %d \n\n", maxComputeUnits);

	delete[] value;
}


void clErrNoAssert(cl_int err)
{
	if (err != 0)
	{
		switch (err)
		{
		case CL_SUCCESS:                            printf("Success!\n"); break;
		case CL_DEVICE_NOT_FOUND:                   printf("Device not found.\n"); break;
		case CL_DEVICE_NOT_AVAILABLE:               printf("Device not available\n"); break;
		case CL_COMPILER_NOT_AVAILABLE:             printf("Compiler not available\n"); break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:      printf("Memory object allocation failure\n"); break;
		case CL_OUT_OF_RESOURCES:                   printf("Out of resources\n"); break;
		case CL_OUT_OF_HOST_MEMORY:                 printf("Out of host memory\n"); break;
		case CL_PROFILING_INFO_NOT_AVAILABLE:       printf("Profiling information not available\n"); break;
		case CL_MEM_COPY_OVERLAP:                   printf("Memory copy overlap\n"); break;
		case CL_IMAGE_FORMAT_MISMATCH:              printf("Image format mismatch\n"); break;
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:         printf("Image format not supported\n"); break;
		case CL_BUILD_PROGRAM_FAILURE:              printf("Program build failure\n"); break;
		case CL_MAP_FAILURE:                        printf("Map failure\n"); break;
		case CL_INVALID_VALUE:                      printf("Invalid value\n"); break;
		case CL_INVALID_DEVICE_TYPE:                printf("Invalid device type\n"); break;
		case CL_INVALID_PLATFORM:                   printf("Invalid platform\n"); break;
		case CL_INVALID_DEVICE:                     printf("Invalid device\n"); break;
		case CL_INVALID_CONTEXT:                    printf("Invalid context\n"); break;
		case CL_INVALID_QUEUE_PROPERTIES:           printf("Invalid queue properties\n"); break;
		case CL_INVALID_COMMAND_QUEUE:              printf("Invalid command queue\n"); break;
		case CL_INVALID_HOST_PTR:                   printf("Invalid host pointer\n"); break;
		case CL_INVALID_MEM_OBJECT:                 printf("Invalid memory object\n"); break;
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    printf("Invalid image format descriptor\n"); break;
		case CL_INVALID_IMAGE_SIZE:                 printf("Invalid image size\n"); break;
		case CL_INVALID_SAMPLER:                    printf("Invalid sampler\n"); break;
		case CL_INVALID_BINARY:                     printf("Invalid binary\n"); break;
		case CL_INVALID_BUILD_OPTIONS:              printf("Invalid build options\n"); break;
		case CL_INVALID_PROGRAM:                    printf("Invalid program\n"); break;
		case CL_INVALID_PROGRAM_EXECUTABLE:         printf("Invalid program executable\n"); break;
		case CL_INVALID_KERNEL_NAME:                printf("Invalid kernel name\n"); break;
		case CL_INVALID_KERNEL_DEFINITION:          printf("Invalid kernel definition\n"); break;
		case CL_INVALID_KERNEL:                     printf("Invalid kernel\n"); break;
		case CL_INVALID_ARG_INDEX:                  printf("Invalid argument index\n"); break;
		case CL_INVALID_ARG_VALUE:                  printf("Invalid argument value\n"); break;
		case CL_INVALID_ARG_SIZE:                   printf("Invalid argument size\n"); break;
		case CL_INVALID_KERNEL_ARGS:                printf("Invalid kernel arguments\n"); break;
		case CL_INVALID_WORK_DIMENSION:             printf("Invalid work dimension\n"); break;
		case CL_INVALID_WORK_GROUP_SIZE:            printf("Invalid work group size\n"); break;
		case CL_INVALID_WORK_ITEM_SIZE:             printf("Invalid work item size\n"); break;
		case CL_INVALID_GLOBAL_OFFSET:              printf("Invalid global offset\n"); break;
		case CL_INVALID_EVENT_WAIT_LIST:            printf("Invalid event wait list\n"); break;
		case CL_INVALID_EVENT:                      printf("Invalid event\n"); break;
		case CL_INVALID_OPERATION:                  printf("Invalid operation\n"); break;
		case CL_INVALID_GL_OBJECT:                  printf("Invalid OpenGL object\n"); break;
		case CL_INVALID_BUFFER_SIZE:                printf("Invalid buffer size\n"); break;
		case CL_INVALID_MIP_LEVEL:                  printf("Invalid mip-map level\n"); break;
		default: printf("Unknown\n"); break;
		}
		system("PAUSE");
		exit(1);
	}


}

void GetDeviceIds(std::vector<cl_device_id>& out)
{
	cl_uint platformCount;
	cl_platform_id* platforms;

	cl_uint deviceCount;
	cl_device_id* devices;

	clGetPlatformIDs(0, nullptr, &platformCount);
	platforms = new cl_platform_id[platformCount];
	clGetPlatformIDs(platformCount, platforms, nullptr);

	for (unsigned int i = 0; i < platformCount; i++)
	{
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
		devices = new cl_device_id[deviceCount];
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, nullptr);

		for (unsigned int j = 0; j < deviceCount; j++)
		{
			out.push_back(devices[j]);
		}
		delete[] devices;
	}
	delete[] platforms;
}