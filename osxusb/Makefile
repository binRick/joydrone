all:
	gcc main.c -o joystick \
		-L /usr/local/lib \
		-I /usr/local/Cellar/libusb/1.0.26/include/libusb-1.0 -l usb-1.0 \
		-I /usr/local/Cellar/hidapi/0.14.0/include/hidapi -l hidapi
	./joystick
