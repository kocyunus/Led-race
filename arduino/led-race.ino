#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    150
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define RED_BUTTON_PIN   2
#define GREEN_BUTTON_PIN 3

#define STEP_SIZE    5
#define NUM_TURNS    2

CRGB leds[NUM_LEDS];

int redSteps = 0;
int greenSteps = 0;
bool gameOver = false;
CRGB winnerColor;
unsigned long gameOverTime = 0;
bool gameStarted = false;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);

  startCountdown();  // Geri sayım ve yarış başlangıcı
}

void loop() {
  if (!gameStarted) return;

  if (gameOver) {
    blinkWinner();

    // 10 saniye sonra oyunu sıfırla
    if (millis() - gameOverTime >= 10000) {
      resetGame();
      startCountdown();
    }
    return;
  }

  // KIRMIZI oyuncu
  if (digitalRead(RED_BUTTON_PIN) == LOW) {
    redSteps += STEP_SIZE;
    if (redSteps >= NUM_LEDS * NUM_TURNS) {
      gameOver = true;
      winnerColor = CRGB::Red;
      gameOverTime = millis();
    }
    updateLEDs();
    delay(100);
  }

  // YEŞİL oyuncu
  if (digitalRead(GREEN_BUTTON_PIN) == LOW) {
    greenSteps += STEP_SIZE;
    if (greenSteps >= NUM_LEDS * NUM_TURNS) {
      gameOver = true;
      winnerColor = CRGB::Green;
      gameOverTime = millis();
    }
    updateLEDs();
    delay(100);
  }
}

void updateLEDs() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (int i = 0; i < STEP_SIZE; i++) {
    int posRed = (redSteps - i + NUM_LEDS) % NUM_LEDS;
    if (redSteps >= i) leds[posRed] = CRGB::Red;

    int posGreen = (greenSteps - i + NUM_LEDS) % NUM_LEDS;
    if (greenSteps >= i) leds[posGreen] = CRGB::Green;
  }

  FastLED.show();
}

void blinkWinner() {
  static bool state = false;
  static unsigned long lastToggle = 0;
  const int blinkInterval = 500;

  unsigned long now = millis();
  if (now - lastToggle >= blinkInterval) {
    state = !state;
    lastToggle = now;

    fill_solid(leds, NUM_LEDS, state ? winnerColor : CRGB::Black);
    FastLED.show();
  }
}

void resetGame() {
  redSteps = 0;
  greenSteps = 0;
  gameOver = false;
  gameStarted = false;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

// 🚦 Geri Sayım: 3-2-1 başla!
void startCountdown() {
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(1000);

  fill_solid(leds, NUM_LEDS, CRGB::Orange);
  FastLED.show();
  delay(1000);

  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(1000);

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  gameStarted = true;
}
