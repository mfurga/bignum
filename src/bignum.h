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
#pragma once

#include <string>
#include <vector>

#define U16(n) (static_cast<uint16_t>(n))
#define U32(n) (static_cast<uint32_t>(n))
#define U64(n) (static_cast<uint64_t>(n))

#define S16(n) (static_cast<int16_t>(n))
#define S32(n) (static_cast<int32_t>(n))
#define S64(n) (static_cast<int64_t>(n))

namespace bignum {

enum Base {
  BIN, DEC, HEX
};

enum Sign {
  NEGATIVE, POSITIVE
};

class Bignum {
 public:
  Bignum() {};
  Bignum(int64_t number);
  Bignum(const char *number);
  Bignum(const std::string& number);
  // Bignum(const std::string& number, const Base& repr);

  ~Bignum();

  Bignum operator=(int64_t number);
  Bignum operator=(const char *number);
  Bignum operator=(const std::string& number);

  int operator[](int index) const;
  bool operator==(const Bignum& other) const;
  // bool operator>(const Bignum& other) const;

  // Addition
  void add(int64_t number);
  void add(const char *number);
  void add(const std::string& number);
  void add(const Bignum& other);

  Bignum& operator+=(int64_t number);
  Bignum& operator+=(const char *number);
  Bignum& operator+=(const std::string& number);
  Bignum& operator+=(const Bignum& number);

  // Subtraction
  void sub(int64_t number);
  void sub(const char *number);
  void sub(const std::string& number);
  void sub(const Bignum& other, bool reverse=false);

  Bignum& operator-=(int64_t number);
  Bignum& operator-=(const char *number);
  Bignum& operator-=(const std::string& number);
  Bignum& operator-=(const Bignum& number);

  // Input / Output
  friend std::ostream& operator<<(std::ostream& os, const Bignum& number);

 public:
  int size = 0;
  Sign sign = POSITIVE;
  std::vector<uint32_t> number;

  void translate_string_to_number(const char *number, int length, 
                                  const Base& repr);
};

}  // namespace bignum