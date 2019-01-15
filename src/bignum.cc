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
#include <iomanip>
#include <cmath>

#include "bignum.h"

#define UNUSED(x) (void)(x)

namespace bignum {

Bignum::Bignum(int number)
{
  if (number >= 0)
    this->sign = POSITIVE;
  else
    this->sign = NEGATIVE;

  number = llabs(number);
  while (number) {
    this->number.push_back(number % 1000000000);
    number /= 1000000000;
  }

  this->normalizate();
}

Bignum::Bignum(int64_t number)
{
  if (number >= 0)
    this->sign = POSITIVE;
  else
    this->sign = NEGATIVE;

  number = llabs(number);
  while (number) {
    this->number.push_back(number % 1000000000);
    number /= 1000000000;
  }

  this->normalizate();
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

inline int translate_char_to_digit(const char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return -1;
}

void Bignum::normalizate()
{
  if (this->number.size() == 0) {
    this->number.push_back(0);
    this->size = 1;
    return;
  }

  uint32_t last = this->number.back();
  int size = last == 0 ? 1 : 0;

  while (last) {
    last /= 10;
    size++;
  }
  if (this->number.size() > 1)
    size += (this->number.size() - 1) * 9;

  this->size = size;
}

void Bignum::translate_string_to_number(const char *number, int length, 
                                        const Base& repr)
{
  // Converts a c-string into chunks of 32-bit unsigned integers.
  // TODO: This method only support a decimal base system. Add support for hexadecimal and
  // binary numeral system. 

  UNUSED(repr);

  int base = 10, mul = 1;
  int MIN_VALUE = 0, MAX_VALUE = 9;

  uint32_t chunk = 0;

  for (int i = length - 1; i >= 0; i--) {
    if (mul == 1000000000) {
      this->number.push_back(chunk);
      chunk = 0; mul = 1;
    }
    if ((translate_char_to_digit(number[i]) >= MIN_VALUE) && 
        (translate_char_to_digit(number[i]) <= MAX_VALUE)) {
      chunk += translate_char_to_digit(number[i]) * mul;
      mul *= base;
    }
  }

  if (chunk > 0)
    this->number.push_back(chunk);

  this->normalizate();
}

void Bignum::add(int number)
{
  this->add(Bignum(number));
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

  // Calls a subtraction method for the following scenarios: 
  // - two numbers (A and B) number A with a positive sign and number B with a negative
  //   sign: A + (-B) = A - B
  if (this->sign == POSITIVE && other.sign == NEGATIVE) {
    number.sign = POSITIVE;
    return this->sub(number);
  }

  // Calls a subtraction method for the following scenarios: 
  // - two numbers (A and B) number A with a negative sign and number B with a positive
  //   sign: -A + B = B - A
  if (this->sign == NEGATIVE && other.sign == POSITIVE) {
    this->sign = POSITIVE;
    return this->sub(other, true);  // NOTE: Calls a sub method as reverse
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
    uint64_t sum = U64(this->number[i]) + U64(number.number[i]) + U64(carry);
    carry = sum / 1000000000;
    this->number[i] = sum % 1000000000;
  }

  if (carry > 0)
    this->number.push_back(carry);

  this->normalizate();
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
  // - two numbers (A and B) with a positive sign: A - B or B - A

  Bignum number = other;

  // Calls an addition method for the following scenarios:
  // - two numbers (A and B) number A with a positive sign and number B with a negative 
  //   sign: A - (-B) = A + B
  // - two numbers (A and B) with a negative sign: -A - (-B) = -A + B
  if ((this->sign == POSITIVE && other.sign == NEGATIVE) ||
      (this->sign == NEGATIVE && other.sign == NEGATIVE)) {
    number.sign = POSITIVE;
    return this->add(number);
  }

  // Calls an addition method for the following scenarios:
  // - two numbers (A and B) number A with a negative sign and number B with a positive 
  //   sign: -A - B = -(A + B)
  if (this->sign == NEGATIVE && other.sign == POSITIVE) {
    this->sign = POSITIVE;
    this->add(other);
    this->sign = NEGATIVE;
    return;
  }

  Bignum greater = *this, smaller = other;

  if (this->size < other.size) {
    greater = other; smaller = *this;
    greater.sign = NEGATIVE;
  }
  else if (this->size == other.size) {
    for (int i = 0; i < this->size; i++) {
      if (this->operator[](i) > other[i]) {
        greater = *this; smaller = other;
        greater.sign = POSITIVE; break;
      }
      else if (this->operator[](i) < other[i]) {
        greater = other; smaller = *this;
        greater.sign = NEGATIVE; break;
      }
    }
  }

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

  while (!greater.number.back() && greater.number.size() > 1)
    greater.number.pop_back();

  greater.normalizate();

  if (reverse && greater != 0)
    greater.sign = greater.sign == POSITIVE ? NEGATIVE : POSITIVE;

  *this = greater;
}

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
  int idx = this->size - index - 1;
  return int(this->number[idx / 9] / (pow(10, idx % 9))) % 10;
}

bool Bignum::operator==(const Bignum& other) const
{
  if (this->sign != other.sign || this->size != other.size)
    return false;

  for (size_t i = 0; i < this->number.size(); i++)
    if (this->number[i] != other.number[i])
      return false;

  return true;
}

bool Bignum::operator!=(const Bignum& other) const
{
  if (this->sign != other.sign || this->size != other.size)
    return true;

  for (size_t i = 0; i < this->number.size(); i++)
    if (this->number[i] != other.number[i])
      return true;

  return false;
}

Bignum& Bignum::operator+=(int number)
{
  this->add(number);
  return *this;
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

std::vector<uint32_t> Bignum::repr() const
{
  return this->number;
}

}  // namespace bignum