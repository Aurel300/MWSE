#include "LuaCalcTrainingPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateTrainingPriceEvent::CalculateTrainingPriceEvent(TES3::MobileActor * mobileActor, int basePrice, int price, int skillId) :
				ObjectFilteredEvent("calcTrainingPrice", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_BasePrice(basePrice),
				m_Price(price),
				m_SkillId(skillId)
			{

			}

			sol::table CalculateTrainingPriceEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_MobileActor;
				if (m_MobileActor) {
					eventData["reference"] = m_MobileActor->reference;
				}

				eventData["skill"] = TES3::WorldController::get()->getMobilePlayer()->getSkillStatistic(m_SkillId);
				eventData["skillId"] = m_SkillId;

				eventData["basePrice"] = m_BasePrice;
				eventData["price"] = m_Price;

				return eventData;
			}

			bool CalculateTrainingPriceEvent::m_EventEnabled = false;
		}
	}
}
