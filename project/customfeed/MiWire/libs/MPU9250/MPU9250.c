/**
 * MPU9250 class. Based on the InvenSense MPU-9250 register map (PDF).
 */
 /**
  *
  * Note: This class runs in I2C mode by default.
  */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include "MPU9250.h"
#include "MPU9250_reg.h"

int file;
unsigned long addr;


void mpu9250_init(int i2c_adaptor, bool use_alt_addr) {
    adapter_nr = i2c_adaptor;

    if(use_alt_addr)
      addr = MPU9250_I2C_ADDRESS_ALT;
    else
      addr = MPU9250_I2C_ADDRESS;

    mpu9250_configure_i2c(i2c_adaptor);

}


void mpu9250_configure_i2c(int adapter_nr) {
    char filename[20];
    int rc;


    sprintf(filename,"/dev/i2c-%d",adapter_nr);

    if ((file = open(filename,O_RDWR)) < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      fprintf(stderr, "%s: failed to open %s, err=%d\n",argv[0],filename,file);
      exit(1);
    }


    if ((rc=ioctl(file,I2C_SLAVE,addr)) < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      fprintf(stderr, "%s: addr ioctl failed, addr=0x%03lX, err=%d\n",argv[0],addr,rc);
      exit(1);
    } else
      fprintf(stderr, "%s: ioctl set I2C_SLAVE addr=0x%03lX\n",argv[0],addr);
    }
}

/**
 *
 *
 *
 */
void mpu9250_configure_intn() {
  // @TODO Implement INT and FSYNC features.
//    struct extint_chan_conf eic_conf;
//
//    /* Configure the external interrupt channel. */
//    extint_chan_get_config_defaults(&eic_conf);
//    eic_conf.gpio_pin            = PORT_PA18A_EIC_EXTINT2;
//    eic_conf.gpio_pin_mux        = PINMUX_PA18A_EIC_EXTINT2;
//    eic_conf.gpio_pin_pull       = EXTINT_PULL_UP;
//    eic_conf.detection_criteria  = EXTINT_DETECT_FALLING;
//    eic_conf.filter_input_signal = true;
//    extint_chan_set_config(SW0_EIC_LINE, &eic_conf);
//
//    /* Register and enable the callback function. */
//    extint_register_callback(p_handler,
//            SW0_EIC_LINE,
//            EXTINT_CALLBACK_TYPE_DETECT);
//    extint_chan_enable_callback(SW0_EIC_LINE,
//            EXTINT_CALLBACK_TYPE_DETECT);
}


uint8_t mpu9250_self_test_x_gyro_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_self_test_x_gyro_set(uint8_t value) {

}

uint8_t mpu9250_self_test_y_gyro_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_self_test_y_gyro_set(uint8_t value) {}

uint8_t mpu9250_self_test_z_gyro_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_self_test_z_gyro_set(uint8_t value) {}

uint8_t mpu9250_self_test_x_accel_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_self_test_x_accel_set(uint8_t value) {}

uint8_t mpu9250_self_test_y_accel_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_self_test_y_accel_set(uint8_t value) {}

uint8_t mpu9250_self_test_z_accel_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_self_test_z_accel_set(uint8_t value) {}

uint8_t mpu9250_gyro_offset_registers_x_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_gyro_offset_registers_x_set(uint8_t value) {}

uint8_t mpu9250_gyro_offset_registers_y_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_gyro_offset_registers_y_set(uint8_t value) {}

uint8_t mpu9250_gyro_offset_registers_z_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_gyro_offset_registers_z_set(uint8_t value) {}

uint8_t mpu9250_sample_rate_divider_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_sample_rate_divider_set(uint8_t value) {}

uint8_t mpu9250_config_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_config_set(uint8_t value) {}

uint8_t mpu9250_gyroscope_config_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_gyroscope_config_set(uint8_t value) {}

uint8_t mpu9250_accelerometer_config_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_accelerometer_config_set(uint8_t value) {}

uint8_t mpu9250_accelerometer_config2_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_accelerometer_config2_set(uint8_t value) {}

uint8_t mpu9250_low_power_accelerometer_odr_control_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_low_power_accelerometer_odr_control_set(uint8_t value) {}

uint8_t mpu9250_wake_on_motion_threshold_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_wake_on_motion_threshold_set(uint8_t value) {}

uint8_t mpu9250_fifo_enable_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_fifo_enable_set(uint8_t value) {}

uint8_t mpu9250_master_control_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_master_control_set(uint8_t value) {}

uint8_t mpu9250_slave0_address_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave0_address_set(uint8_t value) {}

uint8_t mpu9250_slave0_register_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave0_register_set(uint8_t value) {}

uint8_t mpu9250_slave0_control_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave0_control_set(uint8_t value) {}

uint8_t mpu9250_slave1_address_get() {
  return 0; // @TODO Implement this.
};
void mpu9250_slave1_address_set(uint8_t value) {}

uint8_t mpu9250_slave1_register_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave1_register_set(uint8_t value) {}

uint8_t mpu9250_slave1_control_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave1_control_set(uint8_t value) {}

uint8_t mpu9250_slave2_address_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave2_address_set(uint8_t value) {}

uint8_t mpu9250_slave2_register_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave2_register_set(uint8_t value) {}

uint8_t mpu9250_slave2_control_get() {
  return 0; // @TODO Implement this.
}
void mpu9250_slave2_control_set(uint8_t value) {}

uint8_t mpu9250_slave3_address_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave3_address_set(uint8_t value) {}

uint8_t mpu9250_slave3_register_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave3_register_set(uint8_t value) {}

uint8_t mpu9250_slave3_control_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave3_control_set(uint8_t value) {}

uint8_t mpu9250_slave4_address_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave4_address_set(uint8_t value) {}

uint8_t mpu9250_slave4_register_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave4_register_set(uint8_t value) {}

uint8_t mpu9250_slave4_data_output_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave4_data_output_set(uint8_t value) {}

uint8_t mpu9250_slave4_control_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave4_control_set(uint8_t value) {}

uint8_t mpu9250_slave4_data_input_get() {
    return 0; //@TODO Implement this.
}
// No setter for I2C_SLV4_DI (Read-only)
// void mpu9250_slave4_data_input_set(uint8_t value) {}

uint8_t mpu9250_master_status_get() {
    return 0; //@TODO Implement this.
}
// No setter for I2C_MST_STATUS (Read-only)
// void mpu9250_master_status_set(uint8_t value) {}

uint8_t mpu9250_int_pin_config_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_int_pin_config_set(uint8_t value) {}

uint8_t mpu9250_interrupt_enable_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_interrupt_enable_set(uint8_t value) {}

uint8_t mpu9250_interrupt_status_get() {
    return 0; //@TODO Implement this.
}
// No setter for INT_STATUS (Read-only)
// void mpu9250_interrupt_status_set(uint8_t value) {}

uint8_t mpu9250_accel_xout_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_accel_xout_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_accel_yout_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_accel_yout_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_accel_zout_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_accel_zout_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_temp_out_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_temp_out_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_gyro_xout_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_gyro_xout_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_gyro_yout_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_gyro_yout_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_gyro_zout_h() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_gyro_zout_l() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_ext_sensor_data_get(int registerNumber) {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_slave0_data_out_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave0_data_out_set(uint8_t value) {}

uint8_t mpu9250_slave1_data_out_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave1_data_out_set(uint8_t value) {}

uint8_t mpu9250_slave2_data_out_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave2_data_out_set(uint8_t value) {}

uint8_t mpu9250_slave3_data_out_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_slave3_data_out_set(uint8_t value) {}

uint8_t mpu9250_master_delay_control_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_master_delay_control_set(uint8_t value) {}

uint8_t mpu9250_signal_path_reset_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_signal_path_reset_set(uint8_t value) {}

uint8_t mpu9250_accelerometer_interrupt_control_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_accelerometer_interrupt_control_set(uint8_t value) {}

uint8_t mpu9250_user_control_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_user_control_set(uint8_t value) {}

uint8_t mpu9250_power_management_1_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_power_management_1_set(uint8_t value) {}

uint8_t mpu9250_power_management_2_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_power_management_2_set(uint8_t value) {}

uint8_t mpu9250_fifo_counth_get() {
    return 0; //@TODO Implement this.
}
// No setter for FIFO_COUNTH (Read-Only)
// void mpu9250_fifo_counth_set(uint8_t value) {}

uint8_t mpu9250_fifo_countl_get() {
    return 0; //@TODO Implement this.
}
// No setter for FIFO_COUNTL (Read-Only)
// void mpu9250_fifo_countl_set(uint8_t value) {}

uint8_t mpu9250_fifo_read_write_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_fifo_read_write_set(uint8_t value) {}

uint8_t mpu9250_whoami_get() {
    return 0; //@TODO Implement this.
}
// No setter for WHOAMI (Read-Only)
// void mpu9250_whoami_set(uint8_t value) {}

uint8_t mpu9250_x_accel_offset_h_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_x_accel_offset_h_set(uint8_t value) {}

uint8_t mpu9250_x_accel_offset_l_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_x_accel_offset_l_set(uint8_t value) {}

uint8_t mpu9250_y_accel_offset_h_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_y_accel_offset_h_set(uint8_t value) {}

uint8_t mpu9250_y_accel_offset_l_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_y_accel_offset_l_set(uint8_t value) {}

uint8_t mpu9250_z_accel_offset_h_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_z_accel_offset_h_set(uint8_t value) {}

uint8_t mpu9250_z_accel_offset_l_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_z_accel_offset_l_set(uint8_t value) {}

uint8_t mpu9250_magnetometer_wia_device_id_get() {
    return 0; //@TODO Implement this.
}
// No setter for WIA: Device ID (Read-Only)

uint8_t mpu9250_magnetometer_information_get() {
    return 0; //@TODO Implement this.
}
// No setter for INFO: Information (Read-Only)

uint8_t mpu9250_magnetometer_status_1_get() {
    return 0; //@TODO Implement this.
}
// No setter for ST1: Status 1

uint8_t mpu9250_magnetometer_measurement_data_hxl_get() {
    return 0; //@TODO Implement this.
}
uint8_t mpu9250_magnetometer_measurement_data_hxh_get() {
    return 0; //@TODO Implement this.
}
uint8_t mpu9250_magnetometer_measurement_data_hyl_get() {
    return 0; //@TODO Implement this.
}
uint8_t mpu9250_magnetometer_measurement_data_hyh_get() {
    return 0; //@TODO Implement this.
}
uint8_t mpu9250_magnetometer_measurement_data_hzl_get() {
    return 0; //@TODO Implement this.
}
uint8_t mpu9250_magnetometer_measurement_data_hzh_get() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_magnetometer_status_2_get() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_magnetometer_control_1_get() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_magnetometer_control_2_get() {
    return 0; //@TODO Implement this.
}

uint8_t mpu9250_magnetometer_self_test_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_magnetometer_self_test_set(uint8_t value) {}

uint8_t mpu9250_magnetometer_i2c_disable_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_magnetometer_i2c_disable_set(uint8_t value) {}

uint8_t mpu9250_magnetometer_sensitivity_adjustment_x_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_magnetometer_sensitivity_adjustment_x_set(uint8_t value) {}
uint8_t mpu9250_magnetometer_sensitivity_adjustment_y_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_magnetometer_sensitivity_adjustment_y_set(uint8_t value) {}
uint8_t mpu9250_magnetometer_sensitivity_adjustment_z_get() {
    return 0; //@TODO Implement this.
}
void mpu9250_magnetometer_sensitivity_adjustment_z_set(uint8_t value) {}
