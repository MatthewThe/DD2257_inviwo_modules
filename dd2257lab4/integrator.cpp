/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Wednesday, September 20, 2017 - 12:04:15
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab4/integrator.h>

namespace inviwo
{

Integrator::Integrator()
{
}

// TODO: Implementation of the functions defined in the header file integrator.h

vec2 Integrator::sampleFromField(const VolumeRAM* vr, size3_t dims, const vec2& position)
{
	// Sampled outside the domain!
	if (position[0] < 0 || position[0] > dims[0] - 1 || position[1] < 0 || position[1] > dims[1] - 1)
	{
		return vec2(0, 0);
	}

	int x0 = int(position[0]);
	int y0 = int(position[1]);

	// Leads to accessing only inside the volume
	// Coefficients computation takes care of using the correct values
	if (x0 == dims[0] - 1)
	{
		x0--;
	}
	if (y0 == dims[1] - 1)
	{
		y0--;
	}

	auto f00 = vr->getAsDVec2(size3_t(x0, y0, 0));
	auto f10 = vr->getAsDVec2(size3_t(x0 + 1, y0, 0));
	auto f01 = vr->getAsDVec2(size3_t(x0, y0 + 1, 0));
	auto f11 = vr->getAsDVec2(size3_t(x0 + 1, y0 + 1, 0));

	float x = position[0] - x0;
	float y = position[1] - y0;

	vec2 f;

	for (int i = 0; i < 2; i++)
	{
		f[i] = f00[i] * (1 - x) * (1 - y) + f01[i] * (1 - x) * y + f10[i] * x * (1 - y) + f11[i] * x * y;
	}

	return f;
}

// TODO: Implement a single integration step here
//Access the vector field with sampleFromField(vr, dims, ...)

vec2 Integrator::Euler(const VolumeRAM* vr, size3_t dims, const vec2& position, const float stepsize, bool normalized)
{
	vec2 direction = sampleFromField(vr, dims, position);
	if (normalized)
	{
		double vectorLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (vectorLength > 0)
		{
			direction /= vectorLength;
		}
	}
	return position + stepsize*direction;
}

vec2 Integrator::RK4(const VolumeRAM* vr, size3_t dims, const vec2& position, const float stepsize, bool normalized)
{
	//RK4 helping functions
	vec2 k1 = sampleFromField(vr, dims, position);
	vec2 k2 = sampleFromField(vr, dims, position + stepsize*0.5f*k1);
	vec2 k3 = sampleFromField(vr, dims, position + stepsize*0.5f*k2);
	vec2 k4 = sampleFromField(vr, dims, position + stepsize*k3);

	vec2 direction = 0.166666667f*(k1 + 2.0f*k2 + 2.0f*k3 + k4);
	if (normalized)
	{
		double vectorLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (vectorLength > 0)
		{
			direction /= vectorLength;
		}
	}

	return position + stepsize*direction;
}

} // namespace

