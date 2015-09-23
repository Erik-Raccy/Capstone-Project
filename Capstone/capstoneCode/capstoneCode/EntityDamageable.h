#ifndef ENTITYDAMAGEABLE_H
#define ENTITYDAMAGEABLE_H

//Engine Headers

//My Headers
#include "EntityBase.h"

namespace C4
{
	class EntityDamageable : public EntityBase
	{
	public:
		//constructors
		EntityDamageable();
		EntityDamageable(EntityType _type, Point3D _location, float _health, float _maxHealth, char* _model);
		~EntityDamageable();

		//Methods
		virtual void OnDamaged();
		virtual void OnDeath();

		float getHealth()
		{
			return mHealth;
		}
		float getTotalHealth()
		{
			return mTotalHealth;
		}
		float getScraps()
		{
			return mScraps;
		}

		void setHealth(float newHealth)
		{
			mHealth = newHealth;
		}
		void setTotalHealth(float newHealth)
		{
			mTotalHealth = newHealth;
		}
		void setScraps(float newScraps)
		{
			mScraps = newScraps;
		}

	protected:
		//Variables
		float mHealth;
		float mTotalHealth;
		float mScraps;

	};

}

#endif