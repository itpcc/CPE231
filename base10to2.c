#include <stdio.h>
#include <string.h>

#define MAX_BUFFER 24

int isRealNumber(char *str, int len){
    int i;
    for(i = 0; i < len; i++){
        //printf("%d => %d %c\n", i, str[i], str[i]);
        if(i == 0 && (str[i] == '+' || str[i] == '-'))
            continue;
        else if(str[i] >= '0' && str[i] <= '9')
            continue;
        else
            return (i+1)*(-1);
    }
    return 1;
}

char intToCharSymbol(int n){
    if(n < 10) return n + '0';
    else return n - 10 + 'A';
}

int charSymbolToInt(char n){
    if(n >= '0' && n <= '9') return n - '0';
    else return n + 10 - 'A';
}

void binaryComplement(char *n, size_t len){
    int i;
    char isPlusOne = 0;
    for(i = len - 1; i >= 0; i--){
        //printf("%d -> %c\n", i, n[i]);
        n[i] = ((n[i] == '0')?'1':'0')+(i == (len - 1))+isPlusOne;
        isPlusOne = (n[i] == '2');
        if(isPlusOne)
            n[i] = '0';
    }
}

void bigPlusInt(char *n, int nBase, int pluser){
    int i, tmp;

    tmp = pluser;
    for(i = strlen(n)-1; i >= 0; i--){
        tmp += charSymbolToInt(n[i]);
        if(tmp >= nBase){
            n[i] = intToCharSymbol(tmp%nBase);
            if(i-1 != 0)
                n[i-1] += (int)(tmp/nBase);
        }else{
            n[i] = intToCharSymbol(tmp);
            tmp = 0;
        }
    }
}



void bigMultiplyInt(char *n, int nBase, int multiplier){
    size_t nLen, tmp = 0;
    int startPos, i;
    nLen = strlen(n);
    for(startPos = 0; n[startPos] == '0' && startPos < nLen; startPos++)    continue;
    //printf("startPos = %d\n", startPos);
    for(i = nLen - 1; i >= startPos; i--){
        tmp += charSymbolToInt(n[i]);
        tmp *= multiplier;
        //printf("tmp -> %d\t", tmp);
        if(tmp >= nBase){
            n[i] = intToCharSymbol(tmp%nBase);
            if(i-1 != 0)
                n[i-1] += (int)(tmp/nBase);
        }else{
            n[i] = intToCharSymbol(tmp);
            tmp = 0;
        }
    }
}

char* base(int from, int to, char *baseFrom, char *baseTo){
    size_t baseFromLen, baseToLen, i, fromPos = 0, toPos;
    int tempVal = 0;
    char isNegative = 0, isStillZero = 0, *tempBase2;
    if(to == 2){
        baseFromLen = strlen(baseFrom);
        baseToLen   = strlen(baseTo);
        toPos = baseToLen - 1;

        if(baseFrom[0] == '-'){
            isNegative = 1;
            fromPos++;
        }else if(baseFrom[0] == '+'){
            fromPos++;
        }

        while(!(baseFrom[fromPos] == '0' && baseFrom[fromPos+1] == '\0')){
            //long division
            tempVal = 0;
            //printf("%s/2 = ", &baseFrom[fromPos]);
            for(i = fromPos, isStillZero = 0; i < baseFromLen; i++){
                //check for leading zero
                if(isStillZero == 0){
                    isStillZero += charSymbolToInt(baseFrom[i]);
                    if(isStillZero == 0){ //leading zero
                        fromPos++;
                        continue; //skip this number
                    }
                }

                //real division
                tempVal += charSymbolToInt(baseFrom[i]);
                //printf("temp = %d\n", tempVal);
                if(baseFrom[i+1] == '\0'){ //no more number to divide, we got result of modulo!
                    baseFrom[i] = intToCharSymbol(tempVal/2);
                    baseTo[toPos--] = intToCharSymbol(tempVal % 2);
                }else if(tempVal >= 2){ //now we can divide and next number is still can divide, replace with divided number and plus remain
                    baseFrom[i] = intToCharSymbol(tempVal/2);
                    tempVal = (tempVal - ((int)(tempVal/2) * 2))*from;
                }else{ //Oh, we can't divide, let number be zero and go to next number
                    baseFrom[i] = '0';
                    tempVal *= from;
                }
            }
            //printf("%s\n", &baseFrom[fromPos]);
            //getch();
        }
        //puts(baseTo);
        if(isNegative)
            binaryComplement(baseTo, baseToLen); //reverse to binary negative value
        //puts(baseTo);
        return baseTo;

    }else if(from == 2){
        baseFromLen = strlen(baseFrom);
        baseToLen   = strlen(baseTo);
        for(fromPos = 1; fromPos < baseFromLen; fromPos++){
            bigMultiplyInt(baseTo, to, 2);
            if(baseFrom[fromPos] == 1)
                bigPlusInt(baseTo, to, 2);
        }

        if(baseFrom[0]){
            baseTo[0] = '-'; //too lazy to find padding zero
        }
        return baseTo;

    }else{
        tempBase2 = (char*)malloc(sizeof(int)*65);
        memset(tempBase2, '0', 64);
        tempBase2[64] = '\0';
        base(from, 2, baseFrom, tempBase2);
        return base(2, to, tempBase2, baseTo);
    }

}

int main(){

    char buffer[MAX_BUFFER];
    int bufferLen, i;
    char baseConverted[64+1];

    do{
        bufferLen = -1; /* Set&reset normal flag */
        printf("Select Choice : ");
        if(fgets(buffer, MAX_BUFFER-1, stdin) == NULL){ /* If it can't receive input */
            printf("Fail to receive result. Please try again.\n");
        }else if((bufferLen = strlen(buffer)) && buffer[bufferLen-1] == '\n'){
        /* if user type less than MAX_BUFFER-1 character(s), fgets'll leave new line
           character in the end of string. So, I've to check if it has new line.
           Delete it by set it as "end of string" character.  */
            buffer[--bufferLen] = '\0';
        }
        rewind(stdin); /* Wipe out waste of buffer! */
        /* Just for debug. */
        //printf("Buffer: '%s' Length: %d Valid:%d\n", buffer, bufferLen, isRealNumber(buffer, bufferLen));
        if(isRealNumber(buffer, bufferLen) != 1){
            printf("Illegal character(s), Please try again.\n");
            bufferLen = -1;
        }

    }while(bufferLen == -1);

    memset(baseConverted, '0', 64); baseConverted[64] = '\0';
    printf("%s", buffer);
    printf(" -> %s\n", base(10, 2, buffer, baseConverted));
    //puts(buffer); bigMultiplyInt(buffer, 10, 2); printf("* 2 = %s\n", buffer);
    //bigPlusInt(buffer, 10, 2); printf("+ 2 = %s\n", buffer);
    return 0;
}
