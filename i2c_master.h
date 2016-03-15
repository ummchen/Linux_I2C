#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#ifdef __cplusplus
extern "C" {
#endif

struct i2c_master {
	unsigned int	bus_id;		//ID for /dev/i2c-*
	unsigned char	dev_id;		//Slave Address ID
	int		dev_fd;		//File Descriptor
	unsigned int	addr_bytes;	//N Bytes of Register Address
};

int i2c_master_open(struct i2c_master *i2c_master);
int i2c_master_close(struct i2c_master *i2c_master);
int i2c_master_read(struct i2c_master *i2c_master, unsigned int addr, unsigned char *data, unsigned int data_len);
int i2c_master_write(struct i2c_master *i2c_master, unsigned int addr, unsigned char *data, unsigned int data_len);

#ifdef __cplusplus
}
#endif

#endif
