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
DigitalIn button_set(PTC9);
DigitalIn button_up(PTC10);
DigitalIn button_down(PTC11);

// Define the initial time (you can set this to the current time)
int hours = 0;
int minutes = 0;
int seconds = 0;

// Ticker object for blinking separator
Ticker separatorTicker;
Text timeDisplay("00:00:00", FONT_WIDTH, FONT_HEIGHT, {20, 20}, HORIZONTAL);
// Function to toggle the blinking of separator ":"
void toggleSeparator() {
    static bool separatorVisible = true;
    separatorVisible = !separatorVisible;

    // Set the separator visibility flag for the Text object
    timeDisplay.setSeparatorVisible(separatorVisible);

    // Trigger screen refresh
    g_refresh = 1;
}

// Function to handle button presses for setting time
void handleButtons() {
    // Check if the set button is pressed
    if (button_set == 0) {
        // Increase hours
        hours = (hours + 1) % 24;
    }

    // Check if the up button is pressed
    if (button_up == 0) {
        // Increase minutes
        minutes = (minutes + 1) % 60;
    }

    // Check if the down button is pressed
    if (button_down == 0) {
        // Increase seconds
        seconds = (seconds + 1) % 60;
    }

    // Update the time display
    updateTimeDisplay();
}

// Function to update the time display
void updateTimeDisplay() {
    // Format the time string
    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);

    // Set the time text for the Text object
    timeDisplay.setTimeText(timeStr);

    // Trigger screen refresh
    g_refresh = 1;
}

int main() {
    // Initialize LCD
    lcd_init();

    // Attach the toggleSeparator function to the ticker with the defined interval (500 ms)
    separatorTicker.attach(&toggleSeparator, 0.5);

    // Attach the handleButtons function to be called on button press
    button_set.fall(&handleButtons);
    button_up.fall(&handleButtons);
    button_down.fall(&handleButtons);

    // Main loop
    while (1) {
        // Check for screen refresh
        if (g_refresh) {
            // Clear the screen
            lcd_clear_screen();

            // Draw the time display with its updated text and separator visibility
            timeDisplay.printTime();

            // Reset the screen refresh flag
            g_refresh = 0;
        }
    }

    return 0;
}
