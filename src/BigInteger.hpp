#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#include <string>
#include <bitset>
#include <vector>

class BigInteger {
private:
    char *memPos; // (Starting position of the number in the heap)
    long long leng;

    void toDec(std::string &inpChar);

public:
    BigInteger();

    explicit BigInteger(std::string &number);

    ~BigInteger();


    long long getLength() const;  // In Bytes
    char *getHeapTip(); // Return the pointer to the "head" of the bigInteger in the heap


    static inline bool allZeros(const std::string &str);

    static void printVec(const std::vector<int> &vec);

    void littleEndConstruct(char inputArr[], long long inputArrSize);

    void printBinary();

    char *getLittleBytes();

    void addByte(char byte);

    void printDec();

    static std::string addDecimals(std::string a, std::string b);

    void add(BigInteger &b);
};

#endif //BIGINTEGER_BIGINTEGER_H
