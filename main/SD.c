// #include "SD.h"
// #include <esp_log.h>

// #define MOUNT_POINT "/sdcard"

// sdmmc_card_t* setupSD(int * EC){
//     gpio_pullup_en(GPIO_NUM_12);
//     gpio_pullup_en(GPIO_NUM_13);
//     gpio_pullup_en(GPIO_NUM_14);
//     gpio_pullup_en(GPIO_NUM_15);
//     gpio_pullup_en(GPIO_NUM_4);
//     esp_vfs_fat_sdmmc_mount_config_t mount_config = {
//         .format_if_mount_failed = false,
//         .max_files = 5,
//         .allocation_unit_size = 16 * 1024
//     };
//     sdmmc_card_t *card;
//     const char mount_point[] = MOUNT_POINT;
//     sdmmc_host_t host = SDMMC_HOST_DEFAULT();
//     sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
//     slot_config.width = 4;
//     slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
//     esp_err_t ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
//     if(ret!=0){
//         ESP_LOGI("SDMOUNT","SDMOUNT FAILED");
//         *EC = 1;
//         return card;
//     }
//     sdmmc_card_print_info(stdout, card);
//     *EC = 0;
//     return card;
// }

// void discardSD(sdmmc_card_t* card){
//     const char mount_point[] = MOUNT_POINT;

//     esp_vfs_fat_sdcard_unmount(mount_point, card);
//     ESP_LOGI("SDUNMOUNT", "Card unmounted");
// }


// int writeToSD(const char* path,char * data){
//     FILE *fptr = fopen(path,"a");
//     if(fptr == NULL){
//         ESP_LOGI("SDCARD","FAILED TO WRITE TO SD CARD");
//         return 1;
//     }
//     fprintf(fptr,data);
//     fclose(fptr);
//     return 0;
// }

// void dataLogTask(void * param){
//     passVariables* vars = (passVariables*)param;
    
//     const char* path = MOUNT_POINT"/LoggedData";
//     char* dataString;
//     while(1){
//         struct timeval tv_now;
//         gettimeofday(&tv_now, NULL);
//         int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
//         // dataString = ""+time_us+","+(vars->data[1])+","
//         // +(vars->data[2])+"\n";
//         writeToSD(path,dataString);
//         vTaskDelay(1000/portTICK_PERIOD_MS);
//     }
// }
