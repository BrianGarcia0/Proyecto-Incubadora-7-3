#include <LiquidCrystal_I2C.h>

#include <DHT.h>
#include <Wire.h>

//====================================================
// CONFIGURACIÓN DE PINES (ESP32)
//====================================================

#define DHTPIN      4       // GPIO4
#define DHTTYPE     DHT11

#define COOLER      25      // GPIO25
#define HEATER      26      // GPIO26

//====================================================
// VARIABLES
//====================================================

int contedias = 0;

float tempdht = 0;
float humdht = 0;

// Temperaturas de trabajo
const float TEMP_MIN = 37.7;
const float TEMP_MAX = 38.0;

//====================================================
// OBJETOS
//====================================================

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_h lcd(0x27, 16, 2);

//====================================================
// ACTUALIZAR LCD
//====================================================

void Informacion()
{
    lcd.setCursor(12,0);
    lcd.print("   ");
    lcd.setCursor(12,0);
    lcd.print(contedias);

    lcd.setCursor(2,1);
    lcd.print("     ");
    lcd.setCursor(2,1);
    lcd.print(tempdht,1);

    lcd.setCursor(11,1);
    lcd.print("     ");
    lcd.setCursor(11,1);
    lcd.print(humdht,0);
}

//====================================================
// SETUP
//====================================================

void setup()
{
    Serial.begin(115200);

    dht.begin();

    // SDA = GPIO22
    // SCL = GPIO21
    Wire.begin(22,21);

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Dias:");
    lcd.setCursor(8,0);
    lcd.print("Incub.");

    lcd.setCursor(0,1);
    lcd.print("T:");

    lcd.setCursor(9,1);
    lcd.print("H:");

    pinMode(HEATER,OUTPUT);
    pinMode(COOLER,OUTPUT);

    digitalWrite(HEATER,LOW);
    digitalWrite(COOLER,LOW);

    Serial.println("Incubadora iniciada.");
}

//====================================================
// LOOP
//====================================================

void loop()
{
    tempdht = dht.readTemperature();
    humdht = dht.readHumidity();

    if(isnan(tempdht) || isnan(humdht))
    {
        Serial.println("Error al leer DHT11");
        delay(1000);
        return;
    }

    //------------------------------------------------
    // CONTROL DE TEMPERATURA
    //------------------------------------------------

    if(tempdht < TEMP_MIN)
    {
        digitalWrite(HEATER,HIGH);
        digitalWrite(COOLER,LOW);
    }
    else if(tempdht > TEMP_MAX)
    {
        digitalWrite(HEATER,LOW);
        digitalWrite(COOLER,HIGH);
    }
    else
    {
        digitalWrite(HEATER,LOW);
        digitalWrite(COOLER,LOW);
    }

    //------------------------------------------------
    // LCD
    //------------------------------------------------

    Informacion();

    //------------------------------------------------
    // Monitor Serie
    //------------------------------------------------

    Serial.print("Temperatura: ");
    Serial.print(tempdht);
    Serial.print(" °C");

    Serial.print("    Humedad: ");
    Serial.print(humdht);
    Serial.println(" %");

    delay(1000);
}
