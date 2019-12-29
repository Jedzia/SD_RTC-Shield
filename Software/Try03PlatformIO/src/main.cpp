#include "mbed.h"
#include "DS1307.h"

/*RTC*/
#define I2C1_SDA PB_9
#define I2C1_SCL PB_8

DigitalOut myled(LED1);



Serial usart2(PA_2, PA_3);

int main() {

	int count = 42;
	
	usart2.baud(115200);
	printf("Fuck you\n");
	
	/*init RTC, get time and date for filename*/
    //I2C i2c1(I2C1_SDA, I2C1_SCL);
    I2C i2c1(PB_9, PB_8);
    i2c1.frequency(100000);
    
	RtcDs1307 rtc(i2c1);
    //rtc.startClock();
	
		 if (! rtc.isRunning()) {
    printf("RTC 1 is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

	
	
	
   DateTime dt = rtc.now();                     // check clock value
   if (dt.year() > 2090 || dt.year() < 2014) {
		printf("Setting up time\n");
        DateTime compiled(__DATE__, __TIME__);      // to set RT clock initially
        rtc.adjust(compiled);
        //DateTime dt = rtc.now();                // check again
    }
	//set_time(0);                        // set active clock
	
	 if (! rtc.isRunning()) {
    printf("RTC 2 is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
    while(1) {
        myled = 1;
        wait_us(500 * 1000);
        myled = 0;
        wait_us(500 * 1000);

		usart2.printf("Round %d\n", count);
		count++;
		
		DateTime dt = rtc.now();

		usart2.printf("%u.%u.%02u_%02u.%02u.%02u"
        ,dt.month(),dt.day(),dt.year()
        ,dt.hour(),dt.minute(),dt.second()
    );    
    }
}