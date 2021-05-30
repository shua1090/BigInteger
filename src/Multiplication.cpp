#include <iostream>
#include "BigInteger.hpp"

BigInteger BigInteger::multiply(const BigInteger& b) const {
    BigInteger result;
    BigInteger thisInteger = *this;
    thisInteger.sign = false;

    for (long long int i = 0; i < b.getLength(); i++){
        std::bitset<8> theBits(b.memPos[i]);

        for (int l = 0; l < 8; l++){
            if (theBits[l] == 1){
                result = result + (thisInteger << BigInteger((i) * 8 + l));
                result.shrink();
            }
        }

    }

    if (b.getSign() == 0 && this->getSign() == 0){
        result.sign = false;
    } else if (b.getSign() && this->getSign()){
        result.sign = false;
    } else {
        result.sign = true;
    }

    return result;
}

BigInteger BigInteger::operator*(const BigInteger& b) const{
    return this->multiply(b);
}

BigInteger BigInteger::pow(BigInteger &b) const{
    if (b < BigInteger("0")) return 0;
    if (b == BigInteger("0")){
        return 1;
    }

    BigInteger result = BigInteger("1");

    for (BigInteger f("0"); f < b; f++){
        result = result * *this;
    }

    return result;
}

