#include "drv_hex.h"
#include "stm32f4xx.h"
#include "drv_mempool.h"
#include "string.h"



//块擦除
static rt_uint8_t _hex_page_erase(rt_uint32_t flash_addr)
{
	FLASH_EraseInitTypeDef erase_init;
	
	if(flash_addr < HEX_APP_ADDR)
	{
		return RT_FALSE;
	}
	
	if(0 == (flash_addr - HEX_APP_ADDR) % HEX_BYTES_PER_SECTOR)
	{
		erase_init.TypeErase	= FLASH_TYPEERASE_SECTORS;
		erase_init.Banks		= FLASH_BANK_1;
		erase_init.Sector		= FLASH_SECTOR_5 + ((flash_addr - HEX_APP_ADDR) / HEX_BYTES_PER_SECTOR);
		erase_init.NbSectors	= 1;
		erase_init.VoltageRange	= FLASH_VOLTAGE_RANGE_3;
		HAL_FLASHEx_Erase(&erase_init, &flash_addr);
		if(0xffffffff != flash_addr)
		{
			return RT_FALSE;
		}
	}

	return RT_TRUE;
}

//数据编程
static rt_uint8_t _hex_data_program(rt_uint32_t flash_addr, rt_uint8_t const *pdata, rt_uint8_t data_len)
{
	rt_uint8_t const	*pmem = (rt_uint8_t *)0;
	rt_uint8_t			mem_len;
	
	if(flash_addr < HEX_APP_ADDR)
	{
		return RT_FALSE;
	}
	if(0 == data_len)
	{
		return RT_FALSE;
	}

	if(data_len % 4)
	{
		mem_len = data_len;
		mem_len /= 4;
		mem_len++;
		mem_len *= 4;
		pmem = (rt_uint8_t *)mempool_req(mem_len, RT_WAITING_FOREVER);
		while((rt_uint8_t *)0 == pmem);
		memset((void *)pmem, 0xff, mem_len);
		memcpy((void *)pmem, (void *)pdata, data_len);
		pdata		= pmem;
		data_len	= mem_len;
	}

	HAL_FLASH_Unlock();
	while(data_len)
	{
		_hex_page_erase(flash_addr);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addr, *(rt_uint32_t *)pdata);
		flash_addr	+= 4;
		pdata		+= 4;
		data_len	-= 4;
	}
	HAL_FLASH_Lock();

	if((rt_uint8_t *)0 != pmem)
	{
		rt_mp_free((void *)pmem);
	}

	return RT_TRUE;
}



//校验一条hex数据
rt_uint8_t hex_item_data_verify(rt_uint8_t const *pdata, rt_uint8_t data_len)
{
	rt_uint8_t crc = 0;

	if(0 == data_len)
	{
		return RT_FALSE;
	}

	while(data_len)
	{
		data_len--;
		crc += *pdata++;
	}

	return (0 == crc) ? RT_TRUE : RT_FALSE;
}

//hex数据解码
rt_uint8_t hex_item_data_decoder(rt_uint8_t const *pdata, rt_uint8_t data_len)
{
	static rt_uint32_t	base_addr = 0;
	rt_uint8_t			len;
	rt_uint16_t			addr;
	
	if((data_len < HEX_BYTES_ITEM_DATA_MIN) || (data_len > HEX_BYTES_ITEM_DATA_MAX))
	{
		return RT_FALSE;
	}
	if(RT_FALSE == hex_item_data_verify(pdata, data_len))
	{
		return RT_FALSE;
	}

	len = *pdata++;
	if((len + 5) != data_len)
	{
		return RT_FALSE;
	}
	addr = *pdata++;
	addr <<= 8;
	addr += *pdata++;
	switch(*pdata++)
	{
	case HEX_TYPE_DATA_RECORD:
		if(RT_FALSE == _hex_data_program(base_addr + addr, pdata, len))
		{
			return RT_FALSE;
		}
		break;
	case HEX_TYPE_EXTEND_LINEAR:
		if(2 != len)
		{
			return RT_FALSE;
		}
		base_addr = *pdata++;
		base_addr <<= 8;
		base_addr += *pdata;
		base_addr <<= 16;
		break;
	}

	return RT_TRUE;
}

