/************************************************************************
	
	xSetMaxCondition.cpp - Copyright (c) 2008 The MWSE Project
	http://www.sourceforge.net/projects/mwse

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

using namespace mwse;

namespace mwse
{
	class xSetMaxCondition : mwse::InstructionInterface_t
	{
	public:
		xSetMaxCondition();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xSetMaxCondition xSetMaxConditionInstance;

	xSetMaxCondition::xSetMaxCondition() : mwse::InstructionInterface_t(OpCode::xSetMaxCondition) {}

	void xSetMaxCondition::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xSetMaxCondition::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameter from the stack.
		mwLong maxCondition = mwse::Stack::getInstance().popLong();
		bool success = false;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetMaxCondition: No reference provided." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get the base record.
		TES3::BaseObject* record = reinterpret_cast<TES3::BaseObject*>(reference->objectPointer);
		if (record == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetMaxCondition: No record found for reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set the max condition based on record type.
		if (record->objectType == TES3::ObjectType::ARMOR) {
			reinterpret_cast<TES3::Armor*>(record)->maxCondition = maxCondition;
		}
		else if (record->objectType == TES3::ObjectType::WEAPON) {
			reinterpret_cast<TES3::Weapon*>(record)->maxCondition = maxCondition;
		}
		else if (record->objectType == TES3::ObjectType::LOCK) {
			reinterpret_cast<LOCKRecord_t*>(record)->maxCondition = maxCondition;
		}
		else if (record->objectType == TES3::ObjectType::PROBE) {
			reinterpret_cast<PROBRecord_t*>(record)->maxCondition = maxCondition;
		}
		else if (record->objectType == TES3::ObjectType::REPAIR) {
			reinterpret_cast<REPARecord_t*>(record)->maxCondition = maxCondition;
		}
		else {
#if _DEBUG
			mwse::log::getLog() << "xSetMaxCondition: Invalid record type: " << record->objectType << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(success);
			return 0.0f;
		}

		// If there's a variable node containing condition, and we need to change it, do so.
		mwVarHolderNode_t* varNode = tes3::getAttachedVarHolderNode(reference);
		if (varNode && varNode->unknown_0x0C > maxCondition) {
			varNode->unknown_0x0C = maxCondition;
			success = true;
		}

		// Push success state.
		mwse::Stack::getInstance().pushLong(success);

		return 0.0f;
	}
}