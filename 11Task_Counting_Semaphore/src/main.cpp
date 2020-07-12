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

SemaphoreHandle_t semaforoContador;

TaskHandle_t taskTrataBTHandle;

void vTaskTrataBT(void * pvParameters);

void ISR_CallBack(){

  BaseType_t xHighPriorityTaskWoken = pdTRUE;

  xSemaphoreGiveFromISR(semaforoContador,&xHighPriorityTaskWoken); // Releases semaphore from ISR

  if(xHighPriorityTaskWoken == pdTRUE){
    portYIELD_FROM_ISR(); // Forces context switching to the highest priority task
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  pinMode(BT,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT),ISR_CallBack,FALLING);

  semaforoContador = xSemaphoreCreateCounting(255, 0); // Counts from 0 to 255
  xTaskCreate(vTaskTrataBT, "Task BT",configMINIMAL_STACK_SIZE + 1024,NULL,3,&taskTrataBTHandle); // highest priority

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

  UBaseType_t x;

  // The idea is to accumulate several events(interrupts); For each event/interrupt the counting semaphore increases
  // Tests this application with switch-bouncing; It will trigger several interrupts at the same time

  while(1){
    xSemaphoreTake(semaforoContador,portMAX_DELAY);
    Serial.print("Handling the Button ISR: ");
    x = uxSemaphoreGetCount(semaforoContador); // Get counter from semaphore
    Serial.println(x);
    delay(1000); // The task will wait 1s to handle the events
  }
}