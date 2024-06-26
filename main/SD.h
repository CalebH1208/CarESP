#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_sntp.h"
//#include "Helper.h"
sdmmc_card_t* setupSD(int * EC);
void discardSD(sdmmc_card_t* card);
int writeToSD(const char*,char*);
void dataLogTask(void * param);

