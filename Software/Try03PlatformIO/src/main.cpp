#include "mbed.h"
#include "DS1307.h"

/*RTC*/
#define I2C1_SDA PB_9
#define I2C1_SCL PB_8

DigitalOut myled(LED1);

RtcDs1307 *rtc;

int main() {
	
	/*init RTC, get time and date for filename*/
    I2C i2c1(I2C1_SDA, I2C1_SCL);
    i2c1.frequency(100000);
    rtc = new RtcDs1307(i2c1);
	
    while(1) {
        myled = 1;
        wait(1);
        myled = 0;
        wait(1);
    }
}