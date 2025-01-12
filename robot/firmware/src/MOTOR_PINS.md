
# pins layout

## swerve A

precision motor AH:
    motor: CRC_PWM_4
    enco: CRC_ENCO_A, CRC_DIG_2
    pids:
        speed: K 0.60000 0.00001 0.10000
        angle: ???

precision motor AB:
    motor: CRC_PWM_3
    enco: CRC_I2C_SDA, CRC_DIG_4
    pids:
        speed: K 0.60000 0.00001 0.06000
        angle: ???

abs enco:
    pins: ???
    pid:
        angle: ???

## swerve B

precision motor BH:
    motor: CRC_PWM_7
    enco: CRC_I2C_SCL, CRC_DIG_5
    pids:
        speed: K 0.60000 0.00001 0.15500
        angle: ???

precision motor BB:
    motor: CRC_PWM_1
    enco: CRC_ENCO_B, CRC_DIG_3
    pids:
        speed: K 0.60000 0.00001 0.12000
        angle: ???

abs enco:
    pins: ???
    pid:
        angle: ???

## launcher

precision motor BH:
    motor: ???
    enco: ???, ???
    pids:
        speed: K 1.00000 0.00001 0.28500
        angle: ???
