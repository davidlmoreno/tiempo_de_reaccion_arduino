#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <Adafruit_NeoPixel.h>

unsigned long tiempoPasado;
unsigned long tiempoInicio;
unsigned long tiempo;
unsigned long dato;

LiquidCrystal_I2C lcd_1(0x27, 16, 2);
Adafruit_NeoPixel neo_pixel = Adafruit_NeoPixel(16, 5, NEO_GRB + NEO_KHZ800);

double math_random_int(int a, int b) {
	if (a > b) {
		int c = a;
		a = b;
		b = c;
	}
	return (double)random(a, b + 1);
}

void setup() {
	// entrada digital: pinMode(2, INPUT);
	// entrada analogica: pinMode(2, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
	randomSeed(analogRead(0));

	lcd_1.init();
	lcd_1.noCursor();
	lcd_1.backlight();
	lcd_1.backlight();
	neo_pixel.begin();
	neo_pixel.setBrightness(255);
	for (int i = 0; i <= 15; i = i + 1) {
		neo_pixel.setPixelColor(i, neo_pixel.Color(255, 0, 0));
	}
	neo_pixel.show();
	tiempoInicio = millis();
	// Se pone verde entre 3 y 5.5 segundos despues de arrancar
	// para que no sea facil hacer trampas
	tiempo = math_random_int(3000, 5500);
	lcd_1.setCursor(0, 0);
	lcd_1.print(String("READY..."));
}


void loop() {
	tiempoPasado = (millis() - tiempoInicio);
	lcd_1.setCursor(0, 1);
	lcd_1.print(tiempoPasado);
	if (tiempoPasado > 1500) {
		lcd_1.setCursor(0, 0);
		lcd_1.print(String("STEADY..."));
	}
	if (tiempoPasado > 2500) {
		lcd_1.setCursor(0, 0);
		lcd_1.print(String("STEADY..."));
	}
	if ((tiempoPasado > tiempo)) {
		if ((digitalRead(2) == false)) {
			lcd_1.clear();
			lcd_1.setCursor(0, 0);
			lcd_1.print(String("ERROR"));
			while (true)
				;
		}
		for (int i = 0; i < 16; i = i + 1) {
			neo_pixel.setPixelColor(i, neo_pixel.Color(0, 255, 0));
		}
		neo_pixel.show();	
		unsigned long tVerde = micros();

		while (true) {
			tiempoPasado = (millis() - tiempoInicio);
			lcd_1.setCursor(0, 0);
			lcd_1.print(String("GO!         "));
			lcd_1.setCursor(0, 1);
			lcd_1.print(tiempoPasado);
			if ((digitalRead(2) == false)) {
				unsigned long tiempo_us = micros() - tVerde;
				float dato = tiempo_us / 1000.0;
				lcd_1.clear();
				lcd_1.setCursor(0, 0);
				lcd_1.print(dato,2);
				lcd_1.print(" ms");
				while (true);
			}
		}
	}
}