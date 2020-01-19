#include "TES3GameLua.h"

#include "sol.hpp"
#include "LuaManager.h"
#include "LuaUtil.h"

#include "NINode.h"

#include "TES3Game.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		void bindTES3Game() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Game>("tes3game");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["activationAmbientLight"] = sol::readonly_property(&TES3::Game::activationAmbientLight);
			usertypeDefinition["debugRoot"] = sol::readonly_property(&TES3::Game::debugRoot);
			usertypeDefinition["fogProperty"] = sol::readonly_property(&TES3::Game::fogProperty);
			usertypeDefinition["parentWindowHandle"] = sol::readonly_property(&TES3::Game::parentWindowHandle);
			usertypeDefinition["renderDistance"] = &TES3::Game::renderDistance;
			usertypeDefinition["sceneGraphCollideString"] = sol::readonly_property(&TES3::Game::collideString);
			usertypeDefinition["sceneGraphGridString"] = sol::readonly_property(&TES3::Game::gridString);
			usertypeDefinition["sceneGraphTextureString"] = sol::readonly_property(&TES3::Game::textureString);
			usertypeDefinition["screenShotsEnabled"] = &TES3::Game::screenShotsEnabled;
			usertypeDefinition["screenX"] = sol::readonly_property(&TES3::Game::screenX);
			usertypeDefinition["screenY"] = sol::readonly_property(&TES3::Game::screenY);
			usertypeDefinition["soundQuality"] = &TES3::Game::soundQuality;
			usertypeDefinition["volumeEffect"] = &TES3::Game::volumeEffect;
			usertypeDefinition["volumeFootsteps"] = &TES3::Game::volumeFootsteps;
			usertypeDefinition["volumeMaster"] = &TES3::Game::volumeMaster;
			usertypeDefinition["volumeMedia"] = &TES3::Game::volumeMedia;
			usertypeDefinition["volumeVoice"] = &TES3::Game::volumeVoice;
			usertypeDefinition["windowHandle"] = sol::readonly_property(&TES3::Game::windowHandle);
			usertypeDefinition["wireframeProperty"] = sol::readonly_property(&TES3::Game::wireframeProperty);
			usertypeDefinition["worldLandscapeRoot"] = sol::readonly_property(&TES3::Game::worldLandscapeRoot);
			usertypeDefinition["worldObjectRoot"] = sol::readonly_property(&TES3::Game::worldObjectRoot);
			usertypeDefinition["worldPickRoot"] = sol::readonly_property(&TES3::Game::worldPickObjectRoot);
			usertypeDefinition["worldSceneGraphRoot"] = sol::readonly_property(&TES3::Game::worldRoot);

			// Basic function binding.
			usertypeDefinition["setGamma"] = &TES3::Game::setGamma;

			// Access to other objects that need to be packaged.
			usertypeDefinition["playerTarget"] = sol::readonly_property([](TES3::Game& self) { return self.playerTarget; });
		}
	}
}
