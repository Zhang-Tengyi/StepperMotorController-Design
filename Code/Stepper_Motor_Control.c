#include  <reg51.h>
#define uchar unsigned char
#define uint unsigned int
char code key[]={0xee,0xde,0xbe,0x7e,
            	0xed,0xdd,0xbd,0x7d,
            	0xeb,0xdb,0xbb,0x7b,
				0xe7,0xd7,0xb7,0x77};//键位值
uchar code display[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code display_point[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

uchar code wei[]={0x7f,0xbf,0xdf,0xef,0xf7};

uchar Starget[]={0xc0,0xc0,0xc0,0xc0,0xff};
			 //  right				  left
uchar Temp_Starget[]={0xc0,0xc0,0xc0,0xc0};

uchar st[]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9};//电机数组
char count=0, count1=0, count2=0, num=0, change=0 ,KEYNum;
bit tempt=0,temps=0, tempz=0,b = 0;
uchar w=0, c=0, keychoice =0, digit, ten, hundred, thousand, t=0, information;
uint Velocity = 0 ;
sbit buzzer = P3^7;

//延时函数
void delay(unsigned int time)
{
  unsigned char j;
  while(time--)
  for(j=0;j<=121;j++);
}		    

//按键选择
void keycho()
{
  uchar temp,k;
  char key_scan[]={0xef,0xdf,0xbf,0x7f}; //4*4键位，按列扫描
  KEYNum = 16; //空键值
  for(k=0;k<4;k++)
  {
	 P1 = key_scan[k];
	 temp = P1 & 0x0f;  //判断是否有按键按下
	 if(temp != 0x0f)
	 {
		delay(10);  //消抖
	    temp = P1 & 0x0f;
		if(temp != 0x0f)
		{
		   temp = P1;
		   buzzer = 0;
           switch(temp)	
				{
					case 0xee:
						KEYNum = 7;	   //第1行第1列
						break;
					case 0xde:
						KEYNum = 8;	   //第1行第2列
						break;
					case 0xbe:
						KEYNum = 9;	   //第1行第3列
						break;
					case 0x7e:
						KEYNum = 10;	//第1行第4列
						break;
					case 0xed:
						KEYNum = 4;	   //第2行第1列
						break;
					case 0xdd:
						KEYNum = 5;	   //第2行第2列
						break;
					case 0xbd:
						KEYNum = 6;	   //第2行第3列
						break;
					case 0x7d:
						KEYNum = 11;	//第2行第4列
						break;
					case 0xeb:
						KEYNum = 1;	   //第3行第1列
						break;
					case 0xdb:
						KEYNum = 2;	   //第3行第2列
						break;
					case 0xbb:
						KEYNum = 3;   //第3行第3列
						break;
					case 0x7b:
						KEYNum = 12;   //第3行第4列
						break;	
					case 0xe7:
						KEYNum = 13;   //第4行第1列
						break;
					case 0xd7:
						KEYNum = 0;   //第4行第2列
						break;
					case 0xb7:
						KEYNum = 14;   //第4行第3列
						break;
					case 0x77:
						KEYNum = 15;   //第4行第4列
						break;			
		   }
		}
	   while(temp != 0x0f)		 //等待按键释放
			{
				temp = P1 & 0x0f;
				buzzer = !buzzer;	//蜂鸣器响						  
			}
	   buzzer = 1;
	 }
  }
}			   

//按键功能键
void function()
{
	 char check,check1,tempdis;
	 if(KEYNum == 10){	//电机启停
	  tempt = !tempt;
	 }

	  if(KEYNum == 11 && tempt==1){  //电机正/反转
			tempz = !tempz;
		 if (tempz == 1){
			 Starget[4] = 0xbf; 
		 }
		 else if (tempz == 0){
			 Starget[4] = 0xff; 
		 }
			  }
	  if(KEYNum == 15){  //速度设置开关
			 temps = !temps;
			 Velocity = digit + ten*10 + hundred*100 + thousand*1000; //转速计算
		 if(Velocity == 0){
			 t = 0;
		 	 }
		 else{
			 t = 30000/(48*Velocity);
			 }

		 if (temps == 1){  //进入速度设置模式
			 Starget[0] = Starget[0]-0x80;  //光标位置初始化
			 for(tempdis=0;tempdis<4;tempdis++){
			 	Temp_Starget[tempdis] = Starget[tempdis];  //将更改前的速度数值进行存储
			 }			   
		   }
		else if (temps == 0){  //确定速度并退出速度设置模式
			 for(check1=0;check1<4;check1++){			  //检测带小数点的数字并将其换成无小数点的数字
			   for(check=0;check<10;check++){
				 if (Starget[check1] == display_point[check])
					 Starget[check1] = display[check];
			   }
			 }
		}
	}
	  if(KEYNum == 12 && temps == 1){	   //速度设置取消按键
	  		 temps = 0;
			 for(tempdis=0;tempdis<4;tempdis++){			
			 	Starget[tempdis] = Temp_Starget[tempdis];	//将更改前的速度数值赋值回
			 }
			 for(check=0;check<10;check++){
				if (Starget[0] == display_point[check])
				Starget[0] = display[check];
			   }

	  }  
}

//电机转动/换向
void motor()
{                           
		if (tempz == 0){	 //电机正向旋转
	    	if(num==8){                                  
			  num=0;                                      
		    }
		  P3 = st[num];
		   num++;                              
		  }
		if (tempz == 1){	 //电机反向旋转
	    	if(num<0){                                  
			 num=7;
		 }
		  P3 = st[num];
		  num--;                               
		   }
}

//实时速度显示
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

//移位功能
void move()
{
	if(KEYNum == 13 && temps == 1){  //左移
	Starget[change] = Starget[change]+0x80;	   //小数点消去
	change++;								   //移位
		if (change >3 ){
	 	change=0;
		}
	Starget[change] = Starget[change]-0x80;	   //小数点添加
	}

	else if(KEYNum == 14 && temps == 1){	//右移
	Starget[change] = Starget[change]+0x80;	   //小数点消去
	change--;								   //移位
		if (change <0 ){
	 	change=3;
		}
	Starget[change] = Starget[change]-0x80;	   //小数点添加
	}	
}

//目标速度
void target()
{

	if (KEYNum < 10){
		 Starget[change] = display[KEYNum]-0x80;	//数值改变
		if (change == 0)
		  digit = KEYNum;						   //分别储存个、十、百、千位的数值
		else if (change == 1)
		  ten = KEYNum;
		else if (change == 2)
		  hundred = KEYNum;
		else if (change == 3)
		  thousand = KEYNum;
		 }
		  
		 P2 = wei[w];		//送位码
		 P0 = Starget[w];	//送段码
		 w++;
		 if(w>4){
		  	w=0;
		 }
}									  

void main()
{
  P1=0xff;	  //初始化
  P0=0xff;
  P2=0xff;
  P3=0xff;
  buzzer = 1;

  PCON = 0x00; //电源控制寄存器，SMOD等于0，波特率不加倍
  SCON = 0x50; //串口控制寄存器，串行口工作方式1，波特率由定时器T1决定
  TMOD=0x21;//T0定时方式2，T1定时方式1
  TH0=0xFC;	//晶振 
  TL0=0x18;
  TH1=0xfd;	//波特率9600
  TL1=0xfd;
  EA=1;
  ET0=ES=1;
  TR0=TR1=1;
  RI=0;
  TI=0;

  while(1)
  {
   keycho();
   if(KEYNum != 16)	   //当有按键按下后
   	function();		   //功能子程序
	if(KEYNum == 13 || KEYNum == 14)	 //当左右位移键按下后
	  move();	 //移位子程序
  }
}

//T0定时器 1ms
void Timer0() interrupt 1
{
    TH0 = 0xFC ; 
	TL0 = 0x18 ;

	if(tempt == 1){	//电机转动
		count++;
		if(count == t && t != 0){ 
		motor();
		count = 0;
		}
	}
	
	if(temps == 0) {  //显示实时速度
		count1++;
       if(count1 == 1){
		leddisplay();
		count1 = 0;
		}
	} 
		
	if(temps == 1) {  //显示目标速度
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

void receive() interrupt 4{	   //串口通信中断
   if(RI==1)
   {
   		information=SBUF;	   //接收串口信息
		RI=0;
		buzzer=0;
		switch(information)
			{
					case 0xee:
						KEYNum = 7;	   //第1行第1列
						break;
					case 0xde:
						KEYNum = 8;	   //第1行第2列
						break;
					case 0xbe:
						KEYNum = 9;	   //第1行第3列
						break;
					case 0x7e:
						KEYNum = 10;	//第1行第4列
						break;
					case 0xed:
						KEYNum = 4;	   //第2行第1列
						break;
					case 0xdd:
						KEYNum = 5;	   //第2行第2列
						break;
					case 0xbd:
						KEYNum = 6;	   //第2行第3列
						break;
					case 0x7d:
						KEYNum = 11;	//第2行第4列
						break;
					case 0xeb:
						KEYNum = 1;	   //第3行第1列
						break;
					case 0xdb:
						KEYNum = 2;	   //第3行第2列
						break;
					case 0xbb:
						KEYNum = 3;   //第3行第3列
						break;
					case 0x7b:
						KEYNum = 12;   //第3行第4列
						break;	
					case 0xe7:
						KEYNum = 13;   //第4行第1列
						break;
					case 0xd7:
						KEYNum = 0;   //第4行第2列
						break;
					case 0xb7:
						KEYNum = 14;   //第4行第3列
						break;
					case 0x77:
						KEYNum = 15;   //第4行第4列
						break;	
					
		   }
   }
}
