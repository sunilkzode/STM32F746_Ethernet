/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : UDPServer.c
  * @brief          : UDP server functions file
  ******************************************************************************
  * This file is used for separating the UDP related code from main.
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef INC_UDPSERVER_H_
#define INC_UDPSERVER_H_

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void udpServer_init(void);


#endif /* INC_UDPSERVER_H_ */
