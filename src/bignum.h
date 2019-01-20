//
// Copyright (c) 2019 Mateusz Furga <matfurga@gmail.com>
// This software is released under the MIT license (see LICENSE).
// ================================================================

#pragma once

#include <string>
#include <vector>

namespace bignum {

// Allowed representations of the number.
enum class Base
{
  BIN,  // Binary numeral system (K = 2).
  DEC,  // Decimal numeral system (K = 10).
  HEX   // Hexadecimal numeral system (K = 16).
};

// Allowed signs of the number.
enum Sign 
{
  NEGATIVE,
  POSITIVE
};

class Bignum 
{
 public:
  int size = 0;
  Sign sign = POSITIVE;

  Bignum() {};
  Bignum(int number);
  Bignum(int64_t number);
  Bignum(const char *number);
  Bignum(const std::string& number);
  // Bignum(const std::string& number, const Base& repr);

  ~Bignum();

  Bignum& operator=(int number);
  Bignum& operator=(int64_t number);
  Bignum& operator=(const char *number);
  Bignum& operator=(const std::string& number);

  // Addition
  void add(int number);
  void add(int64_t number);
  void add(const char *number);
  void add(const std::string& number);
  void add(const Bignum& other);

  Bignum& operator+=(int number);
  Bignum& operator+=(int64_t number);
  Bignum& operator+=(const char *number);
  Bignum& operator+=(const std::string& number);
  Bignum& operator+=(const Bignum& number);

  // Subtraction
  void sub(int number, bool reverse=false);
  void sub(int64_t number, bool reverse=false);
  void sub(const char *number, bool reverse=false);
  void sub(const std::string& number, bool reverse=false);
  void sub(const Bignum& other, bool reverse=false);

  Bignum& operator-=(int number);
  Bignum& operator-=(int64_t number);
  Bignum& operator-=(const char *number);
  Bignum& operator-=(const std::string& number);
  Bignum& operator-=(const Bignum& number);

  // Input / Output
  friend std::ostream& operator<<(std::ostream& os, const Bignum& number);
  friend std::istream& operator>>(std::istream& is, Bignum& number);

  // Operators
  bool operator==(const Bignum& other) const;
  bool operator!=(const Bignum& other) const;
  int operator[](int index) const;

  std::vector<uint32_t> repr() const;

 private:
  std::vector<uint32_t> number;

  void translate_string_to_number(const char *number, int length, 
                                  const Base& repr);
  void normalizate();
};

}  // namespace bignum