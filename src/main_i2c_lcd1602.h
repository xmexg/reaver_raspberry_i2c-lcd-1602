#ifndef MAIN_I2C_LCD1602_H
#define MAIN_I2C_LCD1602_H

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>

// IIC LCD1602 液晶模块写入字
void makerobo_write_word(int data);

// IIC LCD1602 发送命令
void makerobo_send_command(int comm);

// IIC LCD1602 发送数据
void makerobo_send_data(int data);

// IIC LCD1602 初始化
void makerobo_init();

// LCD 1602 清空显示函数
void makerobo_clear();

// LCD 1602 显示函数
void makerobo_write(int lcd_x, int lcd_y, char data[]);

//初始化，要把这一段在第一次发送文字之前运行一次
void i2c_initialize();

// 主函数
int main_lcd1602(int x,int y,char *text,int clear);
#endif