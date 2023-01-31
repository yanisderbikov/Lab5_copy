
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
int s;
int code1;
char out;
char out_pr;
int outt;
char flag;
int count1;
int count2;
int lengthOfPassword;
int x;
int s=1;
char pad[4][4] = {{'1', '2', '3', 'A'},
                  {'4', '5', '6', 'B'},
                  {'7', '8', '9', 'C'},
                  {'*', '0', '#', 'D'}};
char input[6];
char outputPassword[17];
char WRONG        = 0xAA;


//------------------------------------------------ // Function PROTOTYPES //------------------------------------------//
void Init_Device(void);
void write_command (int write_data);
void write_code (int write_data);
void display_init (void);
void write_sentence (unsigned char *STR);
void delete_sentence (void);
void write_sentence_2 (unsigned char *STR);
void first_str (void);
void second_str (void);
void read_code (void);
void UART_transfer (int data_uart);
unsigned int UART_recieve (void);
void wait(long count);
void test(void );
void getPasswordFromController(void );
void read_console (void );
int compartion(void );
void Interrupts_Init();
int check(void);
void exe(void);
//-----------------------------------------------------------------------------

// MAIN Routine //-----------------------------------------------------------------------------

// TODO нужно выбрать C51 COMPILER V8.01
//  не работают массивы. Как строить массивы. Как вообще запустить эту залупену

void main (void){
    PCA0MD &= ~0x40;
    Init_Device();
    E = 0;
    display_init();
    exe();
}
void exe(){
    while (1) {
        getPasswordFromController();
        if (check() == 1){
            delete_sentence();
            write_sentence("CORRECT");
            break;
        } else{
            write_sentence("WRONG");
            wait(10000);
            delete_sentence();
        }
    }
}
int check(){
//    проверка пароля. Нужно отправлять на компуктер побитово
//  сначала отправляется в бите длина
    for(i = 0; i < lengthOfPassword; i++){
        UART_transfer(outputPassword[i]);
        if (UART_recieve()){
            return 0;
        }
    }
    return 1;
}
void getPasswordFromController(){
    write_sentence("write a password:");
    second_str();
    lengthOfPassword = 0;

    while (1){
        read_console();
        if (out != '-'){
            if (out == '#'){
                delete_sentence();
                break;
            }
            write_code(out);
            outputPassword[1 + lengthOfPassword++] = out;
        }
    }
    outputPassword[0] = lengthOfPassword; // закидывается первый бит длины, который будет проверяться в программе
}

void test(){
    while (1) {
        UART_transfer('A');
    }
}

void read_console (){
// в столбце должен быть ноль
    out = '-';
    COL1 = 1;   COL2 = 1;   COL3 = 1;   COL4 = 1;
    for (i = 0; i < 4; i++){
        LINE1 = 1; LINE2 = 1; LINE3 = 1; LINE4 = 1;
        if(i == 0) {LINE1 = 0;}
        else if(i == 1) {LINE2 = 0;}
        else if(i == 2) {LINE3 = 0;}
        else if(i == 3) {LINE4 = 0;}

        if (COL1 == 0){
            out = pad[i][0];
            return;
        } else if (COL2 == 0){
            out = pad[i][1];
            return;
        } else if (COL3 == 0){
            out = pad[i][2];
            return;
        } else if (COL4 == 0){
            out = pad[i][3];
            return;
        }
    }
}

void Timer_Init(){ // there some shit
    TCON  = 0x40;
    TMOD = 0x20;
    CKCON = 0x08;
    TH1 = 0x60;
}
void UART_Init(){
    SCON0     = 0x10;
}
void Port_IO_Init(){
// there was some stranges
    SFRPAGE   = CONFIG_PAGE;
    XBR0      = 0x01;
    XBR1      = 0x40;

}
void Interrupts_Init(){

    SFRPAGE   = CONFIG_PAGE;
    IE        = 0x82;
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

void Init_Device(void){
    Timer_Init();
    UART_Init();
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
void wait(long count){
     for(ii = 0; ii < count; ii++){}
}
//---------------------------------------------- // End Of File
//-------
