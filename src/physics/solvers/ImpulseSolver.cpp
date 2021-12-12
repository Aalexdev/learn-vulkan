#include "physics/collisions/solvers/ImpulseSolver.hpp"
#include "ECS/components/ECS_RigidBody.hpp"

// libs
#include <glm/glm.hpp>

namespace Physics::Collisions::Solvers {
	void ImpulseSolver::Solve(std::vector<Collision>& collisions, FrameInfo &info) const{
		for (Collision& collision : collisions) {
			auto &manifold = collision.points;
			// Replaces non dynamic objects with default values.

			auto _aBody = info.coordinator.GetComponent<ECS::Components::RigidBody>(collision.A);
			auto _bBody = info.coordinator.GetComponent<ECS::Components::RigidBody>(collision.B);

			ECS::Components::RigidBody* aBody = (_aBody.isDynamic) ? aBody : nullptr;
			ECS::Components::RigidBody* bBody = (_bBody.isDynamic) ? bBody : nullptr;

			glm::vec3 aVel = aBody ? aBody->velocity : glm::vec3(0.0f);
			glm::vec3 bVel = bBody ? bBody->velocity : glm::vec3(0.0f);
			
			glm::vec3 rVel = bVel - aVel;
			float  nSpd = glm::dot(rVel, manifold.normal);

			float aInvMass = aBody ? aBody->mass : 1.0f;
			float bInvMass = bBody ? bBody->mass : 1.0f;

			// Impluse

			// This is important for convergence
			// a negitive impulse would drive the objects closer together
			if (nSpd >= 0)
				continue;

			float e = (aBody ? aBody->restitution : 1.0f)
				     * (bBody ? bBody->restitution : 1.0f);

			float j = -(1.0f + e) * nSpd / (aInvMass + bInvMass);

			glm::vec3 impluse = j * manifold.normal;

			if (aBody ? aBody->isKinematic : false) {
				aVel -= impluse * aInvMass;
			}

			if (bBody ? bBody->isKinematic : false) {
				bVel += impluse * bInvMass;
			}

			// Friction

			rVel = bVel - aVel;
			nSpd = glm::dot(rVel, manifold.normal);

			glm::vec3 tangent = rVel - nSpd * manifold.normal;

			if (glm::length(tangent) > 0.0001f) { // safe normalize
				tangent = glm::normalize(tangent);
			}

			float fVel = glm::dot(rVel, tangent);

			float aSF = aBody ? aBody->staticFriction  : 0.0f;
			float bSF = bBody ? bBody->staticFriction  : 0.0f;
			float aDF = aBody ? aBody->dynamicFriction : 0.0f;
			float bDF = bBody ? bBody->dynamicFriction : 0.0f;
			float mu  = (float)glm::vec2(aSF, bSF).length();

			float f  = -fVel / (aInvMass + bInvMass);

			glm::vec3 friction;
			if (abs(f) < j * mu) {
				friction = f * tangent;
			}

			else {
				mu = glm::length(glm::vec2(aDF, bDF));
				friction = -j * tangent * mu;
			}

			if (aBody ? aBody->isKinematic : false) {
				aBody->velocity = aVel - friction * aInvMass;
			}

			if (bBody ? bBody->isKinematic : false) {
				bBody->velocity = bVel + friction * bInvMass;
			}
		}
	}
}