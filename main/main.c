#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_log.h"

/*log tag define*/
static const char *TaskTAG = "Task";
// static const char *TestTAG = "Test";

/* task structure define */
typedef struct A_struct
{
    int cStructMember1;
    char cStructMember2;
} AStruct;

AStruct xParameter = {1, 'b'};

/* queue structure create */
int Msg;
//--------------------------------------------------------------------------------------------------------------------
/* task function */
void vTask1(void *pvParameters)
{
    /****Parameter process*********/
    AStruct *pxParameters;
    pxParameters = (AStruct *)pvParameters;
    while (1)
    {
        ESP_LOGI(TaskTAG, "task running! %d \n", pxParameters->cStructMember1);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
//--------------------------------------------------------------------------------------------------------------------
/*  queue send task */
void QueueSendTask(void *pvParameters)
{
    QueueHandle_t xQueue;

    xQueue = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueueSend(xQueue, &i, 0);

        if (xStatus != pdPASS)
        {
            printf("send failed!\n");
        }
        else
        {
            printf("send msg %d \n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
            i++;

            if (i > 125)
                i = 0;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
/* queue Rcv task */
void QueueRcvTask(void *pvParameters)
{
    QueueHandle_t xQueue;

    xQueue = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i;
    while (1)
    {
        xStatus = xQueueReceive(xQueue, &i, 10);

        if (xStatus != pdPASS)
        {
            printf("Rcv failed!\n");
        }
        else
        {
            printf("Rcv msg %d \n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
/* queueset send task */
void QueueSetSendTask1(void *pvParameters)
{
    QueueHandle_t xQueue;

    xQueue = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueueSend(xQueue, &i, 20);

        if (xStatus != pdPASS)
        {
            ESP_LOGE("Queueset", "Send failed \n");
        }
        else
        {
            ESP_LOGI("Queueset", "Send msg 1 : %d \n", i);

            vTaskDelay(pdMS_TO_TICKS(1000));
            i++;

            if (i > 125)
                i = 0;
        }
    }
}

void QueueSetSendTask2(void *pvParameters)
{
    QueueHandle_t xQueue;

    xQueue = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueueSend(xQueue, &i, 20);

        if (xStatus != pdPASS)
        {
            ESP_LOGI("Queueset", "Send failed \n");
        }
        else
        {
            ESP_LOGI("Queueset", "Send msg 2 : %d \n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
            i++;

            if (i > 125)
                i = 0;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
/* queueset Rcv task */
void QueueSetRcvTask(void *pvParameters)
{
    QueueSetHandle_t xQueueSet;

    xQueueSet = (QueueSetHandle_t)pvParameters;

    QueueSetMemberHandle_t QueueData;

    BaseType_t xStatus;

    int i;
    while (1)
    {
        QueueData = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
        xStatus = xQueueReceive(QueueData, &i, portMAX_DELAY);

        if (xStatus != pdPASS)
        {
            ESP_LOGE("Queueset", "Rcv msg failed");
        }
        else
        {
            ESP_LOGI("Queueset", "Rcv msg %d \n", i);
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
/* queue mailbox */
void QueueBoxWriteTask(void *pvParameters)
{
    QueueHandle_t MailBox;

    MailBox = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueueOverwrite(MailBox, &i);

        if (xStatus != pdPASS)
        {
            ESP_LOGE("MailBox", "Send failed \n");
        }
        else
        {
            ESP_LOGI("MailBox", "Write mailbox : %d \n", i);

            vTaskDelay(pdMS_TO_TICKS(1000));
            i++;

            if (i > 125)
                i = 0;
        }
    }
}

void QueueBoxReadTask1(void *pvParameters)
{
    QueueHandle_t MailBox;

    MailBox = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueuePeek(MailBox, &i, 20);

        if (xStatus != pdPASS)
        {
            ESP_LOGE("MailBox", "Read failed \n");
        }
        else
        {
            ESP_LOGI("MailBox", "Read Mailbox : %d \n", i);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void QueueBoxReadTask2(void *pvParameters)
{
    QueueHandle_t MailBox;

    MailBox = (QueueHandle_t)pvParameters;

    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueuePeek(MailBox, &i, 20);

        if (xStatus != pdPASS)
        {
            ESP_LOGE("MailBox", "Read failed \n");
        }
        else
        {
            ESP_LOGI("MailBox", "Read Mailbox : %d \n", i);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//--------------------------------------------------------------------------------------------------------------------
/* soft timer */
void TimerCallbackFun(TimerHandle_t xTimer)
{

    const char *strName;
    strName = pcTimerGetName(xTimer);

    uint32_t id;
    id = (uint32_t)pvTimerGetTimerID(xTimer);

    ESP_LOGI("SoftTimer", "Hi, 5sec Timer! \n timerName : %s ; TimerID = %d", strName, id);
}

//--------------------------------------------------------------------------------------------------------------------
/* Semaphore */

int Semaph_data1 = 0;
int Semaph_data2 = 0;
int Semaph_data3 = 0;
int Semaph_data4 = 0;

SemaphoreHandle_t xSemaphoreHand;
SemaphoreHandle_t xSemaphoreHand2;
SemaphoreHandle_t xSemaphoreHand3;

/* Binary */
void SemaBOp1(void *pvParameters)
{
    printf("semaphore task success!");

    int iRet;
    while (1)
    {
        iRet = xSemaphoreTake(xSemaphoreHand, portMAX_DELAY);
        if (iRet == pdPASS)
        {
            for (int i = 0; i < 10; i++)
            {
                Semaph_data1++;
                Semaph_data2++;
            }

            xSemaphoreGive(xSemaphoreHand);
            printf("OP1  data1 : %d\n data2 : %d\n", Semaph_data1, Semaph_data2);

            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void SemaBOp2(void *pvParameters)
{

    int iRet;
    while (1)
    {
        iRet = xSemaphoreTake(xSemaphoreHand, portMAX_DELAY);
        if (iRet == pdPASS)
        {
            for (int i = 0; i < 10; i++)
            {
                Semaph_data1++;
                Semaph_data2++;
            }

            xSemaphoreGive(xSemaphoreHand);
            printf("OP2  data1 : %d\n data2 : %d\n", Semaph_data1, Semaph_data2);

            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

/* Counting */
void SemaCOp1(void *pvParameters)
{

    int CountingCnt;
    while (1)
    {
        CountingCnt = uxSemaphoreGetCount(xSemaphoreHand2);

        printf("cnt : %d\n", CountingCnt);

        if ((xSemaphoreTake(xSemaphoreHand2, 0)) == pdTRUE)
        {
            printf("Take success!\n");
        }
        else
        {
            printf("Take failed\n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
void SemaCOp2(void *pvParameters)
{

    int CountingCnt;
    while (1)
    {
        CountingCnt = uxSemaphoreGetCount(xSemaphoreHand2);

        printf("cnt : %d\n", CountingCnt);

        if ((xSemaphoreGive(xSemaphoreHand2)) == pdPASS)
        {
            printf("Give success!\n");
        }
        else
        {
            printf("Give failed\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1800));
    }
}
/* Mutex */
void SemaMOp1(void *pvParameters)
{

    BaseType_t iRet;
    printf("mutex task1 begin!\n");
    while (1)
    {
        iRet = xSemaphoreTake(xSemaphoreHand3, pdMS_TO_TICKS(1000));
        if (iRet == pdPASS)
        {
            printf("mutex task1 take!\n");

            for (int i = 0; i <= 10; i++)
            {
                Semaph_data3++;
                printf("mutex task1 : %d!\n", Semaph_data3);
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            printf("mutex task1 give!\n");
            xSemaphoreGive(xSemaphoreHand3);

            // Semaph_data3
        }
    }
}

void SemaMOp2(void *pvParameters)
{

    BaseType_t iRet;
    printf("mutex task1 begin!\n");
    while (1)
    {
        iRet = xSemaphoreTake(xSemaphoreHand3, pdMS_TO_TICKS(1000));
        if (iRet == pdPASS)
        {
            printf("mutex task2 take!\n");

            for (int i = 0; i <= 10; i++)
            {
                Semaph_data3++;
                printf("mutex task2 : %d!\n", Semaph_data3);
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            printf("mutex task2 give!\n");
            xSemaphoreGive(xSemaphoreHand3);

            vTaskDelay(pdMS_TO_TICKS(230));
            // Semaph_data3
        }
    }
}
//===================================================================================================================
//===================================================================================================================
//===================================================================================================================

void app_main(void)
{

    /*******task create********/
    TaskHandle_t xHandle1;

    if (xTaskCreate(vTask1,              /* callback function */
                    "task1",             /* readable name */
                    2048,                /* stack size */
                    (void *)&xParameter, /* parameters */
                    1,                   /* priority */
                    &xHandle1            /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("Task", "can't create task!");
    }
    else
    {
        // vTaskPrioritySet(xHandle1, 2); /* change task prioriry */
        ESP_LOGI(TaskTAG, "task created! \n");
    }

    //--------------------------------------------------------------------------------------------------------

    /* queue tasks */

    QueueHandle_t xQueue;
    xQueue = xQueueCreate(5, sizeof(int));

    xTaskCreate(QueueSendTask,
                "sendMessage",
                2048 * 3,
                (void *)xQueue,
                1,
                NULL);
    xTaskCreate(QueueRcvTask,
                "RcvMessage",
                2048 * 3,
                (void *)xQueue,
                1,
                NULL);

    //------------------------------------------------------------------------------------------------------------

    /* queueSet task */

    QueueHandle_t xQueueForSet1;
    xQueueForSet1 = xQueueCreate(5, sizeof(int));
    QueueHandle_t xQueueForSet2;
    xQueueForSet2 = xQueueCreate(5, sizeof(int));

    QueueSetHandle_t QueueSet;
    QueueSet = xQueueCreateSet(10);

    xQueueAddToSet(xQueueForSet1, QueueSet);
    xQueueAddToSet(xQueueForSet2, QueueSet);

    if ((xQueueForSet1 != NULL) && (xQueueForSet2 != NULL) && (QueueSet != NULL))
    {
        xTaskCreate(QueueSetSendTask1,
                    "sendSetMessage",
                    2048 * 3,
                    (void *)xQueueForSet1,
                    1,
                    NULL);
        xTaskCreate(QueueSetSendTask2,
                    "sendSetMessage2",
                    2048 * 3,
                    (void *)xQueueForSet2,
                    1,
                    NULL);

        xTaskCreate(QueueSetRcvTask,
                    "RcvSetMessage",
                    2048 * 3,
                    (void *)QueueSet,
                    1,
                    NULL);
    }
    else
    {
        ESP_LOGE("QueueSet", "can't create queueset!");
    }

    //----------------------------------------------------------------------------------------------------------

    /* Queue MailBox */
    QueueHandle_t MailBox;
    MailBox = xQueueCreate(1, sizeof(int));
    if (MailBox != NULL)
    {
        xTaskCreate(QueueBoxWriteTask,
                    "QueueBoxWriteRcvTask",
                    2048 * 3,
                    (void *)MailBox,
                    1,
                    NULL);
        xTaskCreate(QueueBoxReadTask1,
                    "QueueBoxWriteRcvTask",
                    2048 * 3,
                    (void *)MailBox,
                    1,
                    NULL);
        xTaskCreate(QueueBoxReadTask2,
                    "QueueBoxWriteRcvTask",
                    2048 * 3,
                    (void *)MailBox,
                    1,
                    NULL);
    }
    else
    {
        ESP_LOGE("MailBox", "can't create mailbox!");
    }

    //---------------------------------------------------------------------------------------------------------

    /* softTimer */

    TimerHandle_t xTimer1;
    xTimer1 = xTimerCreate("Timer1",            /* readable name */
                           pdMS_TO_TICKS(5000), /* timer period */
                           pdTRUE,              /* if auto reload */
                           (void *)1,           /* timer ID */
                           TimerCallbackFun);   /* callback function */

    xTimerStart(xTimer1, pdMS_TO_TICKS(200)); /* start Timer */

    //---------------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------
    /* Semaphore */
    /*Binary*/

    xSemaphoreHand = xSemaphoreCreateBinary();

    if (xSemaphoreHand != NULL)
        printf("semaBinary success!");

    xSemaphoreGive(xSemaphoreHand);

    TaskHandle_t SemaBOp_1;

    if (xTaskCreate(SemaBOp1,   /* callback function */
                    "SemaBOp1", /* readable name */
                    2048,       /* stack size */
                    NULL,       /* parameters */
                    1,          /* priority */
                    &SemaBOp_1  /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("SemaphBinary", "can't create task!");
    }
    else
    {
        // vTaskPrioritySet(xHandle1, 2); /* change task prioriry */
        ESP_LOGI("SemaphBinary", "task created! \n");
    }

    TaskHandle_t SemaBOp_2;

    if (xTaskCreate(SemaBOp2,   /* callback function */
                    "SemaBOp2", /* readable name */
                    2048,       /* stack size */
                    NULL,       /* parameters */
                    1,          /* priority */
                    &SemaBOp_2  /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("SemaphBinary", "can't create task!");
    }
    else
    {
        // vTaskPrioritySet(xHandle1, 2); /* change task prioriry */
        ESP_LOGI("SemaphBinary", "task created! \n");
    }

    /* Counting */
    xSemaphoreHand2 = xSemaphoreCreateCounting(10, 5);

    if (xSemaphoreHand2 != NULL)
        printf("semaCounting success!");

    xSemaphoreGive(xSemaphoreHand2);

    TaskHandle_t SemaCOp_1;

    if (xTaskCreate(SemaCOp1,   /* callback function */
                    "SemaCOp1", /* readable name */
                    2048,       /* stack size */
                    NULL,       /* parameters */
                    1,          /* priority */
                    &SemaCOp_1  /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("SemaphCounting", "can't create task!");
    }
    else
    {
        // vTaskPrioritySet(xHandle1, 2); /* change task prioriry */
        ESP_LOGI("SemaphCounting", "task created! \n");
    }

    TaskHandle_t SemaCOp_2;

    if (xTaskCreate(SemaCOp2,   /* callback function */
                    "SemaCOp2", /* readable name */
                    2048,       /* stack size */
                    NULL,       /* parameters */
                    1,          /* priority */
                    &SemaCOp_2  /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("SemaphCounting", "can't create task!");
    }
    else
    {
        // vTaskPrioritySet(xHandle1, 2); /* change task prioriry */
        ESP_LOGI("SemaphCounting", "task created! \n");
    }
    /* Mutex */

    xSemaphoreHand3 = xSemaphoreCreateMutex();

    TaskHandle_t SemaMOp_1;

    if (xTaskCreate(SemaMOp1,   /* callback function */
                    "SemaMOp1", /* readable name */
                    2048,       /* stack size */
                    NULL,       /* parameters */
                    2,          /* priority */
                    &SemaMOp_1  /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("SemaphMutex", "can't create task!");
    }
    else
    {
        ESP_LOGI("SemaphMutex", "task created! \n");
    }

    TaskHandle_t SemaMOp_2;

    if (xTaskCreate(SemaMOp2,   /* callback function */
                    "SemaMOp2", /* readable name */
                    2048,       /* stack size */
                    NULL,       /* parameters */
                    3,          /* priority */
                    &SemaMOp_2  /* task handle */
                    ) != pdPASS)
    {
        ESP_LOGE("SemaphMutex", "can't create task!");
    }
    else
    {
        // vTaskPrioritySet(xHandle1, 2); /* change task prioriry */
        ESP_LOGI("SemaphMutex", "task created! \n");
    }
}
