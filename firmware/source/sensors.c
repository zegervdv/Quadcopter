/**
 * sensors.c
 *
 * Configure and read data from the onboard sensors
 * of the STM32F3 Discovery
 */

#include "sensors.h"
#include "arm_math.h"

#define SQ(x) ((x) * (x))

/**
 * Moving averages for Pitch, Roll and Yaw
 * Values are N * Moving Average
 */
sensor_average sensor_moving_average = {0};

void gyroscope_init(void) {
  L3GD20_InitTypeDef L3GD20_InitStructure;
  L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;

  /* Configure Mems L3GD20 */
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
  L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
  L3GD20_InitStructure.Endianness = L3GD20_BLE_MSB;
  L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500;
  L3GD20_Init(&L3GD20_InitStructure);

  L3GD20_FilterStructure.HighPassFilter_Mode_Selection = L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;

  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
}

void gyroscope_read(uint8_t* data) {
  L3GD20_Read(data, L3GD20_OUT_X_L_ADDR, 6);
}

void gyroscope_to_float(uint8_t* data, float* value) {
  uint8_t i;
  int32_t temp;
  for (i = 0; i < 3; i++) {
    temp = ((uint32_t) data[2 * i] << 8) + data[2 * i + 1];
    if (temp & 0x8000)
      temp -= 0xFFFF;
    value[i] = (float) temp / L3G_Sensitivity_500dps;
  }
}

void compass_init(void) {
  LSM303DLHCMag_InitTypeDef LSM303DLHC_InitStructure;

  LSM303DLHC_InitStructure.Temperature_Sensor = LSM303DLHC_TEMPSENSOR_DISABLE;
  LSM303DLHC_InitStructure.MagOutput_DataRate = LSM303DLHC_ODR_30_HZ;
  LSM303DLHC_InitStructure.MagFull_Scale = LSM303DLHC_FS_8_1_GA;
  LSM303DLHC_InitStructure.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION;
  LSM303DLHC_MagInit(&LSM303DLHC_InitStructure);
}

void compass_read(uint8_t* data) {
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, data, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M, data + 1, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M, data + 2, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M, data + 3, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M, data + 4, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M, data + 5, 1);
}

void compass_to_float(uint8_t* data, float* value) {
  uint8_t i;
  int32_t temp[3];
  for (i = 0; i < 2; i++) {
    temp[i] = (((uint16_t)data[2 * i] << 8) + data[2 * i + 1]) * 1000;
    temp[i] /= LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;
  }
  temp[2] = (((uint16_t)data[4] << 8) + data[5]) * 1000;
  temp[2] /= LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;

  for (i = 0; i < 3; i++) {
    if (temp[i] & 0x8000)
      value[i] = (float)(temp[i] - 0xFFFF);
  }
}

void accelerometer_init(void) {
  LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitStructure;
  LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;

  LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
  LSM303DLHCAcc_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
  LSM303DLHCAcc_InitStructure.Axes_Enable = LSM303DLHC_AXES_ENABLE;
  LSM303DLHCAcc_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
  LSM303DLHCAcc_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
  LSM303DLHCAcc_InitStructure.Endianness = LSM303DLHC_BLE_MSB;
  LSM303DLHCAcc_InitStructure.High_Resolution = LSM303DLHC_HR_ENABLE;
  /* Configure the accelerometer main parameters */
  LSM303DLHC_AccInit(&LSM303DLHCAcc_InitStructure);

  /* Fill the accelerometer LPF structure */
  LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection = LSM303DLHC_HPM_NORMAL_MODE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE;

  /* Configure the accelerometer LPF main parameters */
  LSM303DLHC_AccFilterConfig(&LSM303DLHCFilter_InitStructure);
}

void accelerometer_read(uint8_t* data) {
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, data, 6);
}

void accelerometer_to_float(uint8_t* data, float* value) {
  uint8_t i;
  int16_t temp;

  for (i = 0; i < 3; i++) {
    temp = (int16_t)((uint16_t)(data[2 * i] << 8) + data[2 * i + 1]) / 16;
    value[i] = (float) temp / LSM_Acc_Sensitivity_2g;
  }
}

void battery_init(void) {
  GPIO_InitTypeDef gpio_init;
  ADC_InitTypeDef adc_init;
  ADC_CommonInitTypeDef adc_common_init;

  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  gpio_init.GPIO_Pin = GPIO_Pin_1;
  gpio_init.GPIO_Mode = GPIO_Mode_AN;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &gpio_init);

  ADC_StructInit(&adc_init);
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);

  /* Calibrate ADC */
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
  while (ADC_GetCalibrationStatus(ADC1) != RESET);
  /* calibration_value = ADC_GetCalibrationValue(ADC1); */

  adc_common_init.ADC_Mode = ADC_Mode_Independent;
  adc_common_init.ADC_Clock = ADC_Clock_AsynClkMode;
  adc_common_init.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  adc_common_init.ADC_DMAMode = ADC_DMAMode_OneShot;
  adc_common_init.ADC_TwoSamplingDelay = 0;
  ADC_CommonInit(ADC1, &adc_common_init);

  adc_init.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
  adc_init.ADC_Resolution = ADC_Resolution_12b;
  adc_init.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
  adc_init.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
  adc_init.ADC_DataAlign = ADC_DataAlign_Right;
  adc_init.ADC_OverrunMode = ADC_OverrunMode_Disable;
  adc_init.ADC_AutoInjMode = ADC_AutoInjec_Disable;
  adc_init.ADC_NbrOfRegChannel = 1;
  ADC_Init(ADC1, &adc_init);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_7Cycles5);
  ADC_Cmd(ADC1, ENABLE);

  while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  ADC_StartConversion(ADC1);
}

void battery_read(float* data) {
  __IO uint16_t raw;
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  raw = ADC_GetConversionValue(ADC1);
  *data = (float)((raw * 3300) / 0xFFF);
}

void battery_to_float(float* data, float* value) {
  *value = (*data) * BAT_SCALE;
}

void sensors_format_data(uint8_t* gyro, uint8_t* accelero, uint8_t* magneto, float altitude, float battery, sensor_data* data) {
  float parsed[3] = {0};
  float val = 0;
  float xh, yh;

  gyroscope_to_float(gyro, parsed);
  data->x_rotation = parsed[0];
  data->y_rotation = parsed[1];
  data->z_rotation = parsed[2];

  // Rotate axes
  compass_to_float(magneto, parsed);
  data->x_magnetic = -1 * parsed[0];
  data->y_magnetic = -1 * parsed[1];
  data->z_magnetic = parsed[2];

  accelerometer_to_float(accelero, parsed);
  data->x_acceleration = -1 * parsed[0];
  data->y_acceleration = -1 * parsed[1];
  data->z_acceleration = parsed[2];

  sensor_moving_average.pitch = sensor_moving_average.pitch + atan2(data->x_acceleration, sqrt(SQ(data->y_acceleration) + SQ(data->z_acceleration))) - (sensor_moving_average.pitch / AVG_WNDW_SIZE);
  sensor_moving_average.roll = sensor_moving_average.roll + atan2(data->y_acceleration, sqrt(SQ(data->x_acceleration) + SQ(data->z_acceleration))) - (sensor_moving_average.roll / AVG_WNDW_SIZE);

  data->pitch = sensor_moving_average.pitch / AVG_WNDW_SIZE;
  data->roll = sensor_moving_average.roll / AVG_WNDW_SIZE;

  // Calculate compensation for X and Y axes
  xh = data->x_magnetic * cos(data->pitch) + data->z_magnetic * sin(data->pitch);
  yh = data->x_magnetic * sin(data->roll) * sin(data->pitch) + data->y_magnetic * cos(data->roll) - data->z_magnetic * sin(data->roll) * cos(data->pitch);

  // Test conditions for xh and yh to assure atan(yh/xh) is valid
  // see: https://www.pololu.com/file/0J434/LSM303DLH-compass-app-note.pdf at
  // p23
  if (xh < 0) {
    val = atan2(yh, xh) + PI;
  } else if (xh > 0) {
    val = atan2(yh, xh);
    if (yh < 0) {
      val += 2 * PI;
    }
  } else {
    val = PI / 2;
    if (yh > 0) {
      val *= 3;
    }
  }
  sensor_moving_average.yaw = sensor_moving_average.yaw + val - (sensor_moving_average.yaw / AVG_WNDW_SIZE);

  data->yaw = sensor_moving_average.yaw / AVG_WNDW_SIZE;

  // TODO: Add Height
  data->altitude = altitude;

  battery_to_float(&battery, &val);
  data->battery = val;
}
