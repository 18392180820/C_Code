#include "my_math.h"

/*******************************************************************************
* 函 数 名: find_max
* 函数功能: 获取一组数据最大值
* 输     入: data	 数据源
* 输     出: len	 数据长度
*******************************************************************************/
static int16_t find_max(int16_t* data, int len)
{
	uint16_t i = 0;
	int16_t maxV = data[0];

	for(i=1;i<len;i++)
	{
		if(maxV < data[i])
		{
			maxV = data[i];
		}
	}

	return maxV;
}

/*******************************************************************************
* 函 数 名: cal_average
* 函数功能: 获取一组数据平均值
* 输     入: data	 数据源
* 输     出: len	 数据长度
*******************************************************************************/
static int16_t cal_average(int16_t* data, int len)
{
	uint16_t i = 0;
	int32_t  sum = 0;

	for(i=0;i<len;i++)
	{
		sum += data[i];
	}

	return (sum/len);
}

/*******************************************************************************
* 函 数 名: bubbleSort
* 函数功能: 冒泡排序，从小到大排列
* 输     入: data	 数据源
* 输     出: len	 数据长度
*******************************************************************************/ 
static void bubbleSort (int16_t* data, int len) 
{
	uint16_t i=0;
	uint16_t j=0;
	int16_t max;

    int16_t temp;

    for (i=0; i<len-1; i++)		
	{						
        for (j=0; j<len-1-i; j++) 
		{ 
            if (data[j] > data[j+1]) 
			{ 
                temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}

/*******************************************************************************
* 函 数 名: find_mode
* 函数功能: 获取数组里的众数，如果有多个众数，则取平均(冒泡排序后)
* 输     入: data	 数据源
* 输     出: len	 数据长度
*******************************************************************************/ 
int16_t find_mode(int16_t* data, uint16_t len)
{	
	uint16_t i = 0;
	uint16_t value_num = 0;	// 记录数据源中不同数值的个数 
	uint16_t value[len];	// 记录单数据值
	uint16_t count[len];	// 记录单数据出现的次数	
	uint16_t count_max = 0;	// 众数个数值	
	uint16_t mode = 0;		// 记录众数值
	uint16_t mode_num = 0;	// 记录多众数值时候的众数个数
	
	memset(value, 0x00, sizeof(value));
	memset(count, 0x00, sizeof(count));
	bubbleSort(data, len);

#if 0	// log
	for(i=0;i<len;i++)
	{
		printf("%d ", data[i]);
	}
	printf("\n");
#endif

    for(i=0;i<len;i++)
    {	
		if(data[i] == data[i+1])
		{
			value[value_num] = data[i];
			count[value_num]++;
		}
		else
		{	
			value[value_num] = data[i];	// 不相同话、将自己的数据也记录，避免一堆数值中两两不同
			count[value_num]++;
			value_num++;	
		}	
	}
	
#if 0	// log 
	LOG_I("value_num = %d", value_num);	
	for(i=0;i<value_num;i++)
	{
		LOG_I("value = %d, count = %d", value[i], count[i]);
	}
#endif

	count_max = find_max(count, value_num);	// 获取众数值出现的次数

	for(i=0;i<value_num;i++)	// 多众数值时候的平均处理
	{
		if(count_max == count[i])
		{
			mode += value[i];
			mode_num++;
		}
	}

	//LOG_I("mode = %d", mode);
	return (mode/mode_num);		// 之前的数据处理、能确认mode_num不为0
}

/*******************************************************************************
* 函 数 名: cal_variance
* 函数功能: 计算数组方差
* 输     入: data	 数据源
* 输     出: len	 数据长度
*******************************************************************************/ 
uint32_t cal_variance(int16_t* data, uint16_t len)
{	
	uint16_t i = 0;
	int16_t  avg = 0;	// 平均值
	uint32_t var = 0;	// 方差
	
	avg = cal_average(data, len);

	LOG_I("avg value = %d", avg);
	for(i=0;i<len;i++)
	{
		//var += pow((data[i]-avg),2);
		var += (data[i]-avg)*(data[i]-avg);
	}

	return var;
}


#if 1
void find_mode_test(void)
{	
	uint16_t i = 0;
	//int16_t  array[20] = {22,11,3,4,5,7,2,3,4,10,1,3,4,5,3,22,22,4,22,20};
	int16_t  array[20] = {1,2,3,4,5,6,7,8,9,10, 21,20,19,18,17,16,15,14,13,12};
	//int16_t  array[20] = {1,2,3,4,5,6,7,8,9,10, 1,2,3,4,5,6,7,8,9,10};
	//LOG_I("mode value = %d", find_mode(array, 20));
	LOG_I("var value = %d", cal_variance(array, 20)+5);
}
#endif
