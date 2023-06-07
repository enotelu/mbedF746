#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>
#include <lvgl.h>


ThreadLvgl threadLvgl(30);

void texte (void) {

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "Le projet de THIBAULT !!!");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);
}

void colorwheel(void) {
    lv_obj_t * cw;

    cw = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_set_size(cw, 200, 200);
    lv_obj_center(cw);
}


int main() {

    AnalogIn micro(A0);
    DigitalIn gate(D2);
    

    texte();
    colorwheel();


    while(1) {
        float microValue = micro.read();
        float microPourcentage = microValue*100;
        
        if (microValue>0.7 && gate==1){
            printf("Le son est FORT\n");
            printf("%.2f\n", static_cast<double>(microValue));
        }

        else if (microValue>0.1 && microValue<0.7 && gate==1){
            printf("Le son est CORRECT\n");
            printf("%.2f\n", static_cast<double>(microValue));
        }
        else if (microValue<0.1 && gate==1){
            printf("Le son est FAIBLE\n");
            printf("%.2f\n", static_cast<double>(microValue));
        }

        

        ThisThread::sleep_for(2ms);

    }
}