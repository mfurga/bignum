# Bignum - Arbitrary-precision arithmetic
[![Build Status](https://travis-ci.org/mfurga/bignum.svg?branch=master)](https://travis-ci.org/mfurga/bignum)
> A C++ library for arbitrary-precision arithmetic (bignum arithmetic/infinite-precision arithmetic). It allows to perform mathematical operations on large numbers larger than the typical ones offered by ALU (Arithmetic Logic Unit).

**Warning:** This library only supports an integer arithmetic!

## API

- **add** - Performs addition. 
```cpp
// Prototype
void Bignum::add(const Bignum& other);

// Example of use
Bignum number = "49065932804922113653927951924251926101";
number.add(-6713600);  // number += -6713600;
```

- **sub** - Performs substraction.
```cpp
// Prototype
void Bignum::sub(const Bignum& other, bool reverse=false);

// Example of use
Bignum number = 12842704;
number.sub(std::string("54343617203369600087"));  // number -= std::string("54343617203369600087");
```

- **mul** - Performs multiplication.
```cpp
// Prototype
void Bignum::mul(const Bignum& other);

// Example of use
Bignum number = "8368197571550797268160096810478314118164";
number.mul("-75064726199551713537");  // number *= "-75064726199551713537";
```

- **div** - Performs division.
```cpp
// Prototype
void Bignum::div(const Bignum& other);

// Example of use
Bignum number = std::string("3604968140033969808820011");
number.div("96085384492");  // number /= "96085384492";
```

- Relational and comparison operators (==, !=, >, <, >=, <=).
- Standard input (cin) / standard output (cout).
```cpp
Bignum number;
std::cin >> number;
std::cout << number << std::endl;
```
- Increment / decrement operators.
- Subscript operator (operator[]).
```cpp
Bignum number = "80414692938193581";
std::cout << number[0];  // prints 8
std::cout << number[4];  // prints 4
```

## License
MIT &copy; Mateusz Furga

