// © Kay Sievers <kay@versioduo.com>, 2022
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <Arduino.h>
#include <Font5x7.h>

// As single device has four digits, every digit needs a separate latch pin.
// Mutliple devices can share the same column and row pins.
template <uint8_t n_digits = 4> class VQC10 {
public:
  struct Pins {
    uint8_t digits[n_digits];
    uint8_t columns[5];
    uint8_t rows[7];
  };

  constexpr VQC10(Pins pins) : _pins{pins} {}

  void begin() {
    for (uint8_t i = 0; i < n_digits; i++) {
      pinMode(_pins.digits[i], OUTPUT);
      digitalWrite(_pins.digits[i], LOW);
    }

    for (uint8_t i = 0; i < 5; i++) {
      pinMode(_pins.columns[i], OUTPUT);
      digitalWrite(_pins.columns[i], LOW);
    }

    for (uint8_t i = 0; i < 7; i++) {
      pinMode(_pins.rows[i], OUTPUT);
      digitalWrite(_pins.rows[i], LOW);
    }
  }

  // Called if tick() is not run periodically from timer.
  void loop() {
    if ((unsigned long)(micros() - _usec) < 200)
      return;
    _usec = micros();

    tick();
  }

  // Draw the next row of LED dots. It is called from loop() or directly with
  // with a ~5kHz timer/software interrupt.
  void tick() {
    // Disable the power of the current row.
    digitalWrite(_pins.rows[_row], LOW);

    _row++;
    if (_row == 7)
      _row = 0;

    // Update the next row of data for all four digits.
    for (uint8_t digit = 0; digit < n_digits; digit++) {
      const uint8_t *bitmap = Font5x7::getBitmap(_character[digit]);

      // Data input, enable/disable the five dots / bits per digit.
      for (uint8_t column = 0; column < 5; column++) {
        const bool bit = bitmap[column] & (1 << _row);
        digitalWrite(_pins.columns[column], bit);
      }

      // Latch / commit the data to the digit.
      digitalWrite(_pins.digits[digit], HIGH);
      digitalWrite(_pins.digits[digit], LOW);
    }

    // Enable the power for the updated row.
    digitalWrite(_pins.rows[_row], HIGH);
  }

  // Show an ASCII character.
  void show(uint8_t position, uint8_t character) {
    if (position >= n_digits)
      return;

    _character[position] = character;
  }

private:
  const Pins _pins;
  uint8_t _character[n_digits]{};
  unsigned long _usec{};
  uint8_t _row{};
};
