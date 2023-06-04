#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID 0x1234  // Replace with your joystick's vendor ID
#define PRODUCT_ID 0x5678 // Replace with your joystick's product ID
#define INTERFACE_NUMBER 0

int main(void) {
    libusb_device_handle* handle;
    libusb_context* ctx;
    libusb_init(&ctx);

    // Open the USB device using vendor and product IDs
    handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
    if (handle == NULL) {
        printf("Failed to open the USB device.\n");
        return 1;
    }

    // Claim the USB interface
    if (libusb_claim_interface(handle, INTERFACE_NUMBER) < 0) {
        printf("Failed to claim the USB interface.\n");
        libusb_close(handle);
        libusb_exit(ctx);
        return 1;
    }

    unsigned char buffer[8];
    int transferred;

    // Continuous loop to read joystick input
    while (1) {
        // Read data from the USB interrupt endpoint
        int result = libusb_interrupt_transfer(handle, 0x81, buffer, sizeof(buffer), &transferred, 1000);
        if (result == LIBUSB_ERROR_TIMEOUT) {
            continue;
        } else if (result < 0) {
            printf("Error while reading USB data: %s\n", libusb_error_name(result));
            break;
        }

        // Process the received joystick input data
        // Modify the code below to interpret the received data according to your joystick's specifications
        printf("Received data: ");
        for (int i = 0; i < transferred; i++) {
            printf("%02x ", buffer[i]);
        }
        printf("\n");
    }

    // Release the USB interface and close the device
    libusb_release_interface(handle, INTERFACE_NUMBER);
    libusb_close(handle);
    libusb_exit(ctx);

    return 0;
}

