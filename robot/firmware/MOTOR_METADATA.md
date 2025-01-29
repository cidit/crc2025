
# pins layout

## swerve A

precision motor AB: (A)
    motor: CRC_PWM_3
    enco: CRC_I2C_SDA, CRC_DIG_4
        on alduino: 1
    pids:
        speed: K 0.60000 0.00001 0.06000
        angle: ???

precision motor AH: (B)
    motor: CRC_PWM_4
    enco: CRC_ENCO_A, CRC_DIG_2
        on alduino: 0
    pids:
        speed: K 0.60000 0.00001 0.10000
        angle: ???

abs enco:
    pins: CRC_DIG_12
    pid:
        angle: ???

## swerve B

precision motor BB: (A)
    motor: CRC_PWM_1
    enco: CRC_ENCO_B, CRC_DIG_3
        on alduino: 2
    pids:
        speed: K 0.60000 0.00001 0.12000
        angle: ???

precision motor BH: (B)
    notes:
        motor is reversed
    motor: CRC_PWM_7
    enco: CRC_I2C_SCL, CRC_DIG_5
        on alduino: 3
    pids:
        speed: K 0.60000 0.00001 0.15500
        angle: ???

abs enco:
    pins: CRC_DIG_1
    pid:
        angle: ???

## launcher

precision motor:
    motor: ???
    enco: ???, ???
    pids:
        speed: K 1.00000 0.00001 0.28500
        angle: ???

## Bras

precision motor LB:
    motor: ???
    enco: 6
    pids:
        speed: K 1.00000 0.00001 0.28500
        angle: ???

precision motor RB:
    motor: ???
    enco: 5
    pids:
        speed: K 1.00000 0.00001 0.28500
        angle: ???

precision motor PB:
    motor: ???
    enco: 8
    pids:
        speed: K 1.00000 0.00001 0.28500
        angle: ???