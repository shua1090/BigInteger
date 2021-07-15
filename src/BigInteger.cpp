#include "BigInteger.hpp"
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <random>
#include <string>
//#include <cstring>
#include<openssl/aes.h>
#include<openssl/evp.h>
#include<openssl/err.h>
#include<cryptopp/seckey.h>
#include<cryptopp/integer.h>


BigInteger::BigInteger(std::string number) {
    if(number[0] == '-'){
        this->sign = 1;
        number = number.substr(1);
    } else this->sign = 0;

    if (number.find_first_not_of("0123456789") != std::string::npos){
        std::cout << "The failing num: " << number << std::endl;
        throw std::exception();
    }
    else this->num = number;
}
BigInteger::BigInteger(long a) {
    std::string number = (std::to_string(a));
    if(number[0] == '-'){
        this->sign = true;
        number = number.substr(1);
    } else this->sign = false;

    if (number.find_first_not_of("0123456789") != std::string::npos){
        std::cout << "The failing num: " << number << std::endl;
        throw std::exception();
    }
    else this->num = number;
}
BigInteger::BigInteger(const std::string& number, bool sign) {
    this->sign = sign;
    if (number.find_first_not_of("0123456789") != std::string::npos){
        throw std::exception();
    }
    else this->num = number;
}
BigInteger::BigInteger(const BigInteger &b) {
    this->sign = b.sign;
    this->num = b.num;
}

BigInteger BigInteger::addDecimals(std::string a, std::string b) {

    std::string temp;

    if (std::min(a.length(), b.length()) == a.length()){
        long long z = b.length() - a.length();
        for (int i = 0; i < z; i++) {
            a = "0" + a;
        }
    } else {
        long long z = a.length() - b.length();
        for (int i = 0; i < z; i++)
            b = "0" + b;
    }

    int remainder = 0;
    for (long long i = a.length() - 1; i >= 0; i--){
        int z = int( (a[i] - '0') ) + int ( (b[i] - '0') ) + remainder;
        remainder = z / 10;
        temp = std::to_string(z % 10) + temp;
    }

    if (remainder > 0) temp = std::to_string( remainder ) + temp;

    return BigInteger(temp);
}
BigInteger BigInteger::subtractDecimals(const std::string& a, const std::string& b) {
    std::string temp;

    bool sign = false;

    std::string top;
    std::string bottom;
    int comparisonResult = compare(a, b);
    if (comparisonResult == -1){
        top = a; bottom = b;
    } else if (comparisonResult == 1){
        top = b; bottom = a; sign = true;
    } else {
        return BigInteger("0");
    }

    if (std::min(top.length(), bottom.length()) == top.length()){
        long long z = bottom.length() - top.length();
        for (int i = 0; i < z; i++) {
            top = "0" + top;
        }
    } else {
        long long z = top.length() - bottom.length();

        for (int i = 0; i < z; i++)
            bottom = "0" + bottom;
    }

    for (int times = top.length() - 1; times >= 0; times--){
        if (top[times] - bottom[times] >= 0)
            temp = char(top[times] - bottom[times] + '0') + temp;
        else {
            if (times < top.length() - 1 && top[times + 1] == '0'){

            }

            for (int reverse = times - 1; reverse >= 0; reverse--){
                if (top[reverse] > '0'){
                    top[reverse] = char(top[reverse] - 1 );
                    for (int i = reverse + 1; i < times; i++){
                        top[i] = '9';
                    }
                    temp = char(top[times] + 10 - bottom[times] + '0') + temp;
                    break;
                }
            }
        }
    }
    int i = 0;
    while (temp[i] == '0'){
        temp = temp.substr(1);
    }
    return BigInteger(((sign) ? "-" : "") + temp);
}

BigInteger BigInteger::add(const BigInteger& b) const {

    if (this->sign && b.sign){
        BigInteger a = addDecimals(b.num, this->num);
        a.sign = true;
        return a;
    } else if (this->sign && !b.sign){
        return subtractDecimals(b.num, this->num);
    } else if (!this->sign && b.sign){
        return subtractDecimals(this->num, b.num);
    } else return BigInteger(addDecimals(this->num, b.num));

}
BigInteger BigInteger::subtract(const BigInteger& b) const {
    if (this->sign && b.sign){
        return this->add(BigInteger(b.num));
    } else if (this->sign && !b.sign){
        return this->add(BigInteger("-" + b.num));
    } else if (!this->sign && b.sign){
        return b.add(BigInteger(this->num));
    } else {
        return this->subtractDecimals(this->num, b.num);
    }
}
BigInteger BigInteger::multiply(const BigInteger &b) const {

    if (this->num.starts_with('0') || b.num == "0") return BigInteger("0");
    if (this->num == "1") return b;
    if (b.num == "1") return *this;

    BigInteger a("0");

    for (long i = b.num.length() - 1; i >= 0; i--){
        a = a + BigInteger(simpleMultiply(this->num, b.num[i]) + std::string (b.num
                .length() - 1  - i, '0'));
    }

    if ((b.sign && !this->sign) || (!b.sign && this->sign)) a.sign = true;
    return a;
}
BigInteger BigInteger::divide(const BigInteger &b) const {
    BigInteger dividend = *this;
    dividend.sign = false;
    BigInteger divisor = b;
    divisor.sign = false;

    BigInteger counter(0);

    while (true) {
        if (dividend < divisor){
            break;
        } else {
            dividend = dividend - divisor;
            counter++;
        }
    }

    if ((b.sign && !this->sign) || (!b.sign && this->sign)) counter.sign = 1;
    return counter;
}
BigInteger BigInteger::mod(const BigInteger &b) const {
    BigInteger temp = *this / b;
    return *this - (b * temp);

//
//    if (b.sign || this->sign) return 0;
//    BigInteger dividend = *this;
//    dividend.sign = false;
//    BigInteger divisor = b;
//    divisor.sign = false;
//
//    if (b.num.length() < this->num.length() -1){
//        int multiplier = 0;
//        while (dividend.num.length() > divisor.num.length() + 1){
//            long amountOfZeroes = this->num.length();
//            std::cout << "Amount of Zeroes: " << amountOfZeroes - multiplier<< std::endl;
//            if (dividend < 0){
//                dividend = dividend + (BigInteger("1" + std::string(amountOfZeroes - multiplier, '0')) * divisor);
//                multiplier += 1;
//            }
//            std::cout << "Old Dividend: " << dividend.toString() << std::endl;
//            dividend = dividend - (BigInteger("1" + std::string(amountOfZeroes - multiplier, '0')) * divisor);
//            std::cout << "New Dividend: " << dividend.toString() << std::endl;
//        }
//        long amountOfZeroes = this->num.length() - divisor.num.length();
//        std::cout << "Newest Dividend: " << dividend.toString() << std::endl;
//        std::cout << "New divisor: " << divisor.toString() << std::endl;
//    }
//
//    while (true) {
//        std::cout << "IN this LOOP" << std::endl;
//        if (dividend < divisor){
//            if (dividend < 0){
//                dividend = dividend + divisor;
//            }
//            break;
//        } else {
//            dividend = dividend - divisor;
//        }
//    }
//
//    return dividend;
}
BigInteger BigInteger::pow(const BigInteger &b) const {
    if (b < BigInteger(0)) return 0;
    if (b == BigInteger(0)) return 1;
    if (b == BigInteger(1)) return *this;

    BigInteger val = *this;
    BigInteger orig = *this;

    for (BigInteger counter = BigInteger(1); counter < b; counter++){
        orig = orig * val;
    }
    return orig;
}


std::string BigInteger::simpleMultiply(const std::string &a, const char b) {
    std::string num = "0";
    // Check this if 0 errors:
    if (a.starts_with('0') || b == '0') return "0";
    if (a == "1") return std::string(1, b);
    if (b == '1') return a;

    for (int i = 0; i < (b-'0'); i++){
        num = addDecimals(num, a).toString();
    }
    return num;
}

int BigInteger::compare(std::string a, std::string b){
    if (b[0] == '-' && a[0] != '-') return -1;
    else if (a[0] == '-' && b[0] != '-') return 1;
    else if (a.length() > b.length()) return (a[0] != '-' && b[0] != '-') ? -1 : 1;
    else if (a.length() < b.length()) return (a[0] != '-' && b[0] != '-') ? 1 : -1;
    else {
        for (int f = 0; f < a.length(); f++){
            if (a[f] > b[f]) return (a[0] == '-') ? 1 : -1;
            else if (b[f] > a[f]) return (b[0] == '-') ? -1 : 1;
        }
        return 0;
    }
}

std::string BigInteger::toString() const {
    std::string temp = this->num;
    return ((this->sign) ? "-" : "") + temp;
}

// Binary Operations
BigInteger BigInteger::operator-(const BigInteger &b) const {
    return this->subtract(b);
}
BigInteger BigInteger::operator+(const BigInteger &b) const {
    return this->add(b);
}
BigInteger BigInteger::operator*(const BigInteger &b) const {
    return this->multiply(b);
}
BigInteger BigInteger::operator/(const BigInteger &b) const{
    return this->divide(b);
}
BigInteger BigInteger::operator%(const BigInteger &b) const{
    return this->mod(b);
}

// Unary Operations
BigInteger BigInteger::operator-() {
    if (this->sign) this->sign = false;
    else this->sign = true;
}

BigInteger BigInteger::operator++() {
    BigInteger temp = this->add(BigInteger("1"));
    this->num = temp.num;
    this->sign = temp.sign;
    return temp;
}
BigInteger BigInteger::operator++(int) {
    BigInteger before = *this;
    ++(*this);
    return before;
}
BigInteger BigInteger::operator--() {
    BigInteger temp = this->subtract(BigInteger("1"));
    this->num = temp.num;
    this->sign = temp.sign;
    return temp;
}
BigInteger BigInteger::operator--(int) {
    BigInteger before = *this;
    --(*this);
    return before;
}

// Comparison Operation
bool BigInteger::operator<(const BigInteger &b) const{
    return (this->compare(this->toString(), b.toString()) == 1);
}
bool BigInteger::operator>(const BigInteger &b) const {
    return (this->compare(this->toString(), b.toString()) == -1);
}
bool BigInteger::operator<=(const BigInteger &b) const {
    return (this->compare(this->toString(), b.toString()) > -1);
}
bool BigInteger::operator>=(const BigInteger &b) const {
    return (this->compare(this->toString(), b.toString()) < 1);
}
bool BigInteger::operator==(const BigInteger &b) const {
    return (this->compare(this->toString(), b.toString()) == 0);
}
bool BigInteger::operator!=(const BigInteger &b) const {
    return (this->compare(this->toString(), b.toString()) != 0);
}

int main(int argc, char **argv) {

    //9223372036854775807
    auto start = std::chrono::high_resolution_clock::now();
//
//    std::cout << (BigInteger(500).pow(500)
//    % BigInteger(2)).toString() << std::endl;
//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//    std::cout << duration.count() << " microseconds" << std::endl;
    return 0;
}
