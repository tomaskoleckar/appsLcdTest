#pragma once
// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 09/2019
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         OpenCV simulator of LCD
//
// **************************************************************************
#include "fonts/font12x16_lsb.h"
#include "lcd_lib.h"
#include <cstdint>
#include <cstdlib>
// Simple graphic interface

struct Point2D {
  int x, y;
};

struct RGB {
  uint8_t r, g, b;
};

class GraphElement {
public:
  // foreground and background color
  RGB m_fg_color, m_bg_color;

  // constructor
  GraphElement(RGB t_fg_color, RGB t_bg_color)
      : m_fg_color(t_fg_color), m_bg_color(t_bg_color) {}

  // ONLY ONE INTERFACE WITH LCD HARDWARE!!!
  void drawPixel(int32_t t_x, int32_t t_y) {
    lcd_put_pixel(t_x, t_y, convert_RGB888_to_RGB565(m_fg_color));
  }

  // Draw graphics element
  virtual void draw() = 0;

  // Hide graphics element
  virtual void hide() {
    swap_fg_bg_color();
    draw();
    swap_fg_bg_color();
  }

private:
  // swap foreground and backgroud colors
  void swap_fg_bg_color() {
    RGB l_tmp = m_fg_color;
    m_fg_color = m_bg_color;
    m_bg_color = l_tmp;
  }

  // DONE!!!
  // conversion of 24-bit RGB color into 16-bit color format
  uint16_t convert_RGB888_to_RGB565(RGB t_color) {
    uint16_t new_r = (t_color.r >> 3) & 0x1F;
    uint16_t new_g = (t_color.g >> 2) & 0x3F;
    uint16_t new_b = (t_color.b >> 3) & 0x1F;
    return (new_r << 11) | (new_g << 5) | new_b;
  }
};

class Pixel : public GraphElement {
public:
  // constructor
  Pixel(Point2D t_pos, RGB t_fg_color, RGB t_bg_color)
      : GraphElement(t_fg_color, t_bg_color), m_pos(t_pos) {}
  // Draw method implementation
  virtual void draw() { drawPixel(m_pos.x, m_pos.y); }
  // Position of Pixel
  Point2D m_pos;
};

class Circle : public GraphElement {
public:
  // Center of circle
  Point2D m_center;
  // Radius of circle
  int32_t m_radius;

  Circle(Point2D t_center, int32_t t_radius, RGB t_fg, RGB t_bg)
      : GraphElement(t_fg, t_bg), m_center(t_center), m_radius(t_radius) {}

  void draw() {
    int x = m_radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
      // Draw pixels at each octant of the circle
      drawPixel(m_center.x + x, m_center.y + y);
      drawPixel(m_center.x + y, m_center.y + x);
      drawPixel(m_center.x - y, m_center.y + x);
      drawPixel(m_center.x - x, m_center.y + y);
      drawPixel(m_center.x - x, m_center.y - y);
      drawPixel(m_center.x - y, m_center.y - x);
      drawPixel(m_center.x + y, m_center.y - x);
      drawPixel(m_center.x + x, m_center.y - y);

      if (err <= 0) {
        y += 1;
        err += 2 * y + 1;
      }

      if (err > 0) {
        x -= 1;
        err -= 2 * x + 1;
      }
    }
  } // IMPLEMENT!!!
};

class Character : public GraphElement {
public:
  // position of character
  Point2D m_pos;
  // character
  char m_character;

  int m_font_width;
  int m_font_height;
  int m_current_val;

  Character(Point2D t_pos, char t_char, RGB t_fg, RGB t_bg, int t_font_width,
            int t_font_height)
      : GraphElement(t_fg, t_bg), m_pos(t_pos), m_character(t_char),
        m_font_width(t_font_width), m_font_height(t_font_height),
        m_current_val(0) {}

  void draw() {
    for (int i = 0; i < m_font_height; i++) {
      m_current_val = static_cast<int>(font[static_cast<int>(m_character)][i]);
      for (int j = 0; j < m_font_width; j++) {
        if (m_current_val % 2 != 0) {
          drawPixel(m_pos.x + j, m_pos.y + i);
        }
        m_current_val >>= 1;
      }
    }
  } // DONE!!!


  void draw1() {
        for (int i = m_font_height - 1; i >= 0; i--) {
          m_current_val = static_cast<int>(font[static_cast<int>(m_character)][i]);
          for (int j = 0; j < m_font_height; j++) {
            if (m_current_val % 2 != 0) {
              drawPixel(m_pos.x + j, m_pos.y + i);
            }
            m_current_val >>= 1;
          }
        }
      }
};

class Line : public GraphElement {
public:
  // the first and the last point of line
  Point2D m_pos1, m_pos2;

  Line(Point2D t_pos1, Point2D t_pos2, RGB t_fg, RGB t_bg)
      : GraphElement(t_fg, t_bg), m_pos1(t_pos1), m_pos2(t_pos2) {}

  void draw() {
    int dx = m_pos2.x - m_pos1.x;
    int dy = m_pos2.y - m_pos1.y;
    int x = m_pos1.x;
    int x_inc = 1;
    int y = m_pos1.y;
    int y_inc = 1;
    int oct_sym = 1;
    find_octant(&oct_sym, &x_inc, &y_inc);
    int incr_side_height = 2 * (dy - (dx * oct_sym));
    int d = 2 * dy - dx;

    drawPixel(x, y);

    int *longer_side_curr = nullptr;
    int *longer_side_end = nullptr;
    int x_longer = 1;
    int y_longer = 1;
    int longer_side_incr = 0;

    if (abs(dx) > abs(dy)) {
      longer_side_curr = &x;
      longer_side_end = &m_pos2.x;
      y_longer = 0;
      longer_side_incr = 2 * dy;
      if (dy < 0) {
        longer_side_incr *= -1;
        incr_side_height *= -1;
      }
    } else {
      longer_side_curr = &y;
      longer_side_end = &m_pos2.y;
      x_longer = 0;
      longer_side_incr = 2 * dx;
      if (dx < 0) {
        longer_side_incr *= -1;
      }
      if (dy > 0) {
        incr_side_height *= -1;
      }
    }

    while (*longer_side_end - *longer_side_curr != 0) {

      if (d <= 0) {
        d += longer_side_incr;
        x += x_inc * x_longer;
        y += y_inc * y_longer;
      } else {
        d += incr_side_height;
        x += x_inc;
        y += y_inc;
      }
      drawPixel(x, y);
    }
    return;
  }

private:
  void find_octant(int *oct_sym, int *x_inc, int *y_inc) {
    int dx = m_pos2.x - m_pos1.x;
    int dy = m_pos2.y - m_pos1.y;

    if (dx > 0) {
      if (dy > 0) {
        return;
      } else if (dy < 0) {
        *oct_sym = -1;
        *y_inc = -1;
        return;
      } else {
        return;
      }
    } else if (dx < 0) {
      if (dy > 0) {
        *oct_sym = -1;
        *x_inc = -1;
        return;
      } else if (dy < 0) {
        *x_inc = -1;
        *y_inc = -1;
        return;
      } else {
        return;
      }
    } else {
      return;
    }
  }
};
