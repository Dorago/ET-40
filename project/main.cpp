#include "mbed.h"

//SPI spi1(xp5, xp6, xp7); //0-9, 0-8, 2-11
//SPI spi2(xp37, xp14, xp13);//2-3, 2-2, 2-1
//SPISlave spis1(xp5, xp6, xp7, xp8); //0-9, 0-8, 2-11, 0-2
//SPI spi2(xp37, xp14, xp13, xp12);//2-3, 2-2, 2-1, 2-0
//I2C i2c(xp42, xp41);//0-5, 0-4
//I2CSlave i2c(xp42, xp41);//0-5, 0-4

/*****注意書き*****/
//servoの制御は20[msec]のPWMで制御している
//サーボの角度制御に必要なパルスは約0.5~2.5[ms]
//その為，0.125~0.025内で変える必要がある
//servo変数は内部の固有の変数であるため，+=等が使えない
//外部からその数値が知りたい場合は別に変数を用意して代入する必要がある

/*****各ポートの割り当て*****/
Serial pc(xp9,xp10);

BusOut led(xp7,xp8,xp11,xp43);
BusIn sw(xp29,xp30);

DigitalIn digital[]={xp21,xp22,xp23,xp25,xp26,xp27};
AnalogIn analog[]={xp15,xp16,xp17,xp18,xp20,xp38};

DigitalOut mot_a1(xp31);
DigitalOut mot_a2(xp32);
DigitalOut mot_apwm(xp33);

DigitalOut mot_b1(xp34);
DigitalOut mot_b2(xp35);
DigitalOut mot_bpwm(xp36);

PwmOut servo[]={xp5,xp6,xp39,xp40};
/************************/

Ticker motor_control;
int duty = 0;			//回転速度(Duty比)		[設定値：0(最小)~100(最大)]
int dir = 0;			//回転方向						[0:正転	1:逆転	2:ブレーキ]
int motor_id = 0;	//回るモータのポート			[0:aのみ回転	1:bのみ回転	2:ab両方回転	]

void motor(void)
{
    const int duty_max = 100;	//Duty比の最大値
    static int cnt = 0;				//
    
    int mot_1, mot_2, mot_pwm;
    
    if(cnt > duty_max)		//出力値にリミットをかける
    {
        cnt = 0;
        
        if(duty > duty_max) duty = duty_max;
        if(duty < 0) duty = 0;
    }
    
    if(cnt < duty)				//モータを回転させる場合
    {
        mot_pwm = 1;				//PWM出力許可
        
        if(dir == 0)				//CW(正転)
        {
            mot_1 = 1;
            mot_2 = 0;
        }
        else if(dir == 1)		//CCW(逆転)
        {
            mot_1 = 0;
            mot_2 = 1;
        }
        else if(dir == 2)		//ブレーキモード
        {
            mot_1 = 1;
            mot_2 = 1;
        }
    }
    else			//モータを停止させる
    {
        mot_pwm = 0;		//PWM出力停止
        
        mot_1 = 0;
        mot_2 = 0;
    }
    
    if(motor_id == 0)					//aポートのみモータ回転
    {
        mot_a1 = mot_1;
        mot_a2 = mot_2;
        mot_apwm = mot_pwm;
    }
    else if(motor_id == 1)		//bポートのみモータ回転
    {
        mot_b1 = mot_1;
        mot_b2 = mot_2;
        mot_bpwm = mot_pwm;
    }
    else if(motor_id == 2)		//ab両ポートのモータ回転
    {
        mot_a1 = mot_1;
        mot_a2 = mot_2;
        mot_apwm = mot_pwm;
			
        mot_b1 = mot_1;
        mot_b2 = mot_2;
        mot_bpwm = mot_pwm;
    }
    
    cnt++;
}

void pc_rx(void)
{
    switch(pc.getc())
    {
        case '0':
            duty = 0;
            break;
        case '1':
            duty = 20;
            break;
        case '2':
            duty = 40;
            break;
        case '3':
            duty = 60;
            break;
        case '4':
            duty = 80;
            break;
        case '5':
            duty = 100;
            break;
        case 'a':
            duty = 0;
            dir = 2;
            motor_id = 0;
            break;
        case 'b':
            duty = 0;
            dir = 2;
            motor_id = 1;
            break;
        case 'c':
            duty = 0;
            dir = 2;
            motor_id = 2;
        case 'l':
            dir = 0;
            break;
        case 'r':
            dir = 1;
            break;
        case 's':
            dir = 2;
            break;
        default:
            break;
    }
}

/*****基本的にはここから下を書き換える*****/

/***** ボタンを押すとサーボが少しずつ動くプログラム *****/

int main() {
		pc.baud(9600);														//通信速度の設定　default:9600bps　(これ以上あげるには別の設定が必要？)
    pc.attach(pc_rx, Serial::RxIrq);				//シリアル通信の受信割り込み許可
    pc.printf("mbed start!!!\n\r");					
    
    motor_control.attach_us(&motor, 100);		//100[usec]毎にモータ制御の割り込みをする
    
    sw.mode(PullUp);												//swピンを全てプルアップ状態にする
	
		servo[0] = 0.019;			//最小値
		servo[1] = 0.019;			//最小値
		servo[2] = 0.019;			//最小値
		servo[3] = 0.019;			//最小値
	
		double s[4]={0,0,0,0}; 
	
    led = 0;																//一番左側のLED(青)のみ点灯させる
		
    while(1) {															//無限Loop
 /*       for(int i=0 ; i<6 ; i++)						//各ポートのanalog値をPCへ送信する
        {
            pc.printf("%4.2f ", analog[i].read());
        }
        pc.printf("\n\r");									//改行
				
        for(int i=0 ; i<6 ; i++)						//各ポートのdigital値をPcへ送信する
        {
            pc.printf("%d ", digital[i].read());
        }
        pc.printf("\n\n\r");								//2行改行
 */       
        switch(sw)													//swの押し方で動作を変更する
        {
            case 0:			//両方のswを押したとき
								led = 4;
//							servo[0] = 0.05;
//							servo[1] = 0.0;
//							servo[2] = 0.0;
//							servo[3] = 0.0;
//							pc.printf("sw both\n\r");
								break;
						case 1:			//左側のswを押したとき
                led = 8;
								servo[0] = servo[0] + 0.001;
								servo[1] = servo[1] + 0.001;
								servo[2] = servo[2] + 0.001;
								servo[3] = servo[3] + 0.001;
								s[0] = servo[0];
								s[1] = servo[1];
								s[2] = servo[2];
								s[3] = servo[3];
								pc.printf("%.3lf\n\r ", s[0]);
								pc.printf("%.3lf\n\r ", s[1]);
								pc.printf("%.3lf\n\r ", s[2]);
								pc.printf("%.3lf\n\r ", s[3]);
								pc.printf("sw left\n\r");
								break;
						case 2:			//右側のswを押したとき
                led = 2;
								servo[0] = servo[0] - 0.001;
								servo[1] = servo[1] - 0.001;
								servo[2] = servo[2] - 0.001;
								servo[3] = servo[3] - 0.001;
								s[0] = servo[0];
								s[1] = servo[1];
								s[2] = servo[2];
								s[3] = servo[3];
								pc.printf("%.3lf\n\r ", s[0]);
								pc.printf("%.3lf\n\r ", s[1]);
								pc.printf("%.3lf\n\r ", s[2]);
								pc.printf("%.3lf\n\r ", s[3]);
								pc.printf("sw right\n\r");
								break;
            case 3:			//両方のswを押していないとき
                led = 1;
//							servo[0] = 0.0;
//							servo[1] = 0.0;
//							servo[2] = 0.0;
//							servo[3] = 0.05;
//							pc.printf("sw off\n\r");
								break;
				}
				wait(0.05);													//0.1[sec]待つ
    }
}

