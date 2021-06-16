#include"factory.h"

int MD5_file(char* file_path,char* encrypted)//加密文件，加密结果
{
	MD5_CTX md5;
	MD5Init(&md5);
	int  i;
	unsigned char buf[1000]={0};
	unsigned char encrypt[16];
	int fd=open(file_path,O_RDONLY);
	if(fd==-1)
	{
		perror("open");
		return -1;
	}
	int ret;
	do{
		ret=read(fd,buf,1000);
		if(ret==-1)
		{
			perror("read");
			return -1;
		}
		MD5Update(&md5,buf,ret);
	}while(ret==1000);//说明读取完
	MD5Final(&md5,encrypt);//加密后16位
	printf("%s\n",encrypt);//测试
	for(i=0;i<16;i++)
	{
		snprintf(encrypted+i*2,3,"%02x",encrypt[i]);//转换成32位
	}
	encrypted[32]='\0';
	printf("%s\n",encrypted);//测试
	return 1;
}


