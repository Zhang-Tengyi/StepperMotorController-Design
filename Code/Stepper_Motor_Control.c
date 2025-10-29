#include  <reg51.h>
#define uchar unsigned char
#define uint unsigned int
char code key[]={0xee,0xde,0xbe,0x7e,
            	0xed,0xdd,0xbd,0x7d,
            	0xeb,0xdb,0xbb,0x7b,
				0xe7,0xd7,0xb7,0x77};//��λֵ
uchar code display[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code display_point[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

uchar code wei[]={0x7f,0xbf,0xdf,0xef,0xf7};

uchar Starget[]={0xc0,0xc0,0xc0,0xc0,0xff};
			 //  right				  left
uchar Temp_Starget[]={0xc0,0xc0,0xc0,0xc0};

uchar st[]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9};//�������
char count=0, count1=0, count2=0, num=0, change=0 ,KEYNum;
bit tempt=0,temps=0, tempz=0,b = 0;
uchar w=0, c=0, keychoice =0, digit, ten, hundred, thousand, t=0, information;
uint Velocity = 0 ;
sbit buzzer = P3^7;

//��ʱ����
void delay(unsigned int time)
{
  unsigned char j;
  while(time--)
  for(j=0;j<=121;j++);
}		    

//����ѡ��
void keycho()
{
  uchar temp,k;
  char key_scan[]={0xef,0xdf,0xbf,0x7f}; //4*4��λ������ɨ��
  KEYNum = 16; //�ռ�ֵ
  for(k=0;k<4;k++)
  {
	 P1 = key_scan[k];
	 temp = P1 & 0x0f;  //�ж��Ƿ��а�������
	 if(temp != 0x0f)
	 {
		delay(10);  //����
	    temp = P1 & 0x0f;
		if(temp != 0x0f)
		{
		   temp = P1;
		   buzzer = 0;
           switch(temp)	
				{
					case 0xee:
						KEYNum = 7;	   //��1�е�1��
						break;
					case 0xde:
						KEYNum = 8;	   //��1�е�2��
						break;
					case 0xbe:
						KEYNum = 9;	   //��1�е�3��
						break;
					case 0x7e:
						KEYNum = 10;	//��1�е�4��
						break;
					case 0xed:
						KEYNum = 4;	   //��2�е�1��
						break;
					case 0xdd:
						KEYNum = 5;	   //��2�е�2��
						break;
					case 0xbd:
						KEYNum = 6;	   //��2�е�3��
						break;
					case 0x7d:
						KEYNum = 11;	//��2�е�4��
						break;
					case 0xeb:
						KEYNum = 1;	   //��3�е�1��
						break;
					case 0xdb:
						KEYNum = 2;	   //��3�е�2��
						break;
					case 0xbb:
						KEYNum = 3;   //��3�е�3��
						break;
					case 0x7b:
						KEYNum = 12;   //��3�е�4��
						break;	
					case 0xe7:
						KEYNum = 13;   //��4�е�1��
						break;
					case 0xd7:
						KEYNum = 0;   //��4�е�2��
						break;
					case 0xb7:
						KEYNum = 14;   //��4�е�3��
						break;
					case 0x77:
						KEYNum = 15;   //��4�е�4��
						break;			
		   }
		}
	   while(temp != 0x0f)		 //�ȴ������ͷ�
			{
				temp = P1 & 0x0f;
				buzzer = !buzzer;	//��������						  
			}
	   buzzer = 1;
	 }
  }
}			   

//�������ܼ�
void function()
{
	 char check,check1,tempdis;
	 if(KEYNum == 10){	//�����ͣ
	  tempt = !tempt;
	 }

	  if(KEYNum == 11 && tempt==1){  //�����/��ת
			tempz = !tempz;
		 if (tempz == 1){
			 Starget[4] = 0xbf; 
		 }
		 else if (tempz == 0){
			 Starget[4] = 0xff; 
		 }
			  }
	  if(KEYNum == 15){  //�ٶ����ÿ���
			 temps = !temps;
			 Velocity = digit + ten*10 + hundred*100 + thousand*1000; //ת�ټ���
		 if(Velocity == 0){
			 t = 0;
		 	 }
		 else{
			 t = 30000/(48*Velocity);
			 }

		 if (temps == 1){  //�����ٶ�����ģʽ
			 Starget[0] = Starget[0]-0x80;  //���λ�ó�ʼ��
			 for(tempdis=0;tempdis<4;tempdis++){
			 	Temp_Starget[tempdis] = Starget[tempdis];  //������ǰ���ٶ���ֵ���д洢
			 }			   
		   }
		else if (temps == 0){  //ȷ���ٶȲ��˳��ٶ�����ģʽ
			 for(check1=0;check1<4;check1++){			  //����С��������ֲ����任����С���������
			   for(check=0;check<10;check++){
				 if (Starget[check1] == display_point[check])
					 Starget[check1] = display[check];
			   }
			 }
		}
	}
	  if(KEYNum == 12 && temps == 1){	   //�ٶ�����ȡ������
	  		 temps = 0;
			 for(tempdis=0;tempdis<4;tempdis++){			
			 	Starget[tempdis] = Temp_Starget[tempdis];	//������ǰ���ٶ���ֵ��ֵ��
			 }
			 for(check=0;check<10;check++){
				if (Starget[0] == display_point[check])
				Starget[0] = display[check];
			   }

	  }  
}

//���ת��/����
void motor()
{                           
		if (tempz == 0){	 //���������ת
	    	if(num==8){                                  
			  num=0;                                      
		    }
		  P3 = st[num];
		   num++;                              
		  }
		if (tempz == 1){	 //���������ת
	    	if(num<0){                                  
			 num=7;
		 }
		  P3 = st[num];
		  num--;                               
		   }
}

//ʵʱ�ٶ���ʾ
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

//��λ����
void move()
{
	if(KEYNum == 13 && temps == 1){  //����
	Starget[change] = Starget[change]+0x80;	   //С������ȥ
	change++;								   //��λ
		if (change >3 ){
	 	change=0;
		}
	Starget[change] = Starget[change]-0x80;	   //С�������
	}

	else if(KEYNum == 14 && temps == 1){	//����
	Starget[change] = Starget[change]+0x80;	   //С������ȥ
	change--;								   //��λ
		if (change <0 ){
	 	change=3;
		}
	Starget[change] = Starget[change]-0x80;	   //С�������
	}	
}

//Ŀ���ٶ�
void target()
{

	if (KEYNum < 10){
		 Starget[change] = display[KEYNum]-0x80;	//��ֵ�ı�
		if (change == 0)
		  digit = KEYNum;						   //�ֱ𴢴����ʮ���١�ǧλ����ֵ
		else if (change == 1)
		  ten = KEYNum;
		else if (change == 2)
		  hundred = KEYNum;
		else if (change == 3)
		  thousand = KEYNum;
		 }
		  
		 P2 = wei[w];		//��λ��
		 P0 = Starget[w];	//�Ͷ���
		 w++;
		 if(w>4){
		  	w=0;
		 }
}									  

void main()
{
  P1=0xff;	  //��ʼ��
  P0=0xff;
  P2=0xff;
  P3=0xff;
  buzzer = 1;

  PCON = 0x00; //��Դ���ƼĴ�����SMOD����0�������ʲ��ӱ�
  SCON = 0x50; //���ڿ��ƼĴ��������пڹ�����ʽ1���������ɶ�ʱ��T1����
  TMOD=0x21;//T0��ʱ��ʽ2��T1��ʱ��ʽ1
  TH0=0xFC;	//���� 
  TL0=0x18;
  TH1=0xfd;	//������9600
  TL1=0xfd;
  EA=1;
  ET0=ES=1;
  TR0=TR1=1;
  RI=0;
  TI=0;

  while(1)
  {
   keycho();
   if(KEYNum != 16)	   //���а������º�
   	function();		   //�����ӳ���
	if(KEYNum == 13 || KEYNum == 14)	 //������λ�Ƽ����º�
	  move();	 //��λ�ӳ���
  }
}

//T0��ʱ�� 1ms
void Timer0() interrupt 1
{
    TH0 = 0xFC ; 
	TL0 = 0x18 ;

	if(tempt == 1){	//���ת��
		count++;
		if(count == t && t != 0){ 
		motor();
		count = 0;
		}
	}
	
	if(temps == 0) {  //��ʾʵʱ�ٶ�
		count1++;
       if(count1 == 1){
		leddisplay();
		count1 = 0;
		}
	} 
		
	if(temps == 1) {  //��ʾĿ���ٶ�
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

void receive() interrupt 4{	   //����ͨ���ж�
   if(RI==1)
   {
   		information=SBUF;	   //���մ�����Ϣ
		RI=0;
		buzzer=0;
		switch(information)
			{
					case 0xee:
						KEYNum = 7;	   //��1�е�1��
						break;
					case 0xde:
						KEYNum = 8;	   //��1�е�2��
						break;
					case 0xbe:
						KEYNum = 9;	   //��1�е�3��
						break;
					case 0x7e:
						KEYNum = 10;	//��1�е�4��
						break;
					case 0xed:
						KEYNum = 4;	   //��2�е�1��
						break;
					case 0xdd:
						KEYNum = 5;	   //��2�е�2��
						break;
					case 0xbd:
						KEYNum = 6;	   //��2�е�3��
						break;
					case 0x7d:
						KEYNum = 11;	//��2�е�4��
						break;
					case 0xeb:
						KEYNum = 1;	   //��3�е�1��
						break;
					case 0xdb:
						KEYNum = 2;	   //��3�е�2��
						break;
					case 0xbb:
						KEYNum = 3;   //��3�е�3��
						break;
					case 0x7b:
						KEYNum = 12;   //��3�е�4��
						break;	
					case 0xe7:
						KEYNum = 13;   //��4�е�1��
						break;
					case 0xd7:
						KEYNum = 0;   //��4�е�2��
						break;
					case 0xb7:
						KEYNum = 14;   //��4�е�3��
						break;
					case 0x77:
						KEYNum = 15;   //��4�е�4��
						break;	
					
		   }
   }
}
