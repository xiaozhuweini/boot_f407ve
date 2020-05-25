#ifndef __DRV_HEX_H__
#define __DRV_HEX_H__



#include "rtthread.h"



#define HEX_APP_ADDR					0x8020000
#define HEX_BYTES_PER_SECTOR			0x20000
#define HEX_BYTES_ITEM_DATA_MIN			5
#define HEX_BYTES_ITEM_DATA_MAX			21

#define HEX_TYPE_DATA_RECORD			0				//���ݼ�¼
#define HEX_TYPE_END_FILE				1				//�ļ�����
#define HEX_TYPE_EXTEND_SEGMENT			2				//��չ�ε�ַ
#define HEX_TYPE_START_SEGMENT			3				//��ʼ�ε�ַ
#define HEX_TYPE_EXTEND_LINEAR			4				//��չ�ߵ�ַ
#define HEX_TYPE_START_LINEAR			5				//��ʼ�ߵ�ַ



rt_uint8_t hex_item_data_verify(rt_uint8_t const *pdata, rt_uint8_t data_len);
rt_uint8_t hex_item_data_decoder(rt_uint8_t const *pdata, rt_uint8_t data_len);



#endif
