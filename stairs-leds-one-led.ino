#include <FastLED.h>

#define DATA_PIN 12 // Пин, к которому подключена адресная лента

#define NUM_LEDS 256 // Количество светодиодов в адресной ленте
int NUM_STEPS = 16; // количество супеней
int LEDS_PER_STEP = NUM_LEDS / NUM_STEPS; // количество светодиодов в одной ступени
#define FADE_DELAY 50 // Задержка между изменениями яркости (миллисекунды)

CRGB leds[NUM_LEDS];

int motionSensorPin1 = 2; // Пин для первого датчика движения
int motionSensorPin2 = 3; // Пин для второго датчика движения

int brightness = 10; // Начальная яркость (от 0 до 255)
CRGB color = CRGB::Blue; // Начальный цвет

bool firstMove = false;
bool downSensor = false;
bool topSensor = false;

unsigned long startTime;
unsigned long previousMillis = 0;
const long interval = 1000; // интервал для измерения температуры (1 секунда)
int time = 2;

int r = 2;

int timeDelay = 400;

 ///
  void onfirstLeds(){
    // if(r == 1) {
    //   leds[0] = CRGB::White;
    //   leds[NUM_LEDS - 1] = CRGB::White;
    // }

    // if(r == 2) {
      for (int i = 0; i < NUM_STEPS; i++) {
        leds[i] = CRGB::White;
        FastLED.show(); // Отображаем изменения на ленте
        delay(0); // Задержка между выключением светодиодов
      }

      for (int i = NUM_LEDS - NUM_STEPS; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
        FastLED.show(); // Отображаем изменения на ленте
        delay(0); // Задержка между выключением светодиодов
      }
    // }
  }
  ///

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  pinMode(motionSensorPin1, INPUT);
  pinMode(motionSensorPin2, INPUT);
  
  Serial.begin(9600); // Инициализация последовательной связи
  FastLED.clear(); // Очищаем ленту

  // Включаем первый и последний светодиод белым цветом при включении питания
 
  // onfirstLeds();

  FastLED.show(); // Отображаем изменения на ленте
  startTime = millis(); // сохраняем время старта
}

void loop() {
  unsigned long currentMillis = millis();

  int motionValue1 = digitalRead(motionSensorPin1); // Считываем значение с первого датчика движения
  int motionValue2 = digitalRead(motionSensorPin2); // Считываем значение со второго датчика движения

  FastLED.setBrightness(brightness); // Устанавливаем яркость

  // идем наверх в первый раз 
  if (motionValue1 == HIGH && !firstMove && !downSensor && !topSensor) { // 1 датчик включаем снизу вверх
    // Serial.println("первый раз пошли наверх");
    onLightToUp();
    firstMove = true;
    downSensor = true;
    topSensor = false;  
  }

  // идем вниз в первый раз
  if (motionValue2 == HIGH && !firstMove && !downSensor && !topSensor) { // 2 датчик, включаем сверху вниз
    // Serial.println("первый раз пошли вниз");
    onLightToDown();
    firstMove = true;
    topSensor = true;
    downSensor = false;  
  }

  // поднимаемся наверх в первый раз
  if (motionValue2 == HIGH && downSensor) { // 2 датчик, включаем сверху вниз
    // Serial.println("первый раз поднимаемся");
    offLightToUp();
    downSensor = false;
    topSensor = false;
    time = 2;
  }

  // спускаемся вниз в первый раз
  if (motionValue1 == HIGH && topSensor) { // 1 датчик, включаем сверху вниз
    // Serial.println("первый раз спускаемся");
    offLightToDown();
    topSensor = false;
    downSensor = false;
    time = 2;
  }

  if (currentMillis - previousMillis >= interval) { // 1 сек, работа в режиме ТЕМПЕРАТУРА
      previousMillis = currentMillis;

    if (firstMove && (!topSensor && !downSensor)) {
      if (time > 0) {
        time = time - 1;
        // Serial.println(time);
      } else {
        // Serial.println("будет сначала");
        firstMove = false;
        topSensor = false;
        downSensor = false;
      }
    }
  }
  onfirstLeds();
}

//// Функции

void onLightToUp() { // ВКЛЮЧЕНИЕ светодиодов СНИЗУ ВВЕРХ
  if(r == 1) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White;
      FastLED.show(); // Отображаем изменения на ленте
      delay(5); // Задержка между выключением светодиодов
    }
  }

  if(r == 2) { // включение снизу вверх по одной ступеньке
    for (int i = 0; i < NUM_STEPS; i++) {
      int startLed = i * LEDS_PER_STEP;
      int endLed = startLed + LEDS_PER_STEP;
      
      for (int j = startLed; j < endLed; j++) {
        leds[j] = CRGB::White;
      }
      
      FastLED.show();
      delay(timeDelay);
    }
  }
}

void onLightToDown() { // ВКЛЮЧЕНИЕ светодиодов СВЕРХУ ВНИЗ
  if(r == 1) {
    for (int i = NUM_LEDS; i >= 0; i--) {
      leds[i] = CRGB::White;
      FastLED.show(); // Отображаем изменения на ленте
      delay(5); // Задержка между включением светодиодов
    }
  }

  if(r == 2) { // включаем по одной ступеньке сверху вниз
    for (int i = NUM_STEPS - 1; i >= 0; i--) {
      int startLed = i * LEDS_PER_STEP;
      int endLed = startLed + LEDS_PER_STEP;
      
      for (int j = startLed; j < endLed; j++) {
        leds[j] = CRGB::White; // Зажигаем все светодиоды в ступени
      }
      
      FastLED.show();
      delay(timeDelay); // Задержка перед переходом к следующей ступени
    }    
  }
}

void offLightToUp() { // ВЫКЛЮЧЕНИЕ светодиодов СНИЗУ ВВЕРХ
  if(r == 1) {
    for (int i = 0;  i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      FastLED.show(); // Отображаем изменения на ленте
      delay(5); // Задержка между выключением светодиодов
    }
  }

  if(r == 2) { // вЫключение снизу вверх по одной ступеньке
    for (int i = 0; i < NUM_STEPS; i++) {
      int startLed = i * LEDS_PER_STEP;
      int endLed = startLed + LEDS_PER_STEP;
      
      for (int j = startLed; j < endLed; j++) {
        leds[j] = CRGB::Black;
      }
      
      FastLED.show();
      delay(timeDelay);
    }    
  }
}

void offLightToDown() { // ВЫКЛЮЧЕНИЕ светодиодов  СВЕРХУ в ВНИЗ
  if(r == 1) {
    for (int i = NUM_LEDS; i >= 0; i--) {
      leds[i] = CRGB::Black;
      FastLED.show(); // Отображаем изменения на ленте
      delay(5); // Задержка между выключением светодиодов
    }
  }

  if(r == 2) {
    for (int i = NUM_STEPS - 1; i >= 0; i--) {
      int startLed = i * LEDS_PER_STEP;
      int endLed = startLed + LEDS_PER_STEP;
      
      for (int j = startLed; j < endLed; j++) {
        leds[j] = CRGB::Black; // Зажигаем все светодиоды в ступени
      }
      
      FastLED.show();
      delay(timeDelay); // Задержка перед переходом к следующей ступени
    }    
  }
}