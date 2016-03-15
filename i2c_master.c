/********************************
* Module:       I2C_MASTER      *
* Author:       Josh Chen       *
* Date:         2016/02/17      *
********************************/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


#include "i2c_master.h"

#define BUF_SIZE	32
static unsigned char sz_buf[BUF_SIZE] = {0};

int i2c_master_open(struct i2c_master *i2c_master)
{
	if (i2c_master == NULL)
		return 0;

	sprintf(sz_buf, "/dev/i2c-%d", i2c_master->bus_id);
	i2c_master->dev_fd = open(sz_buf, O_RDWR, 666);
	if (i2c_master->dev_fd < 0)
		return 0;

	return 1;
}

int i2c_master_close(struct i2c_master *i2c_master)
{
	close(i2c_master->dev_fd);
	return 1;
}

int i2c_master_read(struct i2c_master *i2c_master, unsigned int addr, unsigned char *data, unsigned int data_len)
{
	int i = 0, rtn = 1;
	struct i2c_rdwr_ioctl_data	i2c_rdwr;
	struct i2c_msg			i2c_msg[2];

	if (i2c_master->addr_bytes <= 0 || i2c_master->addr_bytes > 4)
	{
		printf("i2c_master - address bytes out of range\n");
		return 0;
	}

	if (data_len <= 0)
	{
		printf("data length out of range\n");
		return 0;
	}

	memset(sz_buf, 0, sizeof(sz_buf));
	for (i = 0; i < i2c_master->addr_bytes; i++)
		sz_buf[i] = (char) (addr >> (8 * (i2c_master->addr_bytes-i-1))) & 0xFF;

	i2c_msg[0].addr		= i2c_master->dev_id;
	i2c_msg[0].flags	= 0;	//0 for write
	i2c_msg[0].len		= i2c_master->addr_bytes;
	i2c_msg[0].buf		= sz_buf;
	
	i2c_msg[1].addr		= i2c_master->dev_id;
	i2c_msg[1].flags	= I2C_M_RD;
	i2c_msg[1].len		= data_len;
	i2c_msg[1].buf		= data;

	i2c_rdwr.msgs 	= i2c_msg;
	i2c_rdwr.nmsgs	= 2;

	if (ioctl(i2c_master->dev_fd, I2C_RDWR, &i2c_rdwr) < 0)
		return 0;

	return rtn;
}

int i2c_master_write(struct i2c_master *i2c_master, unsigned int addr, unsigned char *data, unsigned int data_len)
{
	int write_len = 0, i = 0, rtn = 1;
	struct i2c_rdwr_ioctl_data	i2c_rdwr;
	struct i2c_msg			i2c_msg[1];

	if (i2c_master->addr_bytes <= 0 || i2c_master->addr_bytes > 4)
	{
		printf("i2c_master - address bytes out of range\n");
		return 0;
	}

	write_len = i2c_master->addr_bytes + data_len;
	if (data_len <= 0 || write_len >= BUF_SIZE)
	{
		printf("i2c_master - data length out of range\n");
		return 0;
	}

	memset(sz_buf, 0, sizeof(sz_buf));
	for (i = 0; i < i2c_master->addr_bytes; i++)
		sz_buf[i] = (char) (addr >> (8 * (i2c_master->addr_bytes-i-1))) & 0xFF;

	memcpy(sz_buf + i2c_master->addr_bytes, data, data_len);

	i2c_msg[0].addr		= i2c_master->dev_id;
	i2c_msg[0].flags	= 0;	//0 for write
	i2c_msg[0].len		= write_len;
	i2c_msg[0].buf		= sz_buf;

	i2c_rdwr.msgs 	= i2c_msg;
	i2c_rdwr.nmsgs	= 1;

	if (ioctl(i2c_master->dev_fd, I2C_RDWR, &i2c_rdwr) < 0)
		return 0;

	return rtn;
}
