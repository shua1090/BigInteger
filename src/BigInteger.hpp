#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#include <string>
#include <bitset>
#include <vector>

class BigInteger {
private:

    BigInteger(const std::string& number, bool sign);

    std::string num;
    bool sign : 1;

    inline static BigInteger addDecimals(std::string a, std::string b);
    inline static BigInteger subtractDecimals(const std::string& a, const std::string& b);
    inline static std::string simpleMultiply(const std::string &a, char b);
    inline static int compare(std::string a, std::string b);
public:
    // Constructor

    BigInteger(std::string number);
    BigInteger(long a);
    BigInteger(const BigInteger &b);

    // Addition
    BigInteger add(const BigInteger& b) const;
    BigInteger subtract(const BigInteger &b) const;
    BigInteger multiply(const BigInteger &b) const;
    BigInteger divide(const BigInteger &b) const;
    BigInteger mod(const BigInteger &b) const;
    BigInteger pow(const BigInteger &b) const;

    // Overloads
    BigInteger operator+(const BigInteger &b) const;
    BigInteger operator-(const BigInteger &b) const;
    BigInteger operator*(const BigInteger &b) const;
    BigInteger operator/(const BigInteger &b) const;
    BigInteger operator%(const BigInteger &b) const;

    BigInteger operator-();

    BigInteger operator++() ;
    BigInteger operator++(int) ;
    BigInteger operator--() ;
    BigInteger operator--(int) ;

    bool operator<(const BigInteger &b) const;
    bool operator>(const BigInteger &b) const;
    bool operator>=(const BigInteger &b) const;
    bool operator<=(const BigInteger &b) const;
    bool operator==(const BigInteger &b) const;
    bool operator!=(const BigInteger &b) const;

    std::string toString() const;

};

#endif //BIGINTEGER_BIGINTEGER_H
