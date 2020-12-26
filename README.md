# GMatrix像素显示屏

## 背景

之前疫情期间(2020-02~2020-03)做过一个大号的像素屏幕作为生日礼物送人了，当时使用的芯片是ESP32，开发语言是micropython。
那个初号机的性能表现不是很好，所以计划做一个使用C/C++的版本，能够更加流畅的运行。

如果只是纯粹的重写固件，那么和大多数人做的AWatrix就没多大区别了，因此决定做一个迷你版本的像素显示屏出来。

很多地方是参考了AWatrix的代码，因为AWatrix除了必须连接服务器才可以使用以外，真的是非常优秀的一个固件。
AWatrix使用到的一些库很具有参考价值，比FastLED_NexPixel如它把LED点阵当作一块"屏幕"来使用，可以很方便的绘制出一些基础图形。
开发工具为PlantformIO,也是一个很方便的工具了，VSCode装上插件就好了。

# 开始整活

## 0x01 绘制电路板
万能的某宝上能够找到的内置WS2812的LED贴片灯最小的就是2020LED，长宽各为2mm，比常规AWatrix使用的5050或者3535小很多。
奈何没有现成品，就只能自己画一个电路板了，好在电路非常简单，就是一个接一个，然后绕成S型就好啦。

这次选择的PCB绘制软件为Eagle PCB，因为免费小巧。（但是我打算以后用KiCad画电路图了，因为开源强大）
磨磨唧唧的画完了电路图。
<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/awatrix_pcb.png" width = 960/>

然后提交Gerber文件到厂家，等待3天后就收到了成品。

## 0x02 焊接LED贴片灯

这一步没什么好说的，我用的是锡膏，熔点在182度，于是先涂抹在板子上，然后风枪对着就是一阵吹。

大概花费了半天时间才焊完所有的贴片灯，因为实在是太麻烦了啊，不小心就碰掉了一个，然后发生一系列连锁反应。
<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/%E7%84%8A%E6%8E%A5.jpg" width = 960/>



## 0x03 获取代码，修改参数
代码位于[Github](https://github.com/chaosgoo/GMatrixSource)
下载以后，使用PlantformIO导入工程。

* 修改位于gmatrix_config.h中的B站ID和城市以及天气的Key

````cpp
#define SUNING_TIME_URL "http://quan.suning.com/getSysTime.do"
#define BILIBILI_SUBSCRIPTION_URL "http://api.bilibili.com/x/relation/stat?vmid=哔哩哔哩UID"
#define WEATHER_URL "http://api.openweathermap.org/data/2.5/weather?q=城市的拼音&appid=天气api"
````

* 修改位于Gmartix.cpp中的WiFi名称和密码。
````cpp
// WiFi名称
const char *ssid = "CloseWrt_9.6G_D96AS5";
// WiFi密码
const char *pwd = "have5seeds";
````
然后直接使用PlantformIO的编译或者下载到板子

## 0x04 烧录固件

使用PlantformIO可以直接下载到板子上，也可也Build以后使用其他烧录工具烧录。
我使用的烧录软件是NodeMCU Firmware Programmer
<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/nodemcu.png"/>

插上板子选择对应的端口号，然后点击Flash就可以进行烧录了。

## 0x05 接线&组装

### 材料清单

 序号 | 名称 | 数量 | 备注
:-: | :-:| :-: | :-:
1 | WeMos D1 mini | 1 | 无
2 | LED板 | 1 | 自制
3 | htu01d | 1 | 温湿度传感器
4 | 导线 | ? | 无
5 | 3D打印外壳 | 1| 无
6 | 亚克力光栅 | 1 | 无
7 | A4纸 | 1 | 无
8 | 深色半透亚克力 | 1 | 无
9 | 后盖 | 1 | 可以采用任意材料自制

### 接线

 WeMos D1 mini引脚名称 | htu01d
:-:| :-:
5V | VCC
GND | GND
D3 | SDA 
D1  | SCL

 WeMos D1 mini引脚名称 | LED屏幕
:-:| :-:
不接 | 正面从上至下第2个孔
GND | 正面从上至下第2个孔
5V | 正面从上至下第3个孔
D2 | 正面从上至下第4个孔

 WeMos D1 mini引脚名称 | 散热风扇
:-:| :-:
5V | 红线
GND | 黑线

<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/%E7%BB%84%E8%A3%85.jpg" width = 960/>

光栅制作使用的是定制亚克力，而外壳使用的3D打印，按照下图的顺序组装，需要注意的是光栅与深色半透亚克力之间需要加一层A4纸进行柔光。

<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/%E7%BB%93%E6%9E%84.png?x-oss-process=style/compress_50" width=960/>

<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/%E6%89%93%E5%8D%B0.jpg?x-oss-process=style/compress_50" width=960/>


## 0x06成品展示

<img src="https://chaosgoo-pic.oss-cn-shanghai.aliyuncs.com/blog_pic/gmatrix/%E6%88%90%E5%93%81.jpg?x-oss-process=style/compress_50" width=960/>

[给自己做个超小的8x32像素显示屏吧](https://www.bilibili.com/video/BV1q54y1v7gH)


[PCB文件(Eagle PCB)](https://github.com/chaosgoo/GMatrixPCB)
