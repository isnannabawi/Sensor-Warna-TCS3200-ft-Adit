#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define S0 8
#define S1 9
#define S2 10
#define S3 11
#define sensorOut 12
#define BUTTON_PIN 7
#define LED_PIN 13     
#define BATTERY_PIN A0 // Analog input for battery voltage

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool ledState = false;

void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BATTERY_PIN, INPUT);
    
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    digitalWrite(LED_PIN, LOW);
    
    Serial.begin(9600);
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        delay(300); // Debounce delay
    }
    
    int red = getRed();
    int green = getGreen();
    int blue = getBlue();
    float batteryVoltage = getBatteryVoltage();
    int batteryLevel = map(batteryVoltage * 100, 300, 420, 0, 4);
    batteryLevel = constrain(batteryLevel, 0, 4);
    
    String colorName = identifyColor(red, green, blue);
    
    Serial.print("R: "); Serial.print(red);
    Serial.print(" G: "); Serial.print(green);
    Serial.print(" B: "); Serial.println(blue);
    Serial.print(" Color: "); Serial.println(colorName);
    Serial.print(" Battery: "); Serial.print(batteryVoltage); Serial.println("V");
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("TCS3200");
//    display.setCursor(0, 10);
//    display.print("R: "); display.print(red);
//    display.setCursor(0, 20);
//    display.print("G: "); display.print(green);
//    display.setCursor(0, 30);
//    display.print("B: "); display.print(blue);
    display.setCursor(0, 40);
//    display.print("Color: ");
    display.print(colorName);
    display.setTextSize(2);
    drawFlashlightIndicator(95, 0, ledState);
    drawBatteryIndicator(110, 0, batteryLevel);
    display.display();
    
    delay(500);
}

int getRed() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    return pulseIn(sensorOut, LOW);
}

int getGreen() {
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    return pulseIn(sensorOut, LOW);
}

int getBlue() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    return pulseIn(sensorOut, LOW);
}

float getBatteryVoltage() {
    int rawValue = analogRead(BATTERY_PIN);
    float voltage = (rawValue / 1023.0) * 5.0 * 2; // Voltage divider correction
    return voltage;
}

void drawFlashlightIndicator(int x, int y, bool state) {
    display.drawRect(x, y, 10, 7, WHITE);
    if (state) {
        display.fillRect(x + 2, y + 2, 6, 3, WHITE);
    }
}

void drawBatteryIndicator(int x, int y, int level) {
    display.drawRect(x, y, 15, 7, WHITE);
    display.fillRect(x + 15, y + 2, 2, 3, WHITE);
    for (int i = 0; i < level; i++) {
        display.fillRect(x + 2 + (i * 3), y + 2, 2, 3, WHITE);
    }
}

String identifyColor(int red, int green, int blue) {
  //----------------------------------------------------------Detect colors based on sensor values
  if (red>43 && red<49 && green>31 && green<37 && blue>36 && blue<49){       // to detect red
    Serial.println(" - KOTOR");
    return " - KOTOR";
  }
  else if (red>31 && red<33 && green>30 && green<35 && blue>19 && blue<26){  // to detect green
    Serial.println(" - BERSIH");
    return " - BERSIH";
  }
  else if (red>55 && red<100 && green>195 && green<220 && blue>164 && blue<185){  // to detect blue
    Serial.println(" - RED detected!");
    return " - RED detected!";
  }
  else{
    return " - ";
  }
  //----------------------------------------------------------
//
//    if (red > 200 && green > 200 && blue > 200) {
//        return "White";
//    } else if (red < 50 && green < 50 && blue < 50) {
//        return "Black";
//    } else if (red > 200 && green > 200 && blue < 100) {
//        return "Yellow";
//    } else if (red > 150 && blue > 150 && green < 100) {
//        return "Purple";
//    } else if (red > 200 && green < 100 && blue < 100) {
//        return "Red";
//    } else if (green > 200 && red < 100 && blue < 100) {
//        return "Green";
//    } else if (blue > 200 && red < 100 && green < 100) {
//        return "Blue";
//    } else if (red > 100 && green > 100 && blue > 100) {
//        return "Gray";
//    } else {
//        return "Unknown";
//    }
}
