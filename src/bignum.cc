/*
  Copyright (c) 2018 Mateusz Furga <matfurga@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include <cstring>
#include <iostream>
#include <iomanip>

#include "bignum.h"

#define UNUSED(x) (void)(x)

namespace bignum {

Bignum::Bignum(int64_t number)
{
  if (number >= 0)
    this->sign = POSITIVE;
  else
    this->sign = NEGATIVE;

  number = llabs(number);
  while (number) {
    this->number.push_back(number % 1000000000ULL);
    this->size++;
    number /= 1000000000ULL;
  }
}

Bignum::Bignum(const char *number)
{
  // It assumes that the default representation of number is decimal.

  if (number[0] == '-')
    this->sign = NEGATIVE;
  else
    this->sign = POSITIVE;

  this->translate_string_to_number(number, strlen(number), Base::DEC);
}

Bignum::Bignum(const std::string& number)
{
  // It assumes that the default representation of number is decimal.

  if (number[0] == '-')
    this->sign = NEGATIVE;
  else
    this->sign = POSITIVE;

  this->translate_string_to_number(number.c_str(), number.length(), Base::DEC);
}

Bignum::~Bignum() {}

bool compare_without_sign(const Bignum& self, const Bignum& other)
{
  if (self.size > other.size)
    return true;
  else if (self.size < other.size)
    return false;

  for (int i = 0; i < self.size; i++) {
    if (self[i] > other[i])
      return true;
    else if (self[i] < other[i])
      return false;
  }

  return false;
}

inline int translate_char_to_number(const char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return -1;
}

void Bignum::translate_string_to_number(const char *number, int length, 
                                        const Base& repr)
{
  // Converts a c-string into chunks of 32-bit unsigned integers.

  UNUSED(repr);

  int base = 10, mul = 1;
  int MIN_VALUE = 0, MAX_VALUE = 9;

  uint32_t chunk = 0;

  for (int i = length - 1; i >= 0; i--) {
    if (mul == 1000000000) {
      this->number.push_back(chunk);
      chunk = 0; mul = 1;
    }
    if (translate_char_to_number(number[i]) >= MIN_VALUE && 
        translate_char_to_number(number[i]) <= MAX_VALUE) {
      chunk += translate_char_to_number(number[i]) * mul;
      mul *= base;
      this->size++;
    }
  }

  if (chunk > 0)
    this->number.push_back(chunk);
}

void Bignum::add(int64_t number)
{
  this->add(Bignum(number));
}

void Bignum::add(const char *number)
{
  this->add(Bignum(number));
}

void Bignum::add(const std::string& number)
{
  this->add(Bignum(number));
}

void Bignum::add(const Bignum& other)
{
  // Performs an addition for the following scenarios:
  // - two numbers (A and B) with a positive sign:  A + B 
  // - two numbers (A and B) with a negative sign: -A + (-B) = -(A + B)

  Bignum number = other;

  // Calls a subtraction method for the following two numbers: A = A + (-B) = A - B
  if (this->sign == POSITIVE && other.sign == NEGATIVE) {
    number.sign = POSITIVE;
    return this->sub(number);
  }

  // Calls a subtraction method for the following two numbers: A = -A + B = B - A
  if (this->sign == NEGATIVE && other.sign == POSITIVE) {
    this->sign = POSITIVE;
    return this->sub(other, true);
  }

  if (this->number.size() > other.number.size()) {
    const int diff = this->number.size() - other.number.size();
    for (int i = 0; i < diff; i++)
      number.number.push_back(0);
  }
  else if (this->number.size() < other.number.size()) {
    const int diff = other.number.size() - this->number.size();
    for (int i = 0; i < diff; i++)
      this->number.push_back(0);
  }

  uint32_t carry = 0;

  for (size_t i = 0; i < this->number.size(); i++) {
    uint64_t sum = U64(this->number[i]) + U64(number.number[i]) + carry;
    carry = sum / 1000000000ULL;
    this->number[i] = sum % 1000000000ULL;
  }

  if (carry > 0)
    this->number.push_back(carry);
}

void Bignum::sub(int64_t number)
{
  this->sub(Bignum(number));
}

void Bignum::sub(const char *number)
{
  this->sub(Bignum(number));
}

void Bignum::sub(const std::string& number)
{
  this->sub(Bignum(number));
}

void Bignum::sub(const Bignum& other, bool reverse)
{
  // Performs a substraction for the following scenarios:
  // - two numbers (A and B) with positive sign: A - B or B - A
  // - two numbers (A and B) number A with negative sign and number B with positive sign: -A - B

  Bignum number = other;

  // Calls an addition method for the following two numbers:
  //  A - (-B) =  A + B
  // -A - (-B) = -A + B
  if ((this->sign == POSITIVE && other.sign == NEGATIVE) ||
      (this->sign == NEGATIVE && other.sign == NEGATIVE)) {
    number.sign = POSITIVE;
    return this->add(number);
  }

  // Calls an addition method for the following two numbers: -A - B = -(A + B)
  if (this->sign == NEGATIVE && other.sign == POSITIVE) {
    this->sign = POSITIVE;
    this->add(other);
    this->sign = NEGATIVE;
    return;
  }

  Bignum greater, smaller;

  if (compare_without_sign(*this, other)) {
    greater = *this; smaller = other;
  } else {
    greater = other; smaller = *this;
  }

  if (this->size < other.size)
    greater.sign = NEGATIVE;
  else if ((this->size > other.size) && this->sign == POSITIVE)
    greater.sign = POSITIVE;
  else if ((this->size > other.size) && this->sign == NEGATIVE)
    greater.sign = NEGATIVE;
  else if (this->size == other.size) {
    for (int i = 0; i < this->size; i++) {
      if ((this->operator[](i) > other[i]) && this->sign == POSITIVE)
        greater.sign = POSITIVE;
      else if ((this->operator[](i) > other[i]) && this->sign == NEGATIVE)
        greater.sign = NEGATIVE; 
      else if ((this->operator[](i) < other[i]))
        greater.sign = NEGATIVE;
    }
  }

  if (reverse)
    greater.sign = greater.sign == POSITIVE ? NEGATIVE : POSITIVE;

  const size_t diff = greater.number.size() - smaller.number.size();

  for (size_t i = 0; i < diff; i++)
    smaller.number.push_back(0);

  for (size_t i = 0; i < greater.number.size(); i++) {
    int64_t n = S64(greater.number[i]) - S64(smaller.number[i]);
    if (n < 0) {
      int j = 1;
      while (!greater.number[i + j]) {
        greater.number[i + j] = U32(999999999);
        j++;
      }
      greater.number[i + j]--;
      n += S64(1000000000);
    }
    greater.number[i] = n;
  }

  // Removes a leading zeros
  while (!greater.number.back() && greater.number.size() > 1)
    greater.number.pop_back();

  *this = greater;
}

// ==================== O P E R A T O R S ====================

std::ostream& operator<<(std::ostream& os, const Bignum& self)
{
  for (int i = self.number.size() - 1; i >= 0; i--) {
    if (i != int(self.number.size()) - 1)
      os << std::setfill('0') << std::setw(9) << self.number[i];
    else
      os << self.number[i];
  }
  return os;
}

int Bignum::operator[](int index) const
{
  std::ostringstream stream;
  stream << *this;
  return stream.str()[index] - '0';
}

bool Bignum::operator==(const Bignum& other) const
{
  if (this->sign != other.sign || this->size != other.size)
    return false;

  for (int i = 0; i < this->size; i++)
    if (this->operator[](i) != other[i])
      return false;

  return true;
}

Bignum& Bignum::operator+=(int64_t number)
{
  this->add(number);
  return *this;
}

Bignum& Bignum::operator+=(const char *number)
{
  this->add(number);
  return *this;
}

Bignum& Bignum::operator+=(const std::string& number)
{
  this->add(number);
  return *this;
}

Bignum& Bignum::operator+=(const Bignum& number)
{
  this->add(number);
  return *this;
}

Bignum& Bignum::operator-=(int64_t number)
{
  this->sub(number);
  return *this;
}

Bignum& Bignum::operator-=(const char *number)
{
  this->sub(number);
  return *this;
}

Bignum& Bignum::operator-=(const std::string& number)
{
  this->sub(number);
  return *this;
}

Bignum& Bignum::operator-=(const Bignum& number)
{
  this->sub(number);
  return *this;
}

}  // namespace bignum