#include <vector>

#include "Torus.h"
#include "../vecmath/quartic.h"

Torus::Torus(Scene *scene, Material *mat, const float &A, const float &B)
	: MaterialSceneObject(scene, mat), A(A), B(B) {} 

bool Torus::intersectLocal(const ray &r, isect &iSect) const {
	
	// Build the torus intersection equation
	const vec3f D = r.getDirection();
	const float Dx = D[0], Dy = D[1], Dz = D[2];
	const vec3f E = r.getPosition();
	const float Ex = E[0], Ey = E[1], Ez = E[2];

	const float G = 4 * this->A * this->A * (Ex * Ex + Ey * Ey);
	const float H = 8 * this->A * this->A * (Dx * Ex + Dy * Ey);
	const float I = 4 * this->A * this->A * (Dx * Dx + Dy * Dy);
	const float J = E.length_squared();
	const float K = 2 * D.dot(E);
	const float L = D.length_squared() + (this->A * this->A - this->B * this->B);

	const float const_term	= L * L - I;
	const float linear_term = 2 * K * L - H;
	const float quad_term	= 2 * J * L + K * K - G;
	const float third_term	= 2 * J * K;
	const float forth_term	= J * J;

	auto *solutions = solve_quartic(third_term / forth_term, quad_term / forth_term, linear_term / forth_term, const_term / forth_term);
	std::vector<double> ts;
	for (auto ti = 0; ti < 4; ti++) 	{
		if (solutions[ti].imag() != 0.0 || solutions[ti].real() < 0 || solutions[ti].real() < RAY_EPSILON) 		{
			continue;
		}
		ts.push_back(solutions[ti].real());
	}
	delete[] solutions;

	if (ts.empty()) { return false; }

	std::sort(ts.begin(), ts.end());

	const auto t = iSect.t = ts[0];

	const auto pp = r.at(t);

	const auto alpha = 1.0 - this->A / sqrt(pp[0] * pp[0] + pp[1] * pp[1]);
	iSect.N = -vec3f {alpha * pp[0], alpha * pp[1], pp[2]}.normalize();
	iSect.obj = this;

	return true;
}

bool Torus::hasBoundingBoxCapability() const {
	return true;
}

BoundingBox Torus::ComputeLocalBoundingBox() {
	BoundingBox localBounds;
	localBounds.min = vec3f(-this->A - this->B, -this->A - this->B, -this->B);
	localBounds.max = vec3f(this->A + this->B, this->A + this->B, this->B);
	return localBounds;
}

