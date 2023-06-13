#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>
#include <lvgl.h>
#include <examples/lv_examples.h>

#include <threadLedsNeo.h>


ThreadLvgl threadLvgl(30);
ThreadLedsNeo threadLeds(A3, 5);

lv_obj_t * bar;
lv_obj_t * cw;
lv_color32_t couleur;
uint8_t rouge, vert, bleu;


void texte (void) {

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "Le projet de THIBAULT !!!");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);
}

void convertir_couleur(lv_color32_t couleur, uint8_t* rouge, uint8_t* vert, uint8_t* bleu) {
    *rouge = couleur.ch.red;
    *vert = couleur.ch.green;
    *bleu = couleur.ch.blue;
}

void colorwheel(void) {
    
    cw = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_set_size(cw, 245, 245);
    lv_obj_align(cw, LV_ALIGN_CENTER, 0,0);
}

static void set_temp(lv_obj_t * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

/**
 * A temperature meter example
 */
void bar_3(void)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    bar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 20, 200);
    lv_obj_align(bar, LV_ALIGN_LEFT_MID, 10,20);
    lv_bar_set_range(bar, -10, 100);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, 0);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_playback_time(&a, 3000);
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, -10, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void allumerLed(float microPourcentage, int rougeMax, int vertMax, int bleuMax) {
    int index;
    int ledRouge, ledVert, ledBleu;
    int NUM_LEDS = 30;

    for (index = 1; index <= NUM_LEDS; index++) {
        if (microPourcentage > (index - 1) * 100 / NUM_LEDS) {
            ledRouge = (microPourcentage - (index - 1) * 100 / NUM_LEDS) * rougeMax / 100;
            ledVert = (microPourcentage - (index - 1) * 100 / NUM_LEDS) * vertMax / 100;
            ledBleu = (microPourcentage - (index - 1) * 100 / NUM_LEDS) * bleuMax / 100;
            threadLeds.setLed(index, ledRouge, ledVert, ledBleu);
        } else {
            threadLeds.setLed(index, 0, 0, 0);
        }
    }
}



int main() {

    AnalogIn micro(A0);
    DigitalIn gate(D2);

    int n = 0;
    

    float microPourcentage = 0.0;

    threadLvgl.lock();
    texte();
    colorwheel();
    
    bar_3();
    threadLvgl.unlock();

    


    while(1) {

        float microValue = micro.read();
        microPourcentage = (microValue*100.0)+10;
        set_temp(bar,microPourcentage);



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

            threadLeds.setLed(n, rouge,vert,bleu);
        }
        

        threadLvgl.lock();

        lv_bar_set_value(bar, microPourcentage, LV_ANIM_OFF);
        couleur = lv_colorwheel_get_rgb(cw);
        convertir_couleur(couleur, &rouge, &vert, &bleu);
        
        threadLvgl.unlock();
        allumerLed(microPourcentage,rouge,vert,bleu);


        ThisThread::sleep_for(2ms);

    }
}