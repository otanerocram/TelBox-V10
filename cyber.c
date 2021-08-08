#define emula 0
#define LCD 16
#include <16F648A.h>
// FIRMWARE PARA TARIFADOR TELBOX V10
// Desarrollado por Renato Beltran
//
#fuses NOWDT,PROTECT,NOLVP,NOMCLR,PUT,NOBROWNOUT,EC_IO
#use delay(clock=3579545, restart_wdt)
#use rs232(baud=9600, xmit=PIN_B2, rcv=PIN_B1, restart_wdt)
#byte   CMCON=0x9C
#byte   RA=0x05
#byte   TRISA=0x85
#bit    dsr_t = TRISA.3
#byte   TRISB=0x86
#byte   RB=0x06
#byte   TMR0=0x01
#byte   STATUS=0x03
#byte   RCSTA=0x18
#byte   RCREG=0x1A
#byte   PIR1=0x0C
#byte   PCLATH=0x0A
#byte   PCL=0x02
#byte   INTCON=0x0B
#byte   W_REG=0x200
#byte   EEADR=0x9B
#byte   EEDATA=0x9A
#byte   EECON2=0x9D
#byte   STATUS=0x03
#bit    EEIF=PIR1.7
#bit    WREN=CMCON.2
#bit    CARRY=STATUS.0
#bit    GIE=INTCON.7
#bit    DC=STATUS.1
#bit    PEIE=INTCON.6
#bit    rcif=PIR1.5
#bit    txif=PIR1.4
#bit    t0if=INTCON.2
#USE    FAST_IO(A)
#USE    FAST_IO(B)

//PUBLICIDAD1_LOW
#rom 0x2108={' ',' ','[',':',':','T','e','l','B','o','x',':',':',']',' ',' '}
#rom 0x2118={' ',' ',' ','L','o','c','u','t','o','r','i','o','s',' ',' ',' '}
#rom 0x2140={0x01} //Condición Inicial de Luz: "Encendido"

byte dreceiv;
byte dreceiv0;   //'P'
byte dreceiv1;   //'C'
byte dreceiv2;   //NUMCAB
byte dreceiv3;   //RFLAG2
byte dreceiv4;   //COSTO_DEC
byte dreceiv5;   //COSTO_ENT
byte CNTSEG_L;
byte CNTSEG_H;
long DELTASEG;
byte OVFLW;
long SEGUNDOS;
long DURMAX;

byte KEY0;
byte KEY1;
byte KEY2;
byte KEY3;
byte KEY8;

byte RFLAG1;
byte RFLAG2;
byte RFLAG3;
byte RFLAG4;
byte CNTRXD;
byte CONT0;
byte CONT1;
byte CONT2;
byte CONT3;
byte CUENTASEG;
byte CUENTASEG1;
byte OVF0;
byte OVF1;
byte OVF2;
byte OVF3;
byte MIN_D;
byte MIN_U;
byte SEG_D;
byte SEG_U;

byte COSTO_ENT;
byte COSTO_DEC;

byte RCOSTO_0;
byte RCOSTO_1;
byte RCOSTO_2;
byte RCOSTO_3;
byte VAR_0;
byte VAR_1;
byte VAR_2;
byte VAR_3;

byte MVAR_0;
byte MVAR_1;
byte MVAR_2;
byte MVAR_3;

byte TMIN_D;
byte TMIN_U;
byte TSEG_D;
byte TSEG_U;

byte TPARCIAL;

byte CMD_LCD;
byte CHAR_LCD;
byte KEYNUM;
byte KEYDAT;
byte KEYSTAD;
byte KEYPOS;
byte KEYPUNT;
byte NUMCAB;
byte MAINPOS;
byte RCOL;
byte CONT5;
byte CAB_DEC;
byte CAB_UNID;

byte RTXD;
byte RDISP;
byte RDELAY;

byte tris_tmp;
byte oe_tmp;

#bit fls = RFLAG1.0
#bit pol1 = RFLAG1.1
#bit pol2 = RFLAG1.2
#bit cambio = RFLAG1.3
#bit t_max = RFLAG1.4
#bit fintec = RFLAG1.5
#bit bit_prueba =RFLAG1.6

#bit fila = RFLAG2.0   //
#bit costear = RFLAG2.1   //
#bit fintemp = RFLAG2.2   //
#bit cortar = RFLAG2.3   //  1=corta la llamada
#bit bloquea = RFLAG2.4   //
#bit fin_funcion = RFLAG2.5   //
#bit fin_graba = RFLAG2.6   //
#bit clave = RFLAG2.7   //

#bit rxdat = RFLAG3.0
#bit txdat = RFLAG3.1
#bit fin_tx = RFLAG3.2
#bit tx_encab = RFLAG3.3
#bit corto = RFLAG3.4
#bit calend = RFLAG3.5
#bit mensaje1 = RFLAG3.6
#bit mensaje2 = RFLAG3.7

#bit mensaje3 = RFLAG4.0
#bit mensaje4 = RFLAG4.1

#bit en = RA.0      //#define es0   RB,0   //48         //PORTB,0
#bit r_w = RA.1
#bit rele = RA.2
#bit dsr = RA.3
#bit vl1 = RB.0
#bit vl2 = RB.7
#bit vlen = RA.4
#bit lcd_bl = RB.3

#bit dtmf_bit1 = RB.5
#bit lcd_bit2 = RB.5
#bit oe = RB.4
#bit std = RA.5
#bit dtmf_bit0 = RB.0
#bit lcd_bit3 = RB.0

#bit dtmf_bit3 = RB.7
#bit dtmf_bit2 = RB.6
#bit lcd_bit0 = RB.7
#bit lcd_bit1 = RB.6
#bit r_s = RA.6

char PUBLICIDAD1[32];
char PUBLICIDAD2[32];
char destino[16];
int i=0,j=0;
char luzstd[1];

#include <funciones.c>

#INT_RDA
void recepcion_rxd()
{
dreceiv = RCREG;
switch (CNTRXD)
   {
   case 0 :
      if (dreceiv=='P')
      {
         CNTRXD=1;
      }
      else if (dreceiv=='/')
      {
         CNTRXD=10;
      }
      else if (dreceiv=='$')
      {
         CNTRXD=11;
      }
   break;

   case 1 :
      if (dreceiv=='C')
      {
         CNTRXD=2;
      }
      else 
      {
         CNTRXD=0;
      }
   break;

   case 2 :
      if (dreceiv == NUMCAB)
      {
         CNTRXD=3;
      }
      else
      {
         CNTRXD=0;
      }
   break;

   case 3 :
      dreceiv0=dreceiv;    //recibe REG_FLAGS
      if(MAINPOS==0)
      {
         bloquea=(bit_test(dreceiv0,5));
         mensaje1=0;
         mensaje2=1;
         CUENTASEG=0;
       }
      if(MAINPOS==4)
      {
        CNTRXD=4;
      }
      else
      {
         rxdat=1;
         CNTRXD=0;
      }
   break;

   case 4 :
         dreceiv1=dreceiv;    //recibe Costo_Soles
         CNTRXD=5;
   break;

   case 5 :
         dreceiv2=dreceiv;   //recibe Costo_Centavos
         CNTRXD=6;
   break;

   case 6 :
         dreceiv3=dreceiv;   //recibe PeriodoTparcial
         CNTRXD=7;
   break;

   case 7 :
         dreceiv4=dreceiv;  //recibe DURMAX_L
         CNTRXD=8;
   break;

   case 8 :
         dreceiv5=dreceiv;  //recibe DURMAX_H
         rxdat=1;
         CNTRXD=9;
   break;
   
   case 9 :
         //Inicializando destino en blanco
         for (i=0;i<16;i++)
         {
            destino[i]=" ";
         }
         gets(destino);
         for(i=0;i<16;i++)
         {
            write_eeprom(i+48, destino[i]);
            delay_ms(20);
         }
         CNTRXD=0;
   break;

   case 10 :
         gets(luzstd);
         write_eeprom(64,luzstd[0]-48);
         delay_ms(20);
         CNTRXD=0;
   break;

   case 11:
         gets(PUBLICIDAD1);
         for(CONT0=0;CONT0<32;CONT0++)
         {
            write_eeprom(CONT0+8, PUBLICIDAD1[CONT0]);
            delay_ms(20);
         }
         CNTRXD=0;
         reset_cpu();
   break;
   }
}

#INT_TIMER1      //viene cada medio segundo
void conteo_timer1()
{
set_timer1(9606);
OVFLW++;
if(OVFLW == 2)
   {
    if(costear)
      {
      ver_costo();
      }
    OVFLW=0;
    CNTSEG_L++;
    if(CNTSEG_L == 0)
      {
      CNTSEG_H++;
      }
    calend=1;
    fls=1;
    OVF3++;
    if(OVF3==6)
      {
      OVF3=0;
      }
    SEGUNDOS++;
    if (SEGUNDOS == DELTASEG)
      {
         DELTASEG = DELTASEG + TPARCIAL;
         costear=1;
      }

    SEG_U++;
    if(SEG_U == 10)
      {
        SEG_U = 0;
        SEG_D++;
        if(SEG_D == 6)
         {
            SEG_D = 0;
            MIN_U ++;
            if (MIN_U == 10)
            {
               MIN_U = 0;
               MIN_D ++;
               }
            }
         }
      }
}

#INT_TIMER0      //viene cada 5 ms
void conteo_timer0()
{
set_timer0(116);
OVF0++;
OVF1++;
OVF2++;
if(OVF0 == 200)
   {
    OVF0 = 0;
    CUENTASEG++;
    if(CUENTASEG == 3)
      {
      mensaje2 = 1;
      }
    if(CUENTASEG == 6)
      {
      mensaje1 = 1;
      }
    if(CUENTASEG == 8)
      {
      mensaje3 = 1;
      }
    if(CUENTASEG == 10)
      {
      mensaje4 = 1;
      CUENTASEG = 0;
      }
    CUENTASEG1++;
    }
}

void main()
{
SETUP_TIMER_0(rtcc_internal|rtcc_div_32);
SETUP_TIMER_1(t1_internal|t1_div_by_8);
SET_TRIS_A(0x28);   //00101000
SET_TRIS_B(0x02);   //00000010
setup_comparator(nc_nc_nc_nc);
CMCON=7;
RA=0x10;    //00010000    en=0;r_s=0;r_w=0;rele=0
RB=0x04;    //00000100    oe=0;lcd_bl=0
OVFLW=0;
delay_ms(50);
CNTRXD=0;
rxdat=0;
costear=0;
fintemp=0;
dreceiv=0;
enable_interrupts(INT_RDA);
dreceiv=RCREG;
enable_interrupts(GLOBAL);
leerluz();
inicializa_lcd();
CMD_LCD=1;      //borra pantalla
send_cmd();
CMD_LCD=6;      //auto incrementa
send_cmd();
CMD_LCD=0x80;   //home
send_cmd();
dreceiv=0;
CMD_LCD=0x0D;   //
send_cmd();
dreceiv=0;
bit_set(dreceiv3,4);
bloquea=1;
cortar=0;
logo_inicial();
delay_ms(500);
MAINPOS=0;
CAB_DEC=read_eeprom(0);
CAB_UNID=read_eeprom(1);
if(RDELAY==0)
   {
   RDELAY=10;
   }
if(RDELAY>10)
   {
   RDELAY=2;
   }
OVF3=0;
NUMCAB=10*CAB_DEC+CAB_UNID;
if(NUMCAB>16)
   {
    NUMCAB=1;
    CAB_DEC=0;
    CAB_UNID=1;
   }
RFLAG1=6;
tau1();
aclara_costos();
aclara_costoinicial();
CONT0=0;
corto=0;
CUENTASEG=0;
for (CONT0=0;CONT0<32;CONT0++)
{
   PUBLICIDAD2[CONT0]=read_eeprom(CONT0+8);
}
do
   {
      restart_wdt();
      if (bit_test(RCSTA,1))
      {
         dreceiv=RCREG;
         bit_clear(RCSTA,4);
         bit_set(RCSTA,4);
      }
      leerluz();
      switch (MAINPOS)
      {
         case 0:      //caso telef colgado
            CMD_LCD=0x0C;   //desactiva el cursor
            send_cmd();
            enable_interrupts(INT_TIMER0);
            if(mensaje1)
            {
               fila=0;
               RCOL=0;
               lcd_xy();
               for(CONT5=0;CONT5<16;++CONT5)
               {
                  sec_zv();
                  send_char();
               }
               
               mensaje1=0;
            }
            if(mensaje2)
            {
               fila=0;
               RCOL=0;
               lcd_xy();
               for(CONT5=16;CONT5<32;++CONT5)
               {
                  sec_zv();
                  send_char();
               }
               
               mensaje2=0;
            }
            if(mensaje3 && SEG_U==0 && SEG_D==0 && MIN_U==0 && MIN_D==0)
            {
               lcd_publi1();
               mensaje3=0;
            }
            if(mensaje4 && SEG_U==0 && SEG_D==0 && MIN_U==0 && MIN_D==0)
            {
               lcd_publi2();
               mensaje4=0;
            }
            if(rxdat)
            {
               procesa();
               lcd_t_c();
            }
            change_dir1();
            if((vl1==0)|(vl2==0))
            {
               delay_ms(100);
               if((vl1==0)|(vl2==0))
               {
                  pol1=vl1;
                  pol2=vl2;
                  change_dir0();
                  aclara_fila1();
                  CMD_LCD=0x0D;   //parpadea cursor
                  send_cmd();
                  fila=0;
                  RCOL=0;
                  lcd_xy();
                  delay_ms(5);
                  transmit_encab();   //transmite descolgó
                     fin_tx=1;
                     RTXD=0xF1;
                  transmit_1d();
                     KEYPOS=0;
                     KEYSTAD=0;
                     KEYPUNT=&KEY0;
                  KEYNUM=0;
                  MAINPOS=1;
                  for(CONT1=0;CONT1<RDELAY;++CONT1)
                  {
                     delay_ms(100);
                  }
                  OVF0=0;
                  CUENTASEG1=0;
                  enable_interrupts(INT_RTCC);
                  }
               }
         change_dir0();
        break;
        
        case 1:      //caso telef descolgado
            teclado();   //primer dígito
            if(CUENTASEG1==16)
            {
               MAINPOS=8;
            }
            if (KEYSTAD==2)
            {
               telef_fin();
               break;
            }
            if(fintec)
            {
               if ((KEYSTAD==1)||(KEYSTAD==4))
               {
                  if ((KEYDAT==0x0C)&(bloquea))
                  {
                        RDISP=KEYDAT;
                        display_num();
                        grabadat();
                        MAINPOS=0;
                  }
                  else
                  {
                        marcando();
                        MAINPOS=2;
                        disable_interrupts(INT_RTCC);
                  }
                  }
               }
            if (rxdat)
            {
               procesa();
               lcd_t_c();
               fila=0;
               RCOL=KEYNUM;
               lcd_xy();
             }
         break;
         
         case 2:
            teclado();   //mas del segundo dígito
            if ((KEYSTAD==1)||(KEYSTAD==4))
            {
               marcando();
            }
            if (KEYSTAD==2)
            {
               telef_fin();
            }
            if (KEY0==1)
            {
               if(KEYNUM>2)
               {
                  verpol();
                  if (cambio==1)
                  {
                     MAINPOS=3;
                  }
                }
             }
            else
            {
               if(KEYNUM>5)
               {
                  verpol();
                  if (cambio==1)
                  {
                     MAINPOS=3;
                  }
                }
             }
            if ((KEYNUM>2)&&(bloquea))
            {
               cab_bloqueada();
               telef_fin();
            }
            if(cortar)
            {
               transmit_encab();
               RTXD=0xF0;      //colgó
               fin_tx=1;
               transmit_1d();
               rele=1;
               no_permitido();
               delay_ms(1500);
               KEYPOS=0;
               MAINPOS=0;
               cortar=0;
               rele=0;
               }
            if(rxdat)
            {
               procesa();
               lcd_t_c();
               fila=0;
               RCOL=KEYNUM;
               lcd_xy();
            }
         break;
         
         case 3:      //contestaron
            transmit_encab();
            RTXD=0xF2;
            fin_tx=1;
            transmit_1d();
            MAINPOS=4;      //debe ser MAINPOS=4
            if(cortar)
            {
               telef_fin();
               cortar=0;
            }
            enable_interrupts(INT_TIMER1);
            tau1();
            CNTSEG_L=1;
            SEGUNDOS=1;
            SEG_U=1;
         break;
         
         case 4:      //temporizador espera datos pc
            if(rxdat)
            {
               show_destino();
               delay_ms(2);
               procesa();
               if(bloquea)
               {
                  MAINPOS=6;
               }
               else
               {
                  MAINPOS=5;
                  enable_interrupts(INT_TIMER1);
               }
            }
            if(fls)
            {
               MAINPOS=6;
            }
         break;
         
         case 5:      //temporizador
            if(fls)
            {
               fls=0;
               if(!DURMAX==0)
               {
                  if(SEGUNDOS==DURMAX)
                  {
                        MAINPOS=6;
                        fintemp=1;
                        disable_interrupts(INT_TIMER1);
                  }
               }
               lcd_t_c();
            }
            verpol();
            if(cambio==0)
            {
               disable_interrupts(INT_TIMER1);
               costear=0;
               MAINPOS=6;
            }
            if(rxdat)
            {
               procesa();
            }
         break;

         case 6:      //terminó sesion
            transmit_encab();
            gracias();
            delay_ms(300);
            RTXD=0xF3;
            transmit_1d();
            RTXD=CNTSEG_L;
            transmit_1d();
            RTXD=CNTSEG_H;
            fin_tx=1;
            transmit_1d();
            bloq();
            delay_ms(100);
            guarda_costos();
            lcd_t_c();
            fintemp=0;
            MAINPOS=0;
         break;
         
         case 7:   //bloqueo
            rele=1;
            delay_ms(1000);
            rele=0;
            MAINPOS=0;
         break;
         
         case 8:   //llamada entrante, espera colgado
            disable_interrupts(INT_RTCC);
            change_dir1();
            if((vl1)&(vl2))
            {
                MAINPOS=7;
            }
            change_dir0();
            if(rxdat)
            {
               procesa();
            }
         break;
       }
   }while(1);
}

