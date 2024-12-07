// Includes
#include "main.h"
#include "Ti_Tmp100.hpp"
#include "Microchip_24fc.hpp"
#include "Logger.hpp"

// Defines
#define TEMP_SENSOR_I2C_ADDRESS (0x48)   // I2C address when A0-A2 are all tied to ground
#define EEPROM_I2C_ADDRESS      (0xA0)   // I2C address of the EEPROM
#define EEPROM_SIZE             (0x8000) // 256Kbit EEPROM

#define MS_PER_S                (1000)
#define SECONDS_PER_MINUTE      (60)
#define TEN_MINUTES             (10 * SECONDS_PER_MINUTE)

// Variables
static I2C_HandleTypeDef hi2c1;
static Ti_Tmp100 tempSensor;
static Microchip_24FC eeprom;
static Logger logger;
static ErrorStatus result;

// Private function prototypes
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

int main(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick
  HAL_Init();

  // Configure the system clock
  SystemClock_Config
  ();

  //  Initialize all configured peripherals
  MX_GPIO_Init();
  MX_I2C1_Init();

  // Init temp sensor (TMP100) and EEPROM (24FC256)
  tempSensor.Init(&hi2c1, TEMP_SENSOR_I2C_ADDRESS);
  eeprom.Init(&hi2c1, EEPROM_I2C_ADDRESS, EEPROM_SIZE);
  logger.Init(&eeprom);

  // Infinite loop
  while (1)
  {
    // Read temperature from TMP100
    if (SUCCESS == tempSensor.ReadTemperature()) {
      // Save raw temp measurement to EEPROM
      logger.AddTemperatureReading(tempSensor.GetTemperatureRaw());
    }

    // Delay for 10 minutes
    HAL_Delay(TEN_MINUTES * MS_PER_S);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add their own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}
