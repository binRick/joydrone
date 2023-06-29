#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libusb.h>
#include <hidapi.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#if defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
 #include <hidapi_darwin.h>
#endif
#include "arduinoserialposix.h"

#define VENDOR_ID 0x046d
#define PRODUCT_ID 0xc215
#define DEBUG_JOYSTICK_CHANNELS false

void print_devs(libusb_device **devs);
void monitor_usb(void);
int scale_value(int value);
bool ensure_root(void);
int write_json_string(char *json_string);
bool setup_serial_port(void);

int status, bytes_read, data_length, x;
char serial_port[1024];
int serial_port_descriptor = 0;
 
int main(int argc, const char * argv[]){
    libusb_device **devs;
    libusb_context *context = NULL;

    size_t list;
    int ret, x;
	
    if( argc != 2 ){
        fprintf( stderr, "ERROR: First argument must be serial port\n" );
        return 1;
    }
    sprintf(&serial_port,argv[1]);
    printf("Using serial port %s\n",serial_port);

    if((ret = libusb_init(&context))<0){
        perror("libusb_init");
        exit(1);
    }

    if((list = libusb_get_device_list(context, &devs))<1){
	printf("No joystick found!\n");
	exit(1);
    }
    
    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);

    if(!ensure_root()){
	fprintf(stderr, "Must be run as root\n");
	exit(1);
    }

    if(!setup_serial_port()){
	printf("Error setting up serial port");
	return 1;
    }
    monitor_usb();

    return 0;
}

void monitor_usb(void) {
    hid_device* device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (device == NULL) {
        printf("Failed to open the HID device.\n");
        return 1;
    }

    unsigned char buffer[8];
    while (true) {
	int pitch = 0, yaw = 0, throttle = 0, roll = 0, button0 = 0, r = 0;
	char buf[4096];
        int result = hid_read(device, buffer, sizeof(buffer));
        if (result < 0) {
            printf("Error while reading joystick input.\n");
            break;
        }
	if (DEBUG_JOYSTICK_CHANNELS){
		printf("Received data: ");
		for (int i = 0; i < result; i++) {
		    printf("%d ", buffer[i]);
		}
		printf("\n");
	}
	throttle = scale_value(buffer[5]);
	pitch = scale_value(buffer[1]);
	yaw = scale_value(buffer[3]);
	roll = scale_value(buffer[0]);
	button0 = scale_value(0);
	sprintf(&buf,"{\"pitch\": %d, \"yaw\": %d, \"roll\": %d, \"throttle\": %d,\"button0\": %d}\n",pitch,yaw,roll,throttle,button0);
	write_json_string(buf);
	    
    }

    hid_close(device);
    return 0;
}

bool ensure_root(void){
  return(geteuid() == 0);
}



bool setup_serial_port(){

    char serialport[256];
    int baudrate = B115200;
    char buf[256];
    int rc,n;

    serial_port_descriptor = serialport_init(serial_port, baudrate);
    if(serial_port_descriptor == -1) return -1;
    printf("enabling serial port\n");
    delay(2000);
    printf("serial port ready\n");

    return true;
}

int write_json_string( char *json_string ){
  int rc = serialport_write(serial_port_descriptor, json_string);
  if( rc < 0 ){
    if(DEBUG_JOYSTICK_CHANNELS)
      printf("error writing to port\n");
    return(-1);
  }else{
    if(DEBUG_JOYSTICK_CHANNELS)
       printf("wrote %d bytes to port:\t%s\n",strlen(json_string),json_string);
  }
  return(strlen(json_string));
}

int scale_value(int value) {
    if (value < 0) {
        value = 0;
    } else if (value > 255) {
        value = 255;
    }
    return (int)((value / 255.0) * 2000);
}
