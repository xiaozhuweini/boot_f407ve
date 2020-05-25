/*占用的存储空间
**flash			:固件(0--1048575)
*/



#include "rtthread.h"
#include "stm32f4xx.h"
#include "drv_w25qxxx.h"
#include "drv_mempool.h"
#include "drv_fyyp.h"
#include "drv_hex.h"
#include "rthw.h"



#define UPDATE_FIRMWARE_IS_VALID		0xa55a5aa5
#define UPDATE_APP_ADDR					0x8020000
#define UPDATE_FIRMWARE_ADDR			0			//固件在flash中的位置
#define UPDATE_BYTES_FIRMWARE_MAX		1048576		//固件占用的最大字节空间，1MB
#define UPDATE_BYTES_DATA_BUF			1000		//一次处理的数据量
#define UPDATE_BYTES_ITEM_ASCII_MAX		45			//":xx...xx\r\n"
#define UPDATE_BYTES_ITEM_ASCII_MIN		13			//":xx...xx\r\n"
#define UPDATE_BYTES_ITEM_HEX			21
#define UPDATE_BYTES_FIRMWARE_INFO		8			//固件信息(有效标志和长度)



static void _update_firmware_erase(void)
{
	w25qxxx_write(UPDATE_FIRMWARE_ADDR, (rt_uint8_t *)_update_firmware_erase, 4);
}

//数据为n行hex文件数据，ascii码
static rt_uint8_t _update_firmware_handler(rt_uint8_t const *pdata, rt_uint32_t data_len, rt_uint8_t (*fun_hex_item_handler)(rt_uint8_t const *pdata, rt_uint8_t data_len))
{
	rt_uint8_t	*pmem, mem_len;
	rt_uint32_t	len;

	if(0 == data_len)
	{
		return RT_FALSE;
	}
	
	while(data_len)
	{
		//找出一条数据，ascii码，长度len
		for(len = 0; len < data_len; len++)
		{
			if(0x0a == pdata[len])
			{
				break;
			}
		}
		if(len >= data_len)
		{
			return RT_FALSE;
		}
		len++;
		if((len < UPDATE_BYTES_ITEM_ASCII_MIN) || (len > UPDATE_BYTES_ITEM_ASCII_MAX))
		{
			return RT_FALSE;
		}
		if((':' != *pdata) || (0x0d != pdata[len - 2]))
		{
			return RT_FALSE;
		}
		//申请hex数据空间，提取hex数据
		pmem = (rt_uint8_t *)mempool_req(UPDATE_BYTES_ITEM_HEX, RT_WAITING_FOREVER);
		while((rt_uint8_t *)0 == pmem);
		mem_len = fyyp_str_to_array(pmem, pdata + 1, len - 3);
		if(0 == mem_len)
		{
			rt_mp_free((void *)pmem);
			return RT_FALSE;
		}
		//对hex数据进行处理
		if(RT_FALSE == fun_hex_item_handler(pmem, mem_len))
		{
			rt_mp_free((void *)pmem);
			return RT_FALSE;
		}
		rt_mp_free((void *)pmem);
		//处理下一条
		pdata		+= len;
		data_len	-= len;
	}

	return RT_TRUE;
}



int main(void)
{
	rt_uint32_t		flash_addr = UPDATE_FIRMWARE_ADDR;
	rt_uint32_t		firmware_valid, firmware_len, pos, len;
	rt_uint8_t		*pmem;
	void (*fun_jump_to_app)(void);

	while(1)
	{
		//固件有效标志，如无有效固件则跳转至app
		if(W25QXXX_ERR_NONE != w25qxxx_read(flash_addr, (rt_uint8_t *)&firmware_valid, sizeof(rt_uint32_t)))
		{
			break;
		}
		if(UPDATE_FIRMWARE_IS_VALID != firmware_valid)
		{
			break;
		}
		flash_addr += sizeof(rt_uint32_t);
		//固件长度
		if(W25QXXX_ERR_NONE != w25qxxx_read(flash_addr, (rt_uint8_t *)&firmware_len, sizeof(rt_uint32_t)))
		{
			_update_firmware_erase();
			break;
		}
		if((firmware_len + UPDATE_BYTES_FIRMWARE_INFO) > UPDATE_BYTES_FIRMWARE_MAX)
		{
			_update_firmware_erase();
			break;
		}
		//固件校验
		flash_addr = UPDATE_FIRMWARE_ADDR + UPDATE_BYTES_FIRMWARE_INFO;
		pos	= 0;
		while(pos < firmware_len)
		{
			//本次处理的数据量
			len = firmware_len - pos;
			if(len > UPDATE_BYTES_DATA_BUF)
			{
				len = UPDATE_BYTES_DATA_BUF;
			}
			//申请数据空间
			pmem = (rt_uint8_t *)mempool_req(len, RT_WAITING_FOREVER);
			while((rt_uint8_t *)0 == pmem);
			//读取固件数据
			if(W25QXXX_ERR_NONE != w25qxxx_read(flash_addr, pmem, len))
			{
				rt_mp_free((void *)pmem);
				firmware_valid = 0;
				break;
			}
			//去掉多余的数据，保留整条的数据
			while(len)
			{
				if(0x0a == pmem[len - 1])
				{
					break;
				}
				len--;
			}
			if(0 == len)
			{
				rt_mp_free((void *)pmem);
				firmware_valid = 0;
				break;
			}
			//处理
			if(RT_FALSE == _update_firmware_handler(pmem, len, hex_item_data_verify))
			{
				rt_mp_free((void *)pmem);
				firmware_valid = 0;
				break;
			}
			rt_mp_free((void *)pmem);
			//下一包
			pos			+= len;
			flash_addr	+= len;
		}
		if(UPDATE_FIRMWARE_IS_VALID != firmware_valid)
		{
			_update_firmware_erase();
			break;
		}
		//固件校验成功，开始更新
		flash_addr = UPDATE_FIRMWARE_ADDR + UPDATE_BYTES_FIRMWARE_INFO;
		pos	= 0;
		while(pos < firmware_len)
		{
			//本次处理的数据量
			len = firmware_len - pos;
			if(len > UPDATE_BYTES_DATA_BUF)
			{
				len = UPDATE_BYTES_DATA_BUF;
			}
			//申请数据空间
			pmem = (rt_uint8_t *)mempool_req(len, RT_WAITING_FOREVER);
			while((rt_uint8_t *)0 == pmem);
			//读取固件数据
			if(W25QXXX_ERR_NONE != w25qxxx_read(flash_addr, pmem, len))
			{
				rt_mp_free((void *)pmem);
				_update_firmware_erase();
				while(1);
			}
			//去掉多余的数据，保留整条的数据
			while(len)
			{
				if(0x0a == pmem[len - 1])
				{
					break;
				}
				len--;
			}
			if(0 == len)
			{
				rt_mp_free((void *)pmem);
				_update_firmware_erase();
				while(1);
			}
			//更新
			if(RT_FALSE == _update_firmware_handler(pmem, len, hex_item_data_decoder))
			{
				rt_mp_free((void *)pmem);
				_update_firmware_erase();
				while(1);
			}
			rt_mp_free((void *)pmem);
			//下一包
			pos			+= len;
			flash_addr	+= len;
		}
		//固件更新成功，重启设备
		_update_firmware_erase();
		HAL_NVIC_SystemReset();
	}

	//栈顶地址
	firmware_valid = *(rt_uint32_t *)UPDATE_APP_ADDR;
	if(0x10000000 == (firmware_valid & 0xffff0000))
	{
		//跳转函数
		*(rt_uint32_t *)&fun_jump_to_app = *(rt_uint32_t *)(UPDATE_APP_ADDR + 4);
		
		rt_hw_interrupt_disable();
		//psp切换至msp
		flash_addr = __get_CONTROL();
		flash_addr &= 0xfffffffd;
		__set_CONTROL(flash_addr);
		//设置msp
		__set_MSP(firmware_valid);
		//跳转至app
		fun_jump_to_app();
	}

	while(1);
}

