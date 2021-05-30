#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#include <string>
#include <bitset>
#include <vector>

class BigInteger {
private:

    char *memPos; // (Starting position of the number in the heap)
    long long leng;
    bool sign : 1;
    void toDec(std::string inpChar);
    static std::string addDecimals(std::string a, std::string b);

    BigInteger rightshift();
    BigInteger leftshift();

public:

    // Constructors
    BigInteger();
    explicit BigInteger(std::string number);
    BigInteger(long long int f);
    BigInteger (const BigInteger &obj);

    BigInteger& operator= (BigInteger &&obj) noexcept;
    BigInteger (BigInteger &&obj) noexcept;

    ~BigInteger();

    // Getters:
    long long getLength() const;  // In Bytes
    char *getHeapTip() const; // Return the pointer to the "head" of the bigInteger in the heap
    bool getSign() const;
    bool isEven() const;

    // Printers (temporary):
    std::string printBin(bool separator = true) const;
    std::string printDec(bool separator = true) const;

    // Tools
    static inline bool allZeros(const std::string &str);
    void addByte(char byte);
    void shrink();

    // Comparison
    int compareTo(BigInteger &other);

    bool operator< (BigInteger b1);
    bool operator> (BigInteger b1);
    bool operator<=(BigInteger b1);
    bool operator>=(BigInteger b1);
    bool operator==(BigInteger b1);
    bool operator!=(BigInteger b1);

    // Generic Operations

    // Bit Operations (Transformations)
    BigInteger onesComplement() const;
    BigInteger twosComplement() const;

    BigInteger leftShift(const BigInteger& b) const;
    BigInteger rightShift(const BigInteger& b) const;

    BigInteger operator ~ () const;
    BigInteger operator << (const BigInteger& b) const;
    BigInteger operator >> (const BigInteger& b) const;

    // Mathematical Operations
    BigInteger add(const BigInteger &b) const;
    BigInteger subtract(const BigInteger &b) const;
    BigInteger multiply(const BigInteger &b) const;
    BigInteger pow(BigInteger &b) const;

    BigInteger operator+(const BigInteger &a) const;
    BigInteger operator+() const;
    BigInteger operator++();
    BigInteger operator++(int);
    BigInteger operator--();
    BigInteger operator--(int);

    BigInteger operator*(const BigInteger& b) const;
};

#endif //BIGINTEGER_BIGINTEGER_H
