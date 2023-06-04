#include <stdio.h>
#include <wiringPi.h>

#define CHANNEL_COUNT 6
#define FRAME_INTERVAL 22500 // CPPM frame interval (20ms)
#define SIGNAL_PIN 21 // GPIO pin for CPPM signal output

int channelValues[CHANNEL_COUNT] = {500, 500, 1500, 1500, 1500, 1500}; // Initialize channel values to neutral position

void generateCppmSignal();

int main(void) {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize WiringPi\n");
        return 1;
    }

    pinMode(SIGNAL_PIN, OUTPUT); // Set SIGNAL_PIN as output

    while (1) {
        generateCppmSignal();
        delayMicroseconds(FRAME_INTERVAL);
    }

    return 0;
}

void generateCppmSignal() {
    static unsigned long lastChannelUpdate = 0;
    const int channelPulseLength = 300; // Pulse length for each channel (400 microseconds)

    if (micros() - lastChannelUpdate >= FRAME_INTERVAL) {
        // Generate CPPM frame
        int signal[16];

        // Set sync pulse (all channels low)
        for (int i = 0; i < 16; i++) {
            signal[i] = 0;
        }

        // Set channel pulses
        for (int i = 0; i < CHANNEL_COUNT; i++) {
            signal[i * 2 + 1] = channelValues[i] / 256; // High byte
            signal[i * 2 + 2] = channelValues[i] % 256; // Low byte
        }

        // Output CPPM signal
        for (int i = 0; i < 16; i++) {
            digitalWrite(SIGNAL_PIN, HIGH); // Set CPPM signal high

            if (signal[i] == 1) {
                delayMicroseconds(channelPulseLength);
            } else {
                delayMicroseconds(channelPulseLength - 300);
            }

            digitalWrite(SIGNAL_PIN, LOW); // Set CPPM signal low
        }

        lastChannelUpdate = micros();
    }
}

