#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define MPU_ADDR 0x68

static void mpu6050_reset()
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(i2c_default, MPU_ADDR, buf, 2, false);
}

static void mpu6050_read(int16_t acc[3], int16_t gyro[3])
{
    uint8_t buffer[6];

    //Reading the accelerometer value
    uint8_t acc_addr = 0x3B;
    i2c_write_blocking(I2C_PORT, MPU_ADDR, &acc_addr, 1, true);
    i2c_read_blocking(I2C_PORT, MPU_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) 
    {
        acc[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }
    printf("AccX: %d, AccY: %d, AccZ: %d\n", acc[0], acc[1], acc[2]);

    //Reading the gyro value
    uint8_t gyro_addr = 0x43;
    i2c_write_blocking(I2C_PORT, MPU_ADDR, &gyro_addr, 1, true);
    i2c_read_blocking(I2C_PORT, MPU_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) 
    {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }
    printf("GX: %d, GY: %d, GZ: %d\n", gyro[0], gyro[1], gyro[2]);

}

int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Example to turn on the Pico W LED
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    printf("Starting the mpu6050");
    mpu6050_reset();
    printf("mpu6050 is started");
    uint16_t acc[3], gyro[3];
    
    while (true) 
    {
        mpu6050_read(acc, gyro);
    }
}
