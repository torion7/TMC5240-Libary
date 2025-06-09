/*******************************************************************************
* Copyright © 2018 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2023 Analog Devices Inc. All Rights Reserved.
* This software is proprietary to Analog Devices, Inc. and its licensors.
*******************************************************************************/


#include "Ramp.h"

void tmc_ramp_init(void *ramp, TMC_RampType type)
{
	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
	default:
		tmc_ramp_linear_init((TMC_LinearRamp *)ramp);
		break;
	}
}

int32_t tmc_ramp_compute(void *ramp, TMC_RampType type, uint32_t delta)
{
	uint32_t i;
	int32_t dxSum = 0;

	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
	default:
		for (i = 0; i < delta; i++)
		{
			dxSum += tmc_ramp_linear_compute((TMC_LinearRamp *)ramp);
		}
		break;
	}

	return dxSum;
}

int32_t tmc_ramp_get_rampVelocity(void *ramp, TMC_RampType type)
{
	int32_t v = 0;
	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
		v = tmc_ramp_linear_get_rampVelocity((TMC_LinearRamp *)ramp);
		break;
	}
	return v;
}

int32_t tmc_ramp_get_rampPosition(void *ramp, TMC_RampType type)
{
	int32_t x = 0;
	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
		x = tmc_ramp_linear_get_rampPosition((TMC_LinearRamp *)ramp);
		break;
	}
	return x;
}

bool tmc_ramp_get_enabled(void *ramp, TMC_RampType type)
{
	bool enabled = false;
	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
		enabled = tmc_ramp_linear_get_enabled((TMC_LinearRamp *)ramp);
		break;
	}
	return enabled;
}

void tmc_ramp_set_enabled(void *ramp, TMC_RampType type, bool enabled)
{
	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
	default:
		tmc_ramp_linear_set_enabled((TMC_LinearRamp *)ramp, enabled);
		break;
	}
}

void tmc_ramp_toggle_enabled(void *ramp, TMC_RampType type)
{
	switch(type) {
	case TMC_RAMP_TYPE_LINEAR:
	default:
		tmc_ramp_linear_set_enabled((TMC_LinearRamp *)ramp, !tmc_ramp_get_enabled(ramp, type));
		break;
	}
}
