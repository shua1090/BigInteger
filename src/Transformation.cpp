#include <iostream>
#include "BigInteger.hpp"

BigInteger BigInteger::onesComplement() const {

    BigInteger newInt = *this;

    for (long long int i = 0; i < this->getLength(); i++){
        newInt.memPos[i] = (char) ~ this->memPos[i];
    }
    newInt.sign = !this->sign;

    newInt.shrink();

    return newInt;
}

BigInteger BigInteger::operator~() const{
    return this->onesComplement();
}

BigInteger BigInteger::twosComplement() const {
    BigInteger k = this->onesComplement();
    BigInteger b("1");
        if (k.sign == 1){
            k.sign = false;
            k = k.add( b );
            k.sign = true;
        } else {
            k = k.add(b);
        }
    k.shrink();

    return k;
}

BigInteger BigInteger::leftshift(){
    BigInteger f(*this);
    bool remainder = false;
    bool temp;
    f.addByte(0b00000000);
    for (long long int i = 0; i < f.getLength(); i++){
        temp = std::bitset<8>(f.memPos[i])[7];
        f.memPos[i] = char((unsigned char)f.memPos[i] << 1);
        f.memPos[i] = char((unsigned char) f.memPos[i] | remainder);
        remainder = temp;
    }
    f.shrink();
    return f;
}
BigInteger BigInteger::rightshift(){
    BigInteger f(*this);
    char remainder = 0;
    bool temp;
    for (long long int i = f.getLength() - 1; i>= 0; i--){
        temp = std::bitset<8>(f.memPos[i])[0];
        f.memPos[i] = char( (unsigned char) (f.memPos[i]) >> 1);
        f.memPos[i] = char((unsigned char) f.memPos[i] | (remainder << 7));
        remainder = (char)temp;
    }
    f.shrink();
    return f;
}

BigInteger BigInteger::leftShift(const BigInteger& b) const {
    BigInteger k = *this;
    for (BigInteger a("0"); a < b; a++){
        k = k.leftshift();
    }
    return k;
}

BigInteger BigInteger::rightShift(const BigInteger& b) const {
    BigInteger k = *this;
    for (BigInteger a("0"); a < b; a++){
        k = k.rightshift();
    }
    return k;
}

BigInteger BigInteger::operator << ( const BigInteger& b) const {
    return this->leftShift(b);
}
BigInteger BigInteger::operator >> ( const BigInteger& b) const {
    return this->rightShift(b);
}
