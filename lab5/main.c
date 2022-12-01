#include <stdio.h>

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "C8051F360.h"                 // SFR declarations
#include <string.h>
//#include <iostream>
//#include <string>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK 3062500                 // Approximate SYSCLK frequency in Hz


sbit RS = P1^5;
sbit RW = P1^6;
sbit SE = P1^7; // E

// at void read_code used
char out;
int ROW0;
int ROW1;
int ROW2;
int ROW3;

char flag;

long li;


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void write_Command(char com);
void write_code(int sym);
void write_sentence(unsigned char *STR);
void displayInit(void);
void read_code(void);


void Port_IO_Init();
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
        write_code(currentNum);
        i++;
    }
    return 0;
}


// written commands
void write_Command(char com){
    int i;

    P2 = com;
    RS = 0;
    RW = 0;
    SE = 1;

    wait(10);
    SE = 0;
    wait(10);
}
void write_code(int write_data){
    P2 = write_data;
    RS = 1;
    RW = 0;

    wait(5);
    SE = 1;
    wait(5000);

    SE = 0;
} //write symbol at screen
void write_sentence(unsigned char *STR){
    for (int n = 0; STR[n] != 0; n++) {
        write_code(STR[n]);
    }
} // write a sentence
void read_code(){
    char tmp;
    out = 'Z';
    P2 = 0xff;
    ROW0 = 0;
    ROW1 = 0;
    ROW2 = 0;
    flag = 'Z';

    tmp = P2;
    wait(10000); // 15ms

    // присвоим переменной flag одно из значений
    // опрашиваем поочередно каждую строчку
//    ---------A-------

    if (!(tmp & 0x1)){
        flag = 'A';
    }

//    ---------B-------
    ROW0 = 1;
    ROW1 = 0;
    tmp = P2;

    if (!(tmp & 0x1)){
        flag = 'B';
    }

//    ---------C-------
    ROW1 = 1;
    ROW2 = 0;

    if (!(tmp & 0x1)){
        flag = 'C';
    }

//    ---------D-------
    ROW2 = 1;
    ROW3 = 0;
    if (!(tmp & 0x1)){
        flag = 'D';
    }
}

char getNum(){
//    здесь должна быть реализация кода, где берется и возвращяется значение с
//    тыкнутой кнопки

    return 0;
}// unwritten


// commands needs to
void wait(long count){
    long i;
    for(i = 0; i < count; i++){}
}
void displayInit(void){ 			    // initialization screen

    wait(10000); //1000 : 10��	�������� ��������  ��������

    write_Command(0x3c);				// ������� ���� ������ 8 ��� 2 ������ ������� 5�10
    wait(10000); //1000 : 10��

    write_Command(0x3c);
    wait(10000); //1000 : 10��

    write_Command(0x3c);

    wait(10000);

    write_Command(0x3c);
    wait(10000);
    write_Command(0x08);			// ����� �����������: ����������� - ���������, ������ - ��������,
    // ������ � ���� ���������� ����� - ��������
    wait(10000);
    write_Command(0x01);			// ������� ������ ������
    wait(10000);
    write_Command(0x06);			// ����� �������� ���������� - �� ����������, ��� ������� ������
    wait(10000);
    write_Command(0xC);
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
