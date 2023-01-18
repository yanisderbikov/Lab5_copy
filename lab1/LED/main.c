//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <c8051F360.h>                 // SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK 3062500                 // Approximate SYSCLK frequency in Hz


sbit CLR = P1^7;
sbit CLK = P2^7;


int i;
long li;
int position;
int* ar;
int x = 'B';

char buff[20];
int CTRL_Q=0x11;
int CTRL_W=0x17;

char code Font[96][6] = {
{0x0,0x0,0x0,0x0,0x0,0x0},
{0x0,0x0,0x5F,0x0,0x0,0x0},
{0x4,0x3,0x0,0x4,0x3,0x0},
{0x14,0x7F,0x14,0x7F,0x14,0x0},
{0x24,0x2A,0x7F,0x2A,0x12,0x0},
{0x63,0x13,0x8,0x64,0x63,0x0},
{0x30,0x4E,0x51,0x2E,0x50,0x0},
{0x0,0x4,0x3,0x0,0x0,0x0},
{0x0,0x1C,0x22,0x41,0x0,0x0},
{0x0,0x41,0x22,0x1C,0x0,0x0},
{0x2A,0x1C,0x7F,0x1C,0x2A,0x0},
{0x8,0x8,0x3E,0x8,0x8,0x0},
{0x0,0x40,0x30,0x0,0x0,0x0},
{0x8,0x8,0x8,0x8,0x8,0x0},
{0x0,0x60,0x60,0x0,0x0,0x0},
{0x60,0x10,0x8,0x4,0x3,0x0},
{0x3E,0x51,0x49,0x45,0x3E,0x0},
{0x0,0x42,0x7F,0x40,0x0,0x0},
{0x42,0x61,0x51,0x49,0x46,0x0},
{0x22,0x41,0x49,0x49,0x36,0x0},
{0x18,0x14,0x12,0x7F,0x10,0x0},
{0x27,0x45,0x45,0x45,0x39,0x0},
{0x3C,0x4A,0x49,0x49,0x30,0x0},
{0x1,0x71,0x9,0x5,0x3,0x0},
{0x36,0x49,0x49,0x49,0x36,0x0},
{0x6,0x49,0x49,0x29,0x1E,0x0},
{0x0,0x36,0x36,0x0,0x0,0x0},
{0x0,0x56,0x36,0x0,0x0,0x0},
{0x8,0x14,0x22,0x41,0x0,0x0},
{0x14,0x14,0x14,0x14,0x14,0x0},
{0x0,0x41,0x22,0x14,0x8,0x0},
{0x2,0x1,0x51,0x9,0x6,0x0},
{0x3E,0x41,0x5D,0x55,0x5E,0x0},
{0x7C,0x12,0x11,0x12,0x7C,0x0},
{0x7F,0x49,0x49,0x49,0x36,0x0},
{0x3E,0x41,0x41,0x41,0x22,0x0},
{0x41,0x7F,0x41,0x41,0x3E,0x0},
{0x7F,0x49,0x49,0x41,0x41,0x0},
{0x7F,0x9,0x9,0x9,0x9,0x0},
{0x3E,0x41,0x41,0x51,0x72,0x0},
{0x7F,0x8,0x8,0x8,0x7F,0x0},
{0x0,0x41,0x7F,0x41,0x0,0x0},
{0x21,0x41,0x41,0x3F,0x1,0x0},
{0x7F,0x8,0x14,0x22,0x41,0x0},
{0x7F,0x40,0x40,0x40,0x40,0x0},
{0x7F,0x2,0x4,0x2,0x7F,0x0},
{0x7F,0x4,0x8,0x10,0x7F,0x0},
{0x3E,0x41,0x41,0x41,0x3E,0x0},
{0x7F,0x9,0x9,0x9,0x6,0x0},
{0x3E,0x41,0x51,0x21,0x5E,0x0},
{0x7F,0x9,0x19,0x29,0x46,0x0},
{0x26,0x49,0x49,0x49,0x32,0x0},
{0x1,0x1,0x7F,0x1,0x1,0x0},
{0x3F,0x40,0x40,0x40,0x3F,0x0},
{0x1F,0x20,0x40,0x20,0x1F,0x0},
{0x7F,0x20,0x18,0x20,0x7F,0x0},
{0x63,0x14,0x8,0x14,0x63,0x0},
{0x3,0x4,0x78,0x4,0x3,0x0},
{0x61,0x51,0x49,0x45,0x43,0x0},
{0x0,0x7F,0x41,0x41,0x0,0x0},
{0x3,0x4,0x8,0x10,0x60,0x0},
{0x0,0x41,0x41,0x7F,0x0,0x0},
{0x4,0x2,0x1,0x2,0x4,0x0},
{0x40,0x40,0x40,0x40,0x40,0x0},
{0x0,0x0,0x3,0x4,0x0,0x0},
{0x30,0x4A,0x4A,0x32,0x7C,0x0},
{0x7F,0x48,0x48,0x30,0x0,0x0},
{0x30,0x48,0x48,0x48,0x0,0x0},
{0x0,0x30,0x48,0x48,0x7F,0x0},
{0x38,0x54,0x54,0x54,0x58,0x0},
{0x8,0x7E,0x9,0x1,0x2,0x0},
{0x6,0x49,0x49,0x45,0x3E,0x0},
{0x7F,0x10,0x8,0x8,0x70,0x0},
{0x0,0x48,0x7A,0x40,0x0,0x0},
{0x20,0x40,0x44,0x3D,0x0,0x0},
{0x7F,0x10,0x28,0x44,0x0,0x0},
{0x0,0x1,0x7F,0x0,0x0,0x0},
{0x78,0x4,0x18,0x4,0x78,0x0},
{0x7C,0x8,0x4,0x4,0x78,0x0},
{0x38,0x44,0x44,0x44,0x38,0x0},
{0x7E,0x12,0x12,0xC,0x0,0x0},
{0x0,0xC,0x12,0x12,0x7E,0x0},
{0x7C,0x8,0x4,0x4,0x8,0x0},
{0x48,0x54,0x54,0x54,0x24,0x0},
{0x4,0x3F,0x44,0x40,0x20,0x0},
{0x3C,0x40,0x40,0x20,0x7C,0x0},
{0x1C,0x20,0x40,0x20,0x1C,0x0},
{0x3C,0x40,0x30,0x40,0x3C,0x0},
{0x44,0x28,0x10,0x28,0x44,0x0},
{0x26,0x48,0x48,0x48,0x3E,0x0},
{0x44,0x64,0x54,0x4C,0x44,0x0},
{0x8,0x36,0x41,0x41,0x0,0x0},
{0x0,0x0,0x77,0x0,0x0,0x0},
{0x0,0x41,0x41,0x36,0x8,0x0},
{0x2,0x1,0x2,0x4,0x2,0x0},
{0x7F,0x7F,0x7F,0x7F,0x7F,0x0}};

int s;
char xdata buf[149];
void setBu();


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Port_IO_Init();
void Timer_Init();
void Interrupts_Init();
void Print(int pos);
void PrintRun(int pos);
int getSymbol(char a);
void symbolLoad(int pos, int symb);
void UART_Init(void);
unsigned int UART_transfer (int data_uart);
unsigned int UART_recive (void);


int k_shift;
int run_count = 10;

int command;
int kk;
int cSymb = -1;
int flag = 0;

//del af
int iii;
int iiii;

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{


    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
    // enable)
    Port_IO_Init();

    //symbol = getSymbol(35);
    CLK = 0;
    CLR = 1;
    position = 0;
    for(li = 0; li <= 1000; li++){}
    CLR = 0;
    P2 = 0x7f^0xff;



    for(li = 0; li<149; li++){
        buf[li] = 0x7f^0xff;
    }




    symbolLoad(0, ' ');
    symbolLoad(1, ' ');
    symbolLoad(2, ' ');
    symbolLoad(3, ' ');
    symbolLoad(4, ' ');
    symbolLoad(5, ' ');
    symbolLoad(6, ' ');
    symbolLoad(7, ' ');
    symbolLoad(8, ' ');
    symbolLoad(9, ' ');
    symbolLoad(10, ' ');
    symbolLoad(11, ' ');
    symbolLoad(12, ' ');
    symbolLoad(13, ' ');
    symbolLoad(14, ' ');

    for(iii = 0; iii < 20; iii++){
        buff[iii] = ' ';
        symbolLoad(iii,' ');
    }

    //   buf[47] = 0x7f^0xff;

    Timer_Init();
    UART_Init();
    Interrupts_Init();

    command = 0x17;
    while (1)
    {

        switch (command)
        {
            //?a?ei aaaa?uae no?iee
            case 0x11:
                flag = 0;
                while (flag == 0){
                    kk = UART_recive();
                    cSymb++;
                    switch (kk)

                        //?a?ei oa?ieiaeuiiai auaiaa
                    {case 0x17:
                            command = 0x17;
                            flag = 1;
                            cSymb = -1;
                            symbolLoad(0, ' ');
                            symbolLoad(1, ' ');
                            symbolLoad(2, ' ');
                            symbolLoad(3, ' ');
                            symbolLoad(4, ' ');
                            symbolLoad(5, ' ');
                            symbolLoad(6, ' ');
                            symbolLoad(7, ' ');
                            symbolLoad(8, ' ');
                            symbolLoad(9, ' ');
                            symbolLoad(10, ' ');
                            symbolLoad(11, ' ');
                            symbolLoad(12, ' ');
                            symbolLoad(13, ' ');
                            symbolLoad(14, ' ');
                            break;

                            //Caaa?oaao aaia neiaieia
                        case 0x0d:
                            cSymb = -1;
                            command = 0x11;
                            break;

                            //Na?in ?a?eia a ia?aeuiia ninoiyiea
                        case 0x03:
                            symbolLoad(0, ' ');
                            symbolLoad(1, ' ');
                            symbolLoad(2, ' ');
                            symbolLoad(3, ' ');
                            symbolLoad(4, ' ');
                            symbolLoad(5, ' ');
                            symbolLoad(6, ' ');
                            symbolLoad(7, ' ');
                            symbolLoad(8, ' ');
                            symbolLoad(9, ' ');
                            symbolLoad(10, ' ');
                            symbolLoad(11, ' ');
                            symbolLoad(12, ' ');
                            symbolLoad(13, ' ');
                            symbolLoad(14, ' ');
                            cSymb = -1;
                            break;


                        default:
                            symbolLoad(cSymb, kk);
                            break;
                    }

                }

                break;

                //?a?ei oa?ieiaeuiiai auaiaa
            case 0x17:
                flag = 0;
                while (flag ==0){
                    kk = UART_recive();
                    cSymb++;
                    switch (kk){
                        //?a?ei aaaa?uae no?iee
                        case 0x11:
                            command = 0x11;
                            flag = 1;
                            cSymb = -1;
                            symbolLoad(0, ' ');
                            symbolLoad(1, ' ');
                            symbolLoad(2, ' ');
                            symbolLoad(3, ' ');
                            symbolLoad(4, ' ');
                            symbolLoad(5, ' ');
                            symbolLoad(6, ' ');
                            symbolLoad(7, ' ');
                            symbolLoad(8, ' ');
                            symbolLoad(9, ' ');
                            symbolLoad(10, ' ');
                            symbolLoad(11, ' ');
                            symbolLoad(12, ' ');
                            symbolLoad(13, ' ');
                            symbolLoad(14, ' ');
                            break;

                            //Oaaeaiea iineaaiaai neiaiea e naaea aeaai
                        case 0x0008:
                            cSymb--;
                            if(cSymb<0){
                                cSymb =0;
                            }

                            buff[cSymb] = ' ';
                            iiii = cSymb-7;
                            if(cSymb >= 8){
                                for(iii = 0; iii <= 7; iii++){
                                    symbolLoad(iii, buff[iiii-1]);
                                    iiii++;
                                }
                            }
                            else{
                                for(iii = 0; iii <= 7; iii++){
                                    symbolLoad(iii, buff[iii]);
                                }
                            }
                            cSymb--;
                            break;

                            //Na?in ?a?eia a ia?aeuiia ninoiyiea
                        case 0x03:
                            for(iii = 0; iii < 20; iii++){
                                buff[iii] = ' ';
                                symbolLoad(iii,' ');
                            }
                            cSymb = -1;
                            break;
                            //IE i?eieiaao io PC aaeou n eiaaie neiaieia
                        default:

                            buff[cSymb] = kk;
                            iiii = cSymb-7;
                            if(cSymb >= 8){
                                for(iii = 0; iii <= 7; iii++){
                                    symbolLoad(iii, buff[iiii]);
                                    iiii++;
                                }
                            }
                            else{
                                for(iii = 0; iii <= 7; iii++){
                                    symbolLoad(iii, buff[iii]);
                                }
                            }
                            break;
                    }
                }
                break;
        }
    }
}

void Port_IO_Init()
{

    SFRPAGE   = CONFIG_PAGE;
    P1MDOUT   = 0x80;
    P2MDOUT   = 0xFF;
    XBR0      = 0x01;
    XBR1      = 0x40;

}

void Timer_Init()
{
    TCON      = 0x50;
    TMOD      = 0x22;
    TL0       = 0xCD;
    TH0       = 0xCD;
    CKCON     = 0x08;
    TH1       = 0x60;
}

void UART_Init()
{
    SCON0     = 0x10;
}


void Interrupts_Init()
{
    SFRPAGE   = CONFIG_PAGE;
    IE        = 0x82;
}

void Timer0_ISR (void) interrupt 1
{
CLK = 1;
CLK = 1;
CLK = 1;
CLK = 1;
CLK = 0;
position++;

//   UART_transfer (position);
//   k = UART_recive();
//   Print(position);
//   PrintRun(position);

switch (command)
{
case 0x11:
PrintRun(position);
break;
case 0x17:
Print(position);
break;
}


}



void Print(int pos){

    if (position >= 48){
        position = -1;
        P2 = buf[position]^0xff;
    }
    else{
        P2 = buf[position]^0xff;
    }
}

void PrintRun(int pos){
    int k;

    if (position >= 48){
        position = -1;
        run_count--;
        if(run_count == 0){
            run_count = 10;
            k_shift++;
            if(k_shift==149){k_shift=0;}
        }

    }
    k = position+k_shift;
    if(k>148){k=k-149;}
    P2 = buf[k]^0xff;

}







int getSymbol(char a){
    int k ;
    k = ((int)(a))-32;
    return k;
}


void setBu(){
    char xdata t[149];
    for(li = 0; li < 149; li++){
        t[li]= li/6+1;
        buf[li] = Font[buff[li/6+1]][li- 6*(li/6)];
    }
}

void symbolLoad(int pos, int symb){
    int l;
    int f;
    l = pos*6;
    f = symb-32;

    for (li = 0; li<6;l++,li++){
        buf[l] = Font[f][li];
    }
}



unsigned int UART_transfer (int data_uart){
    SBUF0 = data_uart;
    while(!TI0){}
    TI0 = 0;

}


unsigned int UART_recive (void){
    while(!RI0){}
    RI0 = 0;
    return SBUF0;
}


//
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------