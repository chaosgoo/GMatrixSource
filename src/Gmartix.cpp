// GMatrix
// Copyright (C) 2020
// by Chaosgoo

#include <FS.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Fonts/TomThumb.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include "displayHelper.h"
#include <ESP8266HTTPClient.h>
#include "gmatrix_config.h"

// 矩阵设置
CRGB leds[256];
FastLED_NeoMatrix *matrix;

// WiFi配置
WiFiClient wifiClient;
// WiFi名称
const char *ssid = "OpenWrt_2.4G_B76BC2";
// WiFi密码
const char *pwd = "need4speed";

// 全局变量
String timeChar = "00000000000000"; //YYYYMMDDHHMMSS
String followers = "0";             // 粉丝数量字符串
int followerTrend = -1;             // 粉丝数量变化趋势
bool firstLauncher = true;          // 初次启动
String HHMM = "0000";               // 时间
String MMDD = "0000";               // 日期
String weather = "cloud";           // 天气
int GlobalX = 0;                    // 全局X，摄影机
int GlobalY = 0;                    // 全局Y，摄影机
float temperature = 0.0f;           // 温度缓存
float humidity = 0.0f;              // 湿度缓存
int fun1 = 0;                       // 功能1状态值
int fun2 = 1;                       // 功能2状态值
int animationDeltaDir = 1;          // 用于模块切换的移动方向插值
int animationFrame = 60;            // 模块动画计数器
long gloablCounter = 0L;            // 全局计时器
// 手势传感器

#define I2C_SDA D3
#define I2C_SCL D1

volatile bool isr_flag = 1;

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// 初始化矩阵显示屏相关参数
void initMatrix()
{
  matrix = new FastLED_NeoMatrix(leds, 32, 8, NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
  FastLED.addLeds<NEOPIXEL, D2>(leds, 256).setCorrection(TypicalLEDStrip);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(50);
  matrix->setFont(&TomThumb);
}

// 初始化WiFi
void initWiFiClient()
{
  WiFi.begin(ssid, pwd);
  showWiFiConnecting(matrix, 0, true);
  int tempCounter = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    showWiFiConnecting(matrix, tempCounter, true);
    tempCounter++;
    tempCounter %= 4;
  }
  showWiFiDoneAnimation(matrix, true);
}

void debuggingWithMatrix(String text)
{
  matrix->setCursor(7, 6);
  matrix->clear();
  matrix->print(text);
  matrix->show();
}

void debuggingWithMatrix(int text)
{
  matrix->setCursor(7, 6);
  matrix->clear();
  matrix->print(text);
  matrix->show();
}

// 查询时间 ,仅作查询并不显示
void queryTimeBySuNing()
{
  // return;
  HTTPClient httpClient;
  httpClient.begin(wifiClient, SUNING_TIME_URL);
  int httpCode = httpClient.GET();
  String payload = httpClient.getString();
  httpClient.end();
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.parseObject(payload);
  if (json.success())
  {
    const char *time = json["sysTime1"];
    MMDD = String(time).substring(4, 8);
    HHMM = String(time).substring(8, 12);
  }
}

// 查询粉丝数量,仅作查询并不显示
void queryBiliBiliSubscription()
{
  // return;
  HTTPClient httpClient;
  httpClient.begin(wifiClient, BILIBILI_SUBSCRIPTION_URL);
  httpClient.addHeader("Content-Type", "application/json; charset=utf-8");
  // httpClient.addHeader("User-Agent", "PostmanRuntime/7.26.1");
  int httpCode = httpClient.GET();
  String payload = httpClient.getString();
  httpClient.end();
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.parseObject(payload);
  if (json.success())
  {
    const char *followers_ = json["data"]["follower"];
    if (!firstLauncher)
    {
      followerTrend = String(followers_).toInt() - followers.toInt();
    }
    else
    {
      followerTrend = followers.toInt();
    }
    followers = String(followers_);
    firstLauncher = false;
    // followerTrend = -1;
  }
}

// 查询天气 ,仅作查询并不显示
void queryWeather()
{
  // return;
  HTTPClient httpClient;
  httpClient.begin(wifiClient, WEATHER_URL);
  httpClient.addHeader("Content-Type", "application/json; charset=utf-8");
  httpClient.addHeader("User-Agent", "PostmanRuntime/7.26.1");
  int httpCode = httpClient.GET();
  String payload = httpClient.getString();
  httpClient.end();
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.parseObject(payload);
  // debuggingWithMatrix(httpCode);
  if (json.success())
  {
    const char *weather_ = json["weather"].asArray().get<JsonObject>(0)["main"];
    // showContent(matrix, weather, 0, 6, 0xff,true);
    weather = String(weather_);
  }
}

// 查询气温温湿度,仅作查询并不显示
void readHtuSensor()
{
  if (!htu.begin())
  {
    return;
  }
  humidity = htu.readHumidity();
  temperature = htu.readTemperature();
}

void ICACHE_RAM_ATTR interruptRoutine()
{
  isr_flag = 1;
}

// 分发绘制
void drawDispatch(int functionCode, int index)
{
  switch (functionCode)
  {
  case 0:
    showTime(matrix, HHMM, 8 + GlobalX, 7 + GlobalY - index * 8, 0xffff, false);
    break;
  case 1:
    showDate(matrix, MMDD, 8 + GlobalX, 7 + GlobalY - index * 8, 0xffff, false);
    break;
  case 2:
    showContent(matrix, weather, GlobalX, 7 + GlobalY - index * 8, 0xffff, false);
    break;
  case 3:
    showContent(matrix, followers, GlobalX + (8 - strlen(followers.c_str())) * 4, 7 + GlobalY - index * 8, matrix->Color(251, 114, 153), false);
    break;
  case 4:
    showTemperature(matrix, temperature, GlobalX, 7 + GlobalY - index * 8, 0xffff, false);
    break;
  case 5:
    showHumidity(matrix, humidity, GlobalX, 7 + GlobalY - index * 8, 0xffff, false);
    break;
  }
}

// 分发数据更新
void dataDispatch(int functionCode)
{
  // return;
  switch (functionCode)
  {
  case 0:
  case 1:
    animationFrame = 30;
    queryTimeBySuNing();
    break;
  case 2:
    animationFrame = 30;
    queryWeather();
    break;
  case 3:
    // 粉丝状态下显示60帧
    animationFrame = 60;
    queryBiliBiliSubscription();
    break;
  case 4:
  case 5:
    animationFrame = 30;
    readHtuSensor();
    break;
  }
}

// 分发模块动画
void animationDispatch(int functionCode)
{
  switch (functionCode)
  {
  case 0:
  case 1:
    // showClock(leds, matrix, 8, 3, matrix->Color(0, 0, 0), false, animationFrame);
    // delay(300);
    // queryTimeBySuNing();
    break;
  case 2:
    // queryWeather();
    break;
  case 3:
    int color;
    if (followerTrend > 0)
    {
      // 绿色加号
      color = matrix->Color(0, 255, 0);
      showPlus(matrix, followerTrend, 2, 2, color, false, animationFrame);
    }
    else if (followerTrend < 0)
    {
      // 红色减号
      color = matrix->Color(255, 0, 0);
      showDecrease(matrix, followerTrend, 2, 2, color, false, animationFrame);
      // debuggingWithMatrix(followerTrend);
    }
    else
    {
      // 无变化则显示跳动心
      color = matrix->Color(251, 114, 153);
      showHeart(matrix, followerTrend, 2, 2, color, false, animationFrame);
    }
    delay(64);
    break;
  case 4:
  case 5:
    // readHtuSensor();
    break;
  }
}

// 绘制画面
void draw()
{
  matrix->clear();
  drawDispatch(fun1, 0);
  drawDispatch(fun2, 1);
  if (animationFrame > 0)
  {
    // 模块动画状态，绘制额外内容
    if (GlobalY == 0)
    {
      animationDispatch(fun1);
    }
    else if (GlobalY == 8)
    {
      animationDispatch(fun2);
    }
    animationFrame--;
  }
  matrix->show();
}

// 生成下一个功能
void generateNxtFun()
{

  // fun2 = fun1 + 1
  if (GlobalY > 8)
  {
    animationDeltaDir = -1;
    fun1 = fun2;
    while (fun1 == fun2)
    {
      fun1 = rand() % 5;
    }
  }
  else if (GlobalY < 0)
  {
    animationDeltaDir = 1;
    fun2 = fun1;
    while (fun1 == fun2)
    {
      fun2 = rand() % 5;
    }
  }
}

// 数据更新
void update()
{
  if (animationFrame == 0)
  {
    // 非模块动画状态更新数据
    animationFrame = 60;
    GlobalY += animationDeltaDir;
    generateNxtFun();
    if (GlobalY == -1)
    {
      dataDispatch(fun2);
    }
    else if (GlobalY == 9)
    {
      dataDispatch(fun1);
    }
    else
    {
      animationFrame = 0;
    }
    GlobalY = GlobalY > 8 ? 8 : GlobalY;
    GlobalY = GlobalY < 0 ? 0 : GlobalY;
  }
  else
  {
    // 模块动画状态中不作更新
  }
}

void setup()
{
  initMatrix();
  initWiFiClient();
  showFirmwareInfo(matrix, 0, 0, true);
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(1000);

  showFirmwareInfoEnd(matrix);

  Serial.begin(19200);
  Serial.println("HTU21D-F test");
  dataDispatch(fun1);
  dataDispatch(fun2);
}

unsigned char serialBuf[256];

static const unsigned char pic[] =
    {
        0x00, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x00};

void badApple()
{
  // matrix->drawBitmap(0,0,pic,8,8,0xffff);
  //   matrix->show();

  // delay(1000);
  matrix->clear();

  if (Serial.available() > 0)
  {
    Serial.readBytes(serialBuf, 256);
    // Serial.write(serialBuf.c_str());
    int index = (int)(serialBuf[0]);
    // Serial.write(serialBuf,2);
    // matrix->clear();
    // leds[index].setRGB(127, 0, 0);
    matrix->drawBitmap(0, 0, serialBuf, 32, 8, 0xffff);
    // matrix->drawbitma
    matrix->show();
    // debuggingWithMatrix(String(serialBuf));
    delay(1000);
    index = 0;
    // matrix->clear();
    // matrix->show();
  }
}

void loop()
{
  draw();
  update();
  gloablCounter++;
  delay(16);

  // badApple();
}
