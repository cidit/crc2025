
# pins layout

## swerve A

precision motor AH:
    motor: CRC_PWM_4
    enco: CRC_ENCO_A, CRC_DIG_2

precision motor AB:
    motor: CRC_PWM_3
    enco: CRC_I2C_SDA, CRC_DIG_4

abs enco: ???

## swerve B

precision motor BH:
    motor: CRC_PWM_7
    enco: CRC_I2C_SCL, CRC_DIG_5

precision motor BB:
    motor: CRC_PWM_1
    enco: CRC_ENCO_B, CRC_DIG_3

abs enco: ???
