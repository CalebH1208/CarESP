#include <stdio.h>
#include <RadioLib.h>
#include "esp32_perfmon.h"

//#include "SD.h"
//#include "can.h"
//#include "Helper.h"

#include "can.c"
#include "Helper.cpp"
//#include "SD.c"

#define SCK 18
#define MISO 19
#define MOSI 23
EspHal* hal = new EspHal(SCK, MISO, MOSI);

#define NSS 5
#define DIO0 32
#define NRST 27
#define DIO1 34

RFM96 radio = new Module(hal, NSS, DIO0, NRST, DIO1);

static const char *TAG = "main";

//comment out this line for IC firmware other wise leave for EV firmware
#define EV 1

#ifdef EV
#define LENGTH 26
#else
#define LENGTH 26
#endif


    extern "C" void app_main(void) {
  // initialize just like with Arduino
    ESP_LOGI(TAG, "Initializing LoRa... ");
    
    int state = radio.begin(915.0F,500.0F,7U,5U,0x12U,17,8U,0U); 
    if (state != RADIOLIB_ERR_NONE) {
            ESP_LOGI(TAG, "failed, code %d\n", state);
        while(true) {
            hal->delay(1000);
        }
    }
    ESP_LOGI(TAG, "success!\n");
    
    //setupCANLED(12);
    #ifdef EV
    dataContainer data = CreateDataContainerEV(LENGTH);
    #else
    dataContainer data = CreateDataContainer(LENGTH);
    #endif
    passVariables passMe;
    passMe.data = &data;
    passMe.radio = &radio;
        
    int ec;
    setupSD(&ec);


    perfmon_start();
    #ifdef EV
    xTaskCreatePinnedToCore(&canRecieveTaskEV,"CAN_TASK",4096,(void *)&passMe,5,NULL,1);
    #else
    xTaskCreatePinnedToCore(&canRecieveTask,"CAN_TASK",4096,(void *)&passMe,5,NULL,1);
    #endif
    xTaskCreatePinnedToCore(&periodicLoRaSend,"LoRa_TASK",16384,(void *)&passMe,5,NULL,1);
    //xTaskCreatePinnedToCore(&dataLogTask,"Data_Log_TASK",16384,(void*)&passMe,4,NULL,0);
    // if(ec ==0){
    // int Hz1len = 5;
    // int Hz10len = 21;
    // int Hz100len = 10;
    // int Hz1000len = 1;
    // int arr1Hz[Hz1len] ={17,19,22,23,26,};
    // int arr10Hz[Hz10len] ={1,2,4,5,6,7,8,9,10,11,12,20,27,28,29,30,31,33,35,36,37};
    // int arr100Hz[Hz100len] ={3,13,14,15,16,18,21,24,25,34};
    // int arrMaxHz[Hz1000len] ={32};
    // DataLoggingSetup(&data,arr1Hz,Hz1len,arr10Hz,Hz10len,arr100Hz,Hz100len,arrMaxHz,Hz1000len);
    // }
    while(1){
    vTaskDelay(1000/portTICK_PERIOD_MS);
    // ESP_LOGI("AHAHAH","HELLO");
    // for(int i = 0;i<LENGTH;i++){
    // printf("%d, ",data.dataArray[i].value);
    // }
    // printf("\n");
    // char hey[100];
    // vTaskGetRunTimeStats(hey);
    // printf("%s",hey);
    }
}

