void leerluz()
{
   luzstd[0]=read_eeprom(64);
   if (luzstd[0])
      lcd_bl=1;
   else
      lcd_bl=0;
}

void transmit_encab()
{
set_timer0(116);
enable_interrupts(int_timer0);
OVF2=0;
dsr=0;
tx_encab=0;
fin_tx=0;
do
   {
   if(dsr)
      {
          dsr_t=0;
          disable_interrupts(global);
          delay_us(10);
          putc(0);
          delay_us(1100);
          putc('Z');
          delay_us(1100);
          putc('V');
          delay_us(1100);
          putc(NUMCAB);
          delay_us(1100);
          tx_encab=1;
          OVF2=4;
         }
   }while(OVF2!=4);
}

void transmit_1d()
{
if(tx_encab)
   {
   putc(RTXD);
   delay_us(1100);
   }
if(fin_tx)
   {
    enable_interrupts(global);
    dsr_t=1;
   }
}

void ver_costo()
{
VAR_0 = VAR_0 + RCOSTO_0;
if(VAR_0 > 9)
   {
    VAR_1++;
    VAR_0=VAR_0-10;
   }

VAR_1 = VAR_1 + RCOSTO_1;
if(VAR_1 > 9)
   {
    VAR_2++;
    VAR_1 = VAR_1-10;
   }

VAR_2 = VAR_2 + RCOSTO_2;
if(VAR_2 > 9)
   {
    VAR_3++;
    VAR_2=VAR_2-10;
   }

VAR_3 = VAR_3 + RCOSTO_3;
costear = 0;
}

void tau0()
{
t0if = 0;
set_timer0(116);
OVF1 = 0;
}

void tau1()
{
set_timer1(9606);
OVFLW = 0;
CNTSEG_L = 0;
CNTSEG_H = 0;
SEGUNDOS = 0;
MIN_D = 0;
MIN_U = 0;
SEG_D = 0;
SEG_U = 0;
fls = 0;
}

void tau2()
{
TMIN_D = 0;
TMIN_U = 0;
TSEG_D = 0;
TSEG_U = 0;
}

void aclara_bits()
{
lcd_bit0 = 0;
lcd_bit1 = 0;
lcd_bit2 = 0;
lcd_bit3 = 0;
}

void tarda()
{
delay_cycles(4);
}

void inicializa_lcd()
{
SET_TRIS_B(0x02);   //00001110
aclara_bits();
r_s = 0;
r_w = 0;
en = 0;
delay_ms(15);
lcd_bit1 = 1;
restart_wdt();
en = 1;
tarda();
en = 0;
delay_us(4500);
en = 1;
tarda();
en=0;
delay_us(336);
en = 1;
tarda();
en = 0;
lcd_bit1 = 1;
en = 1;
tarda();
en = 0;
delay_ms(5);   //9
lcd_bit1 = 1;
en = 1;
tarda();
en = 0;
lcd_bit2 = 1;
lcd_bit3 = 1;
en = 1;
tarda();
en = 0;
SET_TRIS_B(0xE3);   //11101111
}

void change_dir0()
{
   vlen = 1;
   TRISB = tris_tmp;
   if(oe_tmp == 0)
      oe = 0;
   else
      oe = 1;
}

void change_dir1()
{
   tris_tmp = TRISB;
   if(oe)
      oe_tmp = 1;
   else
      oe_tmp = 0;
   oe = 0;
   TRISB = 0xE3;
   vlen = 0;
}

void verpol()
{
change_dir1();
if((vl1 != pol1)&(vl2!=pol2))
   {
   cambio = 1;
   }
else
   {
   cambio = 0;
   }
change_dir0();
}

void bs_check()
{
set_tris_b(0xE3);
oe = 0;
r_s = 0;
r_w = 1;
bit_prueba = 1;
do
   {
    en=1;
    restart_wdt();
    if (!lcd_bit3)
      {
      bit_prueba = 0;
      }
    en=0;
    tarda();
    en=1;
    tarda();
    en=0;
      }while (bit_prueba);
}

void send_char()
{
bs_check();
SET_TRIS_B(0x02);   //00001110
aclara_bits();
if(bit_test(CHAR_LCD,4))
   {
   lcd_bit0 =1;
   }
if(bit_test(CHAR_LCD,5))
   {
   lcd_bit1 =1;
   }
if(bit_test(CHAR_LCD,6))
   {
   lcd_bit2 =1;
   }
if(bit_test(CHAR_LCD,7))
   {
   lcd_bit3 =1;
   }

r_w = 0;
r_s = 1;
en = 1;
restart_wdt();
en = 0;
aclara_bits();
if (bit_test(CHAR_LCD,0))
   {
   lcd_bit0=1;
   }
if (bit_test(CHAR_LCD,1))
   {
   lcd_bit1=1;
   }
if (bit_test(CHAR_LCD,2))
   {
   lcd_bit2=1;
   }
if (bit_test(CHAR_LCD,3))
   {
   lcd_bit3=1;
   }
en = 1;
restart_wdt();
en = 0;
SET_TRIS_B(0xE3);   //11101111
}

void send_cmd()
{
bs_check();
SET_TRIS_B(0x02);   //00001110
aclara_bits();
if (bit_test(CMD_LCD,4))
   {
   lcd_bit0=1;
   }
if (bit_test(CMD_LCD,5))
   {
   lcd_bit1=1;
   }
if (bit_test(CMD_LCD,6))
   {
   lcd_bit2=1;
   }
if (bit_test(CMD_LCD,7))
   {
   lcd_bit3=1;
   }
r_w=0;
r_s=0;
en=1;
restart_wdt();
en=0;
aclara_bits();
if (bit_test(CMD_LCD,0))
   {
   lcd_bit0=1;
   }
if (bit_test(CMD_LCD,1))
   {
   lcd_bit1=1;
   }
if (bit_test(CMD_LCD,2))
   {
   lcd_bit2=1;
   }
if (bit_test(CMD_LCD,3))
   {
   lcd_bit3=1;
   }
en=1;
restart_wdt();
en=0;
SET_TRIS_B(0xE3);   //11101111
}

void sec_zv()
{
switch(CONT5)
   {

   case 0:  CHAR_LCD=' '; break;
   case 1:  CHAR_LCD=' '; break;
   case 2:  CHAR_LCD='['; break;
   case 3:  CHAR_LCD=':'; break;
   case 4:  CHAR_LCD=':'; break;
   case 5:  CHAR_LCD='T'; break;
   case 6:  CHAR_LCD='e'; break;
   case 7:  CHAR_LCD='l'; break;
   case 8:  CHAR_LCD='B'; break;
   case 9:  CHAR_LCD='o'; break;
   case 10: CHAR_LCD='X'; break;
   case 11: CHAR_LCD=':'; break;
   case 12: CHAR_LCD=':'; break;
   case 13: CHAR_LCD=']'; break;
   case 14: CHAR_LCD=' '; break;
   case 15: CHAR_LCD=' '; break;

      case 16:
      if(bloquea)
      {
      CHAR_LCD=' ';
      }
      else
      {
      CHAR_LCD='C';
      }
      break;
      case 17: CHAR_LCD='o'; break;
      case 18:
      if(bloquea)
      {
      CHAR_LCD='f';
      }
      else
      {
      CHAR_LCD='n';
      }
      break;
      case 19:
      if(bloquea)
      {
      CHAR_LCD='f';
      }
      else
      {
      CHAR_LCD='e';
      }
      break;
      case 20:
      if(bloquea)
      {
      CHAR_LCD='l';
      }
      else
      {
      CHAR_LCD='c';
      }
      break;
      case 21:
      if(bloquea)
      {
      CHAR_LCD='i';
      }
      else
      {
      CHAR_LCD='t';
      }
      break;
      case 22:
            if(bloquea)
      {
      CHAR_LCD='n';
      }
      else
      {
      CHAR_LCD='a';
      }
      break;
      case 23:
      if(bloquea)
         {
         CHAR_LCD='e';
         }
         else
         {
         CHAR_LCD='d';
         }
      break;
      case 24:
         if(bloquea)
         {
         CHAR_LCD=' ';
         }
         else
         {
         CHAR_LCD='o';
         }
      break;
      case 25: CHAR_LCD=' '; break;
      case 26: CHAR_LCD='c'; break;
      case 27: CHAR_LCD='a'; break;
      case 28: CHAR_LCD='b'; break;
      case 29: CHAR_LCD=':'; break;
      case 30: CHAR_LCD=CAB_DEC+48; break;
      case 31: CHAR_LCD=CAB_UNID+48; break;
      }
}

void sec_no_permite()
{
switch(CONT3)
   {
      case 0:  
      CHAR_LCD='N'; 
   break;
      case 1:  
      CHAR_LCD='u'; 
   break;
      case 2:  
      CHAR_LCD='m'; 
   break;
      case 3:  
      CHAR_LCD='e'; 
   break;
      case 4:  
      CHAR_LCD='r'; 
   break;
      case 5:  
      CHAR_LCD='o'; 
   break;
      case 6:  
      CHAR_LCD=' '; 
   break;
      case 7:  
      CHAR_LCD='P'; 
   break;
      case 8:  
      CHAR_LCD='r'; 
   break;
      case 9:  
      CHAR_LCD='o'; 
   break;
      case 10: 
      CHAR_LCD='h'; 
   break;
      case 11: 
      CHAR_LCD='i'; 
   break;
      case 12: 
      CHAR_LCD='b'; 
   break;
      case 13: 
      CHAR_LCD='i'; 
   break;
      case 14: 
      CHAR_LCD='d'; 
   break;
      case 15: 
      CHAR_LCD='o'; 
   break;
   }
}

void lcd_xy()
{
CMD_LCD = 128 + (fila)*64 + RCOL;
send_cmd();
}

void no_permitido()
{
fila=0;
RCOL=0;
lcd_xy();
for (CONT3 = 0;CONT3 < 16; ++ CONT3)
   {
    sec_no_permite();
    send_char();
      }
}

void aclara_fila1()
{
CMD_LCD = 0x80;
send_cmd();
for (CMD_LCD = 0x80;CMD_LCD < 0x94; ++ CMD_LCD)
   {
    CHAR_LCD=' ';
    send_char();
      }
}

void aclara_fila2()
{
CMD_LCD = 0xC0;
send_cmd();
for (CMD_LCD=0xC0;CMD_LCD<0xD4;++CMD_LCD)
   {
    CHAR_LCD=' ';
    send_char();
      }
}

// Gracias
void txtgracias()
{
switch (CONT3)
   {
      case 0: CHAR_LCD=' '; break;
      case 1: CHAR_LCD=' '; break;
      case 2: CHAR_LCD=' '; break;
      case 3: CHAR_LCD=' '; break;
      case 4: CHAR_LCD='G'; break;
      case 5: CHAR_LCD='r'; break;
      case 6: CHAR_LCD='a'; break;
      case 7: CHAR_LCD='c'; break;
      case 8: CHAR_LCD='i'; break;
      case 9: CHAR_LCD='a'; break;
      case 10: CHAR_LCD='s'; break;
      case 11: CHAR_LCD='!'; break;
      case 12: CHAR_LCD=' '; break;
      case 13: CHAR_LCD=' '; break;
      case 14: CHAR_LCD=' '; break;
      case 15: CHAR_LCD=' '; break;
   }
}

void gracias()
{
fila=0;
RCOL=0;
lcd_xy();
for (CONT3=0;CONT3<16;++CONT3)
   {
    txtgracias();
    send_char();
   }
}
//-------------------------------------

void lineainf()
{
switch (CONT5)//cont3
   {
      case 0: CHAR_LCD='c'; break;
      case 1: CHAR_LCD='a'; break;
      case 2: CHAR_LCD='r'; break;
      case 3: CHAR_LCD='g'; break;
      case 4: CHAR_LCD='a'; break;
      case 5: CHAR_LCD='n'; break;
      case 6: CHAR_LCD='d'; break;
      case 7: CHAR_LCD='o'; break;
      case 8: CHAR_LCD=' '; break;
      case 9: CHAR_LCD='m'; break;
      case 10: CHAR_LCD='e'; break;
      case 11: CHAR_LCD='m'; break;
      case 12: CHAR_LCD='o'; break;
      case 13: CHAR_LCD='r'; break;
      case 14: CHAR_LCD='i'; break;
      case 15: CHAR_LCD='a'; break;
   }
}

void txtup()
{
switch (CONT5)//cont3
   {
      case 0: CHAR_LCD=' '; break;
      case 1: CHAR_LCD='T'; break;
      case 2: CHAR_LCD='A'; break;
      case 3: CHAR_LCD='R'; break;
      case 4: CHAR_LCD='I'; break;
      case 5: CHAR_LCD='F'; break;
      case 6: CHAR_LCD='A'; break;
      case 7: CHAR_LCD='D'; break;
      case 8: CHAR_LCD='O'; break;
      case 9: CHAR_LCD='R'; break;
      case 10: CHAR_LCD=' '; break;
      case 11: CHAR_LCD='v'; break;
      case 12: CHAR_LCD='9'; break;
      case 13: CHAR_LCD='.'; break;
      case 14: CHAR_LCD='1'; break;
      case 15: CHAR_LCD=' '; break;
   }
}


void proced_0()
{
fila=1;
RCOL=0;
lcd_xy();
for (CONT5 = 0;CONT5 < 16; ++ CONT5)
   {
    lineainf();
    send_char();
   }
}

void proced_1()
{
lcd_xy();
for (CONT0=0;CONT0<16;++CONT0)
   {  
      CHAR_LCD=255;
      send_char();
   }
}

void lineup()
{
fila=0;
RCOL=0;
lcd_xy();
for (CONT5 = 0;CONT5 < 16; ++ CONT5)
   {
    txtup();
    send_char();
   }
}

void logo_inicial()
{
lineup();
proced_0();
delay_ms(500);
fila = 1;
RCOL = 0;
for (CONT0=0;CONT0<16;++CONT0) //8
   {
    RCOL=CONT0;
    lcd_xy();    
    CHAR_LCD=255;
    send_char();
    delay_ms(90);
   }
}

void txtcabbloq()
{
switch (CONT3)
   {
      case 0: CHAR_LCD=' '; break;
      case 1: CHAR_LCD='L'; break;
      case 2: CHAR_LCD='i'; break;
      case 3: CHAR_LCD='b'; break;
      case 4: CHAR_LCD='e'; break;
      case 5: CHAR_LCD='r'; break;
      case 6: CHAR_LCD='a'; break;
      case 7: CHAR_LCD='r'; break;
      case 8: CHAR_LCD=' '; break;
      case 9: CHAR_LCD='C'; break;
      case 10: CHAR_LCD='a'; break;
      case 11: CHAR_LCD='b'; break;
      case 12: CHAR_LCD='i'; break;
      case 13: CHAR_LCD='n'; break;
      case 14: CHAR_LCD='a'; break;
      case 15: CHAR_LCD=' '; break;
   }
}

void cab_bloqueada()
{
fila=0;
RCOL=0;
lcd_xy();
for (CONT3=0;CONT3<16;++CONT3)
   {
    txtcabbloq();
    send_char();
      }
}

void bloq()
{
rele = 1;
delay_ms(1200);
rele = 0;
delay_ms(200);
KEYPOS = 0;
}

void aclara_costoinicial()
{
MVAR_0=0;
MVAR_1=0;
MVAR_2=0;
MVAR_3=0;
TSEG_U=0;
TSEG_D=0;
TMIN_U=0;
TMIN_D=0;
SEG_U=0;
SEG_D=0;
MIN_U=0;
MIN_D=0;
}

void aclara_costos()
{
RCOSTO_0=0;
RCOSTO_1=0;
RCOSTO_2=0;
RCOSTO_3=0;
VAR_0=0;
VAR_1=0;
VAR_2=0;
VAR_3=0;
}

void proced_4()
{
restart_wdt();
bloquea=(bit_test(dreceiv0,5));
cortar=(bit_test(dreceiv0,2));
if(bit_test(dreceiv0,4))
   {
    aclara_costoinicial();
    aclara_costos();
    tau1();
    tau2();
   }
}

void procesa()
{
if (MAINPOS==0)
   {
    proced_4();
    rxdat=0;
   }

if (MAINPOS==1)
   {
    proced_4();
    rxdat=0;
   }

if (MAINPOS==2)
   {
    proced_4();
    rxdat=0;
   }

if (MAINPOS==3)
   {
      rxdat=0;
   }

if (MAINPOS==4)
   {
      proced_4();
      if(bit_test(dreceiv0,4))
      {  //dreceiv0=< 0,cortar,bloquea,ini_sesion,0,fin_sesion,0,0 >
         aclara_costoinicial();}
         COSTO_DEC=dreceiv1;      //
         COSTO_ENT=dreceiv2;      //
         TPARCIAL=dreceiv3;        //
         DELTASEG=TPARCIAL;
         DURMAX= dreceiv5*256+dreceiv4;      //
         RCOSTO_1=0;
         RCOSTO_3=0;
      while(COSTO_DEC>9)
      {
         COSTO_DEC=COSTO_DEC-10;
         RCOSTO_1++;
      }
      RCOSTO_0=COSTO_DEC;
      while(COSTO_ENT>9)
      {
         COSTO_ENT=COSTO_ENT-10;
         RCOSTO_3++;
      }
      RCOSTO_2=COSTO_ENT;
      VAR_0=RCOSTO_0;
      VAR_1=RCOSTO_1;
      VAR_2=RCOSTO_2;
      VAR_3=RCOSTO_3;
      rxdat=0;
   }

if (MAINPOS==5)
   {
      if(bit_test(dreceiv0,2))
      {
         MAINPOS=6;
         rxdat=0;
      }
   }
}

void guarda_costos()
{

MVAR_0=MVAR_0+VAR_0;
if(MVAR_0>9)
   {
    MVAR_0=MVAR_0-10;
    MVAR_1++;
   }

MVAR_1=MVAR_1+VAR_1;
if(MVAR_1>9)
   {
    MVAR_1=MVAR_1-10;
    MVAR_2++;
   }

MVAR_2=MVAR_2+VAR_2;
if(MVAR_2>9)
   {
    MVAR_2=MVAR_2-10;
    MVAR_3++;
   }

MVAR_3=MVAR_3+VAR_3;
VAR_0=MVAR_0;
VAR_1=MVAR_1;
VAR_2=MVAR_2;
VAR_3=MVAR_3;

TSEG_U=TSEG_U+SEG_U;
if(TSEG_U>9)
   {
    TSEG_U=TSEG_U-10;
    TSEG_D++;
   }

TSEG_D=TSEG_D+SEG_D;
if(TSEG_D>5) //Modificado 2008
   {
    TSEG_D=TSEG_D-6;
    TMIN_U++;
   }

TMIN_U=TMIN_U+MIN_U;
if(TMIN_U>9)
   {
    TMIN_U=TMIN_U-10;
    TMIN_D++;
   }

TMIN_D=TMIN_D+MIN_D;
SEG_U=TSEG_U;
SEG_D=TSEG_D;
MIN_U=TMIN_U;
MIN_D=TMIN_D;
}

void teclado()
{
restart_wdt();
switch (KEYPOS)
   {
   case 0:
         KEYSTAD=0;
         KEYDAT=0;
         fintec=0;
         change_dir1();
         if(vl1&vl2)
         {
            tau0();
            enable_interrupts(int_timer0);
            KEYPOS=1;
         }
         change_dir0();
         if (std)
         {
            KEYPOS=3;
         }
   break;

   case 1:
      if(OVF1>44)
         {         //19
            fintec=1;
            KEYSTAD=2;      //colgó
            KEYPOS=0;
         }
      change_dir1();
         if((!vl1)|(!vl2))
         {
           if(OVF1<5)
            {
               KEYSTAD=3;   //pulso muy corto
               corto=1;
               KEYPOS=0;
            }
            else
            {
               tau0();
               KEYDAT++;   //pulso válido
               KEYPOS=2;
            }
            }
      change_dir0();
   break;

   case 2:
         if(OVF1>18)
         {
            KEYSTAD=1;   // fin de la secuencia de pulsos
            if (KEYDAT==10)
            {
            KEYDAT=0;
            }
            fintec=1;
            KEYPOS=0;
         }
         else
         {
            change_dir1();
            if(vl1&vl2)
            {
               tau0();
               KEYPOS=1;
            }
            change_dir0();
         }
   break;

   case 3:
         oe=1;
         set_tris_b(0xE3);  //11100011
         do
         {
         restart_wdt();
            }while(std==1);
         if (dtmf_bit0)
         {
            bit_set(KEYDAT,0);
         }
         if (dtmf_bit1)
         {
            bit_set(KEYDAT,1);
         }
         if (dtmf_bit2)
         {
            bit_set(KEYDAT,2);
         }
         if (dtmf_bit3)
         {
            bit_set(KEYDAT,3);
         }
         if (KEYDAT==10)
         {
            KEYDAT=0;
         }
         oe=0;
         KEYSTAD=4;
         fintec=1;
         KEYPOS=0;
   break;
   }
}

//Envia Tiempo y Costo
void sec_tc()
{

switch(CONT3)
   {
      case 0: CHAR_LCD='T'; break;
      case 1: CHAR_LCD='='; break;
      case 2: CHAR_LCD=MIN_D+48; break;
      case 3: CHAR_LCD=MIN_U+48; break;
      case 4: CHAR_LCD=':'; break;
      case 5: CHAR_LCD=SEG_D+48; break;
      case 6: CHAR_LCD=SEG_U+48; break;
      case 7: CHAR_LCD=' '; break;
      case 8: CHAR_LCD='S'; break;
      case 9: CHAR_LCD='/'; break;
      case 10: CHAR_LCD='.'; break;
      case 11: CHAR_LCD=VAR_3+48; break;
      case 12: CHAR_LCD=VAR_2+48; break;
      case 13: CHAR_LCD='.'; break;
      case 14: CHAR_LCD=VAR_1+48; break;
      case 15: CHAR_LCD=VAR_0+48; break;
      // Mensajes
      case 16: CHAR_LCD=PUBLICIDAD2[0]; break;
      case 17: CHAR_LCD=PUBLICIDAD2[1]; break;
      case 18: CHAR_LCD=PUBLICIDAD2[2]; break;
      case 19: CHAR_LCD=PUBLICIDAD2[3]; break;
      case 20: CHAR_LCD=PUBLICIDAD2[4]; break;
      case 21: CHAR_LCD=PUBLICIDAD2[5]; break;
      case 22: CHAR_LCD=PUBLICIDAD2[6]; break;
      case 23: CHAR_LCD=PUBLICIDAD2[7]; break;
      case 24: CHAR_LCD=PUBLICIDAD2[8]; break;
      case 25: CHAR_LCD=PUBLICIDAD2[9]; break;
      case 26: CHAR_LCD=PUBLICIDAD2[10]; break;
      case 27: CHAR_LCD=PUBLICIDAD2[11]; break;
      case 28: CHAR_LCD=PUBLICIDAD2[12]; break;
      case 29: CHAR_LCD=PUBLICIDAD2[13]; break;
      case 30: CHAR_LCD=PUBLICIDAD2[14]; break;
      case 31: CHAR_LCD=PUBLICIDAD2[15]; break;
            // Mensajes 2
      case 32: CHAR_LCD=PUBLICIDAD2[16]; break;
      case 33: CHAR_LCD=PUBLICIDAD2[17]; break;
      case 34: CHAR_LCD=PUBLICIDAD2[18]; break;
      case 35: CHAR_LCD=PUBLICIDAD2[19]; break;
      case 36: CHAR_LCD=PUBLICIDAD2[20]; break;
      case 37: CHAR_LCD=PUBLICIDAD2[21]; break;
      case 38: CHAR_LCD=PUBLICIDAD2[22]; break;
      case 39: CHAR_LCD=PUBLICIDAD2[23]; break;
      case 40: CHAR_LCD=PUBLICIDAD2[24]; break;
      case 41: CHAR_LCD=PUBLICIDAD2[25]; break;
      case 42: CHAR_LCD=PUBLICIDAD2[26]; break;
      case 43: CHAR_LCD=PUBLICIDAD2[27]; break;
      case 44: CHAR_LCD=PUBLICIDAD2[28]; break;
      case 45: CHAR_LCD=PUBLICIDAD2[29]; break;
      case 46: CHAR_LCD=PUBLICIDAD2[30]; break;
      case 47: CHAR_LCD=PUBLICIDAD2[31]; break;
   }
}

void lcd_t_c()
{
fila=1;
RCOL=0;
lcd_xy();

for (CONT3 = 0;CONT3 < 16; ++ CONT3)
   {
    sec_tc();
    send_char();
   }
}

void lcd_publi1()
{
fila=1;
RCOL=0;
lcd_xy();

for (CONT3 = 16;CONT3 < 32; ++ CONT3)
   {
    sec_tc();
    send_char();
   }
}

void lcd_publi2()
{
fila=1;
RCOL=0;
lcd_xy();

for (CONT3 = 32;CONT3 < 48; ++ CONT3)
   {
    sec_tc();
    send_char();
   }
}
//
//----------------------------------

//Teclas digitadas
void decod()
{
switch (CONT3)
   {
      case 0: CHAR_LCD='0'; break;
      case 1: CHAR_LCD='1'; break;
      case 2: CHAR_LCD='2'; break;
      case 3: CHAR_LCD='3'; break;
      case 4: CHAR_LCD='4'; break;
      case 5: CHAR_LCD='5'; break;
      case 6: CHAR_LCD='6'; break;
      case 7: CHAR_LCD='7'; break;
      case 8: CHAR_LCD='8'; break;
      case 9: CHAR_LCD='9'; break;
      case 10: CHAR_LCD='A'; break;
      case 11: CHAR_LCD='*'; break;
      case 12: CHAR_LCD='#'; break;
      case 13: CHAR_LCD='D'; break;
      case 14: CHAR_LCD='E'; break;
      case 15: CHAR_LCD='F'; break;
   }
}
//----------------------------------

// Grabar numero de cabina
void sec_nc()
{
switch (CONT3)
   {
      case 0: CHAR_LCD=' '; break;
      case 1: CHAR_LCD='N'; break;
      case 2: CHAR_LCD='u'; break;
      case 3: CHAR_LCD='m'; break;
      case 4: CHAR_LCD='e'; break;
      case 5: CHAR_LCD='r'; break;
      case 6: CHAR_LCD='o'; break;
      case 7: CHAR_LCD=' '; break;
      case 8: CHAR_LCD='G'; break;
      case 9: CHAR_LCD='r'; break;
      case 10: CHAR_LCD='a'; break;
      case 11: CHAR_LCD='b'; break;
      case 12: CHAR_LCD='a'; break;
      case 13: CHAR_LCD='d'; break;
      case 14: CHAR_LCD='o'; break;
      case 15: CHAR_LCD=' '; break;
   }
}

void numcab_grabada()
{
fila=0;
RCOL=0;
lcd_xy();
for (CONT3=0;CONT3<16;++CONT3)
   {
    sec_nc();
    send_char();
      }
}

void proced_2()
{
aclara_fila1();
KEYPUNT=&KEY0;
fila=0;
RCOL=0;
lcd_xy();
KEYDAT=0xFF;
}

void proced_3()
{
write_eeprom(0,CAB_DEC);
write_eeprom(1,CAB_UNID);
numcab_grabada();
delay_ms(800);
gracias();
delay_ms(800);
proced_2();
}

void display_num()
{
*KEYPUNT=KEYDAT;
CONT3=KEYDAT;
decod();
send_char();
}

void ver_clave()
{
clave=0;
if((KEY0==6)&&(KEY1==7)&&(KEY2==0)&&(KEY3==5))
   {
   clave=1;
   //programacion();
   //delay_ms(800);
   delay_ms(20);
   return;
   }
/*if((KEY0==PASSW0)&&(KEY1==PASSW1)&&(KEY2==PASSW2)&&(KEY3==PASSW3))
   {
   clave=1;
   }*/
}

void ver_funcion()
{
restart_wdt();
if (KEY0==9)
   {
      switch (KEY1)
      {
         /*case 0:        //flash para hacer llamadas
            bloq();
            do
            {
               teclado();
               }while(KEYSTAD!=2);
         break;*/

         case 6:        //graba número de cabina
            if (KEYNUM==5)
            {
               CAB_DEC=KEY2;
               CAB_UNID=KEY3;
               NUMCAB=10*KEY2+KEY3;
               proced_3();
            }

            if (KEYNUM==4)
            {
               CAB_DEC=0;
               CAB_UNID=KEY2;
               NUMCAB=KEY2;
               proced_3();
            }
         break;
         }
   }
}

void grabadat()
{
KEYPUNT=&KEY0;
KEYNUM=0;
KEYDAT=0xFF;
KEYSTAD=0;
fin_funcion=0;
fin_graba=0;
do
   {
      teclado();
      if (fintec)
      {
         CHAR_LCD='*';
         send_char();
         *KEYPUNT=KEYDAT;
         KEYPUNT++;
         KEYNUM++;
      }
   }while((KEYSTAD!=2)&&(KEYNUM<4));
if(KEYNUM==4)
   {
   ver_clave();
   }
if(clave)
   {
     proced_2();
      KEYNUM=0;
      do
      {
         teclado();
         if(KEYSTAD==4)
         {
            RDISP=KEYDAT;
            display_num();
            *KEYPUNT=KEYDAT;
            KEYPUNT++;
            KEYNUM++;
         }
         if(KEYDAT==0x0C)
         {
            ver_funcion();
            KEYNUM=0;
            proced_2();
         }
         if((KEYDAT==0x0B)||(KEYSTAD==2))
         {
            fin_graba=1;
            bloq();
            proced_2();
         }
         }while(!fin_graba);
   }
MAINPOS=0;
}

void telef_fin()
{
transmit_encab();
RTXD=0xF0;      //colgó
fin_tx=1;
transmit_1d();
bloq();
MAINPOS=0;
}

void marcando()
{
RDISP=KEYDAT;
display_num();
transmit_encab();
RTXD=KEYDAT;
fin_tx=1;
transmit_1d();
KEYNUM++;
if (KEYPUNT<&KEY8)
   {
      KEYPUNT++;
   }
}

void txt_destino()
{
switch (j)
   {
      case 0: CHAR_LCD=destino[0]; break;
      case 1: CHAR_LCD=destino[1]; break;
      case 2: CHAR_LCD=destino[2]; break;
      case 3: CHAR_LCD=destino[3]; break;
      case 4: CHAR_LCD=destino[4]; break;
      case 5: CHAR_LCD=destino[5]; break;
      case 6: CHAR_LCD=destino[6]; break;
      case 7: CHAR_LCD=destino[7]; break;
      case 8: CHAR_LCD=destino[8]; break;
      case 9: CHAR_LCD=destino[9]; break;
      case 10: CHAR_LCD=destino[10]; break;
      case 11: CHAR_LCD=destino[11]; break;
      case 12: CHAR_LCD=destino[12]; break;
      case 13: CHAR_LCD=destino[13]; break;
      case 14: CHAR_LCD=destino[14]; break;
      case 15: CHAR_LCD=destino[15]; break;
   }
}

void show_destino()
{
fila=0;
RCOL=0;
lcd_xy();
for (i=0;i<16;i++)
{
   destino[i]=read_eeprom(i+48);
}
for (j=0;j<16;j++)
{
   delay_ms(2);
   txt_destino();
   send_char();
}
}
