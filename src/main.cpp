#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>

#include "PololuLedStrip.h"

ThreadLvgl threadLvgl(30);

// Déclaration de la broche de contrôle des LED
DigitalOut ledPin(D2);

// Déclaration du nombre de LED dans le bandeau
const uint16_t numLEDs = 30;

// Déclaration de l'objet PololuLedStrip
PololuLedStrip ledStrip(D2, numLEDs);

int main() {
    while (1) {
        // Allumer toutes les LED en rouge
        for (uint16_t i = 0; i < numLEDs; i++) {
            ledStrip.setPixelColor(i, 255, 0, 0);
        }

        // Envoyer les couleurs au bandeau de LED
        ledStrip.show();

        // Attendre pendant 1 seconde
        ThisThread::sleep_for(1s);
    }
}