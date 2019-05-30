#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Vectors.h"

namespace mwse {
	namespace lua {
		namespace event {
			class MobileProjectileActorCollisionEvent : public ObjectFilteredEvent, public DisableableEvent<MobileProjectileActorCollisionEvent> {
			public:
				MobileProjectileActorCollisionEvent(TES3::MobileProjectile* projectile, TES3::Reference* targetReference, TES3::Vector3& point, TES3::Vector3& pos, TES3::Vector3& vel);
				sol::table createEventTable();

			protected:
				TES3::MobileProjectile* m_Projectile;
				TES3::Reference* m_TargetReference;
				TES3::Vector3 m_CollisionPoint;
				TES3::Vector3 m_Position;
				TES3::Vector3 m_Velocity;
			};
		}
	}
}
