#include <FastLED.h>

#define NUM_LEDS 256 // Количество светодиодов в адресной ленте
#define DATA_PIN 12 // Пин, к которому подключена адресная лента

int NUM_STEPS = 16;
int LEDS_PER_STEP = NUM_LEDS / NUM_STEPS; 


CRGB leds[NUM_LEDS];

int motionSensorPin1 = 2; // Пин для первого датчика движения
int motionSensorPin2 = 3; // Пин для второго датчика движения

int brightness = 10; // Начальная яркость (от 0 до 255)
CRGB color = CRGB::Blue; // Начальный цвет

bool motionDetected_1 = false; // Флаг обнаружения движения
bool motionDetected_2 = false; // Флаг обнаружения движения

bool motionDetected = false; // Флаг обнаружения движения

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  pinMode(motionSensorPin1, INPUT);
  pinMode(motionSensorPin2, INPUT);
  
  Serial.begin(9600); // Инициализация последовательной связи
  FastLED.clear(); // Очищаем ленту
  FastLED.show(); // Отображаем изменения на ленте
}
///////////////

void loop() {
  int motionValue1 = digitalRead(motionSensorPin1); // Считываем значение с первого датчика движения
  int motionValue2 = digitalRead(motionSensorPin2); // Считываем значение со второго датчика движения

  FastLED.setBrightness(brightness); // Устанавливаем яркость
  
  if (motionValue1 == HIGH && !motionDetected) { // 1 датчик
    motionDetected = true;    
    onBeginToFinish();
    
    while (motionValue2 != HIGH) {
      Serial.println("Движение обнаружено на первом датчике!");      
      motionValue2 = digitalRead(motionSensorPin2); // Проверяем срабатывание ВТОРОГО датчика
    }

    offBeginToFinish();
    
    motionDetected = false; // Сбрасываем флаг обнаружения движения
    FastLED.clear(); // Очищаем ленту
    FastLED.show(); // Отображаем изменения на ленте
  }

  if (motionValue2 == HIGH && !motionDetected) { // 2 датчик
    motionDetected = true;    
    onFinishToBegin();
    
    while (motionValue1 != HIGH) {
      Serial.println("Движение обнаружено на втором датчике!");      
      motionValue1 = digitalRead(motionSensorPin1); // Проверяем срабатывание ПЕРВОГО датчика
    }
    
    offFinishToBegin();
    
    motionDetected = false; // Сбрасываем флаг обнаружения движения
    FastLED.clear(); // Очищаем ленту
    FastLED.show(); // Отображаем изменения на ленте
  }
}

void onBeginToFinish() { // ВКЛЮЧЕНИЕ светодиодов СНИЗУ ВВЕРХ
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White; // Включаем светодиоды поочередно
    FastLED.show(); // Отображаем изменения на ленте
    delay(5); // Задержка между выключением светодиодов
  }
}

void onFinishToBegin() {
  for (int i = NUM_LEDS; i >= 0; i--) {
    leds[i] = CRGB::Red; // Включаем светодиоды поочередно
    FastLED.show(); // Отображаем изменения на ленте
    delay(5); // Задержка между включением светодиодов
  }
}

void offBeginToFinish() { // ВЫКЛЮЧЕНИЕ светодиодов СНИЗУ ВВЕРХ
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black; // Гасим светодиоды поочередно
    FastLED.show(); // Отображаем изменения на ленте
    delay(5); // Задержка между выключением светодиодов
  }
}

void offFinishToBegin() {
  for (int i = NUM_LEDS; i >= 0; i--) {
    leds[i] = CRGB::Black; // Гасим светодиоды поочередно
    FastLED.show(); // Отображаем изменения на ленте
    delay(5); // Задержка между выключением светодиодов
  }
}