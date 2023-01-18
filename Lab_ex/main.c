
#include <c8051F360.h> // SFR declarations sfr16 TMR2 = 0xCC; // Timer2 counter

#define SYSCLK 3062500
// Approximate SYSCLK frequency in Hz
sbit RS = P1^5;
sbit RW = P1^6;
sbit E = P1^7;
sbit LINE1 = P2^7;
sbit LINE2 = P2^6;
sbit LINE3 = P2^5;
sbit LINE4 = P2^4;
sbit COL1 = P2^3;
sbit COL2 = P2^2;
sbit COL3 = P2^1;
sbit COL4 = P2^0;

// sbit UART_GET = TI0;                 // UART0 TX interrupt flag
// sbit UART_RESIVE = RI0;              // UART0 RX interrupt flag

unsigned long ii;
int i;
int j;
int n;
int k;
int s;
int code1;
unsigned char STR1[]="Hello";
unsigned char STR2[]="world";
unsigned char STR_err[]=" ERROR";
unsigned char STR_cor[]=" CORRECT";
unsigned char STR_COMP[];
char out;
char out_pr;
int outt;
char flag;
int count1;
int count;
int count2;
int x;
int s=1;

char pad[4][4] = {{'1', '2', '3', 'A'},
                  {'4', '5', '6', 'B'},
                  {'7', '8', '9', 'C'},
                  {'*', '0', '#', 'D'}};


//------------------------------------------------ // Function PROTOTYPES //------------------------------------------//
void Init_Device(void);
void PORT_Init (void);
void Timer2_Init (int counts);
void Timer2_ISR (void);
void write_command (int write_data);
void write_code (int write_data);
void display_init (void);
void display_init2(void);
void write_sentence (unsigned char *STR);
void delete_sentence (void);
void write_sentence_2 (unsigned char *STR);
void first_str (void);
void second_str (void);
void read_code (void);
void UART_transfer (int data_uart);
unsigned int UART_recieve (void);
void code_comparison (void);
void wait(long count);
void test(void );
void runCode(void );
void read_console (void );
//-----------------------------------------------------------------------------

// MAIN Routine //-----------------------------------------------------------------------------

// TODO нужно выбрать C51 COMPILER V8.01
//  не работают массивы. Как строить массивы. Как вообще запустить эту залупену

void main (void){
    PCA0MD &= ~0x40;
    Init_Device();
    E = 0;
    display_init();
    flag = '0';
    runCode();
}
void runCode(){
    write_sentence("this is a code");
    second_str();

    while (1) {
        read_console();
        if (count == 6) {
            delete_sentence();
            count = 0;
        }
        if (out != '-'){
            write_code(out);
            count++;
        }
    }

}
void read_console (){
// в столбце должен быть ноль
    out = '-';
    COL1 = 1;   COL2 = 1;   COL3 = 1;   COL4 = 1;
    for (i = 0; i < 4; i++){
        LINE1 = 1; LINE2 = 1; LINE3 = 1; LINE4 = 1;
        if(i == 0) {LINE1 = 0;}
        if(i == 1) {LINE2 = 0;}
        if(i == 2) {LINE3 = 0;}
        if(i == 3) {LINE4 = 0;}

        if (COL1 == 0){
            out = pad[i][0];
            return;
        } if (COL2 == 0){
            out = pad[i][1];
            return;
        } if (COL3 == 0){
            out = pad[i][2];
            return;
        } if (COL4 == 0){
            out = pad[i][3];
            return;
        }
    }
}

void test(){
    while (1) {
        write_sentence("this is the first str");
        write_sentence_2("this is the sec");
        first_str(); // перевод на первую строку курсора
        wait(5000);
        delete_sentence();
        second_str();
        write_sentence_2("ond str");
        first_str();
        write_sentence("end");
        wait(10000);
        wait(10000);
        first_str();
        delete_sentence();
        second_str();
        delete_sentence();
        wait(10000);
        wait(10000);
    }
}

void Timer_Init(){ // there some shit
    TCON  = 0x40;
    TMOD = 0x20;
    CKCON = 0x08;
    TH1 = 0x60;
}
void UART_Init(){
    SCON0 = 0x10;
}
void Port_IO_Init(){
    SFRPAGE   = CONFIG_PAGE;
//    P1MDOUT   = 0xE0;
//    P2MDOUT   = 0xFF;
    XBR1      = 0x40;
}
void Init_Device(void){
//    Timer_Init();
//    UART_Init();
    Port_IO_Init();
}
// вывод команды
void write_command (int write_data){
    P2 = write_data;
    RS = 0;
    RW = 0;
    wait(5);
    E = 1;
    wait(5000);
    E = 0;
}
// вывод символа
void write_code (int write_data){
    P2 = write_data;
    RS = 1;
    RW = 0;

    wait(5);
    E = 1;
    wait(5000);
    E = 0;
}
// инициализация дисплея
void display_init (void){
    wait(5000); //15ms
    write_command (0x30);
    wait(5000); //15ms
    write_command (0x30);
    wait(5000); //15ms
    write_command (0x30);
    wait(5000); //15ms
    write_command (0x38);
    wait(5000); //15ms
    write_command (0x8);
    wait(5000); //15ms
    write_command (0x1);
    wait(5000); //15ms
    write_command (0x6);
    wait(5000); //15ms
    write_command (0xE);
}
// вывод предложения
void write_sentence (unsigned char *STR) {
    for(n=0; STR[n]!=0; n++){
        write_code(STR[n]);
    }
}
//вывод предложения на 2 строку
void write_sentence_2 (unsigned char *STR) {
    write_command(0xC0);
    for(n=0; STR[n]!=0;n++){
        write_code(STR[n]);
    }
}
// очистка экрана
void delete_sentence (void) {
    write_command(0x1);
}
//перевод курсора на 1 строку
void first_str (void){
    write_command(0x80);
}
// перевод курсора на 2 строку
void second_str (void){
    write_command(0xC0);
}
// чтение символа с клавиатуры
void read_code (void){
    char tmp;
    out = 'Z';
    P2 = 0xff;
    LINE1 = 0;
    LINE2 = 0;
    tmp = P2;

    if (!(tmp & 0x1)){
        flag = 'A';
    }
    LINE1 = 1;
    LINE2 = 0;
    tmp = P2;
    if (!(tmp & 0x1)){
        flag = 'B';
    }
    LINE2 = 1;

    LINE3 = 0;
    tmp = P2;
    if (!(tmp & 0x1)){
        flag = 'C';
    }
    LINE3 = 1;
    LINE4 = 0;
    tmp = P2;
    if (!(tmp & 0x1)){
        flag = 'D';
    }

    wait(10000);
    // нажата А (выводятся первые буквы)
    if (flag == 'A'){
         LINE1 = 0;
         tmp = P2;
        // первая строка
        wait(1000);
        if (!(tmp & 0x8)){
            out = '1';
        }
        if (!(tmp & 0x4)){
            out = 'A';
        }
        if (!(tmp & 0x2)){
            out = 'D';
        }
        // 2 строка
        wait(5000);
        LINE1 = 1;
        LINE2 = 0;
        tmp = P2;
        if (!(tmp & 0x8)){
            out = 'G';
        }
        if (!(tmp & 0x4)){
            out = 'J';
        }
        if (!(tmp & 0x2)){
            out = 'M';
        }
        wait(5000);
        // 3 строка
        LINE2 = 1;
        LINE3 = 0;
        tmp = P2;
        if (!(tmp & 0x8)){
            out = 'P';
        }
        if (!(tmp & 0x4)){
            out = 'T';
        }
        if (!(tmp & 0x2)){
            out = 'W';
        }
    }

    wait(10000);
    // Нажата В (выводятся 2е буквы)
        if (flag == 'B'){
        // первая строка
        for (i = 0; i <= 10000; i++) {}
        LINE1 = 0; LINE2 = 0; LINE3 = 0; LINE4 = 0; tmp = P2;
        if (!(tmp & 0x8)){
            out = '1';
        }
        if (!(tmp & 0x4)){
            out = 'B';
        }
        if (!(tmp & 0x2)){
            out = 'E';
        }
        // 2 строка
        LINE1 = 1;
        LINE2 = 0; LINE3 = 0; tmp = P2;
        if (!(tmp & 0x8)){ out = 'H';
        }
        if (!(tmp & 0x4)){ out = 'K';
        }
        if (!(tmp & 0x2)){ out = 'N';
        }
        // 3 строка
        LINE2 = 1; LINE3 = 0; tmp = P2;
        if (!(tmp & 0x8)){ out = 'R';
        }
        if (!(tmp & 0x4)){

        out = 'U';
        }
        if (!(tmp & 0x2)){ out = 'X';
        }
    }
    // Нажата С (выводятся 3и буквы)
    if (flag == 'C'){
        // 1 строка
        LINE1 = 0;
        tmp = P2;
        if (!(tmp & 0x8)){
            out = '1';
        }
        if (!(tmp & 0x4)){
            out = 'C';
        }
        if (!(tmp & 0x2)){
            out = 'F';
        }
        // 2 строка
        LINE1 = 1;
        LINE2 = 0;
        tmp = P2;

        if (!(tmp & 0x8)){
            out = 'I';
        }
        if (!(tmp & 0x4)){
            out = 'L';
        }
        if (!(tmp & 0x2)){
            out = 'O';
        }
        // 3 строка
        LINE2 = 1;
        LINE3 = 0;
        tmp = P2;
        if (!(tmp & 0x8)){
            out = 'S';
        }
        if (!(tmp & 0x4)){
            out = 'V';
        }
        if (!(tmp & 0x2)){
            out = 'Y';
        }
    }
    // Нажата D (выводятся цифры)

    if (flag == 'D'){
        for (i = 0; i <= 10000; i++) {} // 1 строка
        LINE1 = 0; LINE2 = 0; LINE3 = 0; tmp = P2;
        if (!(tmp & 0x8)){ out = '1';
        }
        if (!(tmp & 0x4)){ out = '2';
        }
        if (!(tmp & 0x2)){ out = '3';
        }
        // 2 строка
        LINE1 = 1; LINE2 = 0; tmp = P2;
        if (!(tmp & 0x8)){ out = '4';
        }

        if (!(tmp & 0x4)){ out = '5';
        }
        if (!(tmp & 0x2)){ out = '6';
        }
        // 3 строка
        LINE2 = 1; LINE3 = 0; tmp = P2;;
        if (!(tmp & 0x8)){ out = '7';
        }
        if (!(tmp & 0x4)){ out = '8';
        }
        if (!(tmp & 0x2)){ out = '9';
        }
    } // 4 строка
    LINE3 = 1; LINE4 = 0; tmp = P2;

    if (!(tmp & 0x8)){
        out = '*';
    }
    if (!(tmp & 0x4)){
        out = '0';
    }
    if (!(tmp & 0x2)){
        out = '#';
    }
    LINE4 = 1;
}

//получение данных с UART
 unsigned int UART_recieve (void){
    while (!RI0){}
    RI0 = 0;
    return SBUF0;
}
// отправка данных на UART
void UART_transfer (int data_uart){
    SBUF0 = data_uart;
    while (!TI0) {}
    TI0 = 0;
}
// сравнение паролей
void code_comparison (void) {
    count1 = 1; x=0;
    while ((count1 <= 16) & (out != '0')){
        read_code();
        if ((out != '0')&(out != 'Z')){
            STR_COMP[count1] = out;
            write_code(out);
            count1++;
        }
    }
    UART_transfer(count1-1);
    x = UART_recieve();
    if (x == 0xaa){
        write_sentence (STR_err);
        wait(30000);
        return;
    }
    count2 = 1;
    while (count2 <= (count1-1)){
        UART_transfer(STR_COMP[count2]);
        count2++;
        x = UART_recieve();

        if (x == 0xaa){
            write_sentence (STR_err);
            wait(30000);
            return;
        }
    }
    write_sentence (STR_cor);
    wait(300000000);
}
void wait(long count){
     for(ii = 0; ii < count; ii++){}
}
//---------------------------------------------- // End Of File
//-------
