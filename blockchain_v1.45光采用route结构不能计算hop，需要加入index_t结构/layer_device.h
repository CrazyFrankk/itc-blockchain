#pragma once

//include
#include "layer.h"
//define
#define DEVICE_STATUS_FREE 0
#define DEVICE_STATUS_MASTER 1
#define DEVICE_STATUS_SLAVE 2

#define DEVICE_NODE_HEAVY 0
#define DEVICE_NODE_LIGHT 1

#define DEVICE_LINE_ON 0
#define DEVICE_LINE_OFF 1

#define MAX_MASTER_SLAVE 5 //maybe 7
#define MIN_MASTER_SLAVE 3

#define MAX_METRIC 20 //�����豸�����Ӿ������뾶10m(����������������)

#define MAX_MESH_DEVICE 10 //һ��mesh�е�����豸��(light_device+heavy_device),mesh����������ӳ���65000���豸
#define MIN_MESH_HEAVYDEVICE 1 //һ��mesh�е���С���豸��(heavy_device)
//typedef
//struct
//function
/*
void route_mark(device_t *device,uint32 device_index);
route_t *route_check(device_t *device);
route_t *route_exist(device_t *device,uint32 index);







void device_recurse(device_t *device);
void device_location(device_t *device);
void device_release(device_t *device);
void print_status(void);
void print_route(void);
*/


void route_insert(device_t *device,route_t *route);
uint8 device_seek(device_t *device);
void process_device(device_t *device);
