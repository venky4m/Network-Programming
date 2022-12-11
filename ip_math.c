/*
  Author : venkatesh Marripelly
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<math.h>


#define PREFIX_LEN 16
#define MAX_LEN 32
#define ISBIT(n,pos) ( n&(1<<pos) )
#define TOGGLEBIT(n,pos) ( n^(1<<pos) )
#define COMPLEMENT(num)     (num = num ^ 0xFFFFFFFF)
#define SET_BIT(n, pos)     (n = n | 1 << pos)
#define UNSET_BIT(n, pos)   (n = n & ((1 << pos) ^ 0xFFFFFFFF))

static unsigned int get_mask_value_in_integer_format(char mask)
{
    unsigned int mask_int = 0xFFFFFFFF;
    char last_bit = MAX_LEN - mask;
    int i = 0;
    for(;i<last_bit;i++)
    {
        UNSET_BIT(mask_int,i);
    }
    return mask_int;
}

/// @brief prints the binary format of a given integer--> For debugging purpose
/// @param n integer input
void bin(unsigned int n)
{
    unsigned int i;
    printf("0");
    for (i = 1 << 30; i > 0; i = i / 2)
    {
      if((n & i) != 0)
      {
        printf("1");
      }
      else
      {
        printf("0");
      }
    }
    printf("\n");
  }

/// @param ip_in "1.2.3.4"
/// @param mask a value between 1 to 32
/// @param output return value of function 1.2.3.255 (out put broad cast adress)
void get_broadcast_address(char *ip_in , char mask, char *output)
{
    unsigned int ip_address_int = 0;

    //conversts ip_in = "1.2.3.4" to it's equivant integer IP address
    inet_pton(AF_INET,ip_in,&ip_address_int);
    ip_address_int = htonl(ip_address_int);

    //if subnet is 24, the getting iteger format of 11111111 11111111 11111111 00000000
    unsigned int subnet_mask_int = get_mask_value_in_integer_format(mask);

    //complimenting to do OR operation 00000000 00000000 00000000 11111111
    COMPLEMENT(subnet_mask_int);

    //OR operation with IP_address
    ip_address_int = ip_address_int|subnet_mask_int;
    ip_address_int = ntohl(ip_address_int);

    //conversting from integer to A.B.C.D format
    inet_ntop(AF_INET,&ip_address_int,output,PREFIX_LEN+1);
    output[PREFIX_LEN] = '\0';
}

/// @brief A.B.C.D to its euivalent INTEGER Format
/// @param ip 
unsigned int get_ip_integral_equivalent(char *ip)
{
    unsigned int ip_int = 0;
    inet_pton(AF_INET,ip,&ip_int);
    ip_int = htonl(ip_int);
    return  ip_int;
}

//integer to IP_FORMAT
void get_abcd_ip_format(unsigned int ip_int,char *ip_abcd_foromat)
{
    ip_int = htonl(ip_int);
    inet_ntop(AF_INET, &ip_int,ip_abcd_foromat,PREFIX_LEN+1);
    ip_abcd_foromat[PREFIX_LEN] = '\0';
}

//getting networkid of a givent A.B.C.D/mask 
//example 192.168.23.56/24 means 192.168.23.0

void get_network_id(char *ip,char mask,char *output)
{
    unsigned int ip_int = get_ip_integral_equivalent(ip);
    unsigned int mask_int = get_mask_value_in_integer_format(mask);
    unsigned int network_id_int = ip_int & mask_int;
    get_abcd_ip_format(network_id_int,output);
}

/// @brief compute total number of Interfaces/IP_addresses can be have with the given mask value 
/// @param mask mask
/// @return cardinality
unsigned int get_subnet_cardinality(char mask)
{
    /*
        cardinality = 2^(32 - mask) - 2;
        here -2 is because 0000s and 1111s are reserved for networkID and Broadcast address
        if mask 24
        totat cardinality 254 -> 2^(8) -2
        A.B.C.0000 0000 is networkID 
        A.B.C.1111 1111 is Broadcast Address
    */
   return pow(2,32-mask) - 2; 
}

/// @brief A IP can be part of a subnet if the both have the same NetworkID
/// @param networkId 
/// @param mask 
/// @param check_ip 
/// @return 0 , 1 - false / true
int check_ip_membership(char *network_id,char mask,char *check_ip)
{
    char network_id_of_check_ip[ PREFIX_LEN];
    get_network_id(check_ip,mask,network_id_of_check_ip);
    //converting both networkIds to its equivent integer format and checking

    unsigned int network_id_int;
    inet_pton(AF_INET,network_id,&network_id_int);

    unsigned int networkId_of_check_ip_int;
    inet_pton(AF_INET,network_id_of_check_ip,&networkId_of_check_ip_int);

    return network_id_int == networkId_of_check_ip_int ;
}
int main()
{
    char *ip_addr = "192.168.23.53";
    char mask = 24;

    printf("============================================\n");
    char ipadd_buffer[PREFIX_LEN];
    memset(ipadd_buffer,0, PREFIX_LEN);
    get_broadcast_address(ip_addr,mask,ipadd_buffer);
    printf("%s-->ipadd_buffer\n",ipadd_buffer);


    printf("============================================\n");
    printf("%u",get_ip_integral_equivalent(ip_addr));

    printf("============================================\n");
    char ip_abcd[PREFIX_LEN];
    get_abcd_ip_format(get_ip_integral_equivalent(ip_addr) , ip_abcd);
    printf("%s",ip_abcd);

    printf("============================================\n");
    get_network_id(ip_addr,mask,ip_abcd);
    printf("%s",ip_abcd);

    printf("============================================\n");
    printf("%d",get_subnet_cardinality(mask));

    printf("============================================\n");
    printf("%d\n",check_ip_membership("12.34.56.0",24,"12.64.56.78"));
    return 0;
}
