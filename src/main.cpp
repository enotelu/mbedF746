/*
LAVAL Thibault LP MECSE SESAM

Projet interfaçage, réalisé une interface à l'aide de la carte MBED F746 et de la library LVGL.h

Matériels utilsés :
- carte MBED F746
- capteur sonore SEN12642
- bandeau LED Pololu 30 LEDs SK6812
*/

//----------------------------------------o

#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>
#include <lvgl.h>
#include <examples/lv_examples.h>

#include <threadLedsNeo.h>  //permet d'utiliser les LED Pololu (SK6812)

//----------------------------------------o

ThreadLvgl threadLvgl(30);
ThreadLedsNeo threadLeds(A3, 5);

lv_obj_t * label;
lv_obj_t * bar;
lv_obj_t * cw;
lv_obj_t * sw1;
lv_obj_t * sw2;
lv_obj_t * slider;

lv_color32_t couleur;
uint8_t rouge, vert, bleu;

int valSlide;
int choix1 = 0;
int choix2 = 0;
float microPourcentage;
int NUM_LEDS = 30;


static void slider_event_cb(lv_event_t * e);
static lv_obj_t * slider_label;

// Déclaration des fonctions
static void event_handler(lv_event_t * e);
static void event_handler2(lv_event_t * e);
void convertir_couleur(lv_color32_t couleur, uint8_t* rouge, uint8_t* vert, uint8_t* bleu);
void allumerLed(float valSlide, int rougeMax, int vertMax, int bleuMax);
void intensiteLed(float valSlide, int rougeMax, int vertMax, int bleuMax);
void sonometre(float microPourcentage);

//----------------------------------------o

//FONCTIONS AFFICHAGE INTERFACE

//Affichage du titre
void texte (void) {
    label = lv_label_create(lv_scr_act());
    lv_obj_set_width(label, 200);
    lv_label_set_text(label, "Mode TEST / SONOMETRE :");
    lv_obj_align(label, LV_TEXT_ALIGN_RIGHT, -100, 10); 

}

//Affiche la colorwheel
void colorwheel(void) {
    
    cw = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_set_size(cw, 180, 180);

    lv_obj_align(cw, LV_TEXT_ALIGN_LEFT, 20, 20); 
}

//Slider permettant le controle LED
void slider_1(void) {
    /*Create a slider in the center of the display*/
    slider = lv_slider_create(lv_scr_act());

    lv_obj_align(slider, LV_TEXT_ALIGN_LEFT, 15, 230);

    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*Create a label below the slider*/
    slider_label = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label, "0%");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 10, 10);
}

//affiche la valeur du slider
static void slider_event_cb(lv_event_t * e) {
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

//récupere la valeur en entrée du capteur sonore
static void set_temp(lv_obj_t * bar, int32_t temp) {
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

//Affichage de la bar du sonometre
void bar_1(void) {
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    bar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 20, 200);

    lv_obj_align(bar, LV_TEXT_ALIGN_RIGHT, -10, 70);

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

// Fonction de rappel pour l'événement de changement d'état du switch 1
static void event_handler(lv_event_t * e) {
    lv_event_code_t code1 = lv_event_get_code(e);
    if (code1 == LV_EVENT_VALUE_CHANGED) {
        if (choix1 == 0) {
            choix1 = 1;  // Si le choix actuel est 0, le change à 1
        } else {
            choix1 = 0;  // Sinon, le change à 0
        }
    }
}

// Fonction de rappel pour l'événement de changement d'état du switch 2
static void event_handler2(lv_event_t * e) {
    lv_event_code_t code2 = lv_event_get_code(e);
    if (code2 == LV_EVENT_VALUE_CHANGED) {
        if (choix2 == 0) {
            choix2 = 1;  // Si le choix2 actuel est 0, le change à 1
        } else {
            choix2 = 0;  // Sinon, le change à 0
        }
    }
}

//Switch permettant de choisir entre le mode TEST et SONOMETRE
void switch_1(void){
    sw1 = lv_switch_create(lv_scr_act());
    lv_obj_align(sw1, LV_TEXT_ALIGN_RIGHT, -180, 40);
    lv_obj_add_event_cb(sw1, event_handler, LV_EVENT_ALL, NULL);
    
    lv_obj_add_flag(sw1, LV_OBJ_FLAG_EVENT_BUBBLE);

}

//Switch permettant de choisir entre les deux mode TEST (intensité et affichage LED)
void switch_2(void){
    sw2 = lv_switch_create(lv_scr_act());
    lv_obj_align(sw2, LV_TEXT_ALIGN_RIGHT, -180, 100);
    lv_obj_add_event_cb(sw2, event_handler2, LV_EVENT_ALL, NULL);
    
    lv_obj_add_flag(sw2, LV_OBJ_FLAG_EVENT_BUBBLE);

}

//----------------------------------------o

// Fonction principale
int main() {

    AnalogIn micro(A0);  //valeur de "l'enveloppe" du capteur sonore
    DigitalIn gate(D2); //valeur 1 ou 0, 1 lorsque du son est détecté et 0 lorsqu'il n'y a pas de son

    threadLvgl.lock();

    texte();        //lancement de la fonction texte permettant de l'afficher
    colorwheel();   //lancement de la fonction colorwheel permettant de l'afficher
    switch_1();     //lancement de la fonction colorwheel permettant de l'afficher
    switch_2();     //lancement de la fonction colorwheel permettant de l'afficher
    bar_1();        //lancement de la fonction colorwheel permettant de l'afficher
    slider_1();     //lancement de la fonction colorwheel permettant de l'afficher
    
    threadLvgl.unlock();

      while (1) {
        // Lecture de la valeur analogique du microphone et calcul du pourcentage correspondant
        float microValue = micro.read();
        microPourcentage = (microValue * 105.0) + 22;
        set_temp(bar, microPourcentage);

        // Mise à jour de la barre de progression et de la couleur en fonction de la valeur du microphone et de la colorwheel
        lv_bar_set_value(bar, microPourcentage, LV_ANIM_OFF);
        lv_color_t couleur = lv_colorwheel_get_rgb(cw);     //recupere la valeur de couleur choisi sur la colorwheel
        convertir_couleur(couleur, &rouge, &vert, &bleu);   //renvoi la valeur de couleur vers la fonction convertir_couleur afin d'extraire le code RGB

        valSlide = (int)lv_slider_get_value(slider);        //recupere la valeur du slider dans valSlide

        // Allume ou contrôle l'intensité des LED en fonction des choix effectués (choix, choix2)
        if (choix1 == 0) {            //choix=0 --> mode TEST
            if (choix2 == 0) {
                allumerLed(valSlide, rouge, vert, bleu);
            } else if (choix2 == 1) {
                intensiteLed(valSlide, rouge, vert, bleu);
            }
        } else if (choix1 == 1) {    //choix=0 --> mode SONOMETRE
            sonometre(microPourcentage);
        }

        // Attente
        ThisThread::sleep_for(2ms);
    }

}

//----------------------------------------o

//DECLARATION DES FONCTIONS UTILE AUX LEDS


// Convertit la couleur de la colorwheel en couleur RGB
void convertir_couleur(lv_color32_t couleur, uint8_t* rouge, uint8_t* vert, uint8_t* bleu) {
    *rouge = couleur.ch.red;    // Extrémité rouge de la couleur
    *vert = couleur.ch.green;   // Extrémité verte de la couleur
    *bleu = couleur.ch.blue;    // Extrémité bleue de la couleur
}

// Allume les LED en fonction de la position du slider et de la couleur sélectionnée
void allumerLed(float valSlide, int rougeMax, int vertMax, int bleuMax) {
    int index;
    int ledRouge, ledVert, ledBleu;

    for (index = 0; index <= NUM_LEDS; index++) {
        if (valSlide > (index - 1) * 100 / NUM_LEDS) {
            // Calcul de la valeur de chaque composante de couleur en fonction de la position du slider
            ledRouge = (valSlide - (index - 1) * 100 / NUM_LEDS) * rougeMax / 100;
            ledVert = (valSlide - (index - 1) * 100 / NUM_LEDS) * vertMax / 100;
            ledBleu = (valSlide - (index - 1) * 100 / NUM_LEDS) * bleuMax / 100;
        } else {
            ledRouge = 0;
            ledVert = 0;
            ledBleu = 0;
        }

        // Allume la LED à l'index donné avec la couleur calculée
        threadLeds.setLed(index, ledRouge, ledVert, ledBleu);
    }
}

// Contrôle l'intensité des LED en fonction de la position du slider et de la couleur sélectionnée
void intensiteLed(float valSlide, int rougeMax, int vertMax, int bleuMax) {
    int index;
    float intensity = valSlide / 100.0;
    int ledRouge, ledVert, ledBleu;

    for (index = 0; index <= NUM_LEDS; index++) {
        // Calcul de la valeur de chaque composante de couleur en fonction de l'intensité et des valeurs maximales de couleur
        ledRouge = intensity * rougeMax;
        ledVert = intensity * vertMax;
        ledBleu = intensity * bleuMax;
        
        // Allume la LED à l'index donné avec la couleur calculée
        threadLeds.setLed(index, ledRouge, ledVert, ledBleu);
    }
}

// Sonomètre à partir de la valeur d'entrée du microphone
void sonometre(float microPourcentage) {
    int index;
    int ledRouge, ledVert, ledBleu;

    for (index = 0; index < NUM_LEDS; index++) {
        // Calcul du ratio d'interpolation entre le vert et le rouge
        float ratio = static_cast<float>(index) / (NUM_LEDS - 1);
        
        // Calcul des valeurs des composantes rouge, verte et bleue pour la LED actuelle
        ledRouge = static_cast<int>(ratio * 255);
        ledVert = static_cast<int>((1 - ratio) * 255);
        ledBleu = 0;  // Pas de composante bleue

        // Vérification si le pourcentage micro est supérieur au seuil actuel
        if (microPourcentage > (index * 100 / NUM_LEDS)) {
            // Mise à jour des valeurs des composantes rouge et verte en fonction du pourcentage micro
            ledRouge = static_cast<int>(ledRouge * microPourcentage / 100);
            ledVert = static_cast<int>(ledVert * microPourcentage / 100);
        } else {
            // Si le pourcentage micro est inférieur au seuil, la LED est éteinte (les composantes rouge et verte sont mises à 0)
            ledRouge = 0;
            ledVert = 0;
        }

        // Appel de la fonction setLed pour mettre à jour la LED actuelle avec les valeurs calculées
        threadLeds.setLed(index, ledRouge, ledVert, ledBleu);
    }
}

//----------------------------------------oend