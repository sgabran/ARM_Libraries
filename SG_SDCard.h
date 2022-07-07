// rev 2021.20.12.2

#ifndef _SG_SDCARD_H_
#define _SG_SDCARD_H_
    
#include "FatFs\diskio.h"
#include "FatFs\ff.h"
#include <stdio.h>      // e.g.: sprintf()
#include <stdlib.h>     // e.g.: itoa()
#include <string.h>     // e.g.: strcpy()
#include "project.h"

struct SDstrct_t
{
FATFS fatFs;
DIR pDirectory; // Pointer to directory object structure
FIL pFile; // Pointer to file object structure
FILINFO file_info;
FRESULT result;
UINT n_bytes_written_to_SD;
UINT n_bytes_read_from_SD;
//uint8_t file_counter;    // Temporary file number
};

FRESULT SD_mount(struct SDstrct_t*);
void SD_unmount(void);
FRESULT SD_format(struct SDstrct_t*);
FRESULT SD_dir(CySCB_Type*, struct SDstrct_t*);
TCHAR* SD_read_label(TCHAR*);
FRESULT SD_new_file(struct SDstrct_t*, TCHAR*);
FRESULT SD_close_file(struct SDstrct_t*);
FRESULT write_mic_data_SD(struct SDstrct_t*, uint32_t[], uint16_t);
//bool SD_detect_card(GPIO_PRT_Type *port, uint32_t pin);
bool SD_detect_card();

#define f_unmount(path) f_mount(0, path, 0) // Unregister the work area: specify a NULL to the fs
#define f_FORCEMOUNT 1

#endif 