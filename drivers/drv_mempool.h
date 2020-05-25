#ifndef __DRV_MEMPOOL__
#define __DRV_MEMPOOL__



#include "rtthread.h"



//�ڴ��
#define MEMPOOL_128
#define MEMPOOL_256
#define MEMPOOL_512
#define MEMPOOL_1024
#define MEMPOOL_2048

//�ڴ��128�ֽ�
#ifdef MEMPOOL_128
#define MEMPOOL_128_BLOCK_SIZE			128
#define MEMPOOL_128_BLOCK_NUM			15
#define MEMPOOL_128_POOL_SIZE			((MEMPOOL_128_BLOCK_SIZE + sizeof(rt_uint8_t *)) * MEMPOOL_128_BLOCK_NUM)
#endif

//�ڴ��256�ֽ�
#ifdef MEMPOOL_256
#define MEMPOOL_256_BLOCK_SIZE			256
#define MEMPOOL_256_BLOCK_NUM			10
#define MEMPOOL_256_POOL_SIZE			((MEMPOOL_256_BLOCK_SIZE + sizeof(rt_uint8_t *)) * MEMPOOL_256_BLOCK_NUM)
#endif

//�ڴ��512�ֽ�
#ifdef MEMPOOL_512
#define MEMPOOL_512_BLOCK_SIZE			512
#define MEMPOOL_512_BLOCK_NUM			10
#define MEMPOOL_512_POOL_SIZE			((MEMPOOL_512_BLOCK_SIZE + sizeof(rt_uint8_t *)) * MEMPOOL_512_BLOCK_NUM)
#endif

//�ڴ��1024�ֽ�
#ifdef MEMPOOL_1024
#define MEMPOOL_1024_BLOCK_SIZE			1100
#define MEMPOOL_1024_BLOCK_NUM			10
#define MEMPOOL_1024_POOL_SIZE			((MEMPOOL_1024_BLOCK_SIZE + sizeof(rt_uint8_t *)) * MEMPOOL_1024_BLOCK_NUM)
#endif

//�ڴ��2048�ֽ�
#ifdef MEMPOOL_2048
#define MEMPOOL_2048_BLOCK_SIZE			2000
#define MEMPOOL_2048_BLOCK_NUM			5
#define MEMPOOL_2048_POOL_SIZE			((MEMPOOL_2048_BLOCK_SIZE + sizeof(rt_uint8_t *)) * MEMPOOL_2048_BLOCK_NUM)
#endif



//�����ڴ�
void *mempool_req(rt_uint16_t bytes_req, rt_int32_t ticks);

//�ڴ����Ϣ
rt_uint16_t mempool_info(rt_uint8_t pool);



#endif

