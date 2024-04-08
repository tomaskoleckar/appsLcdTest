#pragma once
#include "lcd_lib.h"
#include "graph_class.hpp"
#include <vector>
#include <cstring>
#include <string>

enum colors {
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    PURPLE,
    TEAL,
    WHITE,
};

static RGB rgb[8] = {
    {0, 0, 0},
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 255, 0},
    {160, 32, 240},
    {0, 128, 128},
    {255, 255, 255},
};

enum Orientation {
    HORIZONTAL,
    VERTICAL,
};

class Text {
public:
    Point2D m_init_pos;
    Character mChar;
    char m_text[20];
    int m_text_len;
    Orientation m_orientation;

    Text(const char *t_text, int font_width, int font_height, Point2D init_pos, Orientation orientation)
        : m_init_pos{init_pos}, mChar{init_pos, t_text[0], rgb[WHITE], rgb[BLACK], font_width, font_height}, m_orientation{orientation} {
        m_text_len = strlen(t_text);
        for (int i = 0; t_text[i] != '\0'; i++) {
            m_text[i] = t_text[i];
        }
        m_text[m_text_len] = 0;
    }

    void printText() {
        Point2D currentPos = m_init_pos;
        for (int i = 0; i < m_text_len; i++) {
            mChar.m_character = m_text[i];
            mChar.m_pos = currentPos;
            mChar.draw();
            if (m_orientation == HORIZONTAL) {
                currentPos.x += mChar.m_font_width;
            } else if (m_orientation == VERTICAL) {
                currentPos.y += mChar.m_font_height;
            }
        }
    }
};


//TEXT FOR HH:MM:SS
class Text2 {
public:
    Point2D m_init_pos;
    Character m_chars[9]; // Characters array for HH:MM:SS and separators
    char m_text[9]; // Text buffer for HH:MM:SS format
    int m_text_len;
    bool m_blinking[3]; // Boolean array for blinking states of separators
    Orientation m_orientation;

    // Constructor
    Text(const char* t_text, int font_width, int font_height, Point2D init_pos, Orientation orientation = HORIZONTAL)
        : m_init_pos(init_pos), m_text_len(strlen(t_text)), m_orientation(orientation) {
        // Initialize blinking states
        for (int i = 0; i < 3; ++i)
            m_blinking[i] = false;

        // Initialize characters and separators
        for (int i = 0; i < 9; ++i) {
            // Set separator characters to ':'
            if (i % 3 == 2) {
                m_chars[i] = Character(m_init_pos, ':', rgb[WHITE], rgb[BLACK], font_width, font_height);
            } else { // Set digit characters
                m_chars[i] = Character(m_init_pos, t_text[i], rgb[WHITE], rgb[BLACK], font_width, font_height);
            }

            // Adjust position for next character
            if (m_orientation == HORIZONTAL)
                m_init_pos.x += font_width;
            else
                m_init_pos.y += font_height;
        }
    }

    // Set the blinking state of separators
    void setBlinking(bool h_blinking, bool m_blinking, bool s_blinking) {
        m_blinking[0] = h_blinking;
        m_blinking[1] = m_blinking;
        m_blinking[2] = s_blinking;
    }

    // Print text with blinking separators
    void printText() {
        for (int i = 0; i < 9; ++i) {
            // Skip separators if blinking
            if ((i % 3 == 2) && m_blinking[i / 3])
                continue;
            m_chars[i].draw();
        }
    }
};
