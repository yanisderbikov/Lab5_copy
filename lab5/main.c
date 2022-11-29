#include <stdio.h>

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "C8051F360.h"                 // SFR declarations
#include <string.h>
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK 3062500                 // Approximate SYSCLK frequency in Hz


sbit RS = P1^5;
sbit RW = P1^6;
sbit SE = P1^7; // E


int i;
long li;


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Port_IO_Init();
void writeCommand(char com);
void writeChar(char sym);
void displayInit(void);
void wait(long count);
char getNum();
char convert(int n);
int pow2(int to);
char AF(int rest);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
int main (){
    int i;
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
    // enable)
    Port_IO_Init();
    displayInit();

    while(i < 3){                       // possible to
        char currentNum = getNum();
        writeChar(currentNum);
        i++;
    }
    return 0;
}


// written commands
void writeCommand(char com){
    int i;

    P2 = com;
    RS = 0;
    RW = 0;
    SE = 1;

    wait(10);
    SE = 0;
    wait(10);
}
void writeChar(char sym){
    int i;

    P2 = sym;
    RS = 1;
    RW = 0;
    SE = 1;

    wait(10);
    SE = 0;
    wait(10);
}
char getNum(){
//    здесь должна быть реализация кода, где берется и возвращяется значение с
//    тыкнутой кнопки

    return 0;
}

// commands needs to
void wait(long count){
    long i;
    for(i = 0; i < count; i++){}
}
void displayInit(void){ 			    // initialization screen

    wait(10000); //1000 : 10��	�������� ��������  ��������

    writeCommand(0x3c);				// ������� ���� ������ 8 ��� 2 ������ ������� 5�10
    wait(10000); //1000 : 10��

    writeCommand(0x3c);
    wait(10000); //1000 : 10��

    writeCommand(0x3c);

    wait(10000);

    writeCommand(0x3c);
    wait(10000);
    writeCommand(0x08);			// ����� �����������: ����������� - ���������, ������ - ��������,
    // ������ � ���� ���������� ����� - ��������
    wait(10000);
    writeCommand(0x01);			// ������� ������ ������
    wait(10000);
    writeCommand(0x06);			// ����� �������� ���������� - �� ����������, ��� ������� ������
    wait(10000);
    writeCommand(0xC);
    wait(10000);
}
void Port_IO_Init(){
    SFRPAGE   = CONFIG_PAGE;
    P1MDOUT   = 0xE0;
    P2MDOUT   = 0xFF;
    XBR1      = 0x40;
}
char convert(int binary){
//    covert to ten
    int j = 0;
    int sumTen = 0;
    int remains = 0;
    while (binary != 0){
        remains = binary % 10;
        sumTen = sumTen + remains * pow2(j);
        binary = binary / 10;
        j++;
    }
    if (remains == 1){
        sumTen = sumTen + 1;
    }
    // ------------------------------------
    //          convert to 16
    // ------------------------------------

    char[] result = char[]
    while (sumTen != 0){
        remains = sumTen % 16;
        sumTen = sumTen/16;
    }


    return sumTen;
}
char AF(int rest){
    switch (rest)
    {
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
    }
}
int pow2(int to){
    if (to == 0){
        return 0;
    }
    int result = 2;
    for (int i = 1; i < to; i++) {
        result = result * 2;
    }
    return result;
}
