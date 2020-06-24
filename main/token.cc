#include <ctime>
#include <string>
#include "token.h"
using namespace std;

/**
 若登录成功，返回给客户端一个token
 */
string buildToken(){
    const int LEN = 62;
    int iLen = 10;
    char g_arrCharElem[LEN] = {
        '0', '1', '2','3', '4', '5','6', '7', '8','9',
        'A', 'B', 'C','D', 'E', 'F','G', 'H', 'I','J',
        'K', 'L', 'M','N', 'O', 'P','Q', 'R', 'S','T',
        'U', 'V', 'W','X', 'Y', 'Z','a', 'b', 'c','d',
        'e', 'f', 'g','h', 'i', 'j','k', 'l', 'm','n',
        'o', 'p', 'q','r', 's', 't','u', 'v', 'w','x',
        'y', 'z'
    };
    char* token = new char[iLen + 1];
    token[iLen] = '\0';
    srand((unsigned)time(0));
    int iRand = 0;
    for (int i = 0; i < iLen; ++i) {
     iRand = rand() % LEN;   // iRand = 0 - 61
     token[i] = g_arrCharElem[iRand];
    }
    delete[] token;
    return token;
}