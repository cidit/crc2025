#pragma once

constexpr uint32_t ONE_SECOND = 1000;
constexpr double EPSILON = 0.000001; // used as tolerance in floating point math

constexpr uint8_t HALF_PWM_OUTPUT = 128;


//----- Defines ------
#define TICKS_RATIO_BRAS 1425.1 * 2
#define TICKS_RATIO_SWERVE 145.1 * 2.5
#define TICKS_RATIO_LANCE 147

#define MAX_RPM_BRAS 117
#define MAX_RPM_SWERVE 400
#define MAX_RPM_LANCE 1150

#define NUM_MOTORS 8
// Index in array (Selon les branchements sur le ALDuino)
#define I_LAS 2
#define I_LBS 3
#define I_RAS 1
#define I_RBS 0
#define I_LB 5
#define I_RB 4
#define I_PB 6
#define I_L 7

// Constants pour la rotation du bras
#define VIT_BRAS_MS M_PI / 2000.0 * 0.1
#define LOW_STOP_BRAS 0.0
#define HIGH_STOP_BRAS 3.14

#define VIT_POIGNET_MS M_PI / 2000.0 * 1.0
#define LOW_STOP_POIGNET -4 // À REVOIR LES LIMITES DU POIGNETS
#define HIGH_STOP_POIGNET 4
