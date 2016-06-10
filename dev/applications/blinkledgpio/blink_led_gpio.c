#include <stdio.h>
#include "xgpiops.h"

#define	LED_R	54
#define	LED_G	55
#define	LED_B	56
#define	LED_ON	0
#define	LED_OFF	1

//void print(char *str);

static void delay(int dly)
{
	int i, j;
	for (i = 0; i < dly; i++) {
		for (j = 0; j < 0xffff; j++) {
			;
		}
	}
}

int main(int argc, char* argv[])
{
	int Status;
	XGpioPs_Config *ConfigPtr;
	XGpioPs Gpio;	/* The driver instance for GPIO Device. */


  print("gpio mio test\n\r");

	/*
	 * Initialize the GPIO driver.
	 */
	ConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		print("cfg init err\n");
		return XST_FAILURE;
	}

	// LED gpio setting
	XGpioPs_SetDirectionPin(&Gpio, LED_R, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, LED_R, 1);
	XGpioPs_SetDirectionPin(&Gpio, LED_G, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, LED_G, 1);
	XGpioPs_SetDirectionPin(&Gpio, LED_B, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, LED_B, 1);

    while (1) {
    	XGpioPs_WritePin(&Gpio, LED_R, LED_ON);
    	delay(1000);
    	XGpioPs_WritePin(&Gpio, LED_G, LED_ON);
    	delay(1000);
    	XGpioPs_WritePin(&Gpio, LED_B, LED_ON);
    	delay(1000);
    	XGpioPs_WritePin(&Gpio, LED_R, LED_OFF);
    	delay(1000);
    	XGpioPs_WritePin(&Gpio, LED_G, LED_OFF);
    	delay(1000);
    	XGpioPs_WritePin(&Gpio, LED_B, LED_OFF);
    	delay(1000);
    }
}
