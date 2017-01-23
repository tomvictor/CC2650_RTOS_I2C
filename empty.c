
/*
 *  ======== empty.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/PIN.h>
// #include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header files */
#include "Board.h"
#include "HAL_BQ27441.h"



#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/* Pin driver handle */
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

static I2C_Handle i2cHandle;
static I2C_Params i2cParams;
I2C_Transaction i2cTransaction;

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config ledPinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void heartBeatFxn(UArg arg0, UArg arg1)
{
    I2C_Transaction i2cTransaction;
    const char echoPrompt[] = "\fEchoing characters:\r\n";
        UART_Handle uart;
        UART_Params uartParams;
        UART_Params_init(&uartParams);
            uartParams.writeDataMode = UART_DATA_BINARY;
            uartParams.readDataMode = UART_DATA_BINARY;
            uartParams.readReturnMode = UART_RETURN_FULL;
            uartParams.readEcho = UART_ECHO_OFF;
            uartParams.baudRate = 9600;
            uart = UART_open(Board_UART0, &uartParams);
            if (uart == NULL) {
                    System_abort("Error opening the UART");
                }
            UART_write(uart, echoPrompt, sizeof(echoPrompt));

    while (1) {
        Task_sleep((UInt)arg0);
        PIN_setOutputValue(ledPinHandle, Board_LED0,
                           !PIN_getOutputValue(Board_LED0));

        i2cTransaction.writeBuf = someWriteBuffer;
        i2cTransaction.writeCount = numOfBytesToWrite;
        i2cTransaction.readBuf = someReadBuffer;
        i2cTransaction.readCount = numOfBytesToRead;
        i2cTransaction.slaveAddress = some7BitI2CSlaveAddress;
        ret = I2C_transfer(handle, &i2cTransaction);
        if (!ret) {
            System_printf("Unsuccessful I2C transfer");
        }
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initI2C();
    // Board_initSPI();
    Board_initUART();
    // Board_initWatchdog();

      I2C_init();
      I2C_Params_init(&i2cParams);
      i2cParams.bitRate = I2C_400kHz;
      i2cHandle = I2C_open(Board_I2C, &i2cParams);
      if (!i2cHandle) {
          System_printf("I2C did not open");
      }

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000000 / Clock_tickPeriod;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle) {
        System_abort("Error initializing board LED pins\n");
    }



    PIN_setOutputValue(ledPinHandle, Board_LED1, 1);

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}


/* Transmits String over UART */
void UART_transmitString( char *pStr )
{
    while( *pStr )
    {
        UART_transmitData(EUSCI_A0_BASE, *pStr );
        UART_write(uart, *pStr, 1);
        pStr++;
    }
}

/* Configures BQ27441 device properties */
bool BQ27441_initConfig()
{
    short result = 0;

    UART_transmitString("*************************************\r\n");
    UART_transmitString("Initializing BQ27441 Configuration\r\n");
    UART_transmitString("*************************************\r\n");

    //Default Config, DesignCapacity = 1200mAh, DesignEnergy = 1200mAh*3.7V, Terminate Voltage = 3200mV, Taper Current = 120mA
    char str[64];
    sprintf(str, "DesignCapacity = %dmAh\r\n", CONF_DESIGN_CAPACITY);
    UART_transmitString(str);
    sprintf(str, "DesignEnergy = %dmAh * 3.7 = %dJ\r\n", CONF_DESIGN_CAPACITY, CONF_DESIGN_ENERGY);
    UART_transmitString(str);
    sprintf(str, "TerminateVoltage = %dmV\r\n", CONF_TERMINATE_VOLTAGE);
    UART_transmitString(str);
    sprintf(str, "TaperRate = %dmAh/(0.1*%dmA) = %d\r\n", CONF_DESIGN_CAPACITY, CONF_TAPER_CURRENT, CONF_TAPER_RATE);
    UART_transmitString(str);

    if (!BQ27441_read16(FLAGS, &result, 1000))
        return 0;

    /* Check if ITPOR bit is set in FLAGS */
    if (result & 0x0020)
    {
        /* Instructs fuel gauge to enter CONFIG UPDATE mode. */
        if (!BQ27441_control(SET_CFGUPDATE, 1000))
            return 0;

        __delay_cycles(1000000);

        result = 0;
        /* Check if CFGUPMODE bit is set in FLAGS */
        while(!(result & 0x0010))
        {
            if (!BQ27441_read16(FLAGS, &result, 1000))
                return 0;
        }

        /* Enable Block data memory control */
        if (!BQ27441_command(BLOCK_DATA_CONTROL, 0x00, 1000))
            return 0;

        /* Set the data class to be accessed */
        if (!BQ27441_command(DATA_CLASS, 0x52, 1000))
            return 0;

        /* Write the block offset loaction */
        if (!BQ27441_command(DATA_BLOCK, 0x00, 1000))
            return 0;

        __delay_cycles(1000000);

        char old_chksum = 0;
        char new_chksum = 0;
        char tmp_chksum = 0;
        char chksum = 0;
        do
        {
            /* Read Block Data Checksum */
            if (!BQ27441_readChecksum(&old_chksum, 1000))
                return 0;

            __delay_cycles(1000000);

            /* Checksum calculation */
            tmp_chksum = old_chksum;

            short old_designCapacity = 0;
            short old_designEnergy = 0;
            short old_terminateVoltage = 0;
            short old_taperRate = 0;

            /* Read old design capacity */
            if (!BQ27441_read16(0x4A, &old_designCapacity, 1000))
                return 0;

            tmp_chksum = computeCheckSum(tmp_chksum, old_designCapacity, CONF_DESIGN_CAPACITY);

            /* Read old design energy */
            if (!BQ27441_read16(0x4C, &old_designEnergy, 1000))
                return 0;

            tmp_chksum = computeCheckSum(tmp_chksum, old_designEnergy, CONF_DESIGN_ENERGY);

            /* Read old terminate voltage */
            if (!BQ27441_read16(0x50, &old_terminateVoltage, 1000))
                return 0;

            tmp_chksum = computeCheckSum(tmp_chksum, old_terminateVoltage, CONF_TERMINATE_VOLTAGE);

            /* Read old taper rate */
            if (!BQ27441_read16(0x5B, &old_taperRate, 1000))
                return 0;

            /* Checksum calculation */
            tmp_chksum = computeCheckSum(tmp_chksum, old_taperRate, CONF_TAPER_RATE);

            /* Write new design capacity */
            if (!BQ27441_write16(0x4A, swapMSB_LSB(CONF_DESIGN_CAPACITY), 1000))
                return 0;

            /* Write new design energy */
            if (!BQ27441_write16(0x4C, swapMSB_LSB(CONF_DESIGN_ENERGY), 1000))
                return 0;

            /* Write new terminate voltage */
            if (!BQ27441_write16(0x50, swapMSB_LSB(CONF_TERMINATE_VOLTAGE), 1000))
                return 0;

            /* Write new taper rate */
            if (!BQ27441_write16(0x5B, swapMSB_LSB(CONF_TAPER_RATE), 1000))
                return 0;

            /* Checksum calculation */
            new_chksum = tmp_chksum;

            /* Write new checksum */
            if (!BQ27441_command(BLOCK_DATA_CHECKSUM, new_chksum, 1000))
                return 0;

            __delay_cycles(1000000);

            /* Read Block Data Checksum */
            if (!BQ27441_readChecksum(&chksum, 1000))
                return 0;

            __delay_cycles(1000000);
        }
        while(new_chksum != chksum);

        /* Send SOFT_RESET control command */
        if (!BQ27441_control(SOFT_RESET, 1000))
            return 0;

        __delay_cycles(1000000);

        result = 0;
        /* Check if CFGUPMODE bit is cleared in FLAGS */
        while(result & 0x0010)
        {
            if (!BQ27441_read16(FLAGS, &result, 1000))
                return 0;
        }

        UART_transmitString("*************************************\r\n");
        UART_transmitString("BQ27441 config inialized succesfully\r\n");
        UART_transmitString("*************************************\r\n");
        return 1;
    }
    else
    {
        UART_transmitString("*************************************\r\n");
        UART_transmitString("BQ27441 config inialized already\r\n");
        UART_transmitString("*************************************\r\n");
        return 1;
    }
}

