#include "BigInteger.hpp"
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <random>

BigInteger::BigInteger() {
    this->memPos =(char*) calloc(0, sizeof(char));
    this->leng = 0;

    this->sign = 0;
}

// Constructor
BigInteger::BigInteger(std::string str) {
    this->memPos = (char *) malloc(1);
    this->leng = 1;

    assert(this->memPos != nullptr);

    toDec(std::move(str));
}

BigInteger::BigInteger(long long int f) {
    this->memPos = (char *) calloc(1, sizeof(char));
    this->leng = 1;
    toDec(std::to_string(f));
}

BigInteger::BigInteger(const BigInteger &obj) {
    this->memPos = (char * ) calloc(obj.getLength(), sizeof(char));

    if (this->memPos == nullptr) throw std::exception();
    this->leng = obj.getLength();

    for (long long int i = 0; i < obj.getLength(); i++){
        this->memPos[i] = obj.memPos[i];
    }


    this->sign = obj.getSign();
}

BigInteger& BigInteger::operator= (BigInteger &&obj) noexcept {
    this->memPos = obj.getHeapTip();

    this->leng = obj.getLength();
    this->sign = obj.getSign();

    obj.memPos = nullptr;

    return *this;
}

BigInteger::BigInteger (BigInteger && obj) noexcept {
    this->memPos = obj.getHeapTip();

    this->leng = obj.getLength();
    this->sign = obj.getSign();

    obj.memPos = nullptr;
}

// Destructor
BigInteger::~BigInteger() {
    free(this->memPos);
}

long long BigInteger::getLength() const {
    return this->leng;
    return 0;
}

char *BigInteger::getHeapTip() const {
    return this->memPos;
}

bool BigInteger::getSign() const{
    return this->sign;
}

void BigInteger::shrink() {
    if (this->memPos[this->leng - 1] != 0) return;

    if (this->leng <= 1) return;

    for (long long int f = this->leng - 1; f >= 0; f--){
        if (this->memPos[f] == 0b00000000){
            this->memPos = (char* )realloc(this->memPos, this->leng-1);
            this->leng--;
        }
        else
            break;
    }
}

inline bool BigInteger::allZeros(const std::string &str) {
    return str.find_first_not_of('0') == std::string::npos;
}


std::string BigInteger::printBin(bool separator) const {
    std::string num = "";

    num = std::to_string((int)this->sign) + num;

    for (long long i = this->leng - 1; i >= 0; i--) {
        num += ((separator) ? "_" : "");
        char shoot = this->memPos[i];
        std::bitset<8> x(shoot);
        for (int i = 7; i >= 0; i--){
            num = num + std::to_string((int)x[i]);
        }
    }
    return num;
}

std::string BigInteger::printDec(bool separator) const {
    std::string num = "0";
    for (long long i = 0; i < this->leng; i++) {
        char shoot = this->memPos[i];
        std::bitset<8> x(shoot);
        for (int f = 0; f < 8; f++){
            if (x[f] == 1){

                std::string waste = "1";
                for (int number_range = 1; number_range <= i * 8 + f; number_range++){
                    if (number_range == 1){
                        waste = addDecimals("0", "2");
                    } else {
                        waste = addDecimals(waste, waste);
                    }
                }

                num = addDecimals(num, waste);

            }
        }

    }
    if (this->getSign() == 1) num = "-" + num;
    return num;
}

std::string BigInteger::addDecimals(std::string a, std::string b){

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

    return temp;
}


void BigInteger::addByte(char byte){
    this->leng++;
    this->memPos = (char*) realloc(this->memPos, this->leng);

    if (this->memPos == nullptr) throw std::exception();
    else{
        this->memPos[this->leng - 1] = byte;

    }
}

void BigInteger::toDec(std::string numString) {

    // TODO: Remove Lambda and replace to inline
    auto toInt = [](char inpChar) {
        return static_cast<int>(inpChar - '0');
    };

    auto toChar = [](int inpNum) {
        return static_cast<char>(inpNum + '0');
    };

    if (numString[0] == '-') {
        this->sign = true;
        numString = numString.substr(1);
    } else {
        this->sign = false;
    } // End Conditional

    short currentBitCount = 0;
    long long currentByteCount = 0;
    this->memPos[currentByteCount] = 0b00000000;

    while (!allZeros(numString)) {
        int remainder = 0;


        for (char & i : numString) {
            int temp = remainder * 10 + toInt(i);
            i = toChar(
                    temp / 2);
            remainder = temp %
                        2;
        }

        if (currentBitCount == 8) {
            this->leng += 1;
            this->memPos = (char *) realloc(this->memPos, this->leng);

            if (this->memPos == nullptr){
                std::cout << "memrr";
                throw std::exception();

            }
            currentBitCount = 0;
            currentByteCount++;
            this->memPos[currentByteCount] = 0b00000000;
        }

        // Put the remainder into the bits
        this->memPos[currentByteCount] |= (remainder
                << currentBitCount);
        currentBitCount++;
    }


}

bool BigInteger::isEven() const {
    if (std::bitset<8>(this->memPos[this->getLength() - 1])[0] == 1){
        return false;
    }
    return true;
}


int main(int argc, char **argv) {

    //9223372036854775807

    auto start = std::chrono::high_resolution_clock::now();

    BigInteger f = BigInteger("500");
    BigInteger k = BigInteger("50");

    for (int i = 0; i < 10; i++){
        unsigned int x =  std::rand() % 100;
        std::cout << x << std::endl;
    }

//    std::cout << f.pow(k).printDec() << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << " microseconds" << std::endl;


    return 0;
}
