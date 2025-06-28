#ifndef __LCD_H
#define __LCD_H

#include "stdlib.h"
#include "stm32f1xx.h"
/*********** 引脚配置 WR RD BL CS RS ***********/
// WR写使能 FSMC_NWE
#define LCD_WR_GPIO_PORT 			GPIOD
#define LCD_WR_GPIO_PIN 			GPIO_PIN_5
#define LCD_WR_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

// RD读使能 FSMC_NOE
#define LCD_RD_GPIO_PORT			GPIOD
#define LCD_RD_GPIO_PIN 			GPIO_PIN_4
#define LCD_RD_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

// BL背光 LCD_BL
#define LCD_BL_GPIO_PORT			GPIOB
#define LCD_BL_GPIO_PIN				GPIO_PIN_0
#define LCD_BL_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)


// CS片选信号 FSMC_NE4
#define LCD_CS_GPIO_PORT			GPIOG
#define LCD_CS_GPIO_PIN				GPIO_PIN_12
#define LCD_CS_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

// RS命令or数据，FSMC_A10
#define LCD_RS_GPIO_PORT			GPIOG
#define LCD_RS_GPIO_PIN				GPIO_PIN_0
#define LCD_RS_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

// 设置复位
#define LCD_RST_GPIO_PORT               GPIOC
#define LCD_RST_GPIO_PIN                GPIO_PIN_5
#define LCD_RST_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

#define	LCD_RST_SET	GPIOC->BSRR=1<<5    //复位			PC5
#define	LCD_RST_CLR	GPIOC->BRR=1<<5    //复位			PC5

/* FSMC相关参数 */
#define LCD_FSMC_NEX	4		/* 使用FSMC_NE4接LCD_CS,取值范围只能是: 1~4 */
#define LCD_FSMC_AX		10		/* 使用FSMC_A10接LCD_RS,取值范围是: 0 ~ 25 */

#define LCD_FSMC_BCRX		FSMC_Bank1->BTCR[(LCD_FSMC_NEX - 1) * 2]			// BCT控制寄存器
#define LCD_FSMC_BTRX		FSMC_Bank1->BTCR[(LCD_FSMC_NEX - 1) * 2 + 1]		// BTR读时序寄存器
#define LCD_FSMC_BWTRX		FSMC_Bank1E->BWTR[(LCD_FSMC_NEX - 1) * 2]			// BWTR写时序寄存器

/******************************************************************************************/

/* LCD重要参数集 */
typedef struct {
    uint16_t width;		/* LCD 宽度 */
    uint16_t height;	/* LCD 高度 */
    uint16_t id;		/* LCD ID */
    uint8_t dir;		/* 横屏还是竖屏控制：0，竖屏；1，横屏。 */

    uint16_t wramcmd;	/* 开始写gram指令 */
    uint16_t setxcmd;	/* 设置x坐标指令 */
    uint16_t setycmd;	/* 设置y坐标指令 */
} _lcd_dev;

/* LCD参数 */
extern _lcd_dev lcddev;

/* LCD的画笔颜色和背景色 */
extern uint32_t  g_point_color;     /* 默认红色 */
extern uint32_t  g_back_color;      /* 背景颜色.默认为白色 */

/* LCD背光控制 */
#define LCD_BL(x)	do{ x ? \
						HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_SET) : \
						HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_RESET); \
					}while(0)

/* LCD地址结构体 */
typedef struct {
    volatile uint16_t LCD_REG;		// 命令地址
    volatile uint16_t LCD_RAM;		// 数据地址
} LCD_TypeDef;

/* LCD_BASE的详细解算方法:
 * 我们一般使用FSMC的块1(BANK1)来驱动TFTLCD液晶屏(MCU屏), 块1地址范围总大小为256MB,均分成4块:
 * 存储块1(FSMC_NE1)地址范围: 0X6000 0000 ~ 0X63FF FFFF
 * 存储块2(FSMC_NE2)地址范围: 0X6400 0000 ~ 0X67FF FFFF
 * 存储块3(FSMC_NE3)地址范围: 0X6800 0000 ~ 0X6BFF FFFF
 * 存储块4(FSMC_NE4)地址范围: 0X6C00 0000 ~ 0X6FFF FFFF
 *
 * 我们需要根据硬件连接方式选择合适的片选(连接LCD_CS)和地址线(连接LCD_RS)
 * 战舰F103开发板使用FSMC_NE4连接LCD_CS, FSMC_A10连接LCD_RS ,16位数据线,计算方法如下:
 * 首先FSMC_NE4的基地址为: 0X6C00 0000;     NEx的基址为(x=1/2/3/4): 0X6000 0000 + (0X400 0000 * (x - 1))
 * FSMC_A10对应地址值: 2^10 * 2 = 0X800;    FSMC_Ay对应的地址为(y = 0 ~ 25): 2^y * 2
 *
 * LCD->LCD_REG,对应LCD_RS = 0(LCD寄存器); LCD->LCD_RAM,对应LCD_RS = 1(LCD数据)
 * 则 LCD->LCD_RAM的地址为:  0X6C00 0000 + 2^10 * 2 = 0X6C00 0800
 *    LCD->LCD_REG的地址可以为 LCD->LCD_RAM之外的任意地址.
 * 由于我们使用结构体管理LCD_REG 和 LCD_RAM(REG在前,RAM在后,均为16位数据宽度)
 * 因此 结构体的基地址(LCD_BASE) = LCD_RAM - 2 = 0X6C00 0800 -2
 *
 * 更加通用的计算公式为((片选脚FSMC_NEx)x=1/2/3/4, (RS接地址线FSMC_Ay)y=0~25):
 *          LCD_BASE = (0X6000 0000 + (0X400 0000 * (x - 1))) | (2^y * 2 -2)
 *          等效于(使用移位操作)
 *          LCD_BASE = (0X6000 0000 + (0X400 0000 * (x - 1))) | ((1 << y) * 2 -2)
 */

#define LCD_BASE 	(uint32_t)((0X60000000 + (0X4000000 * (LCD_FSMC_NEX - 1))) | (((1 << LCD_FSMC_AX) * 2) -2))
#define LCD			((LCD_TypeDef *) LCD_BASE)

/******************************************************************************************/
/* LCD扫描方向和颜色 定义 */

/* 扫描方向定义 */
#define L2R_U2D         0           /* 从左到右,从上到下 */
#define L2R_D2U         1           /* 从左到右,从下到上 */
#define R2L_U2D         2           /* 从右到左,从上到下 */
#define R2L_D2U         3           /* 从右到左,从下到上 */

#define U2D_L2R         4           /* 从上到下,从左到右 */
#define U2D_R2L         5           /* 从上到下,从右到左 */
#define D2U_L2R         6           /* 从下到上,从左到右 */
#define D2U_R2L         7           /* 从下到上,从右到左 */

#define DFT_SCAN_DIR    L2R_U2D     /* 默认的扫描方向 */

/* 常用画笔颜色 */
#define WHITE           0xFFFF      /* 白色 */
#define BLACK           0x0000      /* 黑色 */
#define RED             0xF800      /* 红色 */
#define GREEN           0x07E0      /* 绿色 */
#define BLUE            0x001F      /* 蓝色 */
#define MAGENTA         0XF81F      /* 品红色/紫红色 = BLUE + RED */
#define YELLOW          0XFFE0      /* 黄色 = GREEN + RED */
#define CYAN            0X07FF      /* 青色 = GREEN + BLUE */

/* 非常用颜色 */
#define BROWN           0XBC40      /* 棕色 */
#define BRRED           0XFC07      /* 棕红色 */
#define GRAY            0X8430      /* 灰色 */
#define DARKBLUE        0X01CF      /* 深蓝色 */
#define LIGHTBLUE       0X7D7C      /* 浅蓝色 */
#define GRAYBLUE        0X5458      /* 灰蓝色 */
#define LIGHTGREEN      0X841F      /* 浅绿色 */
#define LGRAY           0XC618      /* 浅灰色(PANNEL),窗体背景色 */
#define LGRAYBLUE       0XA651      /* 浅灰蓝色(中间层颜色) */
#define LBBLUE          0X2B12      /* 浅棕蓝色(选择条目的反色) */

/******************************************************************************************/
/* 函数申明 */

void lcd_wr_data(volatile uint16_t data);            /* LCD写数据 */
void lcd_wr_regno(volatile uint16_t regno);          /* LCD写寄存器编号/地址 */
void lcd_write_reg(uint16_t regno, uint16_t data);   /* LCD写寄存器的值 */

void lcd_init(void);                        /* 初始化LCD */
void LCD_Clear(uint16_t color);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint_Color(uint16_t x,uint16_t y, uint16_t color);
void LCD_DrawPoint_16Bit(uint16_t color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_WriteRAM_Prepare(void);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);     /* 画直线 */

void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color);                       /* 显示一个字符 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);                     /* 显示数字 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);      /* 扩展显示数字 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);   /* 显示字符串 */




#endif

