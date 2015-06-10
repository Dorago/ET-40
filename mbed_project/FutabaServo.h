#ifndef __FUTABASERVO_H
#define __FUTABASERVO_H

#include "mbed.h"

Serial uart(xp9, xp10);

#define MAX_ANGLE 150		//最大角度の定義
#define MIN_ANGLE -150		//最小角度の定義

void FutabaServo_init(int baudrate)
{
		uart.baud(baudrate);
		uart.format(8, Serial::None, 1); 
}

//全てのサーボのトルクをON/OFFする
void Torque_ALL( unsigned char MODE )
{
	const short		servo_no = 12;
	unsigned char	sendbuf[128];
	unsigned char	sum = 0;
	int				i;

	// パケット作成
	sendbuf[0]  = (unsigned char)0xFA;				// ヘッダー1
	sendbuf[1]  = (unsigned char)0xAF;				// ヘッダー2
	sendbuf[2]  = (unsigned char)0x00;				// サーボID
	sendbuf[3]  = (unsigned char)0x00;				// フラグ
	sendbuf[4]  = (unsigned char)0x24;				// アドレス
	sendbuf[5]  = (unsigned char)0x02;				// 長さ
	sendbuf[6]  = (unsigned char)servo_no;			// 個数

	for( i = 1; i <= servo_no; i++)
	{
		sendbuf[((i-1)*2)+7] = (unsigned char)i;
		sendbuf[((i-1)*2)+8] = (unsigned char)MODE;
	}

	// チェックサムの計算
	for( i = 2; i < servo_no*2+7; i++ )
	{
		sum ^= sendbuf[i];
	}

	sendbuf[servo_no*2+7] = (unsigned char)sum;								// チェックサム

	// 送信
	for( i = 0; i < (servo_no*2+8); i++)
	{
		uart.putc(sendbuf[i]);
	}
}

//全てのサーボを指定した角度に動かす
void SendAngle_ALL( double *Angle )
{
	const int		servo_no = 12;
	unsigned char	sendbuf[128];
	unsigned char	sum = 0;
	int				i;

	// パケット作成
	sendbuf[0]  = (unsigned char)0xFA;			// ヘッダー1
	sendbuf[1]  = (unsigned char)0xAF;			// ヘッダー2
	sendbuf[2]  = (unsigned char)0x00;			// サーボID
	sendbuf[3]  = (unsigned char)0x00;			// フラグ
	sendbuf[4]  = (unsigned char)0x1E;			// アドレス
	sendbuf[5]  = (unsigned char)0x03;			// 長さ
	sendbuf[6]  = (unsigned char)servo_no;		// 個数
	
	for( i = 1; i <= servo_no; i++)
	{
		if(*Angle > MAX_ANGLE) 			*Angle = MAX_ANGLE;
		else if(*Angle < MIN_ANGLE)		*Angle = MIN_ANGLE;

		sendbuf[7+(i-1)*3] = (unsigned char)i;
		sendbuf[8+(i-1)*3] = (unsigned char)(((short)(*Angle*10)) & 0x00FF);
		sendbuf[9+(i-1)*3] = (unsigned char)(((short)(*Angle*10) & 0xFF00) >> 8);
		Angle++;
	}

	// チェックサムの計算
	for( i = 2; i < servo_no*3+7; i++ )
	{
		sum ^= sendbuf[i];
	}

	sendbuf[servo_no*3+7] = (unsigned char)sum;								// チェックサム

	// 送信
	for( i = 0; i < servo_no*3+8; i++)
	{
		uart.putc(sendbuf[i]);
	}
}

//サーボ1つを指定した角度に動かす
void SendAngle( unsigned char SERVO_ID, double Angle )
{
	unsigned char	sendbuf[10];
	unsigned char	sum = 0;
	int				i;

	// パケット作成
	sendbuf[0]  = (unsigned char)0xFA;				// ヘッダー1
	sendbuf[1]  = (unsigned char)0xAF;				// ヘッダー2
	sendbuf[2]  = (unsigned char)SERVO_ID;			// サーボID
	sendbuf[3]  = (unsigned char)0x00;				// フラグ
	sendbuf[4]  = (unsigned char)0x1E;				// アドレス
	sendbuf[5]  = (unsigned char)0x02;				// 長さ
	sendbuf[6]  = (unsigned char)0x01;				// 個数

	if(Angle > MAX_ANGLE) 			Angle = MAX_ANGLE;
	else if(Angle < MIN_ANGLE)		Angle = MIN_ANGLE;

	sendbuf[7]  = (unsigned char)((short)(Angle*10) & 0x00FF);			//目標角度(下位)
	sendbuf[8]  = (unsigned char)(((short)(Angle*10) & 0xFF00) >> 8);	//目標角度(上位)

	// チェックサムの計算
	for(i = 2; i < 9; i++)
	{
		sum ^= sendbuf[i];
	}

	sendbuf[9] = (unsigned char)sum;								// チェックサム

	for(i = 0; i < 10; i++)
	{
		uart.putc(sendbuf[i]);
	}
}

//サーボ１つのトルクをON/OFFする
void Torque( unsigned char SERVO_ID,unsigned char MODE )
{
	unsigned char	sendbuf[9];
	unsigned char	sum = 0;
	int				i;

	// パケット作成
	sendbuf[0]  = (unsigned char)0xFA;				// ヘッダー1
	sendbuf[1]  = (unsigned char)0xAF;				// ヘッダー2
	sendbuf[2]  = (unsigned char)SERVO_ID;			// サーボID
	sendbuf[3]  = (unsigned char)0x00;				// フラグ
	sendbuf[4]  = (unsigned char)0x24;				// アドレス
	sendbuf[5]  = (unsigned char)0x01;				// 長さ
	sendbuf[6]  = (unsigned char)0x01;				// 個数
	sendbuf[7]  = (unsigned char)MODE;				// データ

	// チェックサムの計算
	for( i = 2; i < 8; i++)
	{
		sum ^= sendbuf[i];
	}
	
	sendbuf[8] = (unsigned char)sum;								// チェックサム

	//送信
	for( i = 0; i < 9; i++)
	{
		uart.putc(sendbuf[i]);
	}
}

// １つのサーボから現在位置を取得
double ReadAngle( unsigned char SERVO_ID )
{
	unsigned char	sendbuf[32];
	unsigned char	readbuf[128];
	unsigned char	sum = 0;
	short			Angle;
	int				i;

	// パケット作成
	sendbuf[0]  = (unsigned char)0xFA;				// ヘッダー1
	sendbuf[1]  = (unsigned char)0xAF;				// ヘッダー2
	sendbuf[2]  = (unsigned char)SERVO_ID;			// サーボID
	sendbuf[3]  = (unsigned char)0x0F;				// フラグ
	sendbuf[4]  = (unsigned char)0x2A;				// アドレス
	sendbuf[5]  = (unsigned char)0x02;				// データ数
	sendbuf[6]  = (unsigned char)0x00;				// 個数
	
	// チェックサムの計算
	for( i = 2; i < 7; i++)
	{
		sum ^= sendbuf[i];
	}
	
	sendbuf[7] = (unsigned char)sum;				// チェックサム

	for( i = 0; i < 8; i++)
	{
		uart.putc(sendbuf[i]);
	}

	for( i = 0; i < 10; i++)
	{
		readbuf[i] = uart.getc();
	}

	Angle = ((readbuf[8] << 8) & 0xFF00) | (readbuf[7] & 0x00FF);

	return (double)Angle/10;
}

#endif
