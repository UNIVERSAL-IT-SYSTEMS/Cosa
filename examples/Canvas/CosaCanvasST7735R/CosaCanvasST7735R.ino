/**
 * @file CosaCanvasST7735R.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2012, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * @section Description
 * Cosa demonstration of device driver for ST7735R, 262K Color
 * Single-Chip TFT Controller. 
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/Trace.hh"
#include "Cosa/Watchdog.hh"
#include "Cosa/Memory.h"
#include "Cosa/IOStream.hh"
#include "Cosa/Font/System5x7.hh"
#include "Cosa/Canvas.hh"
#include "Cosa/SPI/ST7735R.hh"
#include "Cosa/Icon/arduino_icon_34x32.h"
#include "Cosa/Icon/arduino_icon_64x64.h"
#include "Cosa/Icon/arduino_icon_96x32.h"

ST7735R tft;
IOStream cout(&tft);

void setup()
{
  // Initiate trace stream
  trace.begin(9600, PSTR("CosaST7735R: started"));

  // Check amount of free memory and size of objects
  TRACE(free_memory());
  TRACE(sizeof(Canvas));
  TRACE(sizeof(Font));
  TRACE(sizeof(IOStream));
  TRACE(sizeof(ST7735R));
  TRACE(sizeof(Trace));

  // Start the watchdog with default timeout (16 ms)
  Watchdog::begin();

  // Initiate the display
  TRACE(tft.begin());
}

void loop()
{
  uint32_t start, ms;

  // Fill screen
  start = micros();
  tft.set_canvas_color(Canvas::WHITE);
  tft.fill_screen();
  ms = (micros() - start) / 1000L;
  INFO("fill_screen: %ul ms", ms);

  // Print on the output stream
  start = micros();
  tft.set_pen_color(tft.shade(Canvas::GREEN, 20));
  tft.draw_rect(0, 0, tft.WIDTH - 1, tft.HEIGHT - 1);
  tft.set_text_color(Canvas::BLUE);
  tft.set_text_scale(1);
  tft.set_text_port(2, 2, tft.WIDTH, tft.HEIGHT);
  tft.set_cursor(2, 2);
  cout.print_P(PSTR("CosaST7735R: started"));
  cout.println();
  cout.printf_P(PSTR("text_color(%od)\n"), tft.get_text_color());
  cout.printf_P(PSTR("text_scale(%d)\n"), tft.get_text_scale());
  uint8_t x, y;
  tft.get_cursor(x, y);
  cout.printf_P(PSTR("cursor(x = %d, y = %d)\n"), x, y);
  tft.set_text_color(Canvas::RED);
  tft.set_text_scale(2);
  cout.print_P(PSTR("  Hello\n  World"));
  cout.println();
  tft.set_text_color(Canvas::BLACK);
  tft.set_text_scale(1);
  ms = (micros() - start) / 1000L;
  cout.printf_P(PSTR("draw_intro: %ul ms\n"), ms);
  Watchdog::delay(2048);

  // Grid with draw pixel
  tft.fill_screen();
  tft.set_pen_color(Canvas::BLACK);
  start = micros();
  for (uint8_t x = 0; x < tft.WIDTH; x += 2) {
    for (uint8_t y = 0; y < tft.HEIGHT; y += 2) {
      tft.draw_pixel(x, y);
    }
  }
  ms = (micros() - start) / 1000L;
  INFO("draw_pixel: %ul ms", ms);
  Watchdog::delay(2048);

  // Grid with draw rectangle
  tft.set_pen_color(Canvas::BLACK);
  start = micros();
  for (uint8_t x = 0; x < tft.WIDTH; x += 20) {
    for (uint8_t y = 0; y < tft.HEIGHT; y += 20) {
      tft.draw_rect(x, y, 20, 20);
    }
  }
  ms = (micros() - start) / 1000L;
  INFO("draw_rect: %ul ms", ms);
  Watchdog::delay(2048);
  
  // Fill rectangles
  tft.set_pen_color(Canvas::WHITE);
  start = micros();
  for (uint8_t x = 0; x < tft.WIDTH; x += 20) {
    for (uint8_t y = x; y < tft.HEIGHT; y += 40) {
      tft.fill_rect(x + 1, y + 1, 19, 19);
    }
  }
  ms = (micros() - start) / 1000L;
  INFO("fill_rect: %ul ms", ms);
  Watchdog::delay(2048);
  
  // Fill circles
  tft.fill_screen();
  tft.set_pen_color(Canvas::RED);
  start = micros();
  for (uint8_t x = 0; x < tft.WIDTH; x += 20) {
    for (uint8_t y = 0; y < tft.HEIGHT; y += 20) {
      tft.fill_circle(x + 10, y + 10, 9);
    }
  }
  ms = (micros() - start) / 1000L;
  INFO("fill_circle: %ul ms", ms);

  // Grid with draw circle
  tft.set_pen_color(Canvas::BLACK);
  start = micros();
  for (uint8_t x = 0; x < tft.WIDTH; x += 20) {
    for (uint8_t y = 0; y < tft.HEIGHT; y += 20) {
      tft.draw_circle(x + 10, y + 10, 9);
    }
  }
  ms = (micros() - start) / 1000L;
  INFO("draw_circle: %ul ms", ms);
  Watchdog::delay(2048);

  // Draw lines
  tft.set_canvas_color(tft.shade(Canvas::WHITE, 20));
  tft.fill_screen();
  start = micros();
  tft.set_pen_color(Canvas::RED);
  for (uint8_t x = 0; x < tft.WIDTH; x += 6) {
    tft.draw_line(0, 0, x, tft.HEIGHT - 1);
  }
  tft.set_pen_color(Canvas::GREEN);
  for (uint8_t y = 0; y < tft.HEIGHT; y += 6) {
    tft.draw_line(0, 0, tft.WIDTH - 1, y);
  }
  tft.set_pen_color(Canvas::BLUE);
  for (uint8_t x = 0; x < tft.WIDTH; x += 6) {
    tft.draw_line(tft.WIDTH - 1, 0, x, tft.HEIGHT - 1);
  }
  ms = (micros() - start) / 1000L;
  INFO("draw_line: %ul ms", ms);
  Watchdog::delay(2048);

  tft.set_pen_color(Canvas::BLACK);
  tft.fill_screen();
  start = micros();
  tft.set_pen_color(Canvas::CYAN);
  for (uint8_t y = 0; y < tft.HEIGHT; y += 6) {
    tft.draw_line(tft.WIDTH - 1, 0, 0, y);
  }
  tft.set_pen_color(Canvas::MAGENTA);
  for (uint8_t x = 0; x < tft.WIDTH; x += 6) {
    tft.draw_line(0, tft.HEIGHT - 1, x, 0);
  }
  ms = (micros() - start) / 1000L;
  INFO("draw_line: %ul ms", ms);
  Watchdog::delay(2048);

  start = micros();
  tft.set_orientation(Canvas::LANDSCAPE);
  tft.set_pen_color(Canvas::BLACK);
  tft.fill_screen();
  tft.set_pen_color(Canvas::YELLOW);
  for (uint8_t y = 0; y < tft.HEIGHT; y += 6) {
    tft.draw_line(0, tft.HEIGHT - 1, tft.WIDTH - 1, y);
  }
  for (uint8_t x = 0; x < tft.WIDTH; x += 6) {
    tft.draw_line(tft.WIDTH - 1, tft.HEIGHT - 1, x, 0);
  }
  for (uint8_t y = 0; y < tft.HEIGHT; y += 6) {
    tft.draw_line(tft.WIDTH - 1, tft.HEIGHT - 1, 0, y);
  }
  ms = (micros() - start) / 1000L;
  tft.set_text_color(Canvas::BLACK);
  tft.set_cursor(5, 40);
  tft.set_text_scale(4);
  cout.printf_P(PSTR("%ul ms\n"), ms);
  tft.set_orientation(Canvas::PORTRAIT);
  INFO("draw_line: %ul ms", ms);
  Watchdog::delay(2048);

  // Display the Arduino Icons
  tft.fill_screen();
  tft.set_pen_color(tft.shade(Canvas::CYAN, 80));
  tft.set_text_scale(1);
  tft.draw_icon((tft.WIDTH-34)/2, (tft.HEIGHT-32)/2, arduino_icon_34x32);
  Watchdog::delay(2048);
  tft.fill_screen();
  tft.draw_icon((tft.WIDTH-64)/2, (tft.HEIGHT-64)/2, arduino_icon_64x64);
  Watchdog::delay(2048);
  tft.set_orientation(Canvas::LANDSCAPE);
  tft.fill_screen();
  tft.draw_icon((tft.WIDTH-96)/2, (tft.HEIGHT-32)/2, arduino_icon_96x32);
  tft.set_orientation(Canvas::PORTRAIT);
  for (uint8_t x = 0; x < 2; x++) Watchdog::delay(2048);
}

