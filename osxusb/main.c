#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libusb.h>
#include <hidapi.h>
#include <hidapi_darwin.h>
#define VENDOR_ID 0x046d
#define PRODUCT_ID 0xc215
#define DEBUG_JOYSTICK_CHANNELS false

void print_devs(libusb_device **devs);
int monitor_usb(void);

int main(int argc, const char * argv[]){
    libusb_device **devs;
    libusb_context *context = NULL;

    size_t list;
    int ret;

    if((ret = libusb_init(&context))<0){
        perror("libusb_init");
        exit(1);
    }

    if((list = libusb_get_device_list(context, &devs))<1){
	printf("No joystick found!\n");
	exit(1);
    }

    printf("There are %zd devices found\n", list);

    print_devs(devs);
    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);

monitor_usb();


    return 0;
}


void print_devs(libusb_device **devs){
  libusb_device *dev;
  int           i = 0, j = 0;
  uint8_t       path[8];

  while ((dev = devs[i++]) != NULL) {
    struct libusb_device_descriptor desc;
    int                             r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
      fprintf(stderr, "failed to get device descriptor");
      return;
    }

    printf("%04x:%04x (bus %d, device %d)",
           desc.idVendor, desc.idProduct,
           libusb_get_bus_number(dev), libusb_get_device_address(dev));

    r = libusb_get_port_numbers(dev, path, sizeof(path));
    if (r > 0) {
      printf(" path: %d", path[0]);
      for (j = 1; j < r; j++)
        printf(".%d", path[j]);
    }
    printf("\n");
  }
}

int monitor_usb(void) {
    hid_device* device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (device == NULL) {
        printf("Failed to open the HID device.\n");
        return 1;
    }

    unsigned char buffer[8];
    while (true) {
	int pitch = 0, yaw = 0, throttle = 0, roll = 0;
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
	throttle = buffer[5];
	pitch = buffer[1];
	yaw = buffer[3];
	roll = buffer[0];
	printf("{\"pitch\": %d, \"yaw\": %d, \"roll\": %d, \"throttle\": %d}\n",pitch,yaw,roll,throttle); 
    }

    hid_close(device);
    return 0;
}
