/***************************************************************
 *  Example for passing parameters when creating a Task
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

  /*Tasks creations*/ 
  xTaskCreate(vTaskBlink,"TASK1",configMINIMAL_STACK_SIZE,(void*)LED1,1,&task1Handle);
  xTaskCreate(vTask2,"TASK2",configMINIMAL_STACK_SIZE+1024,(void*)valor,2,&task2Handle);
  xTaskCreate(vTaskBlink,"TASK3",configMINIMAL_STACK_SIZE,(void*)LED2,1,&task3Handle);

}
/*função loop*/
void loop(){
  vTaskDelay(3000); // Releases the CPU for 3 sec
}

void vTaskBlink(void *pvParameters){
    uint8_t pin = (int)pvParameters;
    pinMode(pin,OUTPUT);

    while(1){
      digitalWrite(pin, !digitalRead(pin));
      vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void vTask2(void *pvParameters){
  int cont = (int)pvParameters;

  while(1){
    Serial.println("Task 2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}