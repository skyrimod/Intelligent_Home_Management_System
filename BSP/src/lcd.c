#include "stdlib.h"
#include "lcd.h"
#include "lcdfont.h"
#include "cmsis_os2.h"

SRAM_HandleTypeDef g_sram_handle;    /* SRAM句柄(用于控制LCD) */

/* LCD的画笔颜色和背景色 */
uint32_t g_point_color = 0XF800;    /* 画笔颜色 */
uint32_t g_back_color  = 0XFFFF;    /* 背景色 */

/* 管理LCD重要参数 */
_lcd_dev lcddev;

/* LCD写数据 */
void lcd_wr_data(volatile uint16_t data) {
    LCD->LCD_RAM = data;
}

/* LCD写寄存器编号/地址函数 , 本质是写命令 */
void lcd_wr_regno(volatile uint16_t regno) {
    LCD->LCD_REG = regno;
}

/* LCD写寄存器的值 */
void lcd_write_reg(uint16_t regno, uint16_t data) {
    LCD->LCD_REG = regno;
    LCD->LCD_RAM = data;
}

void LCD_WriteRAM_Prepare(void)
{
    lcd_wr_regno(lcddev.wramcmd);
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    lcd_wr_regno(lcddev.setxcmd);
    lcd_wr_data(Xpos>>8);
    lcd_wr_data(0x00FF&Xpos);
    lcd_wr_data((Xpos+1)>>8);
    lcd_wr_data((Xpos+1));

    lcd_wr_regno(lcddev.setycmd);
    lcd_wr_data(Ypos>>8);
    lcd_wr_data(0x00FF&Ypos);
    lcd_wr_data((Ypos+1)>>8);
    lcd_wr_data((Ypos+1));
    LCD_WriteRAM_Prepare();	//开始写入GRAM
}

void LCD_DrawPoint_16Bit(uint16_t color)
{
    lcd_wr_data(color);
}

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
    LCD_SetCursor(x,y);
    lcd_wr_data(g_point_color);
}

void LCD_DrawPoint_Color(uint16_t x,uint16_t y, uint16_t color) {
    LCD_SetCursor(x,y);
    lcd_wr_data(color);
}


void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x2 - x1;          /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;   /* 设置单步方向 */
    else if (delta_x == 0)incx = 0; /* 垂直线 */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; /* 水平线 */
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x;  /* 选取基本增量坐标轴 */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )   /* 画线输出 */
    {
        LCD_DrawPoint(row, col); /* 画点 */
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

void lcd_ili9486_init(void)
{
    //************* Start Initial Sequence **********//
    lcd_wr_regno(0XF9);
    lcd_wr_data(0x00);
    lcd_wr_data(0x08);

    lcd_wr_regno(0xC0);
    lcd_wr_data(0x19);//VREG1OUT POSITIVE
    lcd_wr_data(0x1a);//VREG2OUT NEGATIVE

    lcd_wr_regno(0xC1);
    lcd_wr_data(0x45);//VGH,VGL    VGH>=14V.
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC2);
    lcd_wr_data(0x33);

    lcd_wr_regno(0XC5);
    lcd_wr_data(0x00);
    lcd_wr_data(0x28);//VCM_REG[7:0]. <=0X80.

    lcd_wr_regno(0xB1);
    lcd_wr_data(0xA0);//0XB0 =70HZ, <=0XB0.0xA0=62HZ
    lcd_wr_data(0x11);

    lcd_wr_regno(0xB4);
    lcd_wr_data(0x02); //2 DOT FRAME MODE,F<=70HZ.

    lcd_wr_regno(0xB6);
    lcd_wr_data(0x00);
    lcd_wr_data(0x42);//0 GS SS SM ISC[3:0];
    lcd_wr_data(0x3B);


    lcd_wr_regno(0xB7);
    lcd_wr_data(0x07);

    lcd_wr_regno(0xE0);
    lcd_wr_data(0x1F);
    lcd_wr_data(0x25);
    lcd_wr_data(0x22);
    lcd_wr_data(0x0B);
    lcd_wr_data(0x06);
    lcd_wr_data(0x0A);
    lcd_wr_data(0x4E);
    lcd_wr_data(0xC6);
    lcd_wr_data(0x39);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0XE1);
    lcd_wr_data(0x1F);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x1F);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x46);
    lcd_wr_data(0x49);
    lcd_wr_data(0x31);
    lcd_wr_data(0x05);
    lcd_wr_data(0x09);
    lcd_wr_data(0x03);
    lcd_wr_data(0x1C);
    lcd_wr_data(0x1A);
    lcd_wr_data(0x00);

    lcd_wr_regno(0XF1);
    lcd_wr_data(0x36);
    lcd_wr_data(0x04);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x0F);
    lcd_wr_data(0xA4);
    lcd_wr_data(0x02);

    lcd_wr_regno(0XF2);
    lcd_wr_data(0x18);
    lcd_wr_data(0xA3);
    lcd_wr_data(0x12);
    lcd_wr_data(0x02);
    lcd_wr_data(0x32);
    lcd_wr_data(0x12);
    lcd_wr_data(0xFF);
    lcd_wr_data(0x32);
    lcd_wr_data(0x00);

    lcd_wr_regno(0XF4);
    lcd_wr_data(0x40);
    lcd_wr_data(0x00);
    lcd_wr_data(0x08);
    lcd_wr_data(0x91);
    lcd_wr_data(0x04);

    lcd_wr_regno(0XF8);
    lcd_wr_data(0x21);
    lcd_wr_data(0x04);

    lcd_wr_regno(0x36);
    lcd_wr_data(0x48);

    lcd_wr_regno(0x3A);
    lcd_wr_data(0x55);

    lcd_wr_regno(0x11);
    osDelay(120);
    lcd_wr_regno(0x29);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    GPIO_InitTypeDef gpio_init_struct = {0};

    // 使能时钟
    __HAL_RCC_FSMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();           /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();           /* 使能GPIOE时钟 */

    /* 初始化PD0,1,8,9,10,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 \
                           | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                  /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;                      /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;            /* 高速 */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);                  /* 初始化 */

    /* 初始化PE7,8,9,10,11,12,13,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 \
                           | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
}

void lcd_gpio_init(void) {
    /* 初始化LCD引脚 */
    GPIO_InitTypeDef gpio_init_struct = {0};
    FSMC_NORSRAM_TimingTypeDef fsmc_read_handle = {0};
    FSMC_NORSRAM_TimingTypeDef fsmc_write_handle = {0};

    // 使能时钟
    LCD_CS_GPIO_CLK_ENABLE();   /* LCD_CS脚时钟使能 */
    LCD_WR_GPIO_CLK_ENABLE();   /* LCD_WR脚时钟使能 */
    LCD_RD_GPIO_CLK_ENABLE();   /* LCD_RD脚时钟使能 */
    LCD_RS_GPIO_CLK_ENABLE();   /* LCD_RS脚时钟使能 */
    LCD_BL_GPIO_CLK_ENABLE();   /* LCD_BL脚时钟使能 */
    LCD_RST_GPIO_CLK_ENABLE();

    // 初始化CS引脚
    gpio_init_struct.Pin = LCD_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpio_init_struct);

    // 初始化WR引脚
    gpio_init_struct.Pin = LCD_WR_GPIO_PIN;
    HAL_GPIO_Init(LCD_WR_GPIO_PORT, &gpio_init_struct);

    // 初始化RD引脚
    gpio_init_struct.Pin = LCD_RD_GPIO_PIN;
    HAL_GPIO_Init(LCD_RD_GPIO_PORT, &gpio_init_struct);

    // 初始化RS引脚
    gpio_init_struct.Pin = LCD_RS_GPIO_PIN;
    HAL_GPIO_Init(LCD_RS_GPIO_PORT, &gpio_init_struct);

    // 初始化BL引脚
    gpio_init_struct.Pin = LCD_BL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(LCD_BL_GPIO_PORT, &gpio_init_struct);

    /* 初始化FSMC */
    g_sram_handle.Instance = FSMC_NORSRAM_DEVICE;
    g_sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

    g_sram_handle.Init.NSBank = FSMC_NORSRAM_BANK4;                        /* 使用NE4 */
    g_sram_handle.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;     /* 地址/数据线不复用 */
    g_sram_handle.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;				   /* 内存类型为SRAM */
    g_sram_handle.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;    /* 16位数据宽度 */
    g_sram_handle.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;   /* 是否使能突发访问,仅对同步突发存储器有效,此处未用到 */
    g_sram_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW; /* 等待信号的极性,仅在突发模式访问下有用 */
    g_sram_handle.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;      /* 存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT */
    g_sram_handle.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;              /* 等待使能位,此处未用到 */
    g_sram_handle.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;       /* 存储器写使能 */
    g_sram_handle.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;           /* 读写使用不同的时序 */
    g_sram_handle.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;  /* 是否使能同步传输模式下的等待信号,此处未用到 */
    g_sram_handle.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;              /* 禁止突发写 */

    // FSMC读时序控制寄存器
    fsmc_read_handle.AddressSetupTime = 1; 				// 地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    fsmc_read_handle.AddressHoldTime = 0; 				// 地址保持时间（ADDHLD）模式A未用到
    fsmc_read_handle.DataSetupTime = 15;				// 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    fsmc_read_handle.AccessMode = FSMC_ACCESS_MODE_A;	// 模式A

    // FSMC写时序控制寄存器
    fsmc_write_handle.AddressSetupTime = 2;
    fsmc_write_handle.DataSetupTime = 5;				// 某些液晶驱动IC的写信号脉宽，最少也得50ns
    fsmc_write_handle.AccessMode = FSMC_ACCESS_MODE_A;

    HAL_SRAM_Init(&g_sram_handle, &fsmc_read_handle, &fsmc_write_handle);
    osDelay(50);        /* 初始化FSMC后,必须等待一定时间才能开始初始化 */
}

void LCD_RESET(void)
{
    LCD_RST_CLR;
    osDelay(100);
    LCD_RST_SET;
    osDelay(50);
}

void LCD_SetParam(void)
{
    lcddev.setxcmd=0x2A;
    lcddev.setycmd=0x2B;
    lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏
    lcddev.dir=1;//横屏
    lcddev.width=480;
    lcddev.height=320;
    lcd_write_reg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//竖屏
    lcddev.dir=0;//竖屏
    lcddev.width=320;
    lcddev.height=480;
    lcd_write_reg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
#endif
}

void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
    lcd_wr_regno(lcddev.setxcmd);
    lcd_wr_data(xStar>>8);
    lcd_wr_data(0x00FF&xStar);
    lcd_wr_data(xEnd>>8);
    lcd_wr_data(0x00FF&xEnd);

    lcd_wr_regno(lcddev.setycmd);
    lcd_wr_data(yStar>>8);
    lcd_wr_data(0x00FF&yStar);
    lcd_wr_data(yEnd>>8);
    lcd_wr_data(0x00FF&yEnd);

    LCD_WriteRAM_Prepare();	//开始写入GRAM
}


void LCD_Clear(uint16_t color) {
    uint32_t index = 0;

    LCD_SetWindows(0,0,lcddev.width-1, lcddev.height-1);
    for(index=0; index<153600; index++)
    {
        LCD->LCD_RAM=color;
    }
}

/* 初始化LCD */
void lcd_init(void) {
    lcd_gpio_init();
    LCD_RESET();

    /* 初始化LCD */
    lcd_ili9486_init();
    LCD_SetParam();//设置LCD参数
    LCD_BL(1);
    LCD_Clear(WHITE);
}

/**
 * @brief       在指定位置显示一个字符
 * @param       x,y  : 坐标
 * @param       chr  : 要显示的字符:" "--->"~"
 * @param       size : 字体大小 12/16/24/32
 * @param       mode : 叠加方式(1); 非叠加方式(0);
 * @param       color : 字符的颜色;
 * @retval      无
 */
void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = 0;
    uint8_t *pfont = 0;

    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* 得到字体一个字符对应点阵集所占的字节数 */
    chr = chr - ' ';    /* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[chr];  /* 调用1206字体 */
            break;

        case 16:
            pfont = (uint8_t *)asc2_1608[chr];  /* 调用1608字体 */
            break;

        case 24:
            pfont = (uint8_t *)asc2_2412[chr];  /* 调用2412字体 */
            break;

        case 32:
            pfont = (uint8_t *)asc2_3216[chr];  /* 调用3216字体 */
            break;

        default:
            return ;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];    /* 获取字符的点阵数据 */

        for (t1 = 0; t1 < 8; t1++)   /* 一个字节8个点 */
        {
            if (temp & 0x80)        /* 有效点,需要显示 */
            {
                LCD_DrawPoint_Color(x, y, color);        /* 画点出来,要显示这个点 */
            }
            else if (mode == 0)     /* 无效点,不显示 */
            {
                LCD_DrawPoint_Color(x, y, g_back_color); /* 画背景色,相当于这个点不显示(注意背景色由全局变量控制) */
            }

            temp <<= 1; /* 移位, 以便获取下一个位的状态 */
            y++;

            if (y >= lcddev.height)return;  /* 超区域了 */

            if ((y - y0) == size)   /* 显示完一列了? */
            {
                y = y0; /* y坐标复位 */
                x++;    /* x坐标递增 */

                if (x >= lcddev.width)return;   /* x坐标超区域了 */

                break;
            }
        }
    }
}

/**
 * @brief       平方函数, m^n
 * @param       m: 底数
 * @param       n: 指数
 * @retval      m的n次方
 */
static uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}

/**
 * @brief       显示len个数字
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @param       color : 数字的颜色;
 * @retval      无
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;   /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', size, 0, color);/* 显示空格,占位 */
                continue;   /* 继续下个一位 */
            }
            else
            {
                enshow = 1; /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, 0, color); /* 显示字符 */
    }
}

/**
 * @brief       扩展显示len个数字(高位是0也显示)
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @param       mode: 显示模式
 *              [7]:0,不填充;1,填充0.
 *              [6:1]:保留
 *              [0]:0,非叠加显示;1,叠加显示.
 * @param       color : 数字的颜色;
 * @retval      无
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;    /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                if (mode & 0X80)   /* 高位需要填充0 */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', size, mode & 0X01, color);  /* 用0占位 */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01, color);  /* 用空格占位 */
                }

                continue;
            }
            else
            {
                enshow = 1; /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, mode & 0X01, color);
    }
}

/**
 * @brief       显示字符串
 * @param       x,y         : 起始坐标
 * @param       width,height: 区域大小
 * @param       size        : 选择字体 12/16/24/32
 * @param       p           : 字符串首地址
 * @param       color       : 字符串的颜色;
 * @retval      无
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   /* 判断是不是非法字符! */
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break;  /* 退出 */

        lcd_show_char(x, y, *p, size, 0, color);
        x += size / 2;
        p++;
    }
}

















