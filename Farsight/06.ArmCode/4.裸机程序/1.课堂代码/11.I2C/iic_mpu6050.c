//****************************************                                                                                         
//  MPU6050�ڲ���ַ                                                                                                                
//****************************************                                                                                         
#define SMPLRT_DIV      0x19    //�����ǲ����ʣ�����ֵ��0x07(125Hz)                                                                
#define CONFIG          0x1A    //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)                                                                  
#define GYRO_CONFIG     0x1B    //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)                                            
#define ACCEL_CONFIG    0x1C    //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)                                
#define ACCEL_XOUT_H    0x3B                                                                                                       
#define ACCEL_XOUT_L    0x3C                                                                                                       
#define ACCEL_YOUT_H    0x3D                                                                                                       
#define ACCEL_YOUT_L    0x3E                                                                                                       
#define ACCEL_ZOUT_H    0x3F                                                                                                       
#define ACCEL_ZOUT_L    0x40                                                                                                       
#define TEMP_OUT_H      0x41                                                                                                       
#define TEMP_OUT_L      0x42                                                                                                       
#define GYRO_XOUT_H     0x43                                                                                                       
#define GYRO_XOUT_L     0x44                                                                                                       
#define GYRO_YOUT_H     0x45                                                                                                       
#define GYRO_YOUT_L     0x46                                                                                                       
#define GYRO_ZOUT_H     0x47                                                                                                       
#define GYRO_ZOUT_L     0x48                                                                                                       
#define PWR_MGMT_1      0x6B    //��Դ��������ֵ��0x00(��������)                                                                 
#define WHO_AM_I        0x75    //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)                                                                
#define SlaveAddress    0xD0    //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ                                                                
void mydelay_ms(int time)                                                                                                          
{                                                                                                                                  
    int i, j;                                                                                                                      
    while(time--)                                                                                                                  
    {                                                                                                                              
        for (i = 0; i < 5; i++)                                                                                                    
            for (j = 0; j < 514; j++);                                                                                             
    }                                                                                                                              
}                                                                                                                                  
/**********************************************************************                                                            
 * @brief       iic read a byte program body                                                                                       
 * @param[in]   slave_addr, addr, &data                                                                                            
 * @return      None                                                                                                               
 **********************************************************************/                                                           
void iic_read(unsigned char slave_addr, unsigned char addr, unsigned char *data)                                                   
{                                                                                                                                  
    I2C5.I2CDS = slave_addr; //���ӻ���ַд��I2CDS�Ĵ�����                                                                         
    I2C5.I2CCON = (1 << 7)|(1 << 6)|(1 << 5); //����ʱ�Ӳ�ʹ���ж�                                                                 
    I2C5.I2CSTAT = 0xf0; //[7:6]����Ϊ0b11����������ģʽ��                                                                         
                         //��[5��4]λд0b11�������������ź�,����IICDS�Ĵ����еĵ�ַ                                                
    while(!(I2C5.I2CCON & (1 << 4))); // �ȴ�������������������I2CCON [4]λΪ1����ʶ���жϷ�����                               
                                      // ��λΪ1ʱ��SCL�߱����ͣ���ʱI2C����ֹͣ��                                                 
    I2C5.I2CDS = addr; //д����ֵ                                                                                                  
    I2C5.I2CCON = I2C5.I2CCON & (~(1 << 4)); // I2CCON [4]λ��0����������                                                          
    while(!(I2C5.I2CCON & (1 << 4))); // �ȴ��������                                                                              
    I2C5.I2CSTAT = 0xD0; // I2CSTAT[5:4]λд0b01,����ֹͣ�ź�                                                                      
                                                                                                                                   
    I2C5.I2CDS = slave_addr | 1; //��ʾҪ��������                                                                                  
    I2C5.I2CCON = (1 << 7)|(1 << 6) |(1 << 5) ; //����ʱ�Ӳ�ʹ���ж�                                                               
    I2C5.I2CSTAT = 0xb0; //[7:6]λ0b10,��������ģʽ��                                                                              
                         //��[5��4]λд0b11�������������ź�,����IICDS�Ĵ����еĵ�ַ                                                
    //I2C5.I2CCON = I2C5.I2CCON & (~(1 << 4));                                                                                     
    while(!(I2C5.I2CCON & (1 << 4))); //�ȴ������������������                                                                     
                                                                                                                                   
    I2C5.I2CCON &= ~((1<<7)|(1 << 4)); // I2CCON [4]λ��0���������䣬�������ݣ�                                                    
                                       // �������������յ����һ�ֽ����ݺ󣬲�����Ӧ���ź� no ack                                  
                                       // �ӻ��������ͷ�SDA�ߣ���������������P�źţ�ֹͣ���䣻                                     
    while(!(I2C5.I2CCON & (1 << 4)));  // �ȴ��������                                                                             
    *data = I2C5.I2CDS;                                                                                                            
    I2C5.I2CSTAT = 0x90;                                                                                                           
    I2C5.I2CCON &= ~(1<<4);       /*clean interrupt pending bit  */                                                                
                                                                                                                                   
}                                                                                                                                  
/**********************************************************************                                                            
 * @brief       iic write a byte program body                                                                                      
 * @param[in]   slave_addr, addr, data                                                                                             
 * @return      None                                                                                                               
 **********************************************************************/                                                           
void iic_write (unsigned char slave_addr, unsigned char addr, unsigned char data)                                                  
{                                                                                                                                  
    I2C5.I2CDS = slave_addr;                                                                                                       
    I2C5.I2CCON = (1 << 7)|(1 << 6)|(1 << 5) ;                                                                                     
    I2C5.I2CSTAT = 0xf0;                                                                                                           
    while(!(I2C5.I2CCON & (1 << 4)));                                                                                              
                                                                                                                                   
    I2C5.I2CDS = addr;                                                                                                             
    I2C5.I2CCON = I2C5.I2CCON & (~(1 << 4));                                                                                       
    while(!(I2C5.I2CCON & (1 << 4)));                                                                                              
                                                                                                                                   
    I2C5.I2CDS = data;                                                                                                             
    I2C5.I2CCON = I2C5.I2CCON & (~(1 << 4));                                                                                       
    while(!(I2C5.I2CCON & (1 << 4)));                                                                                              
                                                                                                                                   
    I2C5.I2CSTAT = 0xd0;                                                                                                           
    I2C5.I2CCON = I2C5.I2CCON & (~(1 << 4));                                                                                       
    mydelay_ms(10);                                                                                                                
                                                                                                                                   
}                                                                                                                                  
                                                                                                                                   
void MPU6050_Init ()                                                                                                               
{                                                                                                                                  
	    iic_write(SlaveAddress, PWR_MGMT_1, 0x00);                                                                                   
	    iic_write(SlaveAddress, SMPLRT_DIV, 0x07);                                                                                   
	    iic_write(SlaveAddress, CONFIG, 0x06);                                                                                       
	    iic_write(SlaveAddress, GYRO_CONFIG, 0x18);                                                                                  
	    iic_write(SlaveAddress, ACCEL_CONFIG, 0x01);                                                                                 
	}                                                                                                                                
	int get_data(unsigned char addr)                                                                                                 
	{                                                                                                                                
	    char data_h, data_l;                                                                                                         
	    iic_read(SlaveAddress, addr, &data_h);                                                                                       
	    iic_read(SlaveAddress, addr+1, &data_l);                                                                                     
	    return (data_h<<8)|data_l;                                                                                                   
	}                                                                                                                                
	                                                                                                                                 
	/*                                                                                                                               
	 *  ������룬��ͬ��LINUX Ӧ�ò㣬 һ����ѭ������                                                                                
	 */                                                                                                                              
	int main(void)                                                                                                                   
	{                                                                                                                                
	    int data;                                                                                                                    
	                                                                                                                                 
	    unsigned char zvalue;                                                                                                        
	    GPB.CON = (GPB.CON & ~(0xff<<8)) | 0x33<<8; // GPBCON[3], I2C_5_SCL GPBCON[2], I2C_5_SDA                                     
	    mydelay_ms(100);                                                                                                             
	    uart_init();                                                                                                                 
	                                                                                                                                 
	/*---------------------------------------------------------------------*/                                                        
	    I2C5.I2CSTAT = 0xD0;                                                                                                         
	    I2C5.I2CCON &= ~(1<<4);       /*clean interrupt pending bit  */                                                              
	/*---------------------------------------------------------------------*/                                                        
	                                                                                                                                 
	    mydelay_ms(100);                                                                                                             
	    MPU6050_Init();                                                                                                              
	    mydelay_ms(100);                                                                                                             
	                                                                                                                                 
	    printf("\n********** I2C test!! ***********\n");                                                                             
	                                                                                                                                 
	    while(1)                                                                                                                     
	    {                                                                                                                            
	        //Turn on                                                                                                                
	                                                                                                                                 
	        data = get_data(GYRO_ZOUT_H);                                                                                            
	        printf(" GYRO --> Z <---:Hex: %x", data);                                                                                
	        data = get_data(GYRO_XOUT_H);                                                                                            
	        printf(" GYRO --> X <---:Hex: %x", data);                                                                                
	        printf("\n");                                                                                                            
	        mydelay_ms(1000);                                                                                                        
	    }                                                                                                                            
	    return 0;                                                                                                                    
	}                                                                                                                                

