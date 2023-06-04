#include <stdio.h>
#include <stdlib.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hidsystem/IOHIDLib.h>
#include <CoreFoundation/CoreFoundation.h>

// Callback function for handling joystick input reports
void handleJoystickInput(void* context, IOReturn result, void* sender, IOHIDReportType type,
                         uint32_t reportID, uint8_t* report, CFIndex reportLength);

int main(void) {
    IOHIDManagerRef hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (hidManager == NULL) {
        printf("Failed to create HID manager.\n");
        return 1;
    }

    // Create a matching dictionary to find USB joysticks
    CFDictionaryRef matchingDict = IOServiceMatching(kIOHIDDeviceKey);
    if (matchingDict == NULL) {
        printf("Failed to create matching dictionary.\n");
        CFRelease(hidManager);
        return 1;
    }

    // Specify the usage page and usage to filter only joysticks
    CFNumberRef usagePageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &(int){kHIDPage_GenericDesktop});
    CFNumberRef usageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &(int){kHIDUsage_GD_Joystick});
    CFDictionarySetValue((CFMutableDictionaryRef)matchingDict, CFSTR(kIOHIDDeviceUsagePageKey), usagePageRef);
    CFDictionarySetValue((CFMutableDictionaryRef)matchingDict, CFSTR(kIOHIDDeviceUsageKey), usageRef);

    CFRelease(usagePageRef);
    CFRelease(usageRef);

    // Set the callback function for handling joystick input reports
    IOHIDManagerRegisterInputReportCallback(hidManager,handleJoystickInput,NULL);

    // Open the HID manager
    IOReturn result = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
    if (result != kIOReturnSuccess) {
        printf("Failed to open HID manager: %x\n", result);
        CFRelease(hidManager);
        return 1;
    }

    // Start the run loop to receive joystick input reports
    CFRunLoopRun();

    // Cleanup
    IOHIDManagerClose(hidManager, kIOHIDOptionsTypeNone);
    CFRelease(hidManager);

    return 0;
}

//static void __deviceReportCallback(void * context, IOReturn result, void * sender, IOHIDReportType type, uint32_t reportID, uint8_t * report, CFIndex reportLength)
void handleJoystickInput(void* context, IOReturn result, void* sender, IOHIDReportType type, uint32_t reportID, uint8_t* report, CFIndex reportLength) {
    // Print the received joystick input data
    for (int i = 0; i < reportLength; i++) {
        printf("%02x ", report[i]);
    }
    printf("\n");
}

