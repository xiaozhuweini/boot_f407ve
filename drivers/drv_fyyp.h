#ifndef __DRV_FYYP_H__
#define __DRV_FYYP_H__



#include "rtthread.h"



rt_uint8_t fyyp_hex_to_bcd(rt_uint8_t hex);
rt_uint8_t fyyp_bcd_to_hex(rt_uint8_t bcd);
rt_uint8_t fyyp_is_bcdcode(rt_uint8_t const *pdata, rt_uint16_t data_len);
rt_uint32_t fyyp_str_to_hex(rt_uint8_t const *pdata, rt_uint8_t data_len);
rt_uint8_t fyyp_is_number(rt_uint8_t const *pdata, rt_uint16_t data_len);
rt_uint8_t fyyp_ip_str_to_bcd(rt_uint8_t *pdst, rt_uint8_t const *psrc, rt_uint8_t src_len);
rt_uint8_t fyyp_sms_str_to_bcd(rt_uint8_t *pdst, rt_uint8_t const *psrc, rt_uint8_t src_len);
rt_uint8_t fyyp_char_to_hex(rt_uint8_t c);
rt_uint16_t fyyp_str_to_array(rt_uint8_t *pdst, rt_uint8_t const *psrc, rt_uint16_t src_len);



#endif

