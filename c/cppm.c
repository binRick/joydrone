#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define PIN 21
#define CHANNEL_COUNT 6
#define FRAME_LENGTH_US 22500
#define MIN_PULSE_WIDTH_US 300
#define MAX_PULSE_WIDTH_US 2100

int channels[CHANNEL_COUNT] = { 1500, 1500, 1500, 1500, 1500, 1500 };

void updateChannels() {
    // Generate CPPM signal frame
    digitalWrite(PIN, HIGH);
    delayMicroseconds(MIN_PULSE_WIDTH_US);

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        digitalWrite(PIN, LOW);
        delayMicroseconds(channels[i]);

        digitalWrite(PIN, HIGH);
        delayMicroseconds(MAX_PULSE_WIDTH_US - channels[i]);
    }

    digitalWrite(PIN, LOW);
    delayMicroseconds(FRAME_LENGTH_US - MAX_PULSE_WIDTH_US);
}

int main(void) {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi.\n");
        return 1;
    }

    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);

    while (1) {
        // Update channel values (modify this part to adjust channel values)
        channels[0] = 1500;  // Channel 1
        channels[1] = 1600;  // Channel 2
        channels[2] = 1700;  // Channel 3
        channels[3] = 1800;  // Channel 4
        channels[4] = 1900;  // Channel 5
        channels[5] = 2000;  // Channel 6

        // Generate CPPM signal frame
        updateChannels();

        // Delay between frames (modify this value to adjust the CPPM frame rate)
        delay(20);
    }

    return 0;
}

