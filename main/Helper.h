#include <stdlib.h>
#include "can.h"
#include "ESPHal.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_sntp.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"




typedef struct {
    int value;
    char name[32];
    char endUnit[10];
    float conv;
    
}datapoint;

typedef struct dataC_t{
    int length;
    datapoint * dataArray;
}dataContainer;

typedef struct {
    RFM96* radio;
    dataContainer* data;
}passVariables;

typedef struct {
    int loggingPeriodHz;
    dataContainer* data;
    int* arrofIndexes;
    int arrLen;
    const char * path;
}DataLoggingParameters;

dataContainer CreateDataContainer(int length);
void freeDataContainer(dataContainer);
int updateArray(twai_message_t message,dataContainer* DataArray);
int LoRaSend(dataContainer DataArray,RFM96);
void periodicLoRaSend(void* radio);
void canRecieveTask(void*);


sdmmc_card_t* setupSD(int * EC);
void discardSD(sdmmc_card_t* card);
int writeToSD(const char*,char*);
void dataLogTask(void * param);
void configureSendMessage(passVariables * vars,char buf[512]);


void quickSDWrite(FILE * fptr, char* data);
void configurePartialMessage(dataContainer* data,int* arrOfIndexes,int arrLen,char * buf,int bufLen);
void configureTitleMessage(dataContainer* data,int* arrOfIndexes,int arrLen,char * buf,int bufLen);
void DataLoggingSetup(dataContainer* data,int* Indexes1Hz,int Hz1Len,int* Indexes10Hz,int Hz10Len,int* Indexes100Hz,int Hz100Len,int* IndexesMaxHz,int HzMaxLen);
void DataLoggingTaskManager(void * params);


int updateArrayEV(twai_message_t message,dataContainer* DataC);
dataContainer CreateDataContainerEV(int length);
void canRecieveTaskEV(void* passMe);


