#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <main_i2c_lcd1602.h>

int makerobo_LCDAddr = 0x27;   // LCD 1602 液晶IIC地址 0x27或者0x3F
int makerobo_BLEN = 1;         // 写入标志位
int makerobo_fd;

// IIC LCD1602 液晶模块写入字
void makerobo_write_word(int data){
	int temp = data;
	if ( makerobo_BLEN == 1 )
		temp |= 0x08;
	else
		temp &= 0xF7;
	wiringPiI2CWrite(makerobo_fd, temp);   //设置IIC LCD1602 液晶模块地址
}

// IIC LCD1602 发送命令
void makerobo_send_command(int comm){
	int lcd_buf;
	// 首先发送 bit7-4 位
	lcd_buf = comm & 0xF0;
	lcd_buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);

	// 其次发送 bit3-0 位
	lcd_buf = (comm & 0x0F) << 4;
	lcd_buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);
}

// IIC LCD1602 发送数据
void makerobo_send_data(int data){
	int lcd_buf;
	// 首先发送 bit7-4 位
	lcd_buf = data & 0xF0;
	lcd_buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);

	// 其次发送 bit3-0 位
	lcd_buf = (data & 0x0F) << 4;
	lcd_buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);
}

// IIC LCD1602 初始化
void makerobo_init(){
	makerobo_send_command(0x33);	// 必须先初始化到8线模式吗
	delay(5);
	makerobo_send_command(0x32);	// 然后初始化为4行模式
	delay(5);
	makerobo_send_command(0x28);	// 2 行 & 5*7 点位
	delay(5);
	makerobo_send_command(0x0C);	// 启用无光标显示
	delay(5);
	//makerobo_send_command(0x01);	// 清除显示
	wiringPiI2CWrite(makerobo_fd, 0x08);
}

// LCD 1602 清空显示函数
void makerobo_clear()
{
	makerobo_send_command(0x01);	// 清除显示
}

// LCD 1602 显示函数
void makerobo_write(int lcd_x, int lcd_y, char data[]){
	int lcd_addr, lcd_i;
	int lcd_tmp;
	// 选择行与列
	if (lcd_x < 0)  lcd_x = 0;
	if (lcd_x > 15) lcd_x = 15;
	if (lcd_y < 0)  lcd_y = 0;
	if (lcd_y > 1)  lcd_y = 1;

	// 移动光标
	lcd_addr = 0x80 + 0x40 * lcd_y + lcd_x;
	
	makerobo_send_command(lcd_addr); //发送地址
	
	lcd_tmp = strlen(data);         // 获取字符串长度
	for (lcd_i = 0; lcd_i < lcd_tmp; lcd_i++){  // 依次发送
		makerobo_send_data(data[lcd_i]);    // 逐一显示
	}
}

// void thestart(int printline,char *text){
// 	//makerobo_clear();                                 // 清空显示
// 	makerobo_fd = wiringPiI2CSetup(makerobo_LCDAddr);  //初始化地址
// 	makerobo_init();                                  //初始化显示屏
// 	char firstline[] = "Hello!";
// 	char secondline[] = "I'm qingguo.";
// 	while(1)
// 	{
// 		printf("请输入第一行:");
// 		fgets(firstline,16,stdin);					//显示器只能显示16个字
// 		printf("请输入第二行:");
// 		fgets(secondline,15,stdin);
// 		//makerobo_clear();                                 // 清空显示
// 		makerobo_write(0, 0, text);                    //第一行显示
// 		makerobo_write(0, 1, secondline);              //第二行显示
// 		delay(2000);                                      // 延时2s
// 	}
// }

//初始化，要把这一段在第一次发送文字之前运行一次
void i2c_initialize() {
	makerobo_fd = wiringPiI2CSetup(makerobo_LCDAddr);  //初始化地址
	makerobo_init();                                  //初始化显示屏
	makerobo_clear();                                 // 清空显示	
}

// 主函数
int main_lcd1602(int x,int y,char *text,int clear){
	//void thestart(int,char *);
	//char thetext[]="Good";
	//thestart(1,thetext);
	if(clear){
		makerobo_clear();                                 // 清空显示
	 //puts("清空显示器");
	}
	//printf("[@] 向1602传入了：x=%d,y=%d,char=%s\n",x,y,text);
	if(x>16) x=16;
	if(x<0) x=0;
	if(y>1) y=1;
	if(y<0) y=0;
	makerobo_fd = wiringPiI2CSetup(makerobo_LCDAddr);  //初始化地址
	makerobo_init();                                  //初始化显示屏
	//makerobo_clear();                                 // 清空显示
	makerobo_write(x, y, text);                    //第一行显示
	//makerobo_write(0, 1, secondline);              //第二行显示
	delay(500);                                      // 延时0.3s
	return 0;
}
