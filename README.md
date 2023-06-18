LAVAL Thibault LP MECSE SESAM

Projet interfaçage, réalisé une interface à l'aide de la carte MBED F746 et de la library LVGL.h

Matériels utilsés :
- carte MBED F746
- capteur sonore SEN12642
- bandeau LED Pololu 30 LEDs SK6812



Documentation LVGL version 8.3

<https://docs.lvgl.io/8.3/index.html>

Pour compiler les démos, il faut modifier le fichier

.pio/libdeps/lvgl/library.json

```
{
	"name": "lvgl",
	"version": "8.3.4",
	"keywords": "graphics, gui, embedded, tft, lvgl",
	"description": "Graphics library to create embedded GUI with easy-to-use graphical elements, beautiful visual effects and low memory footprint. It offers anti-aliasing, opacity, and animations using only one frame buffer.",
	"repository": {
		"type": "git",
		"url": "https://github.com/lvgl/lvgl.git"
	},
	"build": {
		"includeDir": ".",
		"srcFilter": "+<../demos>,+<./>"
	},
	"license": "MIT",
	"homepage": "https://lvgl.io",
	"frameworks": "*",
	"platforms": "*"
}
```

Projet pour vscode et platformio

<https://github.com/profge2/mbedF746>

<https://profge2.iut-cachan.u-psud.fr/git/LP_Mecse/mbedF746.git>

