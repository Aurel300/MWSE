/************************************************************************
	
	xGetKeyBind.cpp - Copyright (c) 2008 The MWSE Project
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
#include <Windows.h>

using namespace mwse;

namespace mwse {
	class xGetKeyBind : mwse::InstructionInterface_t {
	public:
		xGetKeyBind();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	enum xGetKeyBindReturnType {
		ReturnVirtualKeyCode,
		ReturnScanCode
	};

	static xGetKeyBind xGetKeyBindInstance;

	xGetKeyBind::xGetKeyBind() : mwse::InstructionInterface_t(OpCode::xGetKeyBind) {}

	void xGetKeyBind::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xGetKeyBind::execute(mwse::VMExecuteInterface &virtualMachine) {
		// Get parameters.
		mwLong_t inputIndex = Stack::getInstance().popLong();

		// Validate index.
		if (inputIndex < KeyFirst || inputIndex > KeyLast) {
#if _DEBUG
			mwse::log::getLog() << "xGetKeyBind: Index out of bounds." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get our structure.
		InputConfig_t::Input_t& config = tes3::getMaster()->inputConfig->inputMaps[inputIndex];

		// Get the basic codes/return values.
		mwLong_t scanCode = config.keyCode;
		mwLong_t scanDevice = config.device;
		mwLong_t keyCode = 0;

		if (scanDevice == DeviceKeyboard) {
			keyCode = MapVirtualKey(keyCode, MAPVK_VSC_TO_VK);
		}
		else if (scanDevice == DeviceMouse) {
			switch (scanCode) {
			case 0x00:
				keyCode = VK_LBUTTON;
			case 0x01:
				keyCode = VK_RBUTTON;
			case 0x02:
				keyCode = VK_MBUTTON;
			case 0x03:
				keyCode = VK_XBUTTON1;
			case 0x04:
				keyCode = VK_XBUTTON2;
			}
		}
		else {
#if _DEBUG
			mwse::log::getLog() << "xGetKeyBind: Device type " << scanDevice << " cannot be converted." << std::endl;
#endif
		}

		// Return values.
		mwse::Stack::getInstance().pushLong(keyCode);
		mwse::Stack::getInstance().pushLong(scanDevice);
		mwse::Stack::getInstance().pushLong(scanCode);

		return 0.0f;
	}
}