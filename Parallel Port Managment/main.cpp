#include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "KeyPad"


LcdDisp Lcd;
// main() runs in its own thread in the OS
int main()
{
    Lcd.lcdInit();
    while (true) {
        
    }
}

