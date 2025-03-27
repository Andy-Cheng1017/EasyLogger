#ifndef ELOG_AT_H
#define ELOG_AT_H

#include <stdint.h>

typedef struct {
  uint8_t uart_tx_buf[2048];
  void *usart_x;
  void *dmax_channely;
} EasyLoggerDmaConfig_t;

extern EasyLoggerDmaConfig_t ElogDMA;

// // elog_port.c
void ELogDmaTransfer(void);

void Logger_init(void);

#endif