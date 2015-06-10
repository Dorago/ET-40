#include "mbed.h"
#include "FutabaServo.h"

#define ON 1

BusOut led(LED1,LED2,LED3,LED4);

int main()
{
	FutabaServo_init(115200);
	
	wait(1);
	
	Torque(1,ON);
	
	wait_ms(100);
	
	while(1)
	{
		SendAngle(1,100);
		wait(1);
		SendAngle(1,-100);
		wait(1);
	}
}
