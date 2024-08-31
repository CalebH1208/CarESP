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
    
    setupCANLED(12);
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
    xTaskCreatePinnedToCore(&periodicLoRaSend,"LoRa_TASK",8192,(void *)&passMe,5,NULL,1);
    #ifdef EV
    xTaskCreatePinnedToCore(&canRecieveTaskEV,"CAN_TASK",4096,(void *)&passMe,5,NULL,1);
    if(ec ==0){
    int arr1Hz[5] ={0,1,2,3,4};
    int arr10Hz[7] ={5,6,7,8,9,10,11};
    int arr100Hz[3] ={12,13,14};
    int arrMaxHz[11] ={15,16,17,18,19,20,21,22,23,24,25};
    DataLoggingSetup(&data,arr1Hz,5,arr10Hz,7,arr100Hz,3,arrMaxHz,11);
    }
    #else
    xTaskCreatePinnedToCore(&canRecieveTask,"CAN_TASK",4096,(void *)&passMe,5,NULL,1);
    
    if(ec ==0){
    int arr1Hz[1] ={0};
    int arr10Hz[1] ={0};
    int arr100Hz[25] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    int arrMaxHz[1] ={7};
    
    DataLoggingSetup(&data,arr1Hz,1,arr10Hz,1,arr100Hz,25,arrMaxHz,0);
    }
    #endif
    
    //xTaskCreatePinnedToCore(&dataLogTask,"Data_Log_TASK",16384,(void*)&passMe,4,NULL,0);
    
    while(1){
    vTaskDelay(10000/portTICK_PERIOD_MS);
    
    // char hey[100];
    // vTaskGetRunTimeStats(hey);
    // printf("%s",hey);
    }
}

