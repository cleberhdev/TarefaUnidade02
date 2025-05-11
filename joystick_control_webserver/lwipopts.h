#ifndef LWIPOPTS_H
#define LWIPOPTS_H

// Configurações mínimas para lwIP
#define NO_SYS 1
#define LWIP_SOCKET 0
#define LWIP_NETCONN 0
#define LWIP_TCP 1
#define LWIP_UDP 1
#define MEM_ALIGNMENT 4
#define MEM_SIZE 4096
#define MEMP_NUM_PBUF 16
#define PBUF_POOL_SIZE 16
#define LWIP_IPV4 1
#define LWIP_ICMP 1
#define LWIP_DHCP 1
#define LWIP_AUTOIP 0
#define LWIP_NETIF_HOSTNAME 1
#define LWIP_DEBUG 0

// Otimizações para Wi-Fi
#define TCP_MSS 1460
#define TCP_WND (8 * TCP_MSS)
#define TCP_SND_BUF (8 * TCP_MSS)
#define MEMP_NUM_TCP_PCB 5
#define MEMP_NUM_TCP_SEG 32
#define PBUF_POOL_BUFSIZE 1528

#endif /* LWIPOPTS_H */