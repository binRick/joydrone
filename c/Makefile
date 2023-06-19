pi-setup:
	sudo apt install libhidapi-dev

pi:
	gcc joystick.c -o joystick \
		-L /usr/lib/arm-linux-gnueabihf \
		-I /usr/include/libusb-1.0 \
		-I /usr/include/hidapi \
		-l hidapi-libusb \
		-l usb-1.0
	gcc cppm.c -o cppm \
		-l wiringPi
	gcc test_write_serial.c -o test_write_serial
osx:
	gcc joystick.c -o joystick \
		-L /usr/local/lib \
		-I /usr/local/Cellar/libusb/1.0.26/include/libusb-1.0 -l usb-1.0 \
		-I /usr/local/Cellar/hidapi/0.14.0/include/hidapi -l hidapi 
	gcc test_write_serial.c -o test_write_serial
