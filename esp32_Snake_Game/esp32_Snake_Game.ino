#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define BTN_UP 16
#define BTN_DOWN 17
#define BTN_LEFT 18
#define BTN_RIGHT 8
#define BUZZER_PIN 14

#define SCREEN_W 240
#define SCREEN_H 320
#define CELL_SIZE 10
#define TOP_BAR 30
#define BORDER_W 2

#define PLAY_AREA_H (SCREEN_H - TOP_BAR - (2 * BORDER_W))
#define PLAY_AREA_W (SCREEN_W - (2 * BORDER_W))
#define GRID_W (PLAY_AREA_W / CELL_SIZE)
#define GRID_H (PLAY_AREA_H / CELL_SIZE)
#define START_X BORDER_W
#define START_Y (TOP_BAR + BORDER_W)

#define BASE_SPEED 120

#define COL_BG 0x2589
#define COL_BAR 0x0000
#define COL_SNAKE 0x001F
#define COL_HEAD 0xF800
#define COL_FOOD 0xF81F
#define COL_TEXT 0xFFFF
#define COL_BORDER 0xFFFF

struct Point {
  int x, y;
};

Point snake[1000];
int snakeLength = 3;
Point food;
int dirX = 0;
int dirY = -1;
bool gameOver = false;
int score = 0;
int currentSpeed = BASE_SPEED;
Point oldTail;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  tft.init();
  tft.setRotation(2);
  tft.fillScreen(COL_BG);

  drawStaticUI();
  resetGame();
}

void loop() {
  if (!gameOver) {
    handleInput();
    updateGame();
  } else {
    if (digitalRead(BTN_UP) == LOW || digitalRead(BTN_DOWN) == LOW || digitalRead(BTN_LEFT) == LOW || digitalRead(BTN_RIGHT) == LOW) {
      resetGame();
      delay(200);
    }
  }
  delay(currentSpeed);
}

void handleInput() {
  if (digitalRead(BTN_UP) == LOW && dirY == 0) {
    dirX = 0;
    dirY = -1;
  } else if (digitalRead(BTN_DOWN) == LOW && dirY == 0) {
    dirX = 0;
    dirY = 1;
  } else if (digitalRead(BTN_LEFT) == LOW && dirX == 0) {
    dirX = -1;
    dirY = 0;
  } else if (digitalRead(BTN_RIGHT) == LOW && dirX == 0) {
    dirX = 1;
    dirY = 0;
  }
}

void updateGame() {
  oldTail = snake[snakeLength - 1];

  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  snake[0].x += dirX;
  snake[0].y += dirY;

  if (snake[0].x < 0 || snake[0].x >= GRID_W || snake[0].y < 0 || snake[0].y >= GRID_H) {
    triggerGameOver();
    return;
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      triggerGameOver();
      return;
    }
  }

  bool ateFood = false;
  if (snake[0].x == food.x && snake[0].y == food.y) {
    ateFood = true;
    score += 10;
    snakeLength++;

    snake[snakeLength - 1] = oldTail;

    tone(BUZZER_PIN, 1000, 50);
    drawScore();

    int speedDecrease = (score / 100) * 10;
    currentSpeed = BASE_SPEED - speedDecrease;
    if (currentSpeed < 40) currentSpeed = 40;

    spawnFood();
  }

  if (!ateFood) {
    tft.fillRect(START_X + (oldTail.x * CELL_SIZE),
                 START_Y + (oldTail.y * CELL_SIZE),
                 CELL_SIZE, CELL_SIZE, COL_BG);
  }

  tft.fillRoundRect(START_X + (snake[0].x * CELL_SIZE),
                    START_Y + (snake[0].y * CELL_SIZE),
                    CELL_SIZE - 1, CELL_SIZE - 1, 2, COL_HEAD);

  if (snakeLength > 1) {
    tft.fillRoundRect(START_X + (snake[1].x * CELL_SIZE),
                      START_Y + (snake[1].y * CELL_SIZE),
                      CELL_SIZE - 1, CELL_SIZE - 1, 2, COL_SNAKE);
  }
}

void drawStaticUI() {
  tft.fillRect(0, 0, SCREEN_W, TOP_BAR, COL_BAR);
  tft.drawRect(0, TOP_BAR, SCREEN_W, SCREEN_H - TOP_BAR, COL_BORDER);
  drawScore();
}

void drawScore() {
  tft.fillRect(SCREEN_W / 2 + 10, 5, 100, 20, COL_BAR);

  tft.setTextColor(COL_TEXT, COL_BAR);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("SCORE: " + String(score), SCREEN_W / 2, TOP_BAR / 2 + 2);
}

void triggerGameOver() {
  if (!gameOver) {
    tone(BUZZER_PIN, 150, 600);
    drawGameOver();
  }
  gameOver = true;
}

void drawGameOver() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(4);
  tft.drawString("GAME OVER", SCREEN_W / 2, SCREEN_H / 2 - 40);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Final Score: " + String(score), SCREEN_W / 2, SCREEN_H / 2 + 10);

  tft.setTextSize(1);
  tft.drawString("Press Button to Restart", SCREEN_W / 2, SCREEN_H / 2 + 50);
}

void spawnFood() {
  bool valid = false;
  while (!valid) {
    valid = true;
    food.x = random(0, GRID_W);
    food.y = random(0, GRID_H);

    for (int i = 0; i < snakeLength; i++) {
      if (snake[i].x == food.x && snake[i].y == food.y) {
        valid = false;
        break;
      }
    }
  }
  tft.fillCircle(START_X + (food.x * CELL_SIZE) + (CELL_SIZE / 2),
                 START_Y + (food.y * CELL_SIZE) + (CELL_SIZE / 2),
                 (CELL_SIZE / 2) - 1, COL_FOOD);
}

void resetGame() {
  tft.fillRect(BORDER_W, TOP_BAR + BORDER_W, PLAY_AREA_W, PLAY_AREA_H, COL_BG);

  drawStaticUI();

  snakeLength = 3;
  snake[0] = { GRID_W / 2, GRID_H / 2 };
  snake[1] = { GRID_W / 2, GRID_H / 2 + 1 };
  snake[2] = { GRID_W / 2, GRID_H / 2 + 2 };

  for (int i = 0; i < snakeLength; i++) {
    uint16_t c = (i == 0) ? COL_HEAD : COL_SNAKE;
    tft.fillRoundRect(START_X + (snake[i].x * CELL_SIZE),
                      START_Y + (snake[i].y * CELL_SIZE),
                      CELL_SIZE - 1, CELL_SIZE - 1, 2, c);
  }

  dirX = 0;
  dirY = -1;
  score = 0;
  drawScore();
  currentSpeed = BASE_SPEED;
  gameOver = false;
  spawnFood();
}
