# STM32-
基于STM32F10X的一些项目

1.音频滤波功放

采用的器件有：stm32f103c8t6最小系统板、w25q64存储模块、mcp4725芯片、0.96寸oled显示屏、矩阵键盘、音频放大滤波模块、音频滤波功放模块以及一个喇叭。


现象：可以对声音进行高频滤波在通过32最小系统板转换为AD值，然后通过spi协议对w25q64通讯，将采集到的ad值进行存储。并且能将存储的声音读取通过mcp4725芯片转换为模拟电压，经过音频滤波功放可以将声音扩大至10几倍，然后通过喇叭放音。oled显示屏显示必要的信息。

2.实时监测水源系统

器件：stm32f103c8t6最小系统板、tftlcd80x160屏幕、ph检测模块、esp01WIFI模块、浑浊度检测模块

现象：将水质以及温度实时上传到OneNET云平台，采用MQTT协议
