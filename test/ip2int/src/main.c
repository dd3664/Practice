/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define IP_STR_LEN			16
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
unsigned int ip_to_int(const char *ip)
{
	unsigned ret = 0;
	unsigned int octet = 0;
	int i;

	for (i = 0; i < 4; i++)
	{
#ifdef __KERNEL__
		octet = simple_strtol(ip, (char **)&ip, 10);
#else
		octet = strtol(ip, (char **)&ip, 10);
#endif
		ret |= (octet << (8 * (3 - i)));
		if (*ip == '.')
		{
			ip++;
		}
	}
	return ret;
}

void int_to_ip(unsigned int ip, char *str, int size)
{
	unsigned char byte1 = (ip >> 24) & 0xFF;
	unsigned char byte2 = (ip >> 16) & 0xFF;
	unsigned char byte3 = (ip >> 8) & 0xFF;
	unsigned char byte4 = ip & 0xFF;
	snprintf(str, size, "%u.%u.%u.%u", byte1, byte2, byte3, byte4);
	return;
}

int main(int argc, char *argv[])
{
	unsigned int ip = 0;
	char ip_str[IP_STR_LEN] = {0};
	ip = ip_to_int("192.168.0.100");
	int_to_ip(ip, ip_str, sizeof(ip_str));
	printf("ip=%x, ip_str=%s\n", ip, ip_str);
	return 0;
}
