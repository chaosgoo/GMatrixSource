// 辅助显示的相关函数
#ifndef DISPLAYHELER_H
#define DISPLAYHELER_H

#include <FastLED_NeoMatrix.h>
#include <cstring>
#include <FastLED.h>

// 显示固件信息
void showFirmwareInfo(FastLED_NeoMatrix *matrix, int x, int y, bool clear);
// 显示开发者信息
void showChaosgoo(FastLED_NeoMatrix *matrix, bool clear);
// 显示指定内容
void showContent(FastLED_NeoMatrix *matrix, String str, int x, int y, int color, bool clear);
void showTemperature(FastLED_NeoMatrix *matrix, float temp, int x, int y, int color, bool clear);
void showHumidity(FastLED_NeoMatrix *matrix, float hum, int x, int y, int color, bool clear);
void showTime(FastLED_NeoMatrix *matrix, String time, int x, int y, int color, bool clear);
void showDate(FastLED_NeoMatrix *matrix, String date, int x, int y, int color, bool clear);
void showPlus(FastLED_NeoMatrix *matrix, int follower, int x, int y, int color, bool clear, int frame);
void showDecrease(FastLED_NeoMatrix *matrix, int follower, int x, int y, int color, bool clear, int frame);
void showClock(CRGB leds[],FastLED_NeoMatrix *matrix, int x, int y, int color, bool clear, int frame);
void showHeart(FastLED_NeoMatrix *matrix, int follower, int x, int y, int color, bool clear, int frame);
void showFirmwareInfoEnd(FastLED_NeoMatrix *matrix);
// 显示WiFi连接中动画帧
void showWiFiConnecting(FastLED_NeoMatrix *matrix, int status, bool clear);
// 显示WiFi连接完成动画
void showWiFiDoneAnimation(FastLED_NeoMatrix *matrix, bool clear);
// WiFi连接完成动画帧
void showWiFiConnectDone(FastLED_NeoMatrix *matrix, int status, bool clear);
void showAnimationString(FastLED_NeoMatrix *matrix, String content, int fromX, int fromY, int toX, int toY, int color);
// 局部清屏
void clearLocal(FastLED_NeoMatrix *matrix, int l, int t, int r, int b);
// 格式化粉丝数量字符串
const char *formatFollowers(const char *followers, int width, bool clear);
#endif
