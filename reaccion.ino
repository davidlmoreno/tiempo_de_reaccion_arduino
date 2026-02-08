#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <Adafruit_NeoPixel.h>

// Definimos los datos para guardar los tiempos
unsigned long tiempoPasado;
unsigned long tiempoInicio;
unsigned long tiempo;
unsigned long resultado;

// Instancia para la pantalla
LiquidCrystal_I2C lcd_1(0x27, 16, 2);
// Instancia del LED de colores
Adafruit_NeoPixel neo_pixel = Adafruit_NeoPixel(16, 5, NEO_GRB + NEO_KHZ800);

// Esta funcion devuelve un numero aleatorio
// entre dos enteros a y b. La usamos para que el tiempo
// en que se pone verde el LED no sea siempre el mismo
double math_random_int(int a, int b) {
	if (a > b) {
		int c = a;
		a = b;
		b = c;
	}
	return (double)random(a, b + 1);
}

// Esta funcion se ejecuta cuando se arranca arduino
// y solo se llama una vez
void setup() {
	// entrada digital: pinMode(2, INPUT);
	// entrada analogica: pinMode(2, INPUT_PULLUP);
	
	// Configura el pin del interruptor del boton rojo
  pinMode(2, INPUT_PULLUP);

	// Inicializamos el generador de numeros aleatorios
	// con un dato random del pin 0 analógico
	randomSeed(analogRead(0));

	// Inicializamos la pantalla
	lcd_1.init();
	lcd_1.noCursor();
	lcd_1.backlight();
	lcd_1.backlight();

	// Inicializamos el LED todo a rojo, brillo a tope
	neo_pixel.begin();
	neo_pixel.setBrightness(255);
	for (int i = 0; i <= 15; i = i + 1) {
		neo_pixel.setPixelColor(i, neo_pixel.Color(255, 0, 0));
	}
	neo_pixel.show();

	// Guardamos el valor del reloj interno del momento donde empezamos
	// en millisegundos
	tiempoInicio = millis();

	// Elegimos el momento en el que el LED se pondrá verde
	// El LED se pone verde entre 3 y 5.5 segundos despues de arrancar
	// para que no sea facil hacer trampas
	tiempo = math_random_int(3000, 5500);

	// Mostramos READY en la pantalla
	lcd_1.setCursor(0, 0);
	lcd_1.print(String("READY..."));
}

// Esta funcion se llama todo el rato mientras 
// el arduino está encendido
void loop() {
	// Calculamos el tiempo que ha pasado desde el inicio, en millisegundos
	tiempoPasado = (millis() - tiempoInicio);

	// Y lo mostramos en la pantalla
	lcd_1.setCursor(0, 1);
	lcd_1.print(tiempoPasado);

	if (tiempoPasado > 2200) {
		// Si han pasado 2.2 segundos, mostramos STEADY
		lcd_1.setCursor(0, 0);
		lcd_1.print(String("STEADY..."));
	}

	// Si el tiempo transcurrido es mayor que
	// el que tomamos aleatoriamente, ponemos
	// el LED en verde y empezamos a contar el
	// tiempo de reaccion
	if ((tiempoPasado > tiempo)) {
		// Si el pulsador esta pulsado cuando llega el tiempo,
		// el usuario ha hecho trampas: damos un error
		if ((digitalRead(2) == false)) {
			lcd_1.clear();
			lcd_1.setCursor(0, 0);
			lcd_1.print(String("ERROR"));
			// Nos quedamos aqui hasta que alguien reinicie el arduino
			while (true);
		}

		// Ponemos el LED en verde
		for (int i = 0; i < 16; i = i + 1) {
			neo_pixel.setPixelColor(i, neo_pixel.Color(0, 255, 0));
		}
		neo_pixel.show();

		// Y marcamos el momento en microsegundos, el tiempo
		// de reacción se mide contra este momento
		unsigned long tVerde = micros();
	
		// Limpiamos la pantalla y ponemos GO!
		lcd_1.clear();
		lcd_1.setCursor(0, 0);
		lcd_1.print(String("GO!"));
		
		// Bucle infinito
		while (true) {
			// Medimos el tiempo que ha pasado desde el inicio
			tiempoPasado = (millis() - tiempoInicio);
			// Para mostrarlo en la pantalla
			lcd_1.setCursor(0, 1);
			lcd_1.print(tiempoPasado);

			// Si el jugador ha pulsado el boton
			if ((digitalRead(2) == false)) {
				// Calculamos el tiempo que ha pasado, en microsegundos
				unsigned long tiempo_us = micros() - tVerde;
				// Lo pasamos a millisegundos con decimales
				float resultado = tiempo_us / 1000.0;

				// Y lo mostramos en la pantalla, 
				// en millisegundos con dos decimales
				lcd_1.clear();
				lcd_1.setCursor(0, 0);
				// Este 2 es el numero de decimales
				lcd_1.print(resultado,2);
				lcd_1.print(" ms");
				// Y ahora nos quedamos aqui hasta que alguien
				// reinicie el Arduino
				while (true);
			}
		}
	}
}