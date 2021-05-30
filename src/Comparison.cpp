#include <iostream>
#include "BigInteger.hpp"

int BigInteger::compareTo(BigInteger& other){

    this->shrink();
    other.shrink();

    if (other.getSign() == 1 && this->getSign() == 0){
        return 1;
    } else if (other.getSign() == 0 && this->getSign() == 1){
        return -1;
    } else {

        if (this->getLength() > other.getLength()){
            return (this->getSign() == 1) ? -1 : 1;
        } else if (other.getLength() > this->getLength()){
            return (other.getSign() == 1) ? 1 : -1;
        } else {
            int returnValue = 0;


            for (long long int i = this->getLength() - 1; i >= 0; i--){

                if (this->memPos[i] > other.getHeapTip()[i]){
                    returnValue = 1;
                    break;
                } else if (this->memPos[i] < other.getHeapTip()[i]){
                    returnValue = -1;
                    break;
                }
            }

            if (this->getSign() == 1) returnValue = -returnValue;

            return returnValue;
        }
    }
}

bool BigInteger::operator< (BigInteger b1) {
    return this->compareTo(b1) == -1;
}

bool BigInteger::operator> (BigInteger b1){
    return this->compareTo(b1) == 1;
}

bool BigInteger::operator<= (BigInteger b1){
    return this->compareTo(b1) == -1 || this->compareTo(b1) == 0;
}

bool BigInteger::operator>= (BigInteger b1){
    return this->compareTo(b1) == 1 || this->compareTo(b1) == 0;
}

bool BigInteger::operator== (BigInteger b1){
    return this->compareTo(b1) == 0;
}

bool BigInteger::operator!= (BigInteger b1){
    return this->compareTo(b1) != 0;
}
