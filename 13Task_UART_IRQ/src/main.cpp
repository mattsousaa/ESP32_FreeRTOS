#include "main.h"
#include "uart_idf.h"

void vtaskEventUART(void *pvParameters);
void vtaskHandleUART(void * pvParameters);
uint8_t convert_to_capital(uint8_t data);

QueueHandle_t uart0_queue;
SemaphoreHandle_t semaforo;

TaskHandle_t taskHandleUART;
TaskHandle_t taskEventUART;

uint8_t *data = (uint8_t*)malloc(BUF_SIZE);
char data_buffer[100];
char recvd_data;
uint32_t count = 0;

char *str_out = (char *)malloc(sizeof(char) * 20); 

void setup(){

    pinMode(LED1,OUTPUT);

    memset(data_buffer, 0, sizeof(data_buffer));
  
    uart_param_config(EX_UART_NUM, &uart_config);
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart0_queue, 0);
    uart_enable_pattern_det_intr(EX_UART_NUM, '+', 3, 10000, 10, 10);

    semaforo = xSemaphoreCreateBinary();

    xTaskCreate(vtaskHandleUART, "Handle UART",configMINIMAL_STACK_SIZE + 1024,NULL,3,&taskHandleUART); // highest priority
    xTaskCreate(vtaskEventUART,"Event UART",10000,NULL,1,&taskEventUART);

}

void loop(){

    int len = uart_read_bytes(EX_UART_NUM, data, BUF_SIZE, 100 / portTICK_RATE_MS);
    if(len > 0){
        //uart_write_bytes_with_break(UART_NUM_0, (const char *)data, len, 100); 
        //uart_write_bytes(UART_NUM_0, (const char *)data, len);
        vTaskDelay(1000); // Releases the CPU for 1 sec
    }
}

void vtaskEventUART(void *pvParameters){

uart_event_t event; // Create an event handler

BaseType_t xHighPriorityTaskWoken = pdTRUE;

while(1){
    /* Now we wait for an event to occur, then we analyze your kind.*/
    if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)){
        // An event occurred, so we checked its type and then ended the loop, calling break
        switch (event.type){

            case UART_DATA:

                /* This is the most common event expected: communication data. We must take some precautions in this type 
                of event. For example, do not do your treatment within the interruption, because if we spend too much time 
                on the ISR, the buffer may be depleted. Here we can transfer the data to another queue or another buffer 
                and then signal it with a flag. In this way, the main program or another task will manage this data.
                */
                
                xSemaphoreGiveFromISR(semaforo,&xHighPriorityTaskWoken); // Releases semaphore from ISR
                if(xHighPriorityTaskWoken == pdTRUE){
                    portYIELD_FROM_ISR(); // Forces context switching to the highest priority task
                }
                
                break;

            case UART_FIFO_OVF:

                /* This is a problem. If this type of event occurs, we must add a flow control to our application. 
                 * Two decisions can be made here. Reading the buffer somewhere or, already knowing what it is about, 
                 * we simply clear the RX buffer and handle the error. As this is just an example of the interaction with 
                 * the interrupt events, we just clear the buffer.*/
                uart_flush(EX_UART_NUM);
                break;

            case UART_BUFFER_FULL:

                /* Do something
                 * Whenever such an event occurs, keep in mind that the data is not complete. You can read a part or simply 
                 * discard the buffer and generate an alarm, a log etc. In another article shown how to create files in the ESP32 
                 * file system, it is worth checking out.*/
                uart_flush(EX_UART_NUM);
                break;

            case UART_BREAK:
                /* Do something
                 * I don't think the reason for this type of event, but it exists. In the event of its occurrence, having
                 * a system of logs will facilitate their understanding and treatment. */
                break;

            case UART_PARITY_ERR:
                // Do something
                break;

            case UART_FRAME_ERR:
                // Do something
                break;

            case UART_PATTERN_DET:
                // Do something
                break;

            default:
                // An unknown event
                // Do something
                break;
        }
    }
}

}

void vtaskHandleUART(void * pvParameters){

  while(1){
    xSemaphoreTake(semaforo,portMAX_DELAY);
    digitalWrite(LED1, !digitalRead(LED1));
  }
}
