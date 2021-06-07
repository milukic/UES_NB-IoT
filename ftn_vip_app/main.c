#include <atmel_start.h>
#include <stdlib.h>
#include "ftn_vip_lib/usbUART.h"
#include "ftn_vip_lib/debugUART.h"
#include "ftn_vip_lib/timer_1ms.h"

//on-board sensors:
#include "ftn_vip_lib/SHTC3.h"
#include "ftn_vip_lib/bmp280.h"
#include "ftn_vip_lib/BH1750FVI.h"
#include "ftn_vip_lib/LIS2DE12.h"

void sensorTest(void)
{
	while (1)
	{
		char str[64];
		//SHTC3
		SHTC3_update();
		uint32_t shtc3_hum = SHTC3_raw2Percent() * 100;
		uint32_t shtc3_temp = SHTC3_raw2DegC() * 100;
		sprintf(str, "SHTC3 ->\tT = %d.%d C\tH = %d.%d \%\r\n", shtc3_temp / 100, shtc3_temp % 100, shtc3_hum / 100, shtc3_hum % 100);
		usbUARTputString(str);
		
		//BMP280
		int32_t bmp280_temp, bmp280_pres;
		bmp280_measure(&bmp280_temp, &bmp280_pres);
		sprintf(str, "BMP280 ->\tT = %d.%d C\tP = %d.%d mBar\r\n", bmp280_temp / 100, bmp280_temp % 100, bmp280_pres / 100, bmp280_pres % 100);
		usbUARTputString(str);
		
		//BH1750FVI
		uint32_t lum = BH1750FVI_GetLightIntensity();
		sprintf(str, "BH1750FVI ->\tL = %ld lux\r\n\r\n", lum);
		usbUARTputString(str);
		
		delay(1000);
	}
}

void accelTest(void)
{
	while (1)
	{
		accel_3axis accel;
		char str[32];
		LIS2DE12_getAccel(&accel);

		sprintf(str, "%d,%d,%d\r\n", accel.accel_X, accel.accel_Y, accel.accel_Z);
		usbUARTputString(str);
		
		delay(5);
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	debugUARTdriverInit();
	usbUARTinit();

	timer_1ms_init();
	setLEDfreq(FREQ_5HZ);
	enableLED();
	
	char str[64];
	sprintf(str, "--- FTN-VIP NB-IoT ---\r\n");
	usbUARTputString(str);
	setLEDfreq(FREQ_1HZ);
	enableLED();

	//init sensors
	SHTC3_begin();
	bmp280_init();
	BH1750FVI_begin();
	LIS2DE12_init();
	
	//accelTest();
	sensorTest();
	
	while (1);
}

