#include "displayHelper.h"
// 显示固件信息
void showFirmwareInfo(FastLED_NeoMatrix *matrix, int x, int y, bool clear)
{
  for (int i = 32; i > 0; i--)
  {
    matrix->setCursor(i, 6 + y);
    if (clear)
    {
      matrix->clear();
    }
    matrix->setTextColor(matrix->Color(255, 0, 0));
    matrix->print("G");
    matrix->setTextColor(matrix->Color(255, 127, 0));
    matrix->print("M");
    matrix->setTextColor(matrix->Color(255, 255, 0));
    matrix->print("a");
    matrix->setTextColor(matrix->Color(0, 255, 0));
    matrix->print("t");
    matrix->setTextColor(matrix->Color(0, 255, 255));
    matrix->print("r");
    matrix->setTextColor(matrix->Color(46, 43, 95));
    matrix->print("i");
    matrix->setTextColor(matrix->Color(139, 0, 255));
    matrix->print("x");
    matrix->setTextColor(matrix->Color(255, 255, 255));
    // matrix->print("v0.01");
    matrix->show();
    delay(16);
  }
  delay(1000);
}

// 显示开发者信息
void showChaosgoo(FastLED_NeoMatrix *matrix, bool clear)
{
  for (int i = 32; i > -64; i--)
  {
    matrix->setCursor(i, 6);
    if (clear)
    {
      matrix->clear();
    }
    matrix->setTextColor(matrix->Color(255, 165, 0));
    matrix->print("Design By ");
    matrix->setTextColor(matrix->Color(255, 255, 255));
    matrix->print("Chaosgoo.");
    matrix->show();
    delay(16);
  }
}

void showContent(FastLED_NeoMatrix *matrix, String str, int x, int y, int color, bool clear)
{
  matrix->setCursor(x, y);
  if (clear)
  {
    matrix->clear();
  }
  // matrix->setTextColor(matrix->Color(((color & 0xff) << 16), ((color & 0xff) << 8), ((color & 0xff) << 0)));
  matrix->setTextColor(color);
  matrix->print(str); // No problem.
  // matrix->show();
}

void showTime(FastLED_NeoMatrix *matrix, String time, int x, int y, int color, bool clear)
{
  // 绘制小时数
  if (clear)
  {
    matrix->clear();
  }
  matrix->setCursor(x, y);
  matrix->setTextColor(color);
  matrix->print(time.substring(0, 2));
  matrix->drawPixel(x + 8, y - 2, 0x051d);
  matrix->drawPixel(x + 8, y - 4, 0x051d);
  matrix->setCursor(x + 10, y);
  matrix->print(time.substring(2, 4)); // No problem.
  // matrix->show();
}

void showDate(FastLED_NeoMatrix *matrix, String date, int x, int y, int color, bool clear)
{
  // 绘制小时数
  if (clear)
  {
    matrix->clear();
  }
  matrix->setCursor(x, y);
  matrix->setTextColor(color);
  matrix->print(date.substring(0, 2));
  matrix->drawPixel(x + 8, y - 3, matrix->Color(255, 0, 0));
  // matrix->drawPixel(x + 9, y - 3, matrix->Color(255, 0, 0));
  matrix->setCursor(x + 10, y);
  matrix->print(date.substring(2, 4)); // No problem.
  // matrix->show();
}
// 显示WiFi连接中动画帧
void showWiFiConnecting(FastLED_NeoMatrix *matrix, int status, bool clear)
{
  if (clear)
  {
    matrix->clear();
  }
  switch (status)
  {
  case 3:
    matrix->drawPixel(23, 3, 0x07E0);
  case 2:
    matrix->drawPixel(21, 3, 0x07E0);
  case 1:
    matrix->drawPixel(19, 3, 0x07E0);
  case 0:
    matrix->setCursor(5, 6);
    matrix->setTextColor(matrix->Color(255, 255, 255));
    matrix->print("WiFi");
    break;
  }
  matrix->show();
}

// 显示WiFi连接完成动画
void showWiFiDoneAnimation(FastLED_NeoMatrix *matrix, bool clear)
{
  for (int i = 0; i < 8; i++)
  {
    showWiFiConnectDone(matrix, i, true);
    delay(100);
  }
}

// WiFi连接完成动画帧
void showWiFiConnectDone(FastLED_NeoMatrix *matrix, int status, bool clear)
{
  if (clear)
  {
    matrix->clear();
  }
  switch (status)
  {
  case 7:
    matrix->drawPixel(25, 1, 0x07E0);
  case 6:
    matrix->drawPixel(24, 2, 0x07E0);
  case 5:
    matrix->drawPixel(23, 3, 0x07E0);
  case 4:
    matrix->drawPixel(22, 4, 0x07E0);
  case 3:
    matrix->drawPixel(21, 5, 0x07E0);
  case 2:
    matrix->drawPixel(20, 4, 0x07E0);
  case 1:
    matrix->drawPixel(19, 3, 0x07E0);
  case 0:
    matrix->setCursor(5, 6);
    matrix->setTextColor(matrix->Color(255, 255, 255));
    matrix->print("WiFi");
    break;
  }
  matrix->show();
}

// 格式化粉丝数量字符串
const char *formatFollowers(const char *followers, int width, bool clear)
{
  int len = strlen(followers);
  char *dst;
  strcpy(dst, followers);
  while (len < width)
  {
    len++;
    dst += '0';
  }
  return dst;
}

void showTemperature(FastLED_NeoMatrix *matrix, float temp, int x, int y, int color, bool clear)
{
  matrix->setCursor(x, y);
  if (clear)
  {
    matrix->clear();
  }
  matrix->setTextColor(matrix->Color(255, 255, 255));
  matrix->print("Te:");
  if (temp > 30)
  {
    matrix->setTextColor(matrix->Color(213, 0, 0));
  }
  else if (temp > 20)
  {
    matrix->setTextColor(matrix->Color(255, 23, 68));
  }
  else if (temp > 20)
  {
    matrix->setTextColor(matrix->Color(255, 82, 82));
  }
  else if (temp > 10)
  {
    matrix->setTextColor(matrix->Color(255, 138, 128));
  }
  else if (temp > 0)
  {
    matrix->setTextColor(matrix->Color(213, 0, 0));
  }
  matrix->print(int(temp)); // No problem.
  matrix->setTextColor(matrix->Color(255, 255, 255));
  matrix->print("C"); // No problem.
  // matrix->show();
}

void showHumidity(FastLED_NeoMatrix *matrix, float hum, int x, int y, int color, bool clear)
{
  matrix->setCursor(x, y);
  if (clear)
  {
    matrix->clear();
  }
  matrix->setTextColor(matrix->Color(255, 255, 255));
  matrix->print("Hu:"); // No problem.
  if (hum > 80)
  {
    matrix->setTextColor(matrix->Color(0, 1, 118));
  }
  else if (hum > 75)
  {
    matrix->setTextColor(matrix->Color(0, 176, 255));
  }
  else if (hum > 70)
  {
    matrix->setTextColor(matrix->Color(64, 196, 255));
  }
  else if (hum > 65)
  {
    matrix->setTextColor(matrix->Color(128, 216, 255));
  }
  else if (hum > 60)
  {
    matrix->setTextColor(matrix->Color(1, 87, 155));
  }
  matrix->print(int(hum)); // No problem.
  matrix->setTextColor(matrix->Color(255, 255, 255));
  matrix->print("%"); // No problem.
  // matrix->show();
}

void showFirmwareInfoEnd(FastLED_NeoMatrix *matrix)
{
  for (int y = 6; y < 14; y++)
  {
    matrix->setCursor(1, y);
    matrix->clear();
    matrix->setTextColor(matrix->Color(255, 0, 0));
    matrix->print("G");
    matrix->setTextColor(matrix->Color(255, 127, 0));
    matrix->print("M");
    matrix->setTextColor(matrix->Color(255, 255, 0));
    matrix->print("a");
    matrix->setTextColor(matrix->Color(0, 255, 0));
    matrix->print("t");
    matrix->setTextColor(matrix->Color(0, 255, 255));
    matrix->print("r");
    matrix->setTextColor(matrix->Color(46, 43, 95));
    matrix->print("i");
    matrix->setTextColor(matrix->Color(139, 0, 255));
    matrix->print("x");
    matrix->setTextColor(matrix->Color(255, 255, 255));
    // matrix->print("v0.01");
    matrix->show();
    delay(16);
  }
}

void showAnimationString(FastLED_NeoMatrix *matrix, String content, int fromX, int fromY, int toX, int toY, int color)
{
  int xDirection = 1;
  int yDirection = 1;
  if (fromX > toX)
  {
    xDirection = -1;
  }
  else if (fromX == toX)
  {
    xDirection = 0;
  }
  if (fromY > toY)
  {
    yDirection = -1;
  }
  else if (fromY == toY)
  {
    yDirection = 0;
  }
  while (!(fromX == toX && fromY == toY))
  {
    showContent(matrix, content, fromX, fromY, color, true);
    fromX += xDirection;
    fromY += yDirection;
    delay(1000);
  }
}

// 局部清屏
void clearLocal(FastLED_NeoMatrix *matrix, int l, int t, int r, int b)
{
  matrix->drawRect(l, t, r, b, 0xffff);
}

// 显示加号动画
void showPlus(FastLED_NeoMatrix *matrix, int follower, int x, int y, int color, bool clear, int frame)
{
  if (clear)
  {
    matrix->clear();
  }
  matrix->setCursor(x, y);
  switch (frame % 7)
  {
  case 0:
    matrix->drawFastHLine(x, y + 2, 5, color);
    matrix->drawFastVLine(x + 2, y, 5, color);
    break;
  case 1:
    matrix->drawFastHLine(x + 1, y + 2, 3, color);
    matrix->drawFastVLine(x + 2, y + 1, 3, color);
    break;
  case 2:
    matrix->drawPixel(x + 2, y + 2, color);
    break;
  case 3:
    // DO NOTHING
    break;
  case 4:
    matrix->drawPixel(x + 2, y + 2, color);
    break;
  case 5:
    matrix->drawFastHLine(x + 1, y + 2, 3, color);
    matrix->drawFastVLine(x + 2, y + 1, 3, color);
    break;
  case 6:
    matrix->drawFastHLine(x, y + 2, 5, color);
    matrix->drawFastVLine(x + 2, y, 5, color);
    break;
  }
  // matrix->drawFastHLine(0, 7, (frame / 60.0) * 32, matrix->Color(0, 164, 214));
  // matrix->drawFastHLine(0, 0, 32, matrix->Color(0, 0, 0));
  // matrix->drawFastHLine(0, 0, (1.0 - (frame / 60.0)) * 32, matrix->Color(0, 164, 214));
}

void showDecrease(FastLED_NeoMatrix *matrix, int follower, int x, int y, int color, bool clear, int frame)
{
  if (clear)
  {
    matrix->clear();
  }
  matrix->setCursor(x, y);
  switch (frame % 7)
  {
  case 0:
    matrix->drawFastHLine(x, y + 2, 5, color);
    break;
  case 1:
    matrix->drawFastHLine(x + 1, y + 2, 3, color);
    break;
  case 2:
    matrix->drawPixel(x + 2, y + 2, color);
    break;
  case 3:
    // DO NOTHING
    break;
  case 4:
    matrix->drawPixel(x + 2, y + 2, color);
    break;
  case 5:
    matrix->drawFastHLine(x + 1, y + 2, 3, color);
    break;
  case 6:
    matrix->drawFastHLine(x, y + 2, 5, color);
    break;
  }
}

void showHeart(FastLED_NeoMatrix *matrix, int follower, int x, int y, int color, bool clear, int frame)
{
  if (clear)
  {
    matrix->clear();
  }
  if (frame % 10 > 5)
  {
    matrix->fillRect(x + 1, y + 1, 5, 3, color);
    matrix->drawFastHLine(x + 1, y, 2, color);
    matrix->drawFastHLine(x + 4, y, 2, color);
    matrix->drawFastHLine(x + 2, y + 4, 3, color);
    matrix->drawFastVLine(x, y + 1, 2, color);
    matrix->drawFastVLine(x + 6, y + 1, 2, color);
    matrix->drawPixel(x + 3, y + 5, color);
  }
  else
  {
    matrix->fillRect(x + 1, y + 1, 5, 3, color);
    matrix->drawPixel(x + 3, y + 1, 0);
    matrix->drawPixel(x + 1, y + 3, 0);
    matrix->drawPixel(x + 5, y + 3, 0);
    matrix->drawPixel(x + 3, y + 4, color);
  }
}

void showClock(CRGB leds[], FastLED_NeoMatrix *matrix, int x, int y, int color, bool clear, int frame)
{

  if (clear && frame < 6)
  {
    matrix->clear();
  }
  switch (frame)
  {
  case 4:
    matrix->drawPixel(x + 1, y + 1, color);
    matrix->drawPixel(x + 5, y + 1, color);
    matrix->drawPixel(x + 11, y + 1, color);
    matrix->drawPixel(x + 15, y + 1, color);
    break;
  case 3:
    matrix->fillRect(x, y+1, 3, 1, color);
    matrix->fillRect(x + 4, y+1, 3, 1, color);
    matrix->fillRect(x + 10, y+1, 3, 1, color);
    matrix->fillRect(x + 14, y+1, 3, 2, color);
    break;
  case 2:
    matrix->fillRect(x, y, 3, 3, color);
    matrix->fillRect(x + 4, y, 3, 3, color);
    matrix->fillRect(x + 10, y, 3, 3, color);
    matrix->fillRect(x + 14, y, 3, 3, color);
    break;
  case 1:
    matrix->fillRect(x, y, 3, 5, color);
    matrix->fillRect(x + 4, y, 3, 5, color);
    matrix->fillRect(x + 10, y, 3, 5, color);
    matrix->fillRect(x + 14, y, 3, 5, color);
    break;
  case 0:
    break;
  default:
    break;
  }
}