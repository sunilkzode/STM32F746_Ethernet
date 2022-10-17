/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : UDPServer.c
  * @brief          : UDP server functions file
  ******************************************************************************
  * This file is used for separating the UDP related code from main.
  * The prototypes are added in the UDPServer.h file.
  *
  * IMPLEMENTATION FOR UDP Server :   source:https://www.geeksforgeeks.org/udp-server-client-implementation-c/
	1. Create UDP socket.
	2. Bind the socket to server address.
	3. Wait until datagram packet arrives from client.
	4. Process the datagram packet and send a reply to client.
	5. Go back to Step 3.
  ******************************************************************************
   */
/* USER CODE END Header */


#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "stdio.h"
#include "string.h"
#include "UDPServer.h"
#include "netif.h"

extern struct netif gnetif;

void udpServer_init(void)
{
	// UDP Control Block structure
   struct udp_pcb *upcb;
   err_t err;

   /* 1. Create a new UDP control block  */
   upcb = udp_new();

   /* 2. Bind the upcb to the local port */
   ip_addr_t myIPADDR;
   IP_ADDR4(&myIPADDR, 192, 168, 29, 123);

   err = udp_bind(upcb, &myIPADDR, 7);  // 7 is the server UDP port


   /* 3. Set a receive callback for the upcb */
   if(err == ERR_OK)
   {
	   udp_recv(upcb, udp_receive_callback, NULL);
   }
   else
   {
	   udp_remove(upcb);
   }
}

// udp_receive_callback will be called, when the client sends some data to the server
/* 4. Process the datagram packet and send a reply to client. */

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	struct pbuf *txBuf;

	/* Get the IP of the Client */
	char *remoteIP = (char *) ipaddr_ntoa(addr);

	//if(strcmp(*remoteIP, "192.168.29.100"))
	//	if(port)
	//	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);

	char buf[200];
	char Rxbuf[200];
	int len = 0;
	if(p->len < 200)
	{
		len = sprintf (buf, "Payload = %s\n"
							"IP Address = %s\n"
							"Port Used = %d\n"
							"Payload Length = %d\n"
							"Total Length = %d\n",
							(char*)p->payload,
							remoteIP,
							port,
							p->len,
							p->tot_len);

		// This is just how data can be retrieved
		for(int i = 0; i < p->len; i++)
			Rxbuf[i] = *((char*) (p->payload + i));

		if((Rxbuf[0] == 0xAA) && (Rxbuf[1] == 0x55))
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}
	else
		len = 200;

	/* allocate pbuf from RAM*/
	txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);

	/* copy the data into the buffer  */
	pbuf_take(txBuf, buf, len);

	/* Connect to the remote client */
	udp_connect(upcb, addr, port);

	/* Send a Reply to the Client */
	udp_send(upcb, txBuf);

	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(upcb);

	/* Free the p_tx buffer */
	pbuf_free(txBuf);

	/* Free the p buffer */
	pbuf_free(p);

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void ethernetif_notify_conn_changed(struct netif *netif)
{
	if(netif_is_link_up(&gnetif))
	{
		netif_set_up(&gnetif);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	}
	else
	{
		netif_set_down(&gnetif);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	}
}
