#!/usr/bin/env python
import pygame, json, serial, time
port = '/dev/tty.usbserial-A601EGHU'
# Initialize Pygame
pygame.init()

js_chans = {
        "pitch": 0,
        "roll": 0,
        "yaw": 0,
        "throttle": 0,
        "button0": 0,
}

# Check for connected joysticks
joystick_count = pygame.joystick.get_count()
if joystick_count == 0:
    print("No joysticks found.")
    pygame.quit()
    exit()

# Select the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

print("Joystick Name:", joystick.get_name())
print("Number of Axes:", joystick.get_numaxes())
print("Number of Buttons:", joystick.get_numbuttons())

# Main loop
with serial.Serial() as ser:
  ser.baudrate = 115200
  ser.port = port
  ser.open()
  print("opening serial port...")
  time.sleep(2)
  print("serial port ready...")
  # Process events
  #for event in pygame.event.get():
  '''
        if event.type == pygame.JOYAXISMOTION:
            # Joystick axis motion event
            axis = event.axis
            value = event.value
            print("Axis {} moved to {}".format(axis, value))
        elif event.type == pygame.JOYBUTTONDOWN:
            # Joystick button down event
            button = event.button
            print("Button {} down".format(button))
        elif event.type == pygame.JOYBUTTONUP:
            # Joystick button up event
            button = event.button
            print("Button {} up".format(button))
        elif event.type == pygame.JOYHATMOTION:
            # Joystick hat motion event
            hat = event.hat
            value = event.value
            print("Hat {} moved to {}".format(hat, value))
  '''

  js_chans["roll"] = int((joystick.get_axis(0) * 1000) + 1000)
  js_chans["pitch"] = int((joystick.get_axis(1) * -1000) + 1000)
  js_chans["yaw"] = int((joystick.get_axis(2) * 1000) + 1000)
  js_chans["throttle"] = int((joystick.get_axis(3) * -1000) + 1000)
  js_chans["button0"] = joystick.get_button(0)

  js_str = json.dumps(js_chans) + '\n'
  print(f'json to send: {js_str}')
  ser.write(bytes(js_str,'utf-8'))
  print("sent to serial port")


  # Add a small delay to reduce CPU usage
  #pygame.time.wait(10)

