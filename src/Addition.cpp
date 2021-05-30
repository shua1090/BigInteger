#include <iostream>
#include "BigInteger.hpp"

BigInteger BigInteger::add(const BigInteger &b) const {
    char * other = b.getHeapTip();

    int remainder = 0;
    BigInteger f;

    if (this->getSign() == 1 && b.getSign() == 1){
        f.sign = true;
    } else if (this->getSign() == 1 && b.getSign() != 1){
        BigInteger k = *this;
        k.sign = false;
        return b.subtract(k);
    } else if (b.getSign() == 1 & this->getSign() != 1){
        BigInteger k = b;
        k.sign = false;
        return this->subtract(k);
    }

    for (int i = 0; i < std::max(this->leng, b.leng) ; i++){

        std::bitset<8> thatAccessorByte;
        std::bitset<8> thisAccessorByte;

        if ( i >= b.getLength() )
            thatAccessorByte = std::bitset<8>(0b00000000);

        else
            thatAccessorByte = std::bitset<8> ( other[i] );

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
    return f;
}

BigInteger BigInteger::operator+(const BigInteger &a) const{
    return this->add(a);
}

BigInteger BigInteger::operator+() const{
    return *this;
}

BigInteger BigInteger::operator++() {
    BigInteger k = *this + BigInteger("1");
    k.shrink();

    this->sign = k.getSign();
    free(this->memPos);
    this->memPos = k.memPos;
    this->leng = k.leng;

    k.memPos = nullptr;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger k = *this;
    ++(*this);
    return k;
}
