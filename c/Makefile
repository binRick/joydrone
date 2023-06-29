pi-setup:
	sudo apt install libhidapi-dev libusb-1.0-0-dev wiringpi
	wget https://github.com/WiringPi/WiringPi/releases/download/2.61-1/wiringpi-2.61-1-arm64.deb
	sudo dpkg -i wiringpi-2.61-1-arm64.deb
#wget https://github.com/WiringPi/WiringPi/releases/download/2.61-1/wiringpi-2.61-1-armhf.deb
#sudo dpkg -i wiringpi-2.61-1-armhf.deb


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
	gcc write_serial.c \
		-L /usr/local/lib/aarch64-linux-gnu \
		-l cserial \
		-I /usr/local/include -o write_serial
	gcc read_joystick_write_serial.c \
		-L /usr/lib/arm-linux-gnueabihf \
		-I /usr/include/libusb-1.0 \
		-I /usr/include/hidapi \
		-l hidapi-libusb \
		-l usb-1.0 \
		-L /usr/local/lib/aarch64-linux-gnu \
		-l cserial \
		-I /usr/local/include -o read_joystick_write_serial
osx:
	gcc joystick.c -o joystick \
		-L /usr/local/lib \
		-I /usr/local/Cellar/libusb/1.0.26/include/libusb-1.0 -l usb-1.0 \
		-I /usr/local/Cellar/hidapi/0.14.0/include/hidapi -l hidapi 
	gcc test_write_serial.c -o test_write_serial
