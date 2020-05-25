#ifndef __DRV_HEX_H__
#define __DRV_HEX_H__



#include "rtthread.h"



#define HEX_APP_ADDR					0x8020000
#define HEX_BYTES_PER_SECTOR			0x20000
#define HEX_BYTES_ITEM_DATA_MIN			5
#define HEX_BYTES_ITEM_DATA_MAX			21

#define HEX_TYPE_DATA_RECORD			0				//数据记录
#define HEX_TYPE_END_FILE				1				//文件结束
#define HEX_TYPE_EXTEND_SEGMENT			2				//扩展段地址
#define HEX_TYPE_START_SEGMENT			3				//开始段地址
#define HEX_TYPE_EXTEND_LINEAR			4				//扩展线地址
#define HEX_TYPE_START_LINEAR			5				//开始线地址



rt_uint8_t hex_item_data_verify(rt_uint8_t const *pdata, rt_uint8_t data_len);
rt_uint8_t hex_item_data_decoder(rt_uint8_t const *pdata, rt_uint8_t data_len);



#endif
