// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include <uc_spim.h>
#include <uc_gpio.h>


void spim_init(SPI_TypeDef *SPIx,SPIM_CFG_Type *SPI_ConfigStruc)
{
    CHECK_PARAM(PARAM_SPIM(SPIx));
    SPIx->CLKDIV = SPI_ConfigStruc->Clk_rate;
}

void spim_send_data_noaddr(int cmd, char *data, int datalen, int useQpi);

void spim_setup_cmd_addr(SPI_TypeDef *SPIx, int cmd, int cmdlen, int addr, int addrlen) {
    int cmd_reg;
    cmd_reg = cmd << (32 - cmdlen);
    SPIx->SPICMD = cmd_reg;
    SPIx->SPIADR = addr;
    SPIx->SPILEN = (cmdlen & SPILEN_CLR_MASK) | 
           ((addrlen << 8) & SPILEN_ALR_MASK);
}

void spim_setup_dummy(SPI_TypeDef *SPIx, int dummy_rd, int dummy_wr) {
    SPIx->SPIDUM = ((dummy_wr << 16) & SPIDUM_TDR_MASK) | 
                           (dummy_rd & SPIDUM_RDR_MASK);
}

void spim_set_datalen(SPI_TypeDef *SPIx, int datalen) {
    volatile int old_len;
    old_len = SPIx->SPILEN;
    old_len = ((datalen << 16) & (SPILEN_RLR_MASK | SPILEN_TLR_MASK)) | 
                      (old_len & (SPILEN_CLR_MASK | SPILEN_ALR_MASK));
    SPIx->SPILEN = old_len;
}


void spim_start_transaction(SPI_TypeDef *SPIx, int trans_type, int csnum) {
    SPIx->STATUS = ((1 << (csnum + 8)) & SPI_CSN_MASK) | 
                    ((1 << trans_type) & SPI_TRA_MASK);
}

int spim_get_status(SPI_TypeDef *SPIx) {
    volatile int status;
    status = SPIx->STATUS;
    return status;
}

void spim_write_fifo(SPI_TypeDef *SPIx, int *data, int datalen) {
    volatile int num_words, i;

    num_words = (datalen >> 5) & SPILEN_NUM_MASK;

    if ( (datalen & SPILEN_DATA_MASK) != 0)
        num_words++;

    for (i = 0; i < num_words; i++) {
        while ((( (SPIx->STATUS) >> 24) & SPI_TRA_MASK) >= 8);
        SPIx->TXFIFO = data[i];
    }
}

void spim_read_fifo(SPI_TypeDef *SPIx, int *data, int datalen) {
    volatile int num_words;
    /* must use register for, i,j*/
    register int i,j;
    num_words = (datalen >> 5) & SPILEN_NUM_MASK;

    if ( (datalen & SPILEN_DATA_MASK) != 0)
        num_words++;
    i = 0;
    while (1){    
        do {
            j = (((SPIx->STATUS) >> 16) & SPI_TRA_MASK);
        } while (j==0);   
        do {
            data[i++] = SPIx->RXFIFO;
            j--;
        } while (j);
        if (i >= num_words) break;
    }
}

/* last function in spi lib, linked to bootstrap code.
 * calling this cause cache to fill 2nd block, so we have
 * 2 blocks of code in cache */
void spim_no_op()
{
    return;
}

