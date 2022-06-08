/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-08     balanceTWK   first version
 */

#include <board.h>

#ifdef RT_USING_I2C

#include "drv_soft_i2c.h"
//#include "drv_config.h"
#include "uc_timer.h"

//#define DRV_DEBUG
#define LOG_TAG              "drv.i2c"
#include <drv_log.h>

#if !defined(BSP_USING_I2C1) && !defined(BSP_USING_I2C2) && !defined(BSP_USING_I2C3) && !defined(BSP_USING_I2C4)
#error "Please define at least one BSP_USING_I2Cx"
/* this driver can be disabled at menuconfig → RT-Thread Components → Device Drivers */
#endif

static const struct uc8088_soft_i2c_config soft_i2c_config[] =
{
#ifdef BSP_USING_I2C1
    I2C1_BUS_CONFIG,
#endif
#ifdef BSP_USING_I2C2
    I2C2_BUS_CONFIG,
#endif
#ifdef BSP_USING_I2C3
    I2C3_BUS_CONFIG,
#endif
#ifdef BSP_USING_I2C4
    I2C4_BUS_CONFIG,
#endif
};

static struct uc8088_i2c i2c_obj[sizeof(soft_i2c_config) / sizeof(soft_i2c_config[0])];

/**
 * This function initializes the i2c pin.
 *
 * @param uc8088 i2c dirver class.
 */
static void uc8088_i2c_gpio_init(struct uc8088_i2c* i2c)
{
    struct uc8088_soft_i2c_config* cfg = (struct uc8088_soft_i2c_config*)i2c->ops.data;

    rt_pin_mode(cfg->scl, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(cfg->sda, PIN_MODE_OUTPUT_OD);

    rt_pin_write(cfg->scl, PIN_HIGH);
    rt_pin_write(cfg->sda, PIN_HIGH);
}

/**
 * This function sets the sda pin.
 *
 * @param uc8088 config class.
 * @param The sda pin state.
 */
static void uc8088_set_sda(void* data, rt_int32_t state)
{
    struct uc8088_soft_i2c_config* cfg = (struct uc8088_soft_i2c_config*)data;
    rt_pin_mode(cfg->sda, PIN_MODE_OUTPUT_OD);
    if (state)
    {
        rt_pin_write(cfg->sda, PIN_HIGH);
    }
    else
    {
        rt_pin_write(cfg->sda, PIN_LOW);
    }
}

/**
 * This function sets the scl pin.
 *
 * @param uc8088 config class.
 * @param The scl pin state.
 */
static void uc8088_set_scl(void* data, rt_int32_t state)
{
    struct uc8088_soft_i2c_config* cfg = (struct uc8088_soft_i2c_config*)data;
    rt_pin_mode(cfg->scl, PIN_MODE_OUTPUT_OD);
    if (state)
    {
        rt_pin_write(cfg->scl, PIN_HIGH);
    }
    else
    {
        rt_pin_write(cfg->scl, PIN_LOW);
    }
}

/**
 * This function gets the sda pin state.
 *
 * @param The sda pin state.
 */
static rt_int32_t uc8088_get_sda(void* data)
{
    struct uc8088_soft_i2c_config* cfg = (struct uc8088_soft_i2c_config*)data;
    rt_pin_mode(cfg->sda, PIN_MODE_INPUT);
    return rt_pin_read(cfg->sda);
}

/**
 * This function gets the scl pin state.
 *
 * @param The scl pin state.
 */
static rt_int32_t uc8088_get_scl(void* data)
{
    struct uc8088_soft_i2c_config* cfg = (struct uc8088_soft_i2c_config*)data;
    rt_pin_mode(cfg->scl, PIN_MODE_INPUT);
    return rt_pin_read(cfg->scl);
}
/**
 * The time delay function.
 *
 * @param microseconds.
 */
static void uc8088_udelay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t Compare_Value = ((rt_uint32_t)BSP_CLOCK_SYSTEM_FREQ_HZ)  / (5 * RT_TICK_PER_SECOND);

    ticks = us * Compare_Value / (1000000 / RT_TICK_PER_SECOND);
    told = TIMER_GetCount(UC_TIMER0);
    while (1)
    {
        tnow = TIMER_GetCount(UC_TIMER0);
        if (tnow != told)
        {
            if (tnow > told)
            {
                tcnt += tnow - told;
            }
            else
            {
                tcnt += Compare_Value - told + tnow;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

static const struct rt_i2c_bit_ops uc8088_bit_ops_default =
{
    .data     = RT_NULL,
    .set_sda  = uc8088_set_sda,
    .set_scl  = uc8088_set_scl,
    .get_sda  = uc8088_get_sda,
    .get_scl  = uc8088_get_scl,
    .udelay   = uc8088_udelay,
    .delay_us = 1,
    .timeout  = 100
};

/**
 * if i2c is locked, this function will unlock it
 *
 * @param uc8088 config class
 *
 * @return RT_EOK indicates successful unlock.
 */
static rt_err_t uc8088_i2c_bus_unlock(const struct uc8088_soft_i2c_config* cfg)
{
    rt_int32_t i = 0;

    rt_pin_mode(cfg->scl, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(cfg->sda, PIN_MODE_INPUT);
    if (PIN_LOW == rt_pin_read(cfg->sda))
    {
        while (i++ < 9)
        {
            rt_pin_write(cfg->scl, PIN_HIGH);
            uc8088_udelay(100);
            rt_pin_write(cfg->scl, PIN_LOW);
            uc8088_udelay(100);
        }
    }
    if (PIN_LOW == rt_pin_read(cfg->sda))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

/* I2C initialization function */
int rt_hw_i2c_init(void)
{
    rt_size_t obj_num = sizeof(i2c_obj) / sizeof(struct uc8088_i2c);
    rt_err_t result = RT_EOK;

    for (int i = 0; i < obj_num; i++)
    {
        i2c_obj[i].ops = uc8088_bit_ops_default;
        i2c_obj[i].ops.data = (void*)&soft_i2c_config[i];
        i2c_obj[i].i2c2_bus.priv = &i2c_obj[i].ops;
        uc8088_i2c_gpio_init(&i2c_obj[i]);
        result = rt_i2c_bit_add_bus(&i2c_obj[i].i2c2_bus, soft_i2c_config[i].bus_name);
        RT_ASSERT(result == RT_EOK);
        uc8088_i2c_bus_unlock(&soft_i2c_config[i]);

        LOG_D("software simulation %s init done, pin scl: %d, pin sda %d",
              soft_i2c_config[i].bus_name,
              soft_i2c_config[i].scl,
              soft_i2c_config[i].sda);
    }

    return result;
}
INIT_BOARD_EXPORT(rt_hw_i2c_init);

#endif /* RT_USING_I2C */
