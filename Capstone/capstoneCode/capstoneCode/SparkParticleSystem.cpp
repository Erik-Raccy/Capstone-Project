#include "SparkParticleSystem.h"

using namespace C4;

SparkParticleSystem::SparkParticleSystem(int32 count) :
LineParticleSystem(kParticleSystemSpark, &particlePool, "particle/Spark1"),
particlePool(kSmallerMaxParticleCount, particleArray)
{
	type = kNotSelfDestructParticle;
	sparkCount = count;
	SetLengthMultiplier(10.0F);
	SetParticleSystemFlags(kParticleSystemSelfDestruct);
	newGravity = K::gravity;
	radius = 0.1F;
	colour = ColorRGB(1.0F, 0.55F, 0.0F);
	SPEEDMULTIPLIER = 1.0F;
}

SparkParticleSystem::SparkParticleSystem(float radius) :
LineParticleSystem(kParticleSystemSpark, &particlePool, "particle/Spark1"),
particlePool(kSmallerMaxParticleCount, particleArray)
{
	type = kNotSelfDestructParticle;
	sparkCount = 20;
	SetLengthMultiplier(10.0F);
	SetParticleSystemFlags(kParticleSystemSelfDestruct);
	newGravity = -K::gravity/7.0F;
	this->colour = ColorRGB(1.0F, 1.0F, 1.0F),
	this->radius = radius;
	SPEEDMULTIPLIER = 5.0F;
	LIFETIME = 1500;
}

SparkParticleSystem::SparkParticleSystem(int count, float radius) :
LineParticleSystem(kParticleSystemSpark, &particlePool, "particle/Spark1"),
particlePool(kMaxParticleCount, particleArray)
{
	type = kSelfDestructParticle;
	sparkCount = count;
	SetLengthMultiplier(10.0F);
	SetParticleSystemFlags(kParticleSystemSelfDestruct);
	newGravity = K::gravity/2;
	this->colour = ColorRGB(0,0,0),
	this->radius = 0.4F;
	SPEEDMULTIPLIER = 5.0F;
	LIFETIME = 3000; 
}
SparkParticleSystem::SparkParticleSystem() :
LineParticleSystem(kParticleSystemSpark, &particlePool, "particle/Spark1"),
particlePool(kMaxParticleCount, particleArray)
{
	// This constructor gets used when the particle effect is being loaded from
	// a saved game. In this case, we don't need to initialize anything.
}

SparkParticleSystem::~SparkParticleSystem()
{
}

bool SparkParticleSystem::CalculateBoundingSphere(BoundingSphere *sphere) const
{
	sphere->SetCenter(0.0F, 0.0F, 0.0F);
	sphere->SetRadius(100.0F);
	return (true);
}

void SparkParticleSystem::Preprocess(void)
{
	LineParticleSystem::Preprocess();

	if (!GetFirstParticle())
	{
		// Calculate the world-space center.
		Point3D center = GetSuperNode()->GetWorldTransform() * GetNodePosition();
		const ConstVector2D *trig = Math::GetTrigTable();

		int32 count = sparkCount;
		for (machine a = 0; a < count; a++)
		{
			// Grab a new unused particle from the pool.
			Particle *particle = particlePool.NewParticle();
			if (!particle)
			{
				break;
			}
			srand(time(NULL));
			// Calculate a random velocity in a random direction.
			float speed = Math::RandomFloat(0.004F) / SPEEDMULTIPLIER;
			Vector2D csp = trig[Math::Random(128)] * speed;
			const Vector2D& cst = trig[Math::Random(256)];
			if (type == kSelfDestructParticle)
			{
				particle->velocity.Set(15 * cst.x * csp.y, 15 * cst.y * csp.y, 15 * csp.x);
				colour = ColorRGB(0.8 + 0.2 / (Math::Random(128)+1), 0.0F, 0.0F),
				srand(time(NULL));
				colour = ColorRGB(colour.red, 0.4 + 0.6 / (Math::Random(128) + 1), 0.0F),
				particle->color.Set(colour, 1.0F);
			}
			else if (type == kNotSelfDestructParticle)
			{
				particle->velocity.Set(cst.x * csp.y, cst.y * csp.y, csp.x);
				particle->color.Set(colour, 1.0F);	// It's yellow.
			}

			particle->emitTime = 0;							// Particle appears right away.
			particle->lifeTime = LIFETIME + Math::Random(750);	// Particle lives 500-1250 milliseconds.
			particle->radius = radius;						// The radius is 20 mm.
			particle->orientation = 0;						// This doesn't matter for line particles.
			particle->position = center;					// It starts at the effect's center.

			// Add the particle to the particle system.
			AddParticle(particle);
		}
	}
}

void SparkParticleSystem::AnimateParticles(void)
{
	// Times are in milliseconds.
	int32 dt = TheTimeMgr->GetDeltaTime();
	float fdt = TheTimeMgr->GetFloatDeltaTime();

	Particle *particle = GetFirstParticle();
	while (particle)
	{
		// Get the next particle now in case the current one is removed from the system.
		Particle *next = particle->nextParticle;

		int32 life = (particle->lifeTime -= dt);
		if (life > 0)
		{
			// Update velocity with gravity.
			particle->velocity.z += newGravity * fdt;

			// Move the particle and see if it hit the floor plane at z=0.
			float z1 = particle->position.z - particle->radius;
			particle->position += particle->velocity * fdt;
			float z2 = particle->position.z - particle->radius;
			if (z1 * z2 <= 0.0F)
			{
				// The particle hit the floor, so reflect its velocity and remove some energy.
				particle->position.z = 0.05F - z2;
				particle->velocity.z *= -0.5F;
			}

			// If the particle is nearing the end of its life, fade it out.
			if (life < 100)
			{
				particle->color.alpha = (float)life * 0.01F;
			}
		}
		else
		{
			// Particle burned out.
			FreeParticle(particle);
		}

		particle = next;
	}
}
