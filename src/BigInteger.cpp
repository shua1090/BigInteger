#include "BigInteger.hpp"
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <random>
#include <string>

#include <regex>
inline static std::string removePrecedingZeroesAndUnderscores(std::string temp){
    auto numNotOfZero = temp.find_first_not_of('0');

    if (numNotOfZero== std::string::npos) temp = "0";
    else {
        temp = std::regex_replace(temp.substr(numNotOfZero), std::regex("_|,"), "");
    }
    return temp;
}

BigInteger::BigInteger(std::string number) {
    if(number[0] == '-'){
        this->sign = 1;
        number = number.substr(1);
    } else this->sign = 0;

    if (number.find_first_not_of("0123456789") != std::string::npos){
        std::cout << "The failing num: " << number << std::endl;
        throw std::exception();
    }
    else this->num = removePrecedingZeroesAndUnderscores(number);
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
    else this->num = removePrecedingZeroesAndUnderscores(number);
}
BigInteger::BigInteger(const std::string& number, bool sign) {
    this->sign = sign;
    if (number.find_first_not_of("0123456789") != std::string::npos){
        throw std::exception();
    }
    else this->num = removePrecedingZeroesAndUnderscores(number);
}
BigInteger::BigInteger(const BigInteger &b) {
    this->sign = b.sign;
    this->num = removePrecedingZeroesAndUnderscores(b.num);
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

// A // B
BigInteger BigInteger::intDivide(std::string a, const std::string& b){
    if (compare(a, b) == 1) return 0;
    if (a.find_first_not_of('0') == std::string::npos) return 0;
    if (b.find_first_not_of('0') == std::string::npos) throw std::exception();
    else {
        std::string tempDividend = a;
        BigInteger amount(0);
        while (true){
            a = subtractDecimals(a,b).toString();
//            std::cout << "a: " << a << " divisor: " << b << std::endl;
            if (a.starts_with('-')) break;
            else amount = amount + 1;
        }
        return amount;
    }
}
int BigInteger::compare(const std::string& a, const std::string &b){
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

    if (this->num.rfind("0", 0) == 0 || b.num == "0") return BigInteger("0");
    if (this->num == "1") return b;
    if (b.num == "1") return *this;

//    BigInteger a("0");
    std::string a = "0";

    for (unsigned long i = b.num.length() - 1; i >= 0; i--){
        a = a + simpleMultiply(this->num, b.num[i]) + std::string (b.num
                .length() - 1  - i, '0');
        if (i == 0) break;
    }
    BigInteger res(a);
    if ((b.sign && !this->sign) || (!b.sign && this->sign)) res.sign = true;
    return a;
}
BigInteger BigInteger::divide(const BigInteger &b) const {
    if (this->operator<(b)){return BigInteger(0);}
    else {
        std::string tempCount = this->num.substr(0, 1);
        std::string result;
        BigInteger tempThing(0);
        for (long a = 1; a < this->num.length(); a++){
            int numNotOfZero = tempCount.find_first_not_of('0');

            if (numNotOfZero== std::string::npos) tempCount = "0";
            else tempCount = tempCount.substr(numNotOfZero);
                tempThing = intDivide(tempCount, b.num);

            if (tempThing.num[0] == '0'){
                tempCount += this->num[a];
                result += "0";
            } else {
                result += tempThing.num;
                tempCount = subtractDecimals(tempCount, simpleMultiply(b.num, tempThing.num[0])).num +(std::string(1, this->num[a]) );
            }
        }
        auto numNotOfZero = tempCount.find_first_not_of('0');

        if (numNotOfZero== std::string::npos) tempCount = "0";
        else tempCount = tempCount.substr(numNotOfZero);

        bool sign = false;
        if ((b.sign && !this->sign) || (!b.sign && this->sign)) sign = true;
        return result + intDivide(tempCount, b.num).num;
    }
}
BigInteger BigInteger::mod(const BigInteger &b) const {
    if (this->operator<(b)){return *this;}
    else {
        return this->subtract(this->divide(b).multiply(b));
    }
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
    if (a.rfind('0', 0) == 0 || b == '0') return "0";
    if (a == "1") return std::string(1, b);
    if (b == '1') return a;

    for (int i = 0; i < (b-'0'); i++){
        num = addDecimals(num, a).num;
    }
    return num;
}
int BigInteger::toInt() const{
    return std::stoi(this->toString());
}

long BigInteger::toLong() const{
    return std::stoll(this->toString());
}

std::string BigInteger::toString() const {
    std::string temp = this->num;
    return ((this->sign) ? "-" : "") + temp;
}

std::string BigInteger::toString(unsigned int radix) const {
    if (radix > 16 || radix <= 0) throw std::exception();
    else {
        BigInteger cop = *this;
        std::string stringNum;
        while (cop > 0){
            int rem = (cop % radix).toInt();
            if (rem > 9){
                rem = (rem % 10) + 'A';
            } else rem = rem + '0';
            stringNum = char(rem) + stringNum ;
            cop = cop / radix;
        }
        return stringNum;
    }
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
    BigInteger cop = *this;
    cop.sign = (cop.sign) ? 0 : 1;
    return cop;
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

std::ostream &operator<<(std::ostream &os, const BigInteger &dt) {
    os << dt.toString();
    return os;
}

int main(int argc, char **argv) {
    //9223372036854775807
    for (int i = 0; i < 5; i++){
        auto start = std::chrono::high_resolution_clock::now();
        BigInteger a = (500_i).pow(500);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << duration.count() << " milliseconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        a.divide(2);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << duration.count() << " milliseconds" << std::endl;
        std::cout << "====" << std::endl;

    }
    return 0;
}
