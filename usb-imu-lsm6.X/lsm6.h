// Copyright Pololu Corporation.  For more information, see https://www.pololu.com/

// This is the header file for lsm6.c, a library for reading data from the
// LSM6DS33 accelerometer/gyro.

#ifndef _LSM6_H
#define _LSM6_H

#include <stdint.h>

enum LSM6DeviceType {
    LSM6_DEVICE_TYPE_AUTO,
    LSM6_DEVICE_TYPE_DS33
};

enum LSM6SA0State {
    LSM6_SA0_AUTO,
    LSM6_SA0_LOW,
    LSM6_SA0_HIGH
};

// Addresses of LSM6 registers.
enum LSM6RegAddr
{
    LSM6_FUNC_CFG_ACCESS   = 0x01,

    LSM6_FIFO_CTRL1        = 0x06,
    LSM6_FIFO_CTRL2        = 0x07,
    LSM6_FIFO_CTRL3        = 0x08,
    LSM6_FIFO_CTRL4        = 0x09,
    LSM6_FIFO_CTRL5        = 0x0A,
    LSM6_ORIENT_CFG_G      = 0x0B,

    LSM6_INT1_CTRL         = 0x0D,
    LSM6_INT2_CTRL         = 0x0E,
    LSM6_WHO_AM_I          = 0x0F,
    LSM6_CTRL1_XL          = 0x10,
    LSM6_CTRL2_G           = 0x11,
    LSM6_CTRL3_C           = 0x12,
    LSM6_CTRL4_C           = 0x13,
    LSM6_CTRL5_C           = 0x14,
    LSM6_CTRL6_C           = 0x15,
    LSM6_CTRL7_G           = 0x16,
    LSM6_CTRL8_XL          = 0x17,
    LSM6_CTRL9_XL          = 0x18,
    LSM6_CTRL10_C          = 0x19,

    LSM6_WAKE_UP_SRC       = 0x1B,
    LSM6_TAP_SRC           = 0x1C,
    LSM6_D6D_SRC           = 0x1D,
    LSM6_STATUS_REG        = 0x1E,

    LSM6_OUT_TEMP_L        = 0x20,
    LSM6_OUT_TEMP_H        = 0x21,
    LSM6_OUTX_L_G          = 0x22,
    LSM6_OUTX_H_G          = 0x23,
    LSM6_OUTY_L_G          = 0x24,
    LSM6_OUTY_H_G          = 0x25,
    LSM6_OUTZ_L_G          = 0x26,
    LSM6_OUTZ_H_G          = 0x27,
    LSM6_OUTX_L_XL         = 0x28,
    LSM6_OUTX_H_XL         = 0x29,
    LSM6_OUTY_L_XL         = 0x2A,
    LSM6_OUTY_H_XL         = 0x2B,
    LSM6_OUTZ_L_XL         = 0x2C,
    LSM6_OUTZ_H_XL         = 0x2D,

    LSM6_FIFO_STATUS1      = 0x3A,
    LSM6_FIFO_STATUS2      = 0x3B,
    LSM6_FIFO_STATUS3      = 0x3C,
    LSM6_FIFO_STATUS4      = 0x3D,
    LSM6_FIFO_DATA_OUT_L   = 0x3E,
    LSM6_FIFO_DATA_OUT_H   = 0x3F,
    LSM6_TIMESTAMP0_REG    = 0x40,
    LSM6_TIMESTAMP1_REG    = 0x41,
    LSM6_TIMESTAMP2_REG    = 0x42,

    LSM6_STEP_TIMESTAMP_L  = 0x49,
    LSM6_STEP_TIMESTAMP_H  = 0x4A,
    LSM6_STEP_COUNTER_L    = 0x4B,
    LSM6_STEP_COUNTER_H    = 0x4C,

    LSM6_FUNC_SRC          = 0x53,

    LSM6_TAP_CFG           = 0x58,
    LSM6_TAP_THS_6D        = 0x59,
    LSM6_INT_DUR2          = 0x5A,
    LSM6_WAKE_UP_THS       = 0x5B,
    LSM6_WAKE_UP_DUR       = 0x5C,
    LSM6_FREE_FALL         = 0x5D,
    LSM6_MD1_CFG           = 0x5E,
    LSM6_MD2_CFG           = 0x5F,
};

// Represents a single LSM6DS33 device.
typedef struct LSM6 {
    enum LSM6DeviceType device;
    uint8_t address;

    // Functions that perform I2C transfers to the LSM6 will modify this variable
    // to indicate whether they succeeded.  0 means success, non-zero indicates
    // an error.
    uint8_t lastResult;

    // The last set of raw accelerometer readings from the device, in XYZ order.
    int16_t a[3];

    // The last set of raw gyro readings from the device, in XYZ order.
    int16_t g[3];
} LSM6;

// Initializes the library and confirms that we can communicate with
// the device.  Returns 1 for success and 0 for failure.
uint8_t lsm6Init(LSM6 *, enum LSM6DeviceType, enum LSM6SA0State);

// Enables the LSM6's accelerometer and gyro. Also:
//
// - Sets sensor full scales (gain) to default power-on values, which are
//   +/- 2 g for accelerometer and 245 dps for gyro
// - Selects 1.66 kHz (high performance) ODR (output data rate) for
//   accelerometer and 1.66 kHz (high performance) ODR for gyro. (These are the
//   ODR settings for which the electrical characteristics are specified in the
//   datasheet.)
// - Enables automatic increment of register address during multiple byte access
//
// Note that this function will also reset other settings controlled by
// the registers it writes to.
void lsm6EnableDefault(LSM6 *);

// Writes to one of the LSM6 registers.  The "reg" argument should be the
// register address and the "value" argument should be the value to write.
void lsm6WriteReg(LSM6 *, uint8_t reg, uint8_t value);

// Reads from an LSM6 register and returns the value.  The "reg" argument should
// be the register address.
uint8_t lsm6ReadReg(LSM6 *, uint8_t reg);

// Reads from the accelerometer and gyro and stores the results in the "a" and
// "g" members of the LSM6 object.
void lsm6Read(LSM6 *);

// Reads the accelerometer and stores the results in the "a" member of the LSM6
// object.
void lsm6ReadAcc(LSM6 *);

// Reads the gyro and stores the results in the "g" member of the LSM6 object.
void lsm6ReadGyro(LSM6 *);

#endif
