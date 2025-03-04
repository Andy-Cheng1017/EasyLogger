#include "elog_at.h"
#include "elog.h"
#include "CircularBuffer.h"

#if defined(AT_START_F407) || defined(AT_START_F403A)
#include "at32f403a_407.h"
#elif defined AT_START_F403
#include "at32f403.h"
#elif defined AT_START_F413
#include "at32f413.h"
#elif defined AT_START_F415
#include "at32f415.h"
#elif defined AT_START_F421
#include "at32f421.h"
#elif defined AT_START_F425
#include "at32f425.h"
#elif defined AT_START_F435_437
#include "at32f435_437.h"
#endif

EasyLoggerDmaConfig_t ElogDMA = {
    .uart_tx_buf = {0},
};

void ELogDmaTransfer(void) {
  if (usart_flag_get(ElogDMA.usart_x, USART_TDBE_FLAG) != RESET) {
    if (txBuffer.count > 0) {
      uint16_t len = CircularBuffer_ReadBuffer(&txBuffer, ElogDMA.uart_tx_buf, sizeof(ElogDMA.uart_tx_buf));
      dma_channel_enable(ElogDMA.dmax_channely, FALSE);
      dma_data_number_set(ElogDMA.dmax_channely, len);
      dma_channel_enable(ElogDMA.dmax_channely, TRUE);
    } else {
      return;
    }
  } else {
    usart_interrupt_enable(ElogDMA.usart_x, USART_TDBE_INT, TRUE);
  }
}

void Logger_init(void) {
  ElogDMA.dmax_channely = DMA1_CHANNEL4;
  ElogDMA.usart_x = USART1;
  elog_init();
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
  elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
  elog_set_text_color_enabled(TRUE);
  elog_start();
}