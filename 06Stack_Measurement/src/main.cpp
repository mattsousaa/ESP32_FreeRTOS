/***************************************************************
 *  Example for checking task memory consumption - High Water Mark
 *  By: Mateus Sousa
***************************************************************/

/*Arduino Library*/
#include <Arduino.h>

/*FreeRTOS Libraries*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*Pin Mapping*/
#define LED1  2
#define LED2  14

/*Auxiliar values*/
int valor = 500;

/*Variables for storing task Handles*/
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

/*Tasks prototypes*/
void vTaskBlink(void *pvParameters);
void vTask2(void *pvParameters);

/*Setup function*/
void setup(){
  Serial.begin(9600); 

  /*Tasks creations on different cores*/ 
  xTaskCreatePinnedToCore(vTaskBlink,"TASK1",configMINIMAL_STACK_SIZE,(void*)LED1,1,&task1Handle,APP_CPU_NUM);
  xTaskCreatePinnedToCore(vTask2,"TASK2",configMINIMAL_STACK_SIZE+1024,(void*)valor,2,&task2Handle,PRO_CPU_NUM);
  xTaskCreatePinnedToCore(vTaskBlink,"TASK3",configMINIMAL_STACK_SIZE,(void*)LED2,1,&task3Handle,APP_CPU_NUM);

}
/*função loop*/
void loop(){
  vTaskDelay(3000); // Releases the CPU for 3 sec
}

void vTaskBlink(void *pvParameters){
    UBaseType_t uxHighWaterMark;
    int pin = (int)pvParameters;
    pinMode(pin, OUTPUT);

    while (1){
      digitalWrite(pin, !digitalRead(pin));
      vTaskDelay(pdMS_TO_TICKS(200));

      uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); 
      Serial.print(pcTaskGetTaskName(NULL)); 
      Serial.print(" : ");
      Serial.println(uxHighWaterMark); // How much space of memory is available in bytes

    }
}

void vTask2(void *pvParameters){
  UBaseType_t uxHighWaterMark;
  int cont = (int)pvParameters;

  while(1){
    Serial.println("Task 2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000));
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); 
    Serial.print(pcTaskGetTaskName(NULL)); 
    Serial.print(" : ");
    Serial.println(uxHighWaterMark); // How much space of memory is available in bytes
  }
}