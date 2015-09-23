#pragma once

/*
* File:  SparkParticleSystem.h
* --------------------------
* This file contains the SparkParticleSystem class, 
* which is inherited from the LineParticleSystem
* class. This class is responsible for drawing the 
* explosions and particles for pickups, attacks and 
* deaths.
*/

#include "C4Particles.h"
#include "C4Engine.h"
#include "Game.h"
#include <time.h>

namespace C4
{
	enum ParticleType
	{
		kSelfDestructParticle,
		kNotSelfDestructParticle
	};

	class SparkParticleSystem : public LineParticleSystem
	{
	public:
		//Constructors
		SparkParticleSystem(int32 count);
		SparkParticleSystem(float radius);
		SparkParticleSystem(int32 count, float radius);
		~SparkParticleSystem();
		SparkParticleSystem();

		//Methods
		void Preprocess(void) override;
		void AnimateParticles(void) override;
	private:
		//Variables
		int32				sparkCount;
		ColorRGB			colour;
		float				newGravity;
		float				radius;
		float				SPEEDMULTIPLIER;
		float				LIFETIME = 500;
		ParticleType		type;
		enum
		{
			kMaxParticleCount = 1000,
			kSmallerMaxParticleCount = 100
		};

		ParticlePool<>		particlePool;
		Particle			particleArray[kMaxParticleCount];

		//Method
		bool CalculateBoundingSphere(BoundingSphere *sphere) const override;

	};

}