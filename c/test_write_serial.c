#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <termios.h>
#define PORT "/dev/ttyUSB0"
#define BAUD 115200
#define DATA "{\"pitch\": 1898, \"yaw\": 996, \"roll\": 0, \"throttle\": 1000,\"button0\": 0}\n"


int writeToSerialPort(const char* portPath, const char* data, speed_t baudRate);

int writeToSerialPort(const char* portPath, const char* data, speed_t baudRate) {
    int port = open(portPath, O_WRONLY);
    if (port == -1) {
        printf("Failed to open serial port %s.\n", portPath);
        return -1;
    }

    struct termios options;
    tcgetattr(port, &options);
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 10;
    tcsetattr(port, TCSANOW, &options);

    ssize_t bytesWritten = write(port, data, strlen(data));
    close(port);

    if (bytesWritten == -1) {
        printf("Failed to write data to serial port.\n");
        return -1;
    }

    printf("Successfully wrote %zd bytes to serial port.\n", bytesWritten);
    return 0;
}


int writeToSerialPort1(const char* portPath, const char* data) {
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
  sleep(2);
  while(1)
	writeToSerialPort(PORT, DATA, BAUD);
	//writeToSerialPort(PORT, DATA);
}
