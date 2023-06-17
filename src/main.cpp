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
lv_obj_t * slider;
int valSlide;
int choix = 0;
float microPourcentage;
int NUM_LEDS = 30;

lv_obj_t * sw;

static void slider_event_cb(lv_event_t * e);
static lv_obj_t * slider_label;

//Affiche le texte defillant
void texte (void) {

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "Le projet de THIBAULT !!!");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);
}

void texte2 (void){
    lv_obj_t * label3 = lv_label_create(lv_scr_act());
    lv_obj_set_width(label3, 150);
    lv_label_set_text(label3, "Sonometre");
    lv_obj_align(label3, LV_ALIGN_CENTER, 0, 20);

}



//Affiche la colorwheel
void colorwheel(void) {
    
    cw = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_set_size(cw, 180, 180);
    lv_obj_align(cw, LV_ALIGN_CENTER, 0,0);
}



//Slider controle LED
void slider_1(void)
{
    /*Create a slider in the center of the display*/
    slider = lv_slider_create(lv_scr_act());
    lv_obj_align(bar, LV_ALIGN_RIGHT_MID, 10,20);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*Create a label below the slider*/
    slider_label = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label, "0%");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 10, 10);
}

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 10, 10);
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


static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if (choix == 0) {
            choix = 1;
        } else {
            choix = 0;
        }
    }
}

void switch_1(void)
{
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);


    sw = lv_switch_create(lv_scr_act());
    lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);
    
    lv_obj_add_flag(sw, LV_OBJ_FLAG_EVENT_BUBBLE);

}


//Converti la couleur de la colorwheel en couleur RGB
void convertir_couleur(lv_color32_t couleur, uint8_t* rouge, uint8_t* vert, uint8_t* bleu) {
    *rouge = couleur.ch.red;
    *vert = couleur.ch.green;
    *bleu = couleur.ch.blue;
}

void allumerLed(float valSlide, int rougeMax, int vertMax, int bleuMax) {
    int index;
    int ledRouge, ledVert, ledBleu;

    for (index = 0; index <= NUM_LEDS; index++) {
        if (valSlide > (index - 1) * 100 / NUM_LEDS) {
            ledRouge = (valSlide - (index - 1) * 100 / NUM_LEDS) * rougeMax / 100;
            ledVert = (valSlide - (index - 1) * 100 / NUM_LEDS) * vertMax / 100;
            ledBleu = (valSlide - (index - 1) * 100 / NUM_LEDS) * bleuMax / 100;
            threadLeds.setLed(index, ledRouge, ledVert, ledBleu);
        } else {
            threadLeds.setLed(index, 0, 0, 0);
        }
    }
}

void sonometre(float microPourcentage) {
    int index;
    int ledRouge, ledVert, ledBleu;

    for (index = 0; index < NUM_LEDS; index++) {
        float ratio = static_cast<float>(index) / (NUM_LEDS - 1);  // Ratio d'interpolation entre le vert et le rouge
        
        ledRouge = static_cast<int>(ratio * 255);
        ledVert = static_cast<int>((1 - ratio) * 255);
        ledBleu = 0;  // Pas de composante bleue

        if (microPourcentage > (index * 100 / NUM_LEDS)) {
            ledRouge = static_cast<int>(ledRouge * microPourcentage / 100);
            ledVert = static_cast<int>(ledVert * microPourcentage / 100);
        } else {
            ledRouge = 0;
            ledVert = 0;
        }

        threadLeds.setLed(index, ledRouge, ledVert, ledBleu);
    }
}





int main() {

    AnalogIn micro(A0);
    DigitalIn gate(D2);

    int n = 0;

    threadLvgl.lock();
    texte();
    texte2();
    colorwheel();
    switch_1();
    bar_3();
    slider_1();
    
    threadLvgl.unlock();

    while(1) {

        float microValue = micro.read();
        microPourcentage = (microValue*105.0)+22;
        set_temp(bar,microPourcentage);
        

        threadLvgl.lock();

        lv_bar_set_value(bar, microPourcentage, LV_ANIM_OFF);
        couleur = lv_colorwheel_get_rgb(cw);
        convertir_couleur(couleur, &rouge, &vert, &bleu);

        valSlide = (int)lv_slider_get_value(slider);
        threadLeds.setLed(n, rouge,vert,bleu);

        if (choix == 0){
            allumerLed(valSlide,rouge,vert,bleu);
        }
        else if (choix == 1){
            sonometre(microPourcentage);
        }
        
        
        
        threadLvgl.unlock();

       
        
        ThisThread::sleep_for(2ms);

    }
}