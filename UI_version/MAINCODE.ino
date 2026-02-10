#include <TFT_eSPI.h>
#include "UI.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

const uint16_t bgWidth  = 240;
const uint16_t bgHeight = 240;

// -----------------------------------------------------------------------
// PIN BUTTON
// -----------------------------------------------------------------------
#define PIN_UP     42
#define PIN_DOWN   45
#define PIN_LEFT   46
#define PIN_RIGHT  47
#define PIN_OK     48
#define PIN_ACTION 37
#define PIN_BACK   38

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
int option = 3;
float tget = 0;
int op = option -1 ;
float x = 0;

unsigned long lastPressRight = 0;
unsigned long lastPressLeft  = 0;
unsigned long lastFrame      = 0;

const unsigned long pressDelay = 500;
const unsigned long frameDelay = 16; // ~60fps

// -----------------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  pinMode(PIN_UP,     INPUT_PULLUP);
  pinMode(PIN_DOWN,   INPUT_PULLUP);
  pinMode(PIN_LEFT,   INPUT_PULLUP);
  pinMode(PIN_RIGHT,  INPUT_PULLUP);
  pinMode(PIN_OK,     INPUT_PULLUP);
  pinMode(PIN_ACTION, INPUT_PULLUP);
  pinMode(PIN_BACK,   INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);

  spr.createSprite(240,240);
  spr.setSwapBytes(true);

  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, 240, 240, LOGO);
  delay(3000);
}

// -----------------------------------------------------------------------
int clamp(int v, int minV, int maxV) {
		if(v < minV) return minV;
		if(v > maxV) return maxV;
		return v;
}
void drawIconWithBorder(int x, int y, const uint16_t* img){
  
  // ===== VIỀN ĐEN 3PX =====
  spr.fillRect(x-3, y-3, 106, 106, TFT_BLACK);

  // ===== ICON =====
  spr.pushImage(x, y, 100, 100, img);
}
void drawFocusRect(int x,int y){
  spr.drawRect(x,y,100,100,TFT_WHITE);
  spr.drawRect(x-1,y-1,102,102,TFT_WHITE);
  spr.drawRect(x-2,y-2,104,104,TFT_WHITE);
}

void HOME() {

  if (millis() - lastFrame < frameDelay) return;
  lastFrame = millis();

  // ===== EASING MƯỢT =====
  x += (tget - x) * 0.3f;

		// ===== RIGHT =====
	if (digitalRead(PIN_RIGHT) == LOW) {
		if (millis() - lastPressRight >= pressDelay) {
			tget += 300;
			tget = clamp(tget, 0, op * 300); // khóa ngay tại đây
			lastPressRight = millis();
		}
	}

	// ===== LEFT =====
	if (digitalRead(PIN_LEFT) == LOW) {
		if (millis() - lastPressLeft >= pressDelay) {
			tget -= 300;
			tget = clamp(tget, 0, op * 300); // khóa ngay tại đây
			lastPressLeft = millis();
		}
	}


  // ===== GIỚI HẠN =====

	if(x < 0) x = 0;
	if(x > op * 300) x = op * 300;
	if(abs(tget - x) < 0.5) x = tget;



  // ===== VẼ TRONG SPRITE =====
  spr.fillSprite(TFT_BLACK);

  spr.pushImage(0, 0, 240, 240, UI);
	drawIconWithBorder((int)(x - 50 +120), 80, ICON_MENUS);
	drawIconWithBorder((int)(x -350 +120), 80, ICON_DISPLAY);
	drawIconWithBorder((int)(x -650 +120), 80, ICON_CONFIG);


  int posMenus   = (int)(x - 50 +120);
	int posDisplay = (int)(x -350 +120);
	int posConfig  = (int)(x -650 +120);

	int focus = round(x / 300.0);

	// ===== chỉ hiện viền khi gần vị trí =====
	bool snapReady = abs(tget - x) < 15;   // chỉnh số này nếu muốn hiện sớm/muộn

	if(snapReady){
		if(focus == 0) drawFocusRect(posMenus,80);
		if(focus == 1) drawFocusRect(posDisplay,80);
		if(focus == 2) drawFocusRect(posConfig,80);
	}


  spr.pushSprite(0,0);
}

// -----------------------------------------------------------------------

void loop() {
  HOME();
}
