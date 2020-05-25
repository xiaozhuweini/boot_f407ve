#include "drv_mempool.h"



#ifdef MEMPOOL_128
static struct rt_mempool	m_mempool_128_ctrl;
static rt_uint8_t			m_mempool_128_pool[MEMPOOL_128_POOL_SIZE];
#endif

#ifdef MEMPOOL_256
static struct rt_mempool	m_mempool_256_ctrl;
static rt_uint8_t			m_mempool_256_pool[MEMPOOL_256_POOL_SIZE];
#endif

#ifdef MEMPOOL_512
static struct rt_mempool	m_mempool_512_ctrl;
static rt_uint8_t			m_mempool_512_pool[MEMPOOL_512_POOL_SIZE];
#endif

#ifdef MEMPOOL_1024
static struct rt_mempool	m_mempool_1024_ctrl;
static rt_uint8_t			m_mempool_1024_pool[MEMPOOL_1024_POOL_SIZE];
#endif



//≥ı ºªØ
static int _mempool_init(void)
{
	rt_err_t err;
	
#ifdef MEMPOOL_128
	err = rt_mp_init(&m_mempool_128_ctrl, "mem128", (void *)m_mempool_128_pool, MEMPOOL_128_POOL_SIZE, MEMPOOL_128_BLOCK_SIZE);
	while(RT_EOK != err);
#endif
	
#ifdef MEMPOOL_256
	err = rt_mp_init(&m_mempool_256_ctrl, "mem256", (void *)m_mempool_256_pool, MEMPOOL_256_POOL_SIZE, MEMPOOL_256_BLOCK_SIZE);
	while(RT_EOK != err);
#endif
	
#ifdef MEMPOOL_512
	err = rt_mp_init(&m_mempool_512_ctrl, "mem512", (void *)m_mempool_512_pool, MEMPOOL_512_POOL_SIZE, MEMPOOL_512_BLOCK_SIZE);
	while(RT_EOK != err);
#endif
	
#ifdef MEMPOOL_1024
	err = rt_mp_init(&m_mempool_1024_ctrl, "mem1024", (void *)m_mempool_1024_pool, MEMPOOL_1024_POOL_SIZE, MEMPOOL_1024_BLOCK_SIZE);
	while(RT_EOK != err);
#endif

	return 0;
}
INIT_PREV_EXPORT(_mempool_init);



//…Í«Îƒ⁄¥Ê
void *mempool_req(rt_uint16_t bytes_req, rt_int32_t ticks)
{
	void *pmem;
	
	if(0 == bytes_req)
	{
		return (void *)0;
	}
	
#ifdef MEMPOOL_128
	if(bytes_req <= MEMPOOL_128_BLOCK_SIZE)
	{
		pmem = rt_mp_alloc(&m_mempool_128_ctrl, ticks);
		if((void *)0 != pmem)
		{
			return pmem;
		}
		else if(RT_WAITING_NO != ticks)
		{
			return pmem;
		}
	}
#endif
	
#ifdef MEMPOOL_256
	if(bytes_req <= MEMPOOL_256_BLOCK_SIZE)
	{
		pmem = rt_mp_alloc(&m_mempool_256_ctrl, ticks);
		if((void *)0 != pmem)
		{
			return pmem;
		}
		else if(RT_WAITING_NO != ticks)
		{
			return pmem;
		}
	}
#endif
	
#ifdef MEMPOOL_512
	if(bytes_req <= MEMPOOL_512_BLOCK_SIZE)
	{
		pmem = rt_mp_alloc(&m_mempool_512_ctrl, ticks);
		if((void *)0 != pmem)
		{
			return pmem;
		}
		else if(RT_WAITING_NO != ticks)
		{
			return pmem;
		}
	}
#endif
	
#ifdef MEMPOOL_1024
	if(bytes_req <= MEMPOOL_1024_BLOCK_SIZE)
	{
		pmem = rt_mp_alloc(&m_mempool_1024_ctrl, ticks);
		if((void *)0 != pmem)
		{
			return pmem;
		}
		else if(RT_WAITING_NO != ticks)
		{
			return pmem;
		}
	}
#endif

	return (void *)0;
}

