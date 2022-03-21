#include <stdio.h>	//printf()
#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include <string.h>
#include "DEV_Config.h"
#include <time.h>
#include <sys/time.h>
#include "waveshare_PCF85063.h"

void PCF85063_Handler(int signo)
{
	// System Exit
	printf("\r\nHandler:exit\r\n");
	DEV_ModuleExit();

	exit(0);
}

const char *SET_RTC_TIME_PARAMETER = "setrtc";
const char *SET_SYSTEM_TIME = "setsystem";

int main(int argc, char **argv)
{
	int set_rtc = 0;
	int set_system = 0;
	int count = 0;

	for (int i = 1; i < argc; ++i)
	{
		if (strncmp(argv[i], SET_RTC_TIME_PARAMETER, strlen(SET_RTC_TIME_PARAMETER)) == 0)
		{
			set_rtc = 1;
			printf("Setting RTC time\n\n");
		}
		if (strncmp(argv[i], SET_SYSTEM_TIME, strlen(SET_SYSTEM_TIME)) == 0)
		{
			set_system = 1;
			printf("Setting System time\n\n");
		}
	}

	if (set_system && set_rtc)
	{
		set_system = 0;
		set_system = 0;
	}

	// Exception handling:ctrl + c
	signal(SIGINT, PCF85063_Handler);
	DEV_ModuleInit();
	DEV_I2C_Init(PCF85063_ADDRESS);

	time_t T = time(NULL);
	struct tm tm = *localtime(&T);

	printf("***System Date is: %02d/%02d/%04d***\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	printf("***System Time is: %02d:%02d:%02d***\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

	if (set_rtc)
	{
		PCF85063_init();
		PCF85063_SetTime_YMD(tm.tm_year - 100, tm.tm_mon + 1, tm.tm_mday);
		PCF85063_SetTime_HMS(tm.tm_hour, tm.tm_min, tm.tm_sec);
	}

	if (set_system)
	{
		Time_data T;
		T = PCF85063_GetTime();

		struct tm time = {0};
		time.tm_year = T.years + 100;
		time.tm_mon = T.months - 1;
		time.tm_mday = T.days;
		time.tm_hour = T.hours;
		time.tm_min = T.minutes;
		time.tm_sec = T.seconds;

		time_t systemTime = mktime(&time);

		if (systemTime != (time_t)-1)
		{

			struct timeval timeVal = {0};
			timeVal.tv_sec = systemTime;
			int result = settimeofday(&timeVal, NULL);
			if (result < 0)
			{
				printf("Could not set time. Are you running as root?\n");
			}
		}
	}

	while (1)
	{
		Time_data T;
		T = PCF85063_GetTime();
		printf("%d-%02d-%02d %02d:%02d:%02d\r\n", 2000 + T.years, T.months, T.days, T.hours, T.minutes, T.seconds);
		count += 1;
		DEV_Delay_ms(1000);
		if (count > 6)
			break;
	}

	// System Exit
	DEV_ModuleExit();
	return 0;
}
