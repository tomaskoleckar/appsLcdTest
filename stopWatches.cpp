#include "mbed.h"
#include "lcd_lib.h"
#include "graph_class.hpp"
#include "text_class.hpp"

// Define LCD dimensions
#define LCD_WIDTH 320
#define LCD_HEIGHT 240

// Define font dimensions
#define FONT_WIDTH 12
#define FONT_HEIGHT 16

// Define colors
static RGB rgb[8] = {
    {0, 0, 0},       // BLACK
    {255, 0, 0},     // RED
    {0, 255, 0},     // GREEN
    {0, 0, 255},     // BLUE
    {255, 255, 0},   // YELLOW
    {160, 32, 240},  // PURPLE
    {0, 128, 128},   // TEAL
    {255, 255, 255}  // WHITE
};

// Digital inputs for buttons
DigitalIn button_startstop(PTC9);
DigitalIn button_reset(PTC10);

// Define the initial time (zero for stopwatch)
int hours = 0;
int minutes = 0;
int seconds = 0;
bool isRunning = false;

// Ticker object for updating time
Ticker timeTicker;
Text timeDisplay("00:00:00", FONT_WIDTH, FONT_HEIGHT, {20, 20}, HORIZONTAL);

//STOPWATCHES
void updateTime() {
    if (isRunning) {
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }
        // Trigger screen refresh
        g_refresh = 1;
    }
}

//COUNTDOWN
void updateTime2() {
    if (isRunning) {
        if (seconds > 0) {
            seconds--;
        } else {
            // Countdown completed
            isRunning = false;
        }
        // Trigger screen refresh
        g_refresh = 1;
    }
}


// Function to handle button presses
void handleButtons() {
    // Check if the start/stop button is pressed
    if (button_startstop == 0) {
        isRunning = !isRunning;
    }

    // Check if the reset button is pressed
    if (button_reset == 0) {
        // Reset stopwatch
        hours = 0;
        minutes = 0;
        seconds = 10;
        isRunning = false;
        // Trigger screen refresh
        g_refresh = 1;
    }
}

// Function to update the time display
void updateTimeDisplay() {
    // Format the time string
    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);

    // Set the time text for the Text object
    timeDisplay.setTimeText(timeStr);
}

int main() {
    // Initialize LCD
    lcd_init();

    // Attach the updateTime function to the ticker with the defined interval (1 second)
    timeTicker.attach(&updateTime, 1);

    // Attach the handleButtons function to be called on button press
    button_startstop.fall(&handleButtons);
    button_reset.fall(&handleButtons);

    // Main loop
    while (1) {
        // Check for screen refresh
        if (g_refresh) {
            // Clear the screen
            lcd_clear_screen();

            // Update the time display
            updateTimeDisplay();

            // Draw the time display with its updated text
            timeDisplay.printTime();

            // Reset the screen refresh flag
            g_refresh = 0;
        }
    }

    return 0;
}
