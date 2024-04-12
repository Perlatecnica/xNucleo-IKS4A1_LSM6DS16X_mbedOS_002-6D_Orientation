/*
MIT License

Copyright (c) [2024] 
Organization: Perlatecnica APS ETS
Author: Mauro D'Angelo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mbed.h"
#include "LSM6DSV16X.h"

LSM6DSV16X lsm6dsv16x(I2C_SDA, I2C_SCL); 
Serial pc(USBTX, USBRX);
DigitalOut led(LED1);

//Interrupts.
volatile int mems_event = 0;

char report[256];

void INT1Event_cb();
void sendOrientation();

InterruptIn intPin(D5);


// main() runs in its own thread in the OS
int main()
{
    pc.baud(115200);
    led = 0;

    pc.printf("LSM6DSV16X 6D Orientation\r\n");

    // Initialize LSM6DSV16X.
    lsm6dsv16x.begin();
    thread_sleep_for(1000);

    intPin.rise(&INT1Event_cb);

    // Initlialize components.
    lsm6dsv16x.begin();
    lsm6dsv16x.Enable_X();

    // Enable 6D Orientation.
    lsm6dsv16x.Enable_6D_Orientation(LSM6DSV16X_INT1_PIN);

    while (true) {
        if (mems_event) {
            mems_event = 0;
            LSM6DSV16X_Event_Status_t status;
            lsm6dsv16x.Get_X_Event_Status(&status);

            if (status.D6DOrientationStatus) {
            // Send 6D Orientation
            sendOrientation();

            // Led blinking.
            led = !led; 
            thread_sleep_for(100);
            led = !led; 
            
            }
        }
    }
}

void INT1Event_cb()
{
  mems_event = 1;
}

void sendOrientation()
{
  uint8_t xl = 0;
  uint8_t xh = 0;
  uint8_t yl = 0;
  uint8_t yh = 0;
  uint8_t zl = 0;
  uint8_t zh = 0;

  lsm6dsv16x.Get_6D_Orientation_XL(&xl);
  lsm6dsv16x.Get_6D_Orientation_XH(&xh);
  lsm6dsv16x.Get_6D_Orientation_YL(&yl);
  lsm6dsv16x.Get_6D_Orientation_YH(&yh);
  lsm6dsv16x.Get_6D_Orientation_ZL(&zl);
  lsm6dsv16x.Get_6D_Orientation_ZH(&zh);

  if (xl == 0 && yl == 0 && zl == 0 && xh == 0 && yh == 1 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |  *             | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 1 && yl == 0 && zl == 0 && xh == 0 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |             *  | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 0 && zl == 0 && xh == 1 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |  *             | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 1 && zl == 0 && xh == 0 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |             *  | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 0 && zl == 0 && xh == 0 && yh == 0 && zh == 1) {
    sprintf(report, "\r\n  __*_____________  " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 0 && zl == 1 && xh == 0 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |________________| " \
            "\r\n    *               \r\n");
  }

  else {
    sprintf(report, "None of the 6D orientation axes is set in accelerometer.\r\n");
  }

  pc.printf("%s",report);
}
