#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>



ThreadLvgl threadLvgl(30);

int main()
{
    AnalogIn micro(A0);
    DigitalIn gate(D2);
    while(1)
    {
        float microValue = micro.read();
        if (microValue>0.7 && gate==1){
            printf("Le sons est FORT\n");
            printf("%.2f\n", static_cast<double>(microValue));
        }

        else if (microValue>0.2 && microValue<0.7 && gate==1){
            printf("Le sons est CORRECT\n");
            printf("%.2f\n", static_cast<double>(microValue));
        }
        else if (microValue<0.2 && gate==1){
            printf("Le sons est FAIBLE\n");
            printf("%.2f\n", static_cast<double>(microValue));
        }

        ThisThread::sleep_for(2ms);

    }
}