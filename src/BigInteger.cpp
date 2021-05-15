#include "BigInteger.hpp"
#include <vector>
#include <cassert>
#include <iostream>
#include <thread>
#include <typeinfo>
#include <chrono>

BigInteger::BigInteger() {
    this->memPos =(char*) calloc(0, sizeof(char));
    this->leng = 0;
}

// Constructor
BigInteger::BigInteger(std::string& str) {
    this->memPos = (char *) malloc(1);
    this->leng = 1;

    assert(this->memPos != nullptr);

    toDec(str);
}

// Destructor
BigInteger::~BigInteger() {
    free(this->memPos);
}

long long BigInteger::getLength() const {
    return this->leng;
    return 0;
}

char *BigInteger::getHeapTip() {
    return this->memPos;
}

inline bool BigInteger::allZeros(const std::string &str) {
    return str.find_first_not_of('0') == std::string::npos;
}

void BigInteger::printVec(const std::vector<int> &vec) {
    int a = 0;
    for (auto &n : vec){
        printf("%d", n);
        if (a == 8) {
            printf("_");
            a = 0;
        }
        a++;
    }
    printf("\n");
}

void BigInteger::littleEndConstruct(char inputArr[],
                                    long long inputArrSize) {  // Construct this BigInteger from a LittleEndian byte array
    this->memPos = (char *) malloc(inputArrSize);
    this->leng = inputArrSize;



    for (long long i = 0; i < inputArrSize; i++) {
        memPos[this->leng - 1 - i] = inputArr[i];
    }

}

char *BigInteger::getLittleBytes() {
    return this->memPos;
}


void BigInteger::printBinary() {
    for (long long i = this->leng - 1; i >= 0; i--) {
        char shoot = this->memPos[i];
        std::bitset<8> x(shoot);
        std::cout << x << '\n';
    }
} // End Function

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

void BigInteger::printDec(){
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
    std::cout << "It is: " << num << std::endl;
}

void BigInteger::addByte(char byte){
    this->leng++;
    this->memPos = (char*) realloc(this->memPos, this->leng);

    if (this->memPos == nullptr) throw std::exception();
    else{
//        for (int i = this->leng - 2; i >= 0; i--){
//            this->memPos[i + 1] = this->memPos[i];
//        }
        this->memPos[this->leng - 1] = byte;

    }
}

void BigInteger::add(BigInteger& b){
    char * other = b.getHeapTip();

//    auto k = std::min(this->leng, b.leng);

    int remainder = 0;
    BigInteger f;

    for (int i = 0; i < std::max(this->leng, b.leng) ; i++){

        std::bitset<8> thatAccessorByte;
        std::bitset<8> thisAccessorByte;

        if ( i >= b.getLength() )
            thatAccessorByte = std::bitset<8>(0b00000000);

        else
            thatAccessorByte = std::bitset<8> (   other[  i] );

        if ( i >= this->getLength() )
            thisAccessorByte = std::bitset<8> (0b00000000);

        else
            thisAccessorByte = std::bitset<8> (this->memPos[ i ]);

        std::bitset<8> newByte;

        for (int bit = 0; bit < 8; bit++){

            auto inp1 = thatAccessorByte[bit];
            auto inp2 = thisAccessorByte[bit];

            newByte[bit] = (inp1  ^ inp2 ) ^ remainder;
            remainder = (inp1 & inp2) | ( (inp1 ^ inp2) & remainder);
        }


        f.addByte( static_cast<char> ( newByte.to_ulong() ) );
    }


    f.addByte( (char) remainder);

    std::cout << "\nAddition Results in:\n";
//    f.printBinary();
//    f.printDec();

}

void BigInteger::toDec(std::string &numString) {

    // LAMBDA TOINT
    auto toInt = [](char inpChar) {
        return static_cast<int>(inpChar - '0');
    };

    // LAMBDA TOCHAR
    auto toChar = [](int inpNum) {
        return static_cast<char>(inpNum + '0');
    };

//    if (numString[0] == '-') {
//        this->sign = true;
//    } else {
//        this->sign = false;
//    } // End Conditional

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

            if (this->memPos == nullptr)
                throw std::exception();
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

int main(int argc, char **argv) {

    //9223372036854775807

    auto start = std::chrono::high_resolution_clock::now();

    std::string a = "922337203685477580792233720368547758079223372036854775807";
    BigInteger b(a);

    std::string c = "9223372036854775807922337203685477580792233720368547758079223372036854775807922337203685477580792233720368547758079223372036854775807";
    BigInteger d(c);

    std::cout << "Size of b: " << b.getLength() << "--";
    std::cout << "Size of d: " << d.getLength() << std::endl;

    b.add(d);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << " microseconds" << std::endl;

    return 0;
}