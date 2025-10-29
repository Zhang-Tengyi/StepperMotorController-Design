#include  <reg51.h>
#define uchar unsigned char
#define uint unsigned int
char code key[]={0xee,0xde,0xbe,0x7e,
            	0xed,0xdd,0xbd,0x7d,
            	0xeb,0xdb,0xbb,0x7b,
				0xe7,0xd7,0xb7,0x77}; // Key values
uchar code display[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code display_point[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

uchar code wei[]={0x7f,0xbf,0xdf,0xef,0xf7};

uchar Starget[]={0xc0,0xc0,0xc0,0xc0,0xff};
//  right                  left
uchar Temp_Starget[]={0xc0,0xc0,0xc0,0xc0};

uchar st[]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9}; // Motor sequence array
char count=0, count1=0, count2=0, num=0, change=0 ,KEYNum;
bit tempt=0,temps=0, tempz=0,b = 0;
uchar w=0, c=0, keychoice =0, digit, ten, hundred, thousand, t=0, information;
uint Velocity = 0 ;
sbit buzzer = P3^7;

// Delay function
void delay(unsigned int time)
{
  unsigned char j;
  while(time--)
  for(j=0;j<=121;j++);
}		    

// Key scanning
void keycho()
{
  uchar temp,k;
  char key_scan[]={0xef,0xdf,0xbf,0x7f}; // 4×4 keypad, column scanning
  KEYNum = 16; // No key pressed value
  for(k=0;k<4;k++)
  {
	 P1 = key_scan[k];
	 temp = P1 & 0x0f;  // Check if any key is pressed
	 if(temp != 0x0f)
	 {
		delay(10);  // Debounce
	    temp = P1 & 0x0f;
		if(temp != 0x0f)
		{
		   temp = P1;
		   buzzer = 0;
           switch(temp)	
				{
					case 0xee:
						KEYNum = 7;	   // Row 1, Column 1
						break;
					case 0xde:
						KEYNum = 8;	   // Row 1, Column 2
						break;
					case 0xbe:
						KEYNum = 9;	   // Row 1, Column 3
						break;
					case 0x7e:
						KEYNum = 10;	// Row 1, Column 4
						break;
					case 0xed:
						KEYNum = 4;	   // Row 2, Column 1
						break;
					case 0xdd:
						KEYNum = 5;	   // Row 2, Column 2
						break;
					case 0xbd:
						KEYNum = 6;	   // Row 2, Column 3
						break;
					case 0x7d:
						KEYNum = 11;	// Row 2, Column 4
						break;
					case 0xeb:
						KEYNum = 1;	   // Row 3, Column 1
						break;
					case 0xdb:
						KEYNum = 2;	   // Row 3, Column 2
						break;
					case 0xbb:
						KEYNum = 3;    // Row 3, Column 3
						break;
					case 0x7b:
						KEYNum = 12;   // Row 3, Column 4
						break;	
					case 0xe7:
						KEYNum = 13;   // Row 4, Column 1
						break;
					case 0xd7:
						KEYNum = 0;    // Row 4, Column 2
						break;
					case 0xb7:
						KEYNum = 14;   // Row 4, Column 3
						break;
					case 0x77:
						KEYNum = 15;   // Row 4, Column 4
						break;			
		   }
		}
	   while(temp != 0x0f)		 // Wait until key is released
			{
				temp = P1 & 0x0f;
				buzzer = !buzzer;	// Buzzer beeps						  
			}
	   buzzer = 1;
	 }
  }
}			   

// Key function handler
void function()
{
	 char check,check1,tempdis;
	 if(KEYNum == 10){	// Motor start/stop
	  tempt = !tempt;
	 }

	  if(KEYNum == 11 && tempt==1){  // Motor forward/reverse
			tempz = !tempz;
		 if (tempz == 1){
			 Starget[4] = 0xbf; 
		 }
		 else if (tempz == 0){
			 Starget[4] = 0xff; 
		 }
			  }
	  if(KEYNum == 15){  // Speed setting toggle
			 temps = !temps;
			 Velocity = digit + ten*10 + hundred*100 + thousand*1000; // Speed calculation
		 if(Velocity == 0){
			 t = 0;
		 	 }
		 else{
			 t = 30000/(48*Velocity);
			 }

		 if (temps == 1){  // Enter speed-setting mode
			 Starget[0] = Starget[0]-0x80;  // Initialize cursor position
			 for(tempdis=0;tempdis<4;tempdis++){
			 	Temp_Starget[tempdis] = Starget[tempdis];  // Store previous speed value
			 }			   
		   }
		else if (temps == 0){  // Confirm speed and exit setting mode
			 for(check1=0;check1<4;check1++){			  // Check digits with decimal point and convert to normal digits
			   for(check=0;check<10;check++){
				 if (Starget[check1] == display_point[check])
					 Starget[check1] = display[check];
			   }
			 }
		}
	}
	  if(KEYNum == 12 && temps == 1){	   // Cancel speed setting
	  		 temps = 0;
			 for(tempdis=0;tempdis<4;tempdis++){			
			 	Starget[tempdis] = Temp_Starget[tempdis];	// Restore previous speed value
			 }
			 for(check=0;check<10;check++){
				if (Starget[0] == display_point[check])
				Starget[0] = display[check];
			   }

	  }  
}

// Motor rotation/direction
void motor()
{                           
		if (tempz == 0){	 // Motor rotates forward
	    	if(num==8){                                  
			  num=0;                                      
		    }
		  P3 = st[num];
		   num++;                              
		  }
		if (tempz == 1){	 // Motor rotates in reverse
	    	if(num<0){                                  
			 num=7;
		 }
		  P3 = st[num];
		  num--;                               
		   }
}

// Real-time speed display
void leddisplay()
{
       	 if(tempt==0 && w!=4){
		 	P2 = wei[w];			
		 	P0 = 0xc0;
		  }
		 else{
		 	P2 = wei[w];			
		 	P0 = Starget[w];
		  }
		 w++;
		 if(w>4){
		  	w=0;
		 }
}

// Shift (cursor move) function
void move()
{
	if(KEYNum == 13 && temps == 1){  // Shift left
	Starget[change] = Starget[change]+0x80;	   // Remove decimal point
	change++;								   // Move cursor
		if (change >3 ){
	 	change=0;
		}
	Starget[change] = Starget[change]-0x80;	   // Add decimal point
	}

	else if(KEYNum == 14 && temps == 1){	// Shift right
	Starget[change] = Starget[change]+0x80;	   // Remove decimal point
	change--;								   // Move cursor
		if (change <0 ){
	 	change=3;
		}
	Starget[change] = Starget[change]-0x80;	   // Add decimal point
	}	
}

// Target speed setting
void target()
{

	if (KEYNum < 10){
		 Starget[change] = display[KEYNum]-0x80;	// Change digit
		if (change == 0)
		  digit = KEYNum;						   // Store digits separately: ones, tens, hundreds, thousands
		else if (change == 1)
		  ten = KEYNum;
		else if (change == 2)
		  hundred = KEYNum;
		else if (change == 3)
		  thousand = KEYNum;
		 }
		  
		 P2 = wei[w];		// Send digit select code
		 P0 = Starget[w];	// Send segment code
		 w++;
		 if(w>4){
		  	w=0;
		 }
}									  

void main()
{
  P1=0xff;	  // Initialization
  P0=0xff;
  P2=0xff;
  P3=0xff;
  buzzer = 1;

  PCON = 0x00; // Power control register, SMOD=0, baud rate not doubled
  SCON = 0x50; // Serial control register, mode 1, baud rate from Timer1
  TMOD=0x21;   // T0: mode 2, T1: mode 1
  TH0=0xFC;	   // Timer0 initial value
  TL0=0x18;
  TH1=0xfd;	   // Baud rate 9600
  TL1=0xfd;
  EA=1;
  ET0=ES=1;
  TR0=TR1=1;
  RI=0;
  TI=0;

  while(1)
  {
   keycho();
   if(KEYNum != 16)	   // If a key is pressed
   	function();		   // Execute corresponding function
	if(KEYNum == 13 || KEYNum == 14)	 // If shift key pressed
	  move();	 // Execute shift subroutine
  }
}

// T0 Timer interrupt, 1 ms period
void Timer0() interrupt 1
{
    TH0 = 0xFC ; 
	TL0 = 0x18 ;

	if(tempt == 1){	// Motor running
		count++;
		if(count == t && t != 0){ 
		motor();
		count = 0;
		}
	}
	
	if(temps == 0) {  // Display real-time speed
		count1++;
       if(count1 == 1){
		leddisplay();
		count1 = 0;
		}
	} 
		
	if(temps == 1) {  // Display target speed
		count2++;
		if(count2 == 1){
		target();
		count2 = 0;
		}		   	
	}                               
	else{ 
	    change = 0;
	} 
}

// Serial communication interrupt
void receive() interrupt 4{	   
   if(RI==1)
   {
   		information=SBUF;	   // Receive serial data
		RI=0;
		buzzer=0;
		switch(information)
			{
					case 0xee:
						KEYNum = 7;	   // Row 1, Column 1
						break;
					case 0xde:
						KEYNum = 8;	   // Row 1, Column 2
						break;
					case 0xbe:
						KEYNum = 9;	   // Row 1, Column 3
						break;
					case 0x7e:
						KEYNum = 10;	// Row 1, Column 4
						break;
					case 0xed:
						KEYNum = 4;	   // Row 2, Column 1
						break;
					case 0xdd:
						KEYNum = 5;	   // Row 2, Column 2
						break;
					case 0xbd:
						KEYNum = 6;	   // Row 2, Column 3
						break;
					case 0x7d:
						KEYNum = 11;	// Row 2, Column 4
						break;
					case 0xeb:
						KEYNum = 1;	   // Row 3, Column 1
						break;
					case 0xdb:
						KEYNum = 2;	   // Row 3, Column 2
						break;
					case 0xbb:
						KEYNum = 3;    // Row 3, Column 3
						break;
					case 0x7b:
						KEYNum = 12;   // Row 3, Column 4
						break;	
					case 0xe7:
						KEYNum = 13;   // Row 4, Column 1
						break;
					case 0xd7:
						KEYNum = 0;    // Row 4, Column 2
						break;
					case 0xb7:
						KEYNum = 14;   // Row 4, Column 3
						break;
					case 0x77:
						KEYNum = 15;   // Row 4, Column 4
						break;	
					
		   }
   }
}
