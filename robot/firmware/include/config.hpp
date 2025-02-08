#pragma once

#include <communication/enco_peripherals.hpp>
#include <drives/precision_motor.hpp>
#include <drives/swerve_drive.hpp>

/**
 * copy paste the PrecisionMotor config generated by the R ("report") command in the PMOTOR_tuner_over_SPI tool.
 */
void pmotors_config(PrecisionMotor *pmotors)
{
	/* PMOTOR #[0] config */ {
		auto &pm = pmotors[0];
		pm._e.set_inverted(true);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.01000);
		pm._pid_speed.setK(0.60000, 0.00001, 0.10000);
	};
	/* PMOTOR #[1] config */ {
		auto &pm = pmotors[1];
		pm._e.set_inverted(true);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.01000);
		pm._pid_speed.setK(0.60000, 0.00001, 0.06000);
	};
	/* PMOTOR #[2] config */ {
		auto &pm = pmotors[2];
		pm._e.set_inverted(false);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.01000);
		pm._pid_speed.setK(0.60000, 0.00001, 0.12000);
	};
	/* PMOTOR #[3] config */ {
		auto &pm = pmotors[3];
		pm._e.set_inverted(true);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.01000);
		pm._pid_speed.setK(0.60000, 0.00001, 0.15500);
	};
}

/**
 * copy paste the PrecisionMotor config generated by the R ("report") command in the SWERVE_tuner_over_SPI tool.
 */
void swerve_config(SwerveDrive *sdrives)
{
}