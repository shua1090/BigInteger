#include <iostream>
#include "BigInteger.hpp"

BigInteger BigInteger::subtract(const BigInteger &b) const {

    if (this->getSign() == 1 && b.getSign() == 1){
        BigInteger y = b;
        BigInteger x = *this;
        x.sign = false;
        y.sign = false;
        return y.subtract(x);
    } else if (this->getSign() == 1){
        BigInteger y = b;
        BigInteger x = *this;
        x.sign = false;
        y.sign = false;
        BigInteger res = y.add(x);
        res.sign = true;
        return res;
    } else if (b.getSign() == 1){
        BigInteger y = b;
        y.sign = false;
        return this->add(y);
    }

    BigInteger num = b.twosComplement();
    char * other = num.getHeapTip();

    int remainder = 0;
    BigInteger f;

    for (int i = 0; i < std::max(this->leng, num.leng) ; i++){

        std::bitset<8> thatAccessorByte;
        std::bitset<8> thisAccessorByte;

        if (i >= num.getLength() )
            thatAccessorByte = std::bitset<8>(0b11111111);

        else
            thatAccessorByte = std::bitset<8> ( other[ i] );

        if ( i >= this->getLength() )
            thisAccessorByte = std::bitset<8> (0b11111111);

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

    if (remainder != 1){
        f = f.twosComplement();
        f.sign = true;
    }

    return f;
}

BigInteger BigInteger::operator--() {
    BigInteger k = this->subtract(BigInteger("1"));

    this->sign = k.getSign();
    free(this->memPos);
    this->memPos = k.memPos;
    this->leng = k.leng;
    k.memPos = nullptr;

    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger k = *this;
    --(*this);
    return k;
}
