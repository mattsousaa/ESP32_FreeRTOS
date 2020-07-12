/*********************************************************
* Example to demonstrate the use of binary semaphore inside
* from an ISR
* By: Mateus Sousa
*********************************************************/

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define LED 2
#define BT 12

SemaphoreHandle_t semaforo;

TaskHandle_t taskTrataBTHandle;

void vTaskTrataBT(void * pvParameters);

void ISR_CallBack(){

  BaseType_t xHighPriorityTaskWoken = pdTRUE;

  xSemaphoreGiveFromISR(semaforo,&xHighPriorityTaskWoken); // Releases semaphore from ISR

  if(xHighPriorityTaskWoken == pdTRUE){
    portYIELD_FROM_ISR(); // Forces context switching to the highest priority task
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  pinMode(BT,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT),ISR_CallBack,FALLING);

  semaforo = xSemaphoreCreateBinary();
  xTaskCreate(vTaskTrataBT, "Task Button",configMINIMAL_STACK_SIZE + 1024,NULL,3,&taskTrataBTHandle); // highest priority

}

void loop(){
  // Task loop priority = 1 (lowest priority)
  digitalWrite(LED,HIGH);
  vTaskDelay(pdMS_TO_TICKS(100)); // Releases the CPU for 100ms
  digitalWrite(LED,LOW);
  vTaskDelay(pdMS_TO_TICKS(1000)); // Releases the CPU for 1s
  /* There is only context switching when the CPU is free;
   * The CPU must be released so that only one task does not monopolize all processing (task starvation)
   */
}

void vTaskTrataBT(void * pvParameters){

  int contador = 0;

  while(1){
    xSemaphoreTake(semaforo,portMAX_DELAY);
    Serial.println("x: " + String(contador++));
  }
}