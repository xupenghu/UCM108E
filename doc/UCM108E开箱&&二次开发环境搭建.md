# UCM108E开箱&&二次开发环境搭建

## 简介

御芯微UCM108E开发板，是搭载自研UCM108E模组的全功能开发板，可用于有MCU和GNSS定位需求的物联网方案开发。该模组集RISC-V MCU和GPS/北斗三号导航定位为一体，内部集成32位RISC-V处理器、32位浮点运算单元(FPU)、PMU、DCDC、PLL、温度传感器、大容量闪存等，外设支持UART、SPI、PWM、GPIO、I2C，ADC和DAC等，具有低成本、高性能、小尺寸的特点。

## 特点

### RISC-V 32bit CPU核心，带浮点运算单元FPU

- 163MHz最高运行率（FPU及通信DSP除外）
- 支持单周期乘法，硬件整数除法
- 支持RISC-V  IMFC指令集，以及特殊整型扩展指令
- FPU/DSP@131Mhz
- 支持PUF（芯片指纹）
- 国密SM2/3/7/9
- AES128/DES
- CRC/FFT/卷积编译码硬件加速

### 存储器

- 4Mb嵌入式NOR 闪存，数据代码统一存储
- 256KB 数据SRAM
- DSP 内嵌独立128KB Data SRAM

### 低功耗模式

- 内嵌RTC及PMU控制各种低功耗模式
- 主要部件单独时钟门控
- 外设时钟门控
- RTC单独运行低功耗模式

### SPI SLAVE烧录及调试接口

- 烧录端口IO可复用
- 支持4个硬件断点

### 时钟

- 内置DCXO振荡器，需外接晶体
- 内置32Khz RC振荡器，同时内置32Khz晶体振荡器（需外接晶体）
- 内置PLL，对DCXO时钟倍频，同时CPU支持DCXO/PLL时钟切换

### 常用外设

- 2x 16650 兼容UART
- 2x32位计数器
- SPI Master，最多4个外设片选
- 4xPWM 模块，独立输出
- GPIO口，可复用IO端口
- I2C接口
- 3路12bit ADC
- 10bit DAC以及辅助DAC

### GNSS定位

- GPS
- QZSS
- 北斗三号

### 电源/芯片管理

- 内置DCDC转换器
- 内置核心LDO， IO LDO
- 内置锂电池充电功能
- 片内温度检测

## 开发板简介

![image-20220612220129091](.\image\image-20220612220129091.png)

如上图所示，开发板的设计非常精简，主要由UCM108E模组、烧录口和天线组成，并且将所有的IO都外接方便测试。开发板的原理图请点击[这里](./UCM108E开发板_原理图.pdf)获取。

## 开发环境搭建

### 准备工作

- PC一台
- UCM108E开发板一块
- UChip烧录器一个
- [codelite开发环境](https://uc8088.com/t/topic/49)
- [烧录器驱动替换工具](https://uc8088.com/t/topic/49)

### codelite软件安装

1. 双击上面下载的exe可执行文件，安装急成开发环境

![img](.\image\setup1.png)

2. 按照提示一路next直到安装完成，这里没啥注意的安装完成即可

![img](.\image\setup2.png)

![img](.\image\setup3.png)

### 烧录器驱动安装

UCchip系列芯片采用UC-DAP5仿真器，需要安装串口驱动和调试口驱动。安装驱动时，PC必须连接上烧写器，否则安装失败。打开刚才安装的开发环境的安装目录，我们的需要的烧录器驱动都在driver目录下

![image-20220612221852328](.\image\image-20220612221852328.png)

1. 双击CDM21228_Setup.exe，安装串口驱动。这个也比较简单，一直next直到安装完成

![img](.\image\setup4.png)

![img](.\image\setup6.png)

![img](.\image\setup7.png)

2. 双击UC_DAP_Setup.exe，安装串口驱动，这个也是一样一直next即可。

![img](.\image\setup8.png)

3. 全部驱动安装完成后，需要打开[烧录器驱动替换工具](https://uc8088.com/t/topic/49)，选择Options->List All Devices:

![img](.\image\zadig_p1.png)

​			选择 DualRS232-HS(Interface 1)

![img](.\image\zadig_p2.png)

​			将原先的FTDIBUS驱动替换为WinUSB的驱动，点击Replace Driver开始运行

​			![img](.\image\zadig_p3.png)

4. 驱动安装完成后，打开设备管理器，看到如下设备，即表示安装成功

![image-20220612222614018](.\image\image-20220612222614018.png)

> 注意：如果前面的驱动安装步骤都正确，但是设备管理器还是提示未识别的USB设备，可以换一根MicroUSB线缆试一下。

至此，开发环境安装完成。

## 代码烧录

如果上面的步骤都执行完毕，那么我们就可以开始二次开发之旅了。

1. 打开codelite开发环境，选择Open Workspace，打开本项目

![img](.\image\open_workspace.png)

2. 打开的项目如下所示

![image-20220612223154169](.\image\image-20220612223154169.png)

3. 尝试编译本项目，如果编译出错，则可能是没有配置编译工具链。

### 配置编译工具链

1. 点击上方Setting->Build Settigs..

![img](.\image\build_setting_p1.png)

2. 点击+添加一个新的工具链，然后到本软件的安装目录下，找到这个工具链，点击添加

![image-20220612223701716](.\image\image-20220612223701716.png)

3. 为本项目选择编译工具链

![image-20220612223858402](.\image\image-20220612223858402.png)

4. 编译项目

![image-20220612224000085](.\image\image-20220612224000085.png)

### 烧录

1. 编译完成后，需要烧录到开发板

![image-20220612224143335](.\image\image-20220612224143335.png)

> 如果是第一次安装codelite，这个工具栏可能并不会显示，需要在菜单栏点击View-->Show Tool Bar

2. 如果出现以下弹窗，则表示烧录成功

![image-20220612224409645](.\image\image-20220612224409645.png)

3. 按下开发板上的复位按键，接上串口助手(**115200 8N1**)，有如下打印，则表示烧录成功

![image-20220612224546918](.\image\image-20220612224546918.png)