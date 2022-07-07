// rev 2021.20.12.2

#include "FatFs\diskio.h"
#include "FatFs\ff.h"
#include <stdio.h>      // e.g.: sprintf()
#include <stdlib.h>     // e.g.: itoa()
#include <string.h>     // e.g.: strcpy()
#include "project.h"
#include "FatFs/SG_SDCard.h"
#include "project.h"

char string[60] = {0};

FRESULT SD_dir(CySCB_Type* UART, struct SDstrct_t* SDstrct)
{
    Cy_SCB_UART_PutString(UART, "Reading Directories:\n");
    SDstrct->result = f_opendir(&SDstrct->pDirectory, "/");      // open 'root' directory
    if(SDstrct->result != FR_OK) return SDstrct->result;
    else
    {
        while(1) // Loop through all contents
        {
            SDstrct->result = f_readdir(&SDstrct->pDirectory, &SDstrct->file_info);
            //Cy_SCB_UART_PutString(UART, "Read directories:\n"); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
            if (SDstrct->result != 0)
            {
                sprintf(string, "*** ERROR: Code %d\n", SDstrct->result);
                Cy_SCB_UART_PutString(UART, string); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
                break;
            }
            if (SDstrct->file_info.fname[0] == 0)             // end of directory?
            {
                Cy_SCB_UART_PutString(UART, "End of Directories\n");
                break;     
            }
            if (SDstrct->file_info.fattrib & AM_DIR)          // is it a directory?
            {
                UART_PutString(SDstrct->file_info.fname); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
                Cy_SCB_UART_PutString(UART, "\t\t\t<DIR>\n"); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
            }
            else                                    // it is a file.
            {
                UART_PutString(SDstrct->file_info.fname); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
                Cy_SCB_UART_PutString(UART, "\t\t\t"); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
                itoa(SDstrct->file_info.fsize, string, 10);
                Cy_SCB_UART_PutString(UART, string); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
                Cy_SCB_UART_PutString(UART, "Bytes\n"); do{}while(!Cy_SCB_UART_IsTxComplete(UART));
            }
        }
        f_closedir(&SDstrct->pDirectory);
    }
    return SDstrct->result;
}

FRESULT SD_format(struct SDstrct_t* SDstrct)
{
    BYTE work[FF_MAX_SS];
    SDstrct->result = f_mkfs("", FM_EXFAT | FM_FAT32, 0, work, sizeof(work)); // FM_EXFAT, FM_FAT32
    UART_PutString(string);
    return SDstrct->result;
}

FRESULT SD_mount(struct SDstrct_t* SDstrct)
{
    SDstrct->result = f_mount(&SDstrct->fatFs, "", f_FORCEMOUNT);
    return SDstrct->result;
}

void SD_unmount(void)
{
    f_unmount("");
}

TCHAR* SD_read_label(TCHAR* label)
{
    f_getlabel("", label, 0);
    return label;
}

FRESULT SD_new_file(struct SDstrct_t* SDstrct, TCHAR* name)
{
    SDstrct->result = f_open(&SDstrct->pFile, name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ); // | FA_OPEN_ALWAYS);
    return SDstrct->result;
}

FRESULT SD_close_file(struct SDstrct_t* SDstrct)
{
    SDstrct->result = f_close(&SDstrct->pFile);
    return SDstrct->result;
}

FRESULT write_mic_data_SD(struct SDstrct_t* SDstrct, uint32_t data[], uint16_t n_bytes)
{
    SDstrct->result = f_write(&SDstrct->pFile, data, 4*n_bytes, &SDstrct->n_bytes_written_to_SD);
    return SDstrct->result;
}

#ifdef SD_CD_0_PORT
bool SD_detect_card()
{
    bool i;
    i = Cy_GPIO_Read(SD_CD_0_PORT, SD_CD_0_NUM);
    return i;
}
#endif 
//bool SD_detect_card(GPIO_PRT_Type *port, uint32_t pin) // Ex: SD_detect_card(P0_3_PORT, P0_3_NUM)
//{
//    bool i;
//    i = Cy_GPIO_Read(port, pin);
//    return i;
//}
