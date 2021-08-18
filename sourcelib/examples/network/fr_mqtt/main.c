/**
 ******************************************************************************
 * @file    ex19_fr_mqtt/main.c 
 * @author  MDS
 * @date    04022016
 * @brief   MQTT example for publish and subscribe operations. 
 ******************************************************************************
 *  
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOS_CLI.h"

#include "MQTTClient.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Hardware_init();
void ApplicationIdleHook(void); /* The idle hook is used to blink the Blue 'Alive LED' every second */
void MQTT_Task(void);
void CLI_Task(void);


static BaseType_t prvSubCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t prvPubCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

struct Client mqttclient;
int connected = 0;
int mqtt_socket = -1;



/* Task Priorities ------------------------------------------------------------*/
#define mainLED_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainCLI_PRIORITY     ( tskIDLE_PRIORITY + 3 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainLED_TASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 5 )
#define mainCLI_TASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 3 )


#define print debug_printf

CLI_Command_Definition_t xEcho = {/* Structure that defines the "echo" command line command. */
    "echo",
    "echo: Echo the input.\r\n",
    prvEchoCommand,
    1
};

CLI_Command_Definition_t xPub = {/* Structure that defines the "pub" command line command. */
    "pub",
    "pub: <pub> <value> publish topic /np2/\r\n",
    prvPubCommand,
    2
};

CLI_Command_Definition_t xSub = {/* Structure that defines the "sub" command line command. */
    "sub",
    "sub: <topic> subscribe to topic /np2/\r\n",
    prvSubCommand,
    1
};

/**
 * @brief  Starts all the other tasks, then starts the scheduler.
 * @param  None
 * @retval None
 */
int main(void) {

    BRD_init();
    Hardware_init();

    LwIP_Init();

    /* Start the task to flash the LED. */
    xTaskCreate((void *) &MQTT_Task, (const signed char *) "MQTT", configMINIMAL_STACK_SIZE * 3, NULL, mainLED_PRIORITY, NULL);

    xTaskCreate((void *) &CLI_Task, (const signed char *) "CLI", mainCLI_TASK_STACK_SIZE, NULL, mainCLI_PRIORITY, NULL);

    /* Register CLI commands */
    FreeRTOS_CLIRegisterCommand(&xEcho);
    FreeRTOS_CLIRegisterCommand(&xPub);
    FreeRTOS_CLIRegisterCommand(&xSub);

    /* Start the scheduler.

    NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
    The processor MUST be in supervisor mode when vTaskStartScheduler is
    called.  The demo applications included in the FreeRTOS.org download switch
    to supervisor mode prior to main being called.  If you are not using one of
    these demo application projects then ensure Supervisor mode is used here. */

    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler. */
    return 0;
}

/**
 * Message handler for incoming (published) messages.
 * @param data MessageData struct pointer. Contains Qos info and payload.
 */
void mqtt_messagehandler(MessageData* data) {
    int i = 0;
    char buffer[100];
    strncpy(buffer, data->message->payload, data->message->payloadlen);
    debug_printf("Got packet: %s\n", buffer);
}

/**
 * @brief  MQTT Task.
 * @param  None
 * @retval None
 */
void MQTT_Task(void) {

	char buff[100];
    char readbuff[100];
    struct Network network;

	/* initialise MQTT connect data parameters */
    MQTTPacket_connectData connect_data = {
        {'M', 'Q', 'T', 'C'}, 0, 4,
        {NULL,
            {0, NULL}}, 60, 1, 0, MQTTPacket_willOptions_initializer,
        {NULL,
            {0, NULL}},
        {NULL,
            {0, NULL}}
    };

	BRD_LEDOff();   

    HAL_Delay(5000);	//Wait for network connection initialise
    NewNetwork(&network);
    ConnectNetwork(&network, "192.168.0.1", 1883);	//IP of MQTT broker

    debug_printf("Socket opened\n");
    HAL_Delay(1000);

	/* Create client */
    MQTTClient(&mqttclient, &network, 1000, buff, 100, readbuff, 100);
    debug_printf("Client created\n");

    mqttclient.command_timeout_ms = 10000;

	/* Connect to broker */
    MQTTConnect(&mqttclient, &connect_data); // This creates 2 FreeRTOS Tasks

    debug_printf("MQTT - Connected: %d\n", mqttclient.isconnected);

    
    connected = 1;
    int id = 0;
    for (;;) {
        MQTTYield(&mqttclient, 1);

        BRD_LEDToggle();

		vTaskDelay(1);
    }
}

/**
 * @brief  CLI Receive Task.
 * @param  None
 * @retval None
 */
void CLI_Task(void) {

    char cRxedChar;
    char cInputString[100];
    int InputIndex = 0;
    char *pcOutputString;
    BaseType_t xReturned;

    /* Initialise pointer to CLI output buffer. */
    memset(cInputString, 0, sizeof (cInputString));
    pcOutputString = FreeRTOS_CLIGetOutputBuffer();

    for (;;) {

        BRD_LEDToggle();

        /* Receive character */
        cRxedChar = debug_getc();

        /* Process if chacater if not Null */
        if (cRxedChar != '\0') {

            /* Put byte into USB buffer */
            debug_putc(cRxedChar);

            /* Process only if return is received. */
            if (cRxedChar == '\r') {

                //Put new line and transmit buffer
                debug_putc('\n');
                debug_flush();

                /* Put null character in command input string. */
                cInputString[InputIndex] = '\0';

                xReturned = pdTRUE;
                /* Process command input string. */
                while (xReturned != pdFALSE) {

                    /* Returns pdFALSE, when all strings have been returned */
                    xReturned = FreeRTOS_CLIProcessCommand(cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE);

                    /* Display CLI output string */
                    debug_printf("%s\n\r", pcOutputString);
                    vTaskDelay(5); //Must delay between debug_printfs.
                }

                memset(cInputString, 0, sizeof (cInputString));
                InputIndex = 0;

            } else {

                debug_flush(); //Transmit USB buffer

                if (cRxedChar == '\r') {

                    /* Ignore the character. */
                } else if (cRxedChar == '\b') {

                    /* Backspace was pressed.  Erase the last character in the
                     string - if any.*/
                    if (InputIndex > 0) {
                        InputIndex--;
                        cInputString[ InputIndex ] = '\0';
                    }

                } else {

                    /* A character was entered.  Add it to the string
                       entered so far.  When a \n is entered the complete
                       string will be passed to the command interpreter. */
                    if (InputIndex < 20) {
                        cInputString[ InputIndex ] = cRxedChar;
                        InputIndex++;
                    }
                }
            }
        }

        vTaskDelay(50);
    }
}

/**
 * @brief  Hardware Initialisation.
 * @param  None
 * @retval None
 */
void Hardware_init(void) {

    portDISABLE_INTERRUPTS(); //Disable interrupts

    BRD_LEDInit(); //Initialise Blue LED
    BRD_LEDOff(); //Turn off Blue LED

    portENABLE_INTERRUPTS(); //Enable interrupts

}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Pub Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvPubCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;
	const char *cValue_string;
	char topic[50];
	MQTTMessage new_message;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);
	cValue_string = FreeRTOS_CLIGetParameter(pcCommandString, 2, &lParam_len);

    /* Write command echo output string to write buffer. */
	memset(topic, 0, sizeof(topic));
	sprintf((char *) topic, "np2/%s\n\r", cCmd_string);    
	xWriteBufferLen = sprintf((char *) pcWriteBuffer, "publish %s %s\n\r", topic, cValue_string);
	
	/* Create message to send */
    new_message.payloadlen = 7; //lParam_len;
    new_message.payload = "working"; //cValue_string;
    new_message.qos = QOS0;
    MQTTPublish(&mqttclient, topic, &new_message);

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Sub Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvSubCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;
	char topic[50];

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

	sprintf(topic, "np2/%s", cCmd_string);		//create topic

	/* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "Subscribed to %s\n\r", topic);

	if (MQTTSubscribe(&mqttclient, topic, QOS0, mqtt_messagehandler) == FAILURE) {
        print("MQTT - Subscription error\n");
    }


    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Application Tick Task.
 * @param  None
 * @retval None
 */
void vApplicationTickHook(void) {

    BRD_LEDOff();
}

/**
 * @brief  Idle Application Task
 * @param  None
 * @retval None
 */
void vApplicationIdleHook(void) {
    static portTickType xLastTx = 0;

    BRD_LEDOff();

    for (;;) {

        /* The idle hook simply prints the idle tick count, every second */
        if ((xTaskGetTickCount() - xLastTx) > (1000 / portTICK_RATE_MS)) {

            xLastTx = xTaskGetTickCount();

            //debug_printf("IDLE Tick %d\n", xLastTx);

            /* Blink Alive LED */
            BRD_LEDToggle();
        }
    }
}

/**
 * @brief  vApplicationStackOverflowHook
 * @param  Task Handler and Task Name
 * @retval None
 */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
    /* This function will get called if a task overflows its stack.   If the
    parameters are corrupt then inspect pxCurrentTCB to find which was the
    offending task. */

    BRD_LEDOff();
    (void) pxTask;
    (void) pcTaskName;

    for (;;);
}

