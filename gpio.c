#include "gpio.h"

/******************************************************************************
 * @brief Initialize general input output and drive strength
 * @param none
 * @return none
 *****************************************************************************/
void gpio_init(void){

	//Set LED ports to be standard output drive with default off (cleared)
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	GPIO_DriveStrengthSet(SENS_EN_PORT, gpioDriveStrengthWeakAlternateWeak);        // set drive strength to weak for temp sensor enable
	GPIO_PinModeSet(SENS_EN_PORT, SENS_EN_PIN, gpioModePushPull, DISABLE_SENSOR);   // set up GPIO pin PB10 (temp sensor enable)
}
