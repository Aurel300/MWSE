/************************************************************************
	
	xSetBaseGold.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Creature.h"
#include "TES3Reference.h"

using namespace mwse;

namespace mwse
{
	class xSetBaseGold : mwse::InstructionInterface_t
	{
	public:
		xSetBaseGold();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xSetBaseGold xSetBaseGoldInstance;

	xSetBaseGold::xSetBaseGold() : mwse::InstructionInterface_t(OpCode::xSetBaseGold) {}

	void xSetBaseGold::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xSetBaseGold::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		short gold = mwse::Stack::getInstance().popShort();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetBaseGold: Called on invalid reference." << std::endl;
#endif
			return 0.0f;
		}

		// Get the gold based on the base record type.
		TES3::BaseObject* baseRecord = reference->baseObject;
		if (baseRecord->objectType == TES3::ObjectType::NPC) {
		TES3::NPCInstance* npc = reinterpret_cast<TES3::NPCInstance*>(baseRecord);
			if (npc->baseNPC) {
				npc->baseNPC->barterGold = gold;
			}
			else {
				mwse::log::getLog() << "xSetBaseGold: Could not get base NPC record for \"" << npc->objectID << "\"" << std::endl;
			}
		}
		else if (baseRecord->objectType == TES3::ObjectType::Creature) {
			TES3::CreatureInstance* creature = reinterpret_cast<TES3::CreatureInstance*>(baseRecord);
			if (creature->baseCreature) {
				creature->baseCreature->barterGold = gold;
			}
			else {
				mwse::log::getLog() << "xSetBaseGold: Could not get base creature record for \"" << creature->objectID << "\"" << std::endl;
			}
		}

		return 0.0f;
	}
}