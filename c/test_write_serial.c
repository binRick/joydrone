#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define PORT "/dev/ttyUSB0"
#define DATA "{\"pitch\": 1898, \"yaw\": 996, \"roll\": 0, \"throttle\": 2000,\"button0\": 0}"

int writeToSerialPort(const char* portPath, const char* data) {
    int port = open(portPath, O_WRONLY);
    if (port == -1) {
        printf("Failed to open serial port %s.\n", portPath);
        return -1;
    }

    ssize_t bytesWritten = write(port, data, strlen(data));
    close(port);

    if (bytesWritten == -1) {
        printf("Failed to write data to serial port.\n");
        return -1;
    }

    printf("Successfully wrote %zd bytes to serial port.\n", bytesWritten);
    return 0;
}

int main(){
  while(1)
	writeToSerialPort(PORT, DATA);
}
