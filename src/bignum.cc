//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// ================================================================

#include <cstring>
#include <iomanip>
#include <cmath>

#include "bignum.h"

#define UNUSED(x) (void)(x)

#define U16(n) (static_cast<uint16_t>(n))
#define U32(n) (static_cast<uint32_t>(n))
#define U64(n) (static_cast<uint64_t>(n))

#define S16(n) (static_cast<int16_t>(n))
#define S32(n) (static_cast<int32_t>(n))
#define S64(n) (static_cast<int64_t>(n))

namespace bignum {

Bignum::Bignum(int number)
{
  this->sign = number >= 0 ? POSITIVE : NEGATIVE;

  while (number) {
    this->number.push_back(abs(number % 1000000000));
    number /= 1000000000;
  }

  this->normalizate();
}

Bignum::Bignum(int64_t number)
{
  this->sign = number >= 0 ? POSITIVE : NEGATIVE;

  while (number) {
    this->number.push_back(abs(number % 1000000000));
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

static inline int translate_char_to_digit(const char c)
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
  if (!this->number.size()) {
    this->size = 1;
    this->number.push_back(0);
    return;
  }

  uint32_t last = this->number.back();
  this->size = last == 0 ? 1 : 0;

  while (last) {
    last /= 10;
    this->size++;
  }
  if (this->number.size() > 1)
    this->size += (this->number.size() - 1) * 9;
}

void Bignum::translate_string_to_number(const char *number, int length, 
                                        const Base& repr)
{
  // Converts a c-string into chunks of 32-bit unsigned integers.
  // TODO: This method only support a decimal base system. Add support for hexadecimal and
  // binary numeral system. 

  UNUSED(repr);

  int base = 10, mul = 1;
  int MIN_VALUE = 0, MAX_VALUE = 9, MAX_MUL = 1000000000;

  uint32_t chunk = 0;

  for (int i = length - 1; i >= 0; i--) {
    if (mul == MAX_MUL) {
      this->number.push_back(chunk);
      chunk = 0; mul = 1;
    }
    if (translate_char_to_digit(number[i]) >= MIN_VALUE && 
        translate_char_to_digit(number[i]) <= MAX_VALUE) {
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

void Bignum::sub(int number, bool reverse)
{
  this->sub(Bignum(number), reverse);
}

void Bignum::sub(int64_t number, bool reverse)
{
  this->sub(Bignum(number), reverse);
}

void Bignum::sub(const char *number, bool reverse)
{
  this->sub(Bignum(number), reverse);
}

void Bignum::sub(const std::string& number, bool reverse)
{
  this->sub(Bignum(number), reverse);
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

  if (greater < smaller) {
    greater = other; smaller = *this;
    greater.sign = NEGATIVE;
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

void Bignum::mul(const Bignum& other)
{
  this->sign = 1 == !(this->sign ^ other.sign) ? POSITIVE : NEGATIVE;
  std::vector<uint32_t> number;

  for (size_t i = 0; i < other.number.size(); i++) {
    uint64_t carry = 0;

    for (size_t j = 0; j < this->number.size(); j++) {
      uint64_t n = U64(this->number[j]) * U64(other.number[i]) + carry;
      carry = 0;

      if (j + i < number.size()) {
        carry = number[j + i] + n % 1000000000;
        number[j + i] = carry % 1000000000;
        carry /= 1000000000;
      } else   
        number.push_back(n % 1000000000);
      carry += n / 1000000000;
    }

    if (carry > 0)
      number.push_back(carry);
  }

  this->number = number;
  this->normalizate();
}

std::ostream& operator<<(std::ostream& os, const Bignum& self)
{
  if (self.sign == NEGATIVE)
    os << "-";

  for (int i = self.number.size() - 1; i >= 0; i--) {
    if (i != int(self.number.size()) - 1)
      os << std::setfill('0') << std::setw(9) << self.number[i];
    else
      os << self.number[i];
  }
  return os;
}

std::istream& operator>>(std::istream& is, Bignum& number)
{
  std::string other;
  is >> other;
  number = other;
  return is;
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

bool Bignum::operator>(const Bignum& other) const
{
  if (this->sign == POSITIVE && other.sign == NEGATIVE)
    return true;
  else if (this->sign == NEGATIVE && other.sign == POSITIVE)
    return false;
  
  if ((this->size > other.size && this->sign == POSITIVE) ||
      (this->size < other.size && this->sign == NEGATIVE))
    return true;
  else if ((this->size > other.size && this->sign == NEGATIVE) || 
           (this->size < other.size && this->sign == POSITIVE))
    return false;

  for (int i = 0; i < this->size; i++) {
    if ((this->operator[](i) > other.operator[](i) && this->sign == POSITIVE) ||
        (this->operator[](i) < other.operator[](i) && this->sign == NEGATIVE))
      return true;
    else if ((this->operator[](i) > other.operator[](i) && this->sign == NEGATIVE) ||
             (this->operator[](i) < other.operator[](i) && this->sign == POSITIVE))
      return false;
  }
  return false;
}

bool Bignum::operator>=(const Bignum& other) const
{
  return (*this > other || *this == other);
}

bool Bignum::operator<(const Bignum& other) const
{
  if (this->sign == NEGATIVE && other.sign == POSITIVE)
    return true;
  else if (this->sign == POSITIVE && other.sign == NEGATIVE)
    return false;

  if ((this->size < other.size && this->sign == POSITIVE) ||
      (this->size > other.size && this->sign == NEGATIVE))
    return true;
  else if ((this->size < other.size && this->sign == NEGATIVE) || 
           (this->size > other.size && this->sign == POSITIVE))
    return false;

  for (int i = 0; i < this->size; i++) {
    if ((this->operator[](i) < other.operator[](i) && this->sign == POSITIVE) ||
        (this->operator[](i) > other.operator[](i) && this->sign == NEGATIVE))
      return true;
    else if ((this->operator[](i) < other.operator[](i) && this->sign == NEGATIVE) ||
             (this->operator[](i) > other.operator[](i) && this->sign == POSITIVE))
      return false;
  }
  return false;
}

bool Bignum::operator<=(const Bignum& other) const
{
  return (*this < other || *this == other);
}

Bignum& Bignum::operator=(int number)
{
  Bignum other(number);
  *this = other;
  return *this;
}

Bignum& Bignum::operator=(int64_t number)
{
  Bignum other(number);
  *this = other;
  return *this;
}

Bignum& Bignum::operator=(const char *number)
{
  Bignum other(number);
  *this = other;
  return *this;
}

Bignum& Bignum::operator=(const std::string& number)
{
  Bignum other(number);
  *this = other;
  return *this;
}

Bignum Bignum::operator+(int number) const
{
  Bignum other = *this;
  other.add(number);
  return other;
}

Bignum Bignum::operator+(int64_t number) const
{
  Bignum other = *this;
  other.add(number);
  return other;
}

Bignum Bignum::operator+(const char *number) const
{
  Bignum other = *this;
  other.add(number);
  return other;
}

Bignum Bignum::operator+(const std::string& number) const
{
  Bignum other = *this;
  other.add(number);
  return other;
}

Bignum Bignum::operator+(const Bignum& number) const
{
  Bignum other = *this;
  other.add(number);
  return other;
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

Bignum Bignum::operator-(int number) const
{
  Bignum other = *this;
  other.sub(number);
  return other;
}

Bignum Bignum::operator-(int64_t number) const
{
  Bignum other = *this;
  other.sub(number);
  return other;
}

Bignum Bignum::operator-(const char *number) const
{
  Bignum other = *this;
  other.sub(number);
  return other;
}

Bignum Bignum::operator-(const std::string& number) const
{
  Bignum other = *this;
  other.sub(number);
  return other;
}

Bignum Bignum::operator-(const Bignum& number) const
{
  Bignum other = *this;
  other.sub(number);
  return other;
}

Bignum& Bignum::operator-=(int number)
{
  this->sub(number);
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

Bignum& Bignum::operator++(int)
{
  this->add(1);
  return *this;
}

Bignum& Bignum::operator--(int)
{
  this->sub(1);
  return *this;
}

std::vector<uint32_t> Bignum::repr() const
{
  return this->number;
}

}  // namespace bignum