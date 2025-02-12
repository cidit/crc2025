#pragma once

#include <communication/enco_peripherals.hpp>
#include <drives/precision_motor.hpp>
#include <drives/swerve_drive.hpp>

/**
 * copy paste the PrecisionMotor config generated by the R ("report") command in the PMOTOR_tuner_over_SPI tool.
 */
void pmotors_config(PrecisionMotor *pmotors)
{
	/* PMOTOR #[0] Swerve Right B config */ {
		auto &pm = pmotors[0];
		pm._e.set_inverted(true);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.08000);
		pm._pid_speed.setK(0.60000, 0.00000, 0.07000);
	};
	/* PMOTOR #[1] Swerve Right A config */ {
		auto &pm = pmotors[1];
		pm._e.set_inverted(true);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.08000);
		pm._pid_speed.setK(0.60000, 0.00000, 0.07000);
	};
	/* PMOTOR #[2] Swerve Left A config */ {
		auto &pm = pmotors[2];
		pm._e.set_inverted(false);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.08000);
		pm._pid_speed.setK(0.60000, 0.00000, 0.07000);
	};
	/* PMOTOR #[3] Swerve Left B config */ {
		auto &pm = pmotors[3];
		pm._e.set_inverted(true);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.60000, 0.00000, 0.08000);
		pm._pid_speed.setK(0.60000, 0.00000, 0.07000);
	};
	/* PMOTOR #[4] Bras Right config */ {
		auto &pm = pmotors[4];
		pm._e.set_inverted(true);
		pm._m.set_inverted(false);
		pm._pid_angle.setK(0.80000, 0.00001, 0.02000);
		pm._pid_speed.setK(0.00000, 0.00000, 0.00000);
	};
	/* PMOTOR #[5] Bras Left config */ {
		auto &pm = pmotors[5];
		pm._e.set_inverted(false);
		pm._m.set_inverted(true);
		pm._pid_angle.setK(0.80000, 0.00001, 0.02000);
		pm._pid_speed.setK(0.00000, 0.00000, 0.00000);
	};
	/* PMOTOR #[6] Poignet config */ {
		auto &pm = pmotors[6];
		pm._e.set_inverted(false);
		pm._m.set_inverted(false);
		pm._pid_angle.setK(0.80000, 0.00100, 0.00100);
		pm._pid_speed.setK(0.00000, 0.00000, 0.00000);
	};
	/* PMOTOR #[7] Lanceur config */ {
		auto &pm = pmotors[7];
		pm._e.set_inverted(false);
		pm._m.set_inverted(false);
		pm._pid_angle.setK(0.00000, 0.00000, 0.00000);
		pm._pid_speed.setK(0.00000, 0.00000, 0.00000);
	};
}

/**
 * copy paste the PrecisionMotor config generated by the R ("report") command in the SWERVE_tuner_over_SPI tool.
 */
void swerve_config(SwerveDrive *sdrives)
{
}