/***************************************************************
 *  Examples for creating tasks
 *  This example shows how to create tasks in FreeRTOS
 *  By: Mateus Sousa
***************************************************************/

/*Arduino Library*/
#include <Arduino.h>

/*FreeRTOS Libraries*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*Pin Mapping*/ 
#define LED 2

/*Variables for storing task Handles*/
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

/*Tasks prototypes*/
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/*Setup function*/
void setup(){
  Serial.begin(9600); // Configure serial communication with 9600 baud rate
  /*Tasks creations*/
  xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task1Handle);
}
    
/*Loop function*/
void loop(){
  vTaskDelay(3000); // Releases the CPU for 3 sec
}

/*
vTask1 - Toggles LED at 200 ms intervals
*/
void vTask1(void *pvParameters){
    pinMode(LED, OUTPUT);

    while (1){
      digitalWrite(LED, !digitalRead(LED));
      vTaskDelay(pdMS_TO_TICKS(200));
    }
}

/*
vTask2 - Prints count value for every 1 sec
*/
void vTask2(void *pvParameters){
  int cont = 0;

  while (1){
    Serial.println("Task 2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}