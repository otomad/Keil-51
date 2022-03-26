#ifndef _18B20_H_
#define _18B20_H_

#define uchar unsigned char

#define rerom1820            0x33      // 读ROM指令   
#define matchrom1820         0x55      // 匹配ROM指令   
#define skiprom1820          0xCC      // 跳过ROM指令   
#define searchrom1820        0xF0      // 搜索ROM指令   
#define alarmsearch1820      0xEC      // 报警搜索指令   
#define wrscratchpad1820     0x4E      // 写暂存寄存器指令   
#define rescratchpad1820     0xBE      // 读暂存寄存器指令   
#define copyscratchpad1820   0x48      // 复制暂存寄存器指令   
#define convert1820          0x44      // 启动温度转换指令   
#define recalle21820         0xB8      // 重新调出E2PROM的数据 

/***********ds18b20延迟子函数*******/
extern void delaydq(uchar i);
/**********ds18b20初始化函数**********************/
extern void dqinit(void);
/***********ds18b20读一个字节**************/ 
extern uchar re1820b(void);
/*************ds18b20写一个字节****************/ 
extern void wr1820b(uchar dat);
extern float ReadTemp(void);
extern void temp_to_str();

#endif