#include "text_class.hpp"
#define TICKER_INTERVAL_MS 100

Text movingText(customText, FONT_WIDTH, FONT_HEIGHT, textPosition);
Ticker movementTicker;

void moveText() {
    // Move the text towards the left edge of the screen
    textPosition.x -= 5;  // Adjust the speed as needed

    // Reset the text to the right edge when it reaches the left edge
    if (textPosition.x + movingText.m_text_len * FONT_WIDTH < 0) {
        textPosition.x = LCD_WIDTH;
    }

    // Trigger screen refresh
    g_refresh = 1;
}

int main(int argc, char const *argv[])
{
    movementTicker.attach(moveText, std::chrono::milliseconds(TICKER_INTERVAL_MS));

    return 0;
}

