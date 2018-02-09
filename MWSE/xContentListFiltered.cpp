/************************************************************************

	xContentListFiltered.cpp - Copyright (c) 2008 The MWSE Project
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

namespace mwse {
	class xContentListFiltered : mwse::InstructionInterface_t {
	public:
		xContentListFiltered();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);

	private:
		mwLong getBitMaskForRecordType(mwLong recordType);
		bool passesFilter(TES3::BaseObject* record, mwLong filter);

		enum FilterMask {
			FILTER_ACTI = 1,
			FILTER_ALCH = 2,
			FILTER_AMMO = 4,
			FILTER_APPA = 8,
			FILTER_ARMO = 16,
			FILTER_BODY = 32,
			FILTER_BOOK = 64,
			FILTER_CLOT = 128,
			FILTER_CONT = 256,
			FILTER_CREA = 512,
			FILTER_DOOR = 1024,
			FILTER_INGR = 2048,
			FILTER_LEVC = 4096,
			FILTER_LEVI = 8192,
			FILTER_LIGH = 16384,
			FILTER_LOCK = 32768,
			FILTER_MISC = 65536,
			FILTER_NPC = 131072,
			FILTER_PROB = 262144,
			FILTER_REPA = 524288,
			FILTER_STAT = 1048576,
			FILTER_WEAP = 2097152,
			FILTER_ENCH = 4194304
		};
	};

	static xContentListFiltered xContentListFilteredInstance;

	xContentListFiltered::xContentListFiltered() : mwse::InstructionInterface_t(OpCode::xContentListFiltered) {}

	void xContentListFiltered::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xContentListFiltered::execute(mwse::VMExecuteInterface &virtualMachine) {
		// Get parameters.
		TES3::IteratorNode<InventoryNode>* node = reinterpret_cast<TES3::IteratorNode<InventoryNode>*>(mwse::Stack::getInstance().popLong());
		mwLong filter = mwse::Stack::getInstance().popLong();

		// If we're not filtering, abandon ship.
		if (filter == 0) {
#if _DEBUG
			mwse::log::getLog() << "xContentListFiltered: No filter provided." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xContentListFiltered: Called on invalid reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Results.
		char * id = NULL;
		mwLong count = 0;
		mwLong type = 0;
		mwLong value = 0;
		mwFloat weight = 0;
		mwString name = NULL;
		TES3::IteratorNode<InventoryNode>* next = NULL;

		// If we aren't given a node, get the first one.
		if (node == NULL) {
			node = tes3::getFirstInventoryNode(reference);

			// Pass over any records that don't match the current filter.
			while (node && node->data && node->data->object && !passesFilter(node->data->object, filter)) {
				node = node->next;
			}
		}

		// Validate the node we've obtained.
		if (node && node->data && node->data->object) {
			TES3::BaseObject* record = reinterpret_cast<TES3::BaseObject*>(node->data->object);

			id = record->objectID;
			count = node->data->itemCount;
			type = record->objectType;

			// Get value.
			try {
				value = tes3::getValue(reinterpret_cast<TES3::BaseObject*>(record));
			}
			catch (std::exception& e) {
				value = 0;
#if _DEBUG
				mwse::log::getLog() << "xContentListFiltered: Could not get value of object '" << id << "'. " << e.what() << std::endl;
#endif
			}

			// Get weight.
			try {
				weight = tes3::getWeight(reinterpret_cast<TES3::BaseObject*>(record));
			}
			catch (std::exception& e) {
				weight = 0.0f;
#if _DEBUG
				mwse::log::getLog() << "xContentListFiltered: Could not get weight of object '" << id << "'. " << e.what() << std::endl;
#endif
			}

			// Get name.
			try {
				name = tes3::getName(reinterpret_cast<TES3::BaseObject*>(record));
			}
			catch (std::exception& e) {
				name = NULL;
#if _DEBUG
				mwse::log::getLog() << "xContentListFiltered: Could not get name of object '" << id << "'. " << e.what() << std::endl;
#endif
			}

			// Get next node. Pass over any records that don't match the given filter.
			next = node->next;
			while (next && next->data && next->data->object && !passesFilter(next->data->object, filter)) {
				next = next->next;
			}
		}

		// Push values to the stack.
		mwse::Stack::getInstance().pushLong((mwLong)next);
		mwse::Stack::getInstance().pushString(name);
		mwse::Stack::getInstance().pushFloat(weight);
		mwse::Stack::getInstance().pushLong(value);
		mwse::Stack::getInstance().pushLong(type);
		mwse::Stack::getInstance().pushLong(count);
		mwse::Stack::getInstance().pushString(id);

		return 0.0f;
	}

	mwLong xContentListFiltered::getBitMaskForRecordType(mwLong recordType) {
		switch (recordType) {
		case TES3::ObjectType::ACTIVATOR: return FILTER_ACTI;
		case TES3::ObjectType::Alchemy: return FILTER_ALCH;
		case TES3::ObjectType::AMMO: return FILTER_AMMO;
		case TES3::ObjectType::APPARATUS: return FILTER_APPA;
		case TES3::ObjectType::ARMOR: return FILTER_ARMO;
		case TES3::ObjectType::BODY: return FILTER_BODY;
		case TES3::ObjectType::BOOK: return FILTER_BOOK;
		case TES3::ObjectType::CLOTHING: return FILTER_CLOT;
		case TES3::ObjectType::CONTAINER: return FILTER_CONT;
		case TES3::ObjectType::Creature: return FILTER_CREA;
		case TES3::ObjectType::DOOR: return FILTER_DOOR;
		case TES3::ObjectType::INGREDIENT: return FILTER_INGR;
		case TES3::ObjectType::LEVELLEDCREATURE: return FILTER_LEVC;
		case TES3::ObjectType::LEVELLEDITEM: return FILTER_LEVI;
		case TES3::ObjectType::LIGHT: return FILTER_LIGH;
		case TES3::ObjectType::LOCKPICK: return FILTER_LOCK;
		case TES3::ObjectType::MISC: return FILTER_MISC;
		case TES3::ObjectType::NPC: return FILTER_NPC;
		case TES3::ObjectType::PROBE: return FILTER_PROB;
		case TES3::ObjectType::REPAIR: return FILTER_REPA;
		case TES3::ObjectType::STATIC: return FILTER_STAT;
		case TES3::ObjectType::WEAPON: return FILTER_WEAP;
		}

		return 0x0;
	}

	bool xContentListFiltered::passesFilter(TES3::BaseObject* record, mwLong filter) {
		// Filter by record type. Unless we're not filtering only by enchantment.
		if (filter != FILTER_ENCH && !(getBitMaskForRecordType(record->objectType) & filter)) {
			return false;
		}

		// If we're filtering by enchantment, verify that the record has one.
		if (filter & FILTER_ENCH) {
			try {
				if (tes3::getEnchantment(reinterpret_cast<TES3::BaseObject*>(record)) == NULL) {
					return false;
				}
			}
			catch (std::exception& e) {
				return false;
			}
		}

		return true;
	}
}