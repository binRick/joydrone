#!/usr/bin/env python
import json, serial, time, sys
 
port = sys.argv[1]
print("Opening port "+str(port))
 

js_chans = {
	"pitch": 0,
	"roll": 0,
	"yaw": 0,
	"throttle": 0,
	"button0": 0,
}

# -------- Main Program Loop -----------
with serial.Serial() as ser:
  ser.baudrate = 115200
  ser.port = port
  ser.open()
  print("opening serial port...")
  time.sleep(2)
  print("serial port ready...")
  while True:
    js_str = json.dumps(js_chans) + '\n'
    print(f'json to send: {js_str}')
    ser.write(bytes(js_str,'utf-8'))
    print("sent to serial port")
