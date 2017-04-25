//netfilter_firewall
//cute eleven 2014-7-20

#include <linux/module.h>  
#include <linux/ioctl.h>  
#include <linux/wait.h> 
#include <linux/poll.h> 
#include <linux/device.h>
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/errno.h>  
#include <linux/mm.h>  
#include <linux/sched.h>  
#include <linux/init.h>  
#include <linux/cdev.h>
#include <linux/in.h>   
#include <linux/ip.h>   
#include <linux/tcp.h>   
#include <linux/icmp.h>   
#include <linux/netdevice.h>   
#include <linux/netfilter.h>   
#include <linux/netfilter_ipv4.h>   
#include <asm/io.h>  
#include <asm/system.h>  
#include <asm/uaccess.h>

#define CE_FW_DEV_IOC_MAGIC  'k'
#define FW_DEV_IOC_REF_DENY_IP        _IO(CE_FW_DEV_IOC_MAGIC,1)
#define FW_DEV_IOC_REF_DENY_PORT        _IO(CE_FW_DEV_IOC_MAGIC,2)
#define FW_DEV_IOC_ADD_DENY_IP  _IOW(CE_FW_DEV_IOC_MAGIC,3,unsigned int)
#define FW_DEV_IOC_ADD_DENY_PORT  _IOW(CE_FW_DEV_IOC_MAGIC,4,short)

static int ce_fw_major;
struct cdev cdev;
struct ce_fw_dev *ce_fw_devp;  /*�豸�ṹָ��*/ 
dev_t devno;
unsigned int kernel_deny_ip[10];
unsigned short kernel_deny_port[10];

struct ce_fw_dev  //�����豸�ļ��ṹ��
{  
    char *data;  
    unsigned long size;  
};

//����netfilter���Ӻ��� ������ɸı���ַ�������ָ��
const char* hooks[] ={ "NF_IP_PRE_ROUTING",           //·��ǰ����
                             "NF_IP_LOCAL_IN",         //�����ع���
                             "NF_IP_FORWARD",          //��������
                             "NF_IP_LOCAL_OUT",        //���س�����
                             "NF_IP_POST_ROUTING"};    //·�ɺ���

//ip���˺���
unsigned int ip_filter(struct iphdr *iph)
{
	int i;	
	for(i=0;i<10;i++)
	{
		if(kernel_deny_ip[i] == 0)
		{
			continue;
		}
		else
		{
			if((kernel_deny_ip[i] == iph->saddr)||(kernel_deny_ip[i] == iph->daddr)) //�޷��������ж�
			{
				return NF_DROP;
			}
		}
	}
	return NF_ACCEPT;
	 
}

//TCP���˺���
unsigned int tcp_filter(struct tcphdr *th)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(kernel_deny_port[i] == 0)
		{
			continue;
		}
		else
		{
			if((kernel_deny_port[i] == th->source)||(kernel_deny_port[i] == th->dest)) //�޷��Ŷ������ж�
			{
				return NF_DROP;
			}
		}
	}
	return NF_ACCEPT;
}


// hook����
unsigned int packet_filter(unsigned int hooknum,
			       struct sk_buff **skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))  //����Ҫ��������ݸ�ʽ����
{
	struct sk_buff* nskb;  //����socket�������ݽṹָ��
	struct iphdr *iph = NULL;	//����IP��ͷ���ݽṹָ�� ��ʼֵΪNULL
	struct tcphdr *th = NULL;  //����TCP��ͷ���ݽṹָ�� ��ʼֵΪNULL
	
	nskb = *skb;//��ȡsocket��ͷ
	if(nskb==NULL)  //�ж�socket�Ƿ����
		return NF_ACCEPT; //�����������

	iph = nskb->nh.iph; //��ȡIP��ͷ
	if(iph == NULL)    //�ж�IP��ͷ�Ƿ����
		return NF_ACCEPT;  //�����������
	else
		return ip_filter(iph); //����ip��ַ���˺����ж�
		
	th = nskb->h.th;  //��ȡTCP��ͷ
	if(iph == NULL)  //�ж�TCP��ͷ�Ƿ����
		return NF_ACCEPT;  //�����������
	else
		return tcp_filter(th); //����ip��ַ���˺����ж�
		
	return NF_ACCEPT; //�����������й��� ����
}

 //���ƹ���λ�� ��ʱ�ͽ�ȡ�������ص�
static struct nf_hook_ops packet_filter_opt[] = {
        {
                .hook                = packet_filter,
                .owner                = THIS_MODULE,
                .pf                = PF_INET,
                .hooknum        = NF_IP_PRE_ROUTING,
                .priority = NF_IP_PRI_FIRST,
        },
        {
                .hook                = packet_filter,
                .owner                = THIS_MODULE,
                .pf                = PF_INET,
                .hooknum        = NF_IP_LOCAL_IN,
                .priority = NF_IP_PRI_FIRST,
        },
        {
                .hook                = packet_filter,
                .owner                = THIS_MODULE,
                .pf                = PF_INET,
                .hooknum        = NF_IP_FORWARD,
                .priority = NF_IP_PRI_FIRST,
        },
        {
                .hook                = packet_filter,
                .owner                = THIS_MODULE,
                .pf                = PF_INET,
                .hooknum        = NF_IP_LOCAL_OUT,
                .priority = NF_IP_PRI_FIRST,
        },
        {
                .hook                = packet_filter,
                .owner                = THIS_MODULE,
                .pf                = PF_INET,
                .hooknum        = NF_IP_PRE_ROUTING,
                .priority = NF_IP_POST_ROUTING,
        },
};


//�豸��
int ce_fw_open(struct inode *inode,struct file *filp)
{ 
			printk("ce_fw_open---------------------ok\n");
	return 0;
}  

//�豸�ͷ�
int ce_fw_release(struct inode *inode,struct file *filp)  
{  
			printk("ce_fw_release------------------ok\n");
    return 0;  
}  

//�豸����
int  ce_fw_ioctl(struct inode *inode,struct file *filp,
					unsigned int cmd,unsigned long arg)
{
	int ret =0;
	int i;
	unsigned int ip_ioarg =0;
	unsigned short tcp_ioarg =0;
	
	switch(cmd)  //�豸�������
	{
		/*��ȡ����*/
		case FW_DEV_IOC_REF_DENY_IP:  //ͨ���豸�������IP��ַ��Ŀ
			for(i=0;i<10;i++)
			{
				kernel_deny_ip[i] =0;
			}
			printk("FW_DEV_IOC_REF_DENY_IP---------ok\n");
			break;
			
		case FW_DEV_IOC_ADD_DENY_IP:  //ͨ���豸���IP��ַ��Ŀ 
			ret =__get_user(ip_ioarg,(unsigned int *)arg); 
			//���������û���������� user_add_deny_ip תΪָ��
			for(i=0;i<10;i++)
			{
				if(kernel_deny_ip[i] == 0)
				{
					kernel_deny_ip[i] = ip_ioarg;
					break;
				}
			}
			printk("FW_DEV_IOC_ADD_DENY_IP---------ok\n");
			break;
		
		case FW_DEV_IOC_REF_DENY_PORT:  //ͨ���豸�������IP��ַ��Ŀ
			for(i=0;i<10;i++)
			{
				kernel_deny_port[i] =0;
			}
			printk("FW_DEV_IOC_REF_DENY_PORT-------ok\n");
			break;
		
		case FW_DEV_IOC_ADD_DENY_PORT:  //ͨ���豸���TCP��ַ��Ŀ
			ret =__get_user(tcp_ioarg,(unsigned short *)arg);
			//���������û���������� user_add_deny_tcp תΪָ��
			for(i=0;i<10;i++)
			{
				if(kernel_deny_port[i] == 0)
				{
					kernel_deny_port[i] = tcp_ioarg;
					break;
				}
			}
			printk("FW_DEV_IOC_ADD_DENY_PORT-------ok\n");
			break;
			
		default:
			return 0;
	}
	return 0;
}


static const struct file_operations ce_fw_fops =  
{  
    .owner = THIS_MODULE,  
    .llseek = NULL,  
    .read = NULL,  
    .write = NULL,  
    .open = ce_fw_open,  
    .release = ce_fw_release,  
    .ioctl = ce_fw_ioctl,
};  





//�豸��ʼ���͹���ע��
static int ce_fw_init(void)
{
	int result;
	result = alloc_chrdev_region(&devno,0,1,"ce_fw_dev");	
	ce_fw_major = MAJOR(devno); 
	
    if(result < 0)
        return result; 
	
    cdev_init(&cdev,&ce_fw_fops);  
    cdev.owner = THIS_MODULE;  
    cdev.ops = &ce_fw_fops;   
	           
    cdev_add(&cdev,devno,1); 
	ce_fw_devp = kmalloc(sizeof(struct ce_fw_dev),GFP_KERNEL);	
	memset(ce_fw_devp,0,sizeof(struct ce_fw_dev));
	nf_register_hook(&packet_filter_opt);	
	return 1;
}

//�豸ע���͹���ע��
static void ce_fw_exit(void)
{
	cdev_del(&cdev);  
	kfree(ce_fw_devp);
	unregister_chrdev_region(devno,1);
	nf_unregister_hook(&packet_filter_opt);
}

MODULE_AUTHOR("Cute_Eleven");
MODULE_LICENSE("GPL");

module_init(ce_fw_init); 
module_exit(ce_fw_exit); 



