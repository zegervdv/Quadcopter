/**
 * sensors.c
 *
 * Configure and read data from the onboard sensors
 * of the STM32F3 Discovery
 */

#include "sensors.h"
#include "sensor_data.h"

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

  L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;

  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
}

void gyroscope_read(uint8_t* data) {
  L3GD20_Read(data, L3GD20_OUT_X_L_ADDR, 6);
}

void gyroscope_to_float(uint8_t* data, float* value) {
  uint8_t i;
  uint32_t temp;
  for(i = 0; i < 3; i++) {
    temp = ((uint32_t) data[2*i] << 8) + data[2*i+1];
    if(temp & 0x8000)
      temp -= 0xFFFF;
    value[i] = (float) temp / L3G_Sensitivity_500dps;
  }
}

void compass_init(void) {
  LSM303DLHCMag_InitTypeDef LSM303DLHC_InitStructure;

  LSM303DLHC_InitStructure.Temperature_Sensor = LSM303DLHC_TEMPSENSOR_DISABLE;
  LSM303DLHC_InitStructure.MagOutput_DataRate =LSM303DLHC_ODR_30_HZ ;
  LSM303DLHC_InitStructure.MagFull_Scale = LSM303DLHC_FS_8_1_GA;
  LSM303DLHC_InitStructure.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION;
  LSM303DLHC_MagInit(&LSM303DLHC_InitStructure);
}

void compass_read(uint8_t* data) {
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, data, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M, data+1, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M, data+2, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M, data+3, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M, data+4, 1);
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M, data+5, 1);
}

void compass_to_float(uint8_t* data, float* value) {
  uint8_t i;
  uint32_t temp[3];
  for(i = 0; i < 2; i++) {
    temp[i] = (((uint16_t)data[2*i] << 8) + data[2*i+1])*1000;
    temp[i] /= LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;
  }
  temp[2] = (((uint16_t)data[4] << 8) + data[5])*1000;
  temp[2] /= LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;

  for(i = 0; i < 3; i++) {
    if(temp[i] & 0x8000)
      value[i] = (float)(temp[i] - 0xFFFF);
  }
}

void accelerometer_init(void) {
  LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitStructure;
  LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;

  LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
  LSM303DLHCAcc_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
  LSM303DLHCAcc_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
  LSM303DLHCAcc_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
  LSM303DLHCAcc_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
  LSM303DLHCAcc_InitStructure.Endianness=LSM303DLHC_BLE_MSB;
  LSM303DLHCAcc_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;
  /* Configure the accelerometer main parameters */
  LSM303DLHC_AccInit(&LSM303DLHCAcc_InitStructure);
  
  /* Fill the accelerometer LPF structure */
  LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection =LSM303DLHC_HPM_NORMAL_MODE;
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
  uint32_t temp;
  
  for (i = 0; i < 3; i++) {
    temp = ((uint16_t)(data[2*i] << 8) + data[2*i+1]) >> 4;
    value[i] = (float) temp / LSM_Acc_Sensitivity_2g;
  }
}
