/*
 *描述     ：  5层摩斯密码加密和解密
 *作者     ：  何猛
 *创建时间  ：  2013/6/5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

/*字母到字母的映射*/
struct map1
{
	char c1;
	char c2;
};
const struct map1 A_A[26]={
	{ 'A', 'Q' },
	{ 'B', 'W' },
	{ 'C', 'E' },
	{ 'D', 'R' },
	{ 'E', 'T' },
	{ 'F', 'Y' },
	{ 'G', 'U' },
	{ 'H', 'I' },
	{ 'I', 'O' },
	{ 'J', 'P' },
	{ 'K', 'A' },
	{ 'L', 'S' },
	{ 'M', 'D' },
	{ 'N', 'F' },
	{ 'O', 'G' },
	{ 'P', 'H' },
	{ 'Q', 'J' },
	{ 'R', 'K' },
	{ 'S', 'L' },
	{ 'T', 'Z' },
	{ 'U', 'X' },
	{ 'V', 'C' },
	{ 'W', 'V' },
	{ 'X', 'B' },
	{ 'Y', 'N' },
	{ 'Z', 'M' }	
};
/*字母到数字的映射*/
struct map2 
{
	char c;
	char* str;
};
const struct map2 A_D[26]={
	{ 'A', "21" },
	{ 'B', "22" },
	{ 'C', "23" },
	{ 'D', "31" },
	{ 'E', "32" },
	{ 'F', "33" },
	{ 'G', "41" },
	{ 'H', "42" },
	{ 'I', "43" },
	{ 'J', "51" },
	{ 'K', "52" },
	{ 'L', "53" },
	{ 'M', "61" },
	{ 'N', "62" },
	{ 'O', "63" },
	{ 'P', "71" },
	{ 'Q', "72" },
	{ 'R', "73" },
	{ 'S', "74" },
	{ 'T', "81" },
	{ 'U', "82" },
	{ 'V', "83" },
	{ 'W', "91" },
	{ 'X', "92" },
	{ 'Y', "93" },
	{ 'Z', "94" }	
};
const map2 D_M[9] = {
	{ '1', "*----/" },
	{ '2', "**---/" },
	{ '3', "***--/" },
	{ '4', "****-/" },
	{ '5', "*****/" },
	{ '6', "-****/" },
	{ '7', "--***/" },
	{ '8', "---**/" },
	{ '9', "----*/" }
};

char en_conv1(char c);
char * en_conv2(char c);
void encrypt(const char * str);
void en_display(const char * str, int len);

void decrypt(const char *);
char de_conv1(const char * str);
char de_conv2(const char * str);
char de_conv3(char c);
void de_display(const char * str, int len);

int main(int argc, char *argv[])
{
	char * strSrc="IAMMICHAEL"; 
	encrypt(strSrc);

	char *strDst="--***/****-/--***/**---/***--/**---/***--/*----/--***/**---/---**/*----/****-/***--/-****/***--/***--/*----/-****/***--/";
	decrypt(strDst);
	getchar();
	return 0;
}

void encrypt(const char * s)
{	
	/*获取字符串长度*/
	int len=strlen(s);
	s += len-1;
	
	/*动态创建两个字符数字*/
	int len1=(len+1)/2;
	int len2=len-len1;
	char *s1=(char*)malloc(len1);
	char *s2=(char*)malloc(len2);	
	int i;
	
	/*s1、s2字符串赋值*/
	for(i=0;i<len1;i++)
	{
		*s1=en_conv1(*s);		
		if (i<len2)
		{
			*s2=en_conv1(*(s-1));	
			s2++;		
		}
		s-=2;
		s1++;		
	}	
	
	/*移到内存块首地址*/
	s1-=len1;
	s2-=len2;
	
	char * str=(char*)malloc(2*len); /*纯数字字符串*/
	for (int i=0;i<len1;i++)
	{
		char *t=en_conv2(*(s1+i));
		/*printf("%s", t);*/
		*str=*t;
		*(str+1)=*(t+1);
		str+=2;
	}
	for (int i=0;i<len2;i++)
	{
		char *t=en_conv2(*(s2+i));
		/*printf("%s", t);*/
		*str=*t;
		*(str+1)=*(t+1);
		str+=2;
	}
	/*移到内存块首地址*/
	str-=2*len;
	/*打印摩斯密码*/
	en_display(str, 2*len);

	/*释放动态分配的内存*/
	free(str);
	free(s2);
	free(s1);	
}

char en_conv1(char c)
{
	for (int i=0;i<26;i++)
	{
		if (A_A[i].c1 == c)
		{
			return A_A[i].c2;
		}		
	}
	return 0;
}

char * en_conv2(char c)
{
	for (int i=0;i<26;i++)
	{
		if (A_D[i].c == c)
		{
			return A_D[i].str;
		}
	}
	return 0;
}

void en_display(const char * str, int len)
{	
	printf("\n-----Morse Code---\n");
	for (int i=0;i<len;i++)
	{	
		for (int j=0;j<9;j++)
		{
			if (D_M[j].c == *(str+i) )
			{
				printf("%s", D_M[j].str);
			}
		}		
	}	
}

void decrypt(const char * s)
{
	/*获取字符串长度*/
	int len=strlen(s);
	/*第一层解码*/
	int len1=len/6;
	char arrMC_Seg[7];
	
	char *s1=(char*)malloc(len1);
	int i, j;
	for (i=0;i<len1;i++)
	{
		for (j=0;j<6;j++)
		{			
			arrMC_Seg[j]=*(s+j);		
		}
		arrMC_Seg[6]='\0';
		*(s1+i)=de_conv1(arrMC_Seg);
		s+=6;
	}	

	/*第二层解码*/	
	int len2=len1/2;
	char * s2=(char*)malloc(len2);
	char arr2[3];
	for (i=0;i<len2;i++)
	{
		for (j=0;j<2;j++)
		{
			arr2[j]=*(s1+2*i+j);
		}
		arr2[2]='\0';
		*(s2+i)=de_conv2(arr2);		
	}

	/*第三层解码*/	
	char *s3=(char*)malloc(len2);
	for (i=0;i<len2;i++)
	{
		*(s3+i)=de_conv3(*(s2+i));		
	}

	/*第四层解码*/
	char *s4=(char *)malloc(len2);
	int len4_1=(len2+1)/2;
	int len4_2=len2-len4_1;
	for (i=0;i<len4_1;i++)
	{
		*(s4+2*i)=*(s3+i);
		if (i<len4_2)
		{
			*(s4+2*i+1)=*(s3+len4_1+i);
		}		
	}

	/*
	 *第五层解码
	 *倒置输出
	 */
	de_display(s4, len2);
	
	free(s4);
	free(s3);	
	free(s2);	
	free(s1);
}

/*摩斯码到数字*/
char de_conv1(const char * str)
{
	for (int i=0;i<9;i++)
	{
		if (0 == strcmp(D_M[i].str, str))
		{
			return D_M[i].c;
		}
	}
	return 0;
}
/*手机9键盘解码，数字到字母*/
char de_conv2(const char * str)
{
	for (int i=0;i<26;i++)
	{
		if (0==strcmp(A_D[i].str, str))
		{
			return A_D[i].c;
		}
	}
}
/*标准QWERT键盘解码，字母到字母*/
char de_conv3(char c)
{
	for (int i=0;i<26;i++)
	{
		if (A_A[i].c2 == c)
		{
			return A_A[i].c1;
		}
	}
}

void de_display(const char * str, int len)
{
	printf("\n-----Sorce Code---\n");
	for (int i=len-1;i>=0;i--)
	{
		printf("%c", *(str+i));
	}
}