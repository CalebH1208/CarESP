#include "Helper.h"


dataContainer CreateDataContainer(int length){
    dataContainer ret;
    ret.length = length;
    ret.dataArray =(datapoint*) malloc(sizeof(datapoint)*length);
    char namesArr[length][30] = {"source\0","MPH\0","RPM\0","Voltage\0","Water Temperature\0","Oil Temperature\0","Oil Pressure\0","Fuel Pressure\0","Pitot Left\0","Pitot Right\0","Pitot Center\0","Manifold Absolute Pressure\0","Lambda\0","Gear Position\0", "Shift Request\0","Neutral Sensor\0","Steering Angle\0","Lat Load\0","Long Load\0","Brake Pressure Front\0","Brake Pressure Rear\0","Damper Pot FL\0","Damper Pot FR\0","Damper pot RL\0","Damper Pot RR\0","Throttle Position\0"};
    float convArr[length] = {1,.160934,1,.1,.1,.1,0.0145038,1,1,1,1,.0145038,.01,1,1,1,.1,0.00980665012,0.00980665012,0.145038,0.145038,1,1,1,1,.1};
    char endUnit[length][10] = {"N/A","N/A","N/A","V","F","F","PSI","PSI","PSI","PSI","PSI","PSI","N/A","N/A","N/A","N/A","Degrees","G","G","PSI","PSI","MM","MM","MM","MM","Percent"};
    for(int i =0;i<length;i++){
    strcpy(ret.dataArray[i].name, namesArr[i]);
    ret.dataArray[i].conv = convArr[i];
    strcpy(ret.dataArray[i].endUnit, endUnit[i]);
    ret.dataArray[i].value = 0;
    
    }
    
    return ret;
}

void freeDataArray(dataContainer Killme){
    free(Killme.dataArray);
}

dataContainer CreateDataContainerEV(int length){
dataContainer ret;
    ret.length = length;
    ret.dataArray =(datapoint*) malloc(sizeof(datapoint)*length);
    char namesArr[length][30] = {"source\0","Front Brake Pressure\0","Rear Brake Pressure\0","COmmand Torque\0","MPH\0","PDM LV 1\0","PDM LV 2\0","PDM LV 3\0","PDM LV 4\0","PDM HV 1\0","PDM HV 2\0","PDM HV 3\0","PDM HV 4\0","Damper Pot FL\0", "Damper Pot FR\0","Damper Pot RL\0","Damper Pot RR\0","Motor Temp\0","GLV voltage\0","Inverter temp\0","RPM\0","Tractive sys Voltage\0","Highest Battery Temp\0","Average Battery Temp\0","Total Pack Voltage\0","Accumulator Current\0"};
    float convArr[length] = {1,.160934,1,.1,.1,.1,0.0145038,1,1,1,1,.0145038,.01,1,1,1,.1,0.00980665012,0.00980665012,0.145038,0.145038,1,1,1,1,.1};
    char endUnit[length][10] = {"N/A","N/A","N/A","V","F","F","PSI","PSI","PSI","PSI","PSI","PSI","N/A","N/A","N/A","N/A","Degrees","G","G","PSI","PSI","MM","MM","MM","MM","Percent"};
    for(int i =0;i<length;i++){
    strcpy(ret.dataArray[i].name, namesArr[i]);
    ret.dataArray[i].conv = convArr[i];
    strcpy(ret.dataArray[i].endUnit, endUnit[i]);
    ret.dataArray[i].value = 0;
    
    }
    ret.dataArray[0].value = 1;
    return ret;

}
/*
takes a message and a dataContainer pointer
attempts to update any applicable values
if message is not in correct range then it returns -1
if message doesnt update anything -2 is returned
if message updates value in DataC then 0 is returned
*/
int updateArray(twai_message_t message,dataContainer* DataC){
    if((message.identifier/0x100 != 0x7)&&(message.identifier/0x100 !=0x3))return -1;
    switch(message.identifier){
      case 0x700:
        DataC->dataArray[1].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[2].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[3].value = message.data[4];
        DataC->dataArray[4].value = (message.data[5] << 8) | message.data[6];
        //DataC->dataArray[5] = message.data[6];
        DataC->dataArray[7].value = message.data[7];
        return 0;
      case 0x701:
        DataC->dataArray[6].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[11].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[16].value = (message.data[4] << 8) | message.data[5];
        DataC->dataArray[12].value = message.data[6];
        DataC->dataArray[14].value = message.data[7];
        return 0;
      case 0x702:
        DataC->dataArray[17].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[18].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[19].value = (message.data[4] << 8) | message.data[5];
        DataC->dataArray[20].value = (message.data[6] << 8) | message.data[7];
        return 0;
      case 0x703:
        DataC->dataArray[21].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[22].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[23].value = (message.data[4] << 8) | message.data[5];
        DataC->dataArray[24].value = (message.data[6] << 8) | message.data[7];
        return 0;
      case 0x704:
        DataC->dataArray[25].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[13].value = message.data[2];
        DataC->dataArray[15].value = ((message.data[3] & (1))?1:0);
        DataC->dataArray[5].value = (message.data[6] << 8) | message.data[7];
        return 0;
       case 0x3f0: //pitot values here
        DataC->dataArray[8].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[9].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[10].value = (message.data[4] << 8) | message.data[5];
        
        return 0;
      default:
        return -2;
    }
    
}


int updateArrayEV(twai_message_t message,dataContainer* DataC){
    switch(message.identifier){
      case 0x700:
        DataC->dataArray[1].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[2].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[3].value = (message.data[4] << 8) | message.data[5];
        DataC->dataArray[4].value = (message.data[6] << 8) | message.data[7];        
        return 0;
      case 0x701:
        DataC->dataArray[5].value = message.data[0];
        DataC->dataArray[6].value = message.data[1];
        DataC->dataArray[7].value = message.data[2];
        DataC->dataArray[8].value = message.data[3];
        DataC->dataArray[9].value = message.data[4];
        DataC->dataArray[10].value = message.data[5];
        DataC->dataArray[11].value = message.data[6];
        DataC->dataArray[12].value = message.data[7];
        return 0;
      case 0x702:
        DataC->dataArray[13].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[14].value = (message.data[2] << 8) | message.data[3];
        DataC->dataArray[15].value = (message.data[4] << 8) | message.data[5];
        DataC->dataArray[16].value = (message.data[6] << 8) | message.data[7];
        return 0;
      case 0x703:
        DataC->dataArray[17].value = (message.data[0] << 8) | message.data[1];
        DataC->dataArray[18].value = message.data[2];
        
        return 0;
      case 0x0A1:
        DataC->dataArray[19].value = (message.data[1] << 8) | message.data[0];
        return 0;
       case 0x0A5:
        DataC->dataArray[20].value = (message.data[3] << 8) | message.data[2];
        return 0;
        case 0x0A7:
        DataC->dataArray[21].value = (message.data[1] << 8) | message.data[0];
        return 0;
      case 0x6b1:
        DataC->dataArray[22].value = message.data[4];
        DataC->dataArray[23].value = message.data[6];
        return 0;
      case 0x6b0:
        DataC->dataArray[24].value = message.data[2];
        DataC->dataArray[25].value = message.data[0];
        return 0;
      default:
        return -2;
    }
}


int LoRaSend(dataContainer DataArray,RFM96 radio){
uint8_t sendArray[DataArray.length*2] = {0};
for(int i=0;i<DataArray.length;i++){
  sendArray[2*i]=(DataArray.dataArray[i].value/256);
  sendArray[2*i+1]=(DataArray.dataArray[i].value%256);
}
radio.transmit(sendArray,DataArray.length*2);
return 0;
}

void periodicLoRaSend(void* passMe){
  while(1){
  LoRaSend(*(((passVariables *)passMe)->data),*(((passVariables *)passMe)->radio));
  vTaskDelay(100/portTICK_PERIOD_MS); 
  }
}

void canRecieveTask(void* passMe){
  configure_CAN();
  while(1){
  twai_message_t message = recieve_CAN();
    int EC = updateArray(message,(((passVariables *)passMe)->data));    
  }
}

void canRecieveTaskEV(void* passMe){
  configure_CANEV();
  while(1){
  twai_message_t message = recieve_CAN();
    int EC = updateArrayEV(message,(((passVariables *)passMe)->data));    
  }
}


/////////////////////////////

#define MOUNT_POINT "/sdcard"

sdmmc_card_t* setupSD(int * EC){
    ESP_LOGI("","%d",gpio_pullup_en(GPIO_NUM_12));
    ESP_LOGI("","%d",gpio_pullup_en(GPIO_NUM_13));
    ESP_LOGI("","%d",gpio_pullup_en(GPIO_NUM_14));
    ESP_LOGI("","%d",gpio_pullup_en(GPIO_NUM_15));
    ESP_LOGI("","%d",gpio_pullup_en(GPIO_NUM_2));
    ESP_LOGI("","%d",gpio_pullup_en(GPIO_NUM_4));
      
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
      sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = 39999;
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if(ret!=0){
        ESP_LOGI("SDMOUNT","SDMOUNT FAILED");
        *EC = 1;
        return card;
    }
    sdmmc_card_print_info(stdout, card);
    *EC = 0;
    ESP_LOGI("SDMOUNT","SDMOUNT SUCCESS");
    //writeToSD(MOUNT_POINT"/DATA.csv","Time,MPH,RPM,Voltage,Water Temp,Oil Temp,Oil Pressure,Fuel Pressure,Pitot Left,Pitot Right,Pitot Center,MAP,Lambda,Gear Position,shift request,Neutral Sensor,Steer angle,Lat Load,Long Load,Front Brake Pressure,Rear Brake Pressure,Front Left Damper Pot,Front Right Damper Pot,Rear Left Damper Pot,Rear Right Damper Pot,Throttle position\n");
    return card;
}

void discardSD(sdmmc_card_t* card){
    const char mount_point[] = MOUNT_POINT;

    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI("SDUNMOUNT", "Card unmounted");
}


int writeToSD(const char* path,char * data){
    FILE *fptr = fopen(path,"a");
    if(fptr == NULL){
        //ESP_LOGI("SDCARD","FAILED TO WRITE TO SD CARD");
        return 1;
    }
    fprintf(fptr,data);
    //write()
    fclose(fptr);
    return 0;
}

void dataLogTask(void * param){
    passVariables* vars = (passVariables*)param;
    
    const char* path = MOUNT_POINT"/data.csv";
    char* dataString;
    char buf[512];
    while(1){
      configureSendMessage(vars,buf);
        dataString = buf;
        writeToSD(path,dataString);
        //vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void quickSDWrite(const char* path, char* data){
FILE *fptr = fopen(path,"a");
    if(fptr == NULL){
        //ESP_LOGI("SDCARD","FAILED TO WRITE TO SD CARD");
        return ;
    }
fprintf(fptr,data); //TODO Optimize this
fclose(fptr);
return;
}

void quickestSDWrite(FILE* fptr,char* data){
  fprintf(fptr,data);
  return;
}

// void maxFreqPrint(int * arrOfIndexes,int arrLen,char * folderPath,void * param){
// char path[16] = "/MaxHtz.csv";
// strcat(folderPath,path);
// FILE * fptr = fopen(folderPath,"a");
// passVariables* vars = (passVariables*)param;
// if(fptr == NULL) return;
// while(1){
// char* dataString;
// char buf[5*arrLen+20];
// configurePartialMessage(vars->data,arrOfIndexes,arrLen,buf,5*arrLen+20);
// dataString = buf;
// quickSDWrite(fptr,dataString);
// vTaskDelay(10/portTICK_PERIOD_MS);
// }

// }



void configureSendMessage(passVariables * vars,char buf[512]){
  struct timeval tv_now;
  gettimeofday(&tv_now, NULL);
  int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
  sniprintf(buf,sizeof(char)*512,"%lld",time_us);
  for(int i=1;i<vars->data->length;i++){
  //for(int i = 1;i < 10;i++){
    sniprintf(buf,sizeof(char)*512,"%s,%d",buf,vars->data->dataArray[i].value);
  }
  sniprintf(buf,sizeof(char)*512,"%s\n",buf);

  return;
}


void configurePartialMessage(dataContainer* data,int* arrOfIndexes,int arrLen,char * buf,int bufLen){
struct timeval tv_now;
  gettimeofday(&tv_now, NULL);
  int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
  sniprintf(buf,sizeof(char)*bufLen,"%lld",time_us);
  for(int i =0;i<arrLen;i++){
    sniprintf(buf,sizeof(char)*bufLen,"%s,%d",buf,data->dataArray[arrOfIndexes[i]].value);
  }
  sniprintf(buf,sizeof(char)*bufLen,"%s\n",buf);
  return;
}

void configureTitleMessage(dataContainer* data,int* arrOfIndexes,int arrLen,char * buf,int bufLen){
  sniprintf(buf,sizeof(char)*bufLen,"%s","Time");
  for(int i =0;i<arrLen;i++){
    sniprintf(buf,sizeof(char)*bufLen,"%s,%s",buf,data->dataArray[arrOfIndexes[i]].name);
  }
  sniprintf(buf,sizeof(char)*bufLen,"%s\n",buf);
  return;
}


void DataLoggingSetup(dataContainer* data,int* Indexes1Hz,int Hz1Len,int* Indexes10Hz,int Hz10Len,int* Indexes100Hz,int Hz100Len,int* IndexesMaxHz,int HzMaxLen){
  const char* path1 = MOUNT_POINT"/1HzLog.csv";
  const char* path10 = MOUNT_POINT"/10HzLog.csv";
  const char* path100 = MOUNT_POINT"/100HzLog.csv";
  const char* pathMax = MOUNT_POINT"/MaxHzLog.csv";
  char buf[30*Hz1Len];
  configureTitleMessage(data,Indexes1Hz,Hz1Len,buf,20*Hz1Len+20);
  quickSDWrite(path1,(char *)buf);
  char buf10[30*Hz10Len];
  configureTitleMessage(data,Indexes10Hz,Hz10Len,buf10,20*Hz10Len+20);
  quickSDWrite(path10,(char *)buf10);
  char buf100[30*Hz100Len];
  configureTitleMessage(data,Indexes100Hz,Hz100Len,buf100,20*Hz100Len+20);
  quickSDWrite(path100,(char *)buf100);
  char bufMax[30*HzMaxLen];
  configureTitleMessage(data,IndexesMaxHz,HzMaxLen,bufMax,20*HzMaxLen+20);
  quickSDWrite(pathMax,(char *)bufMax);
  DataLoggingParameters oneHz = {1,data,Indexes1Hz,Hz1Len,path1};
  xTaskCreatePinnedToCore(&DataLoggingTaskManager,"1Hz logging task",4096,(void*)&oneHz,5,NULL,0);
  DataLoggingParameters tenHz = {10,data,Indexes10Hz,Hz10Len,path10};
  xTaskCreatePinnedToCore(&DataLoggingTaskManager,"10Hz logging task",4096,(void*)&tenHz,4,NULL,0);
  DataLoggingParameters hundredHz = {100,data,Indexes100Hz,Hz100Len,path100};
  xTaskCreatePinnedToCore(&DataLoggingTaskManager,"100Hz logging task",4096,(void*)&hundredHz,3,NULL,0);
  DataLoggingParameters MaxHz = {1000,data,IndexesMaxHz,HzMaxLen,pathMax};
  xTaskCreatePinnedToCore(&DataLoggingTaskManager,"Max Hz logging task",4096,(void*)&MaxHz,2,NULL,0);
  
  while(1){
    vTaskDelay(10000/portTICK_PERIOD_MS);
    
    // char hey[150];
    // vTaskGetRunTimeStats(hey);
    // printf("%s",hey);
    }
  return;
}

void DataLoggingTaskManager(void * spo0ky){
  DataLoggingParameters* params = (DataLoggingParameters*)spo0ky;
  //ESP_LOGI("LP","%d",params->data->dataArray[2].value);
  FILE* fptr = fopen(params->path,"a");

  int i =0;
while(1){
char* dataString;
char buf[7*params->arrLen+20];
configurePartialMessage(params->data,params->arrofIndexes,params->arrLen,buf,5*params->arrLen+20);
dataString = buf;
//ESP_LOGI("LP","%d",params->data->dataArray[6].value);
quickestSDWrite(fptr,dataString);
if(params->loggingPeriodHz != -1){
vTaskDelay((1000/params->loggingPeriodHz)/portTICK_PERIOD_MS);
if(i>(50000/(1000/params->loggingPeriodHz))){
  fclose(fptr);
  fptr = fopen(params->path,"a");
  i = 0;
  //ESP_LOGI("FYCk","this");
}
}
else{
  if(i>100000){
    fclose(fptr);
  fptr = fopen(params->path,"a");
  i = 0;
  }
}
i++;
}
}