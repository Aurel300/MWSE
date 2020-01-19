#include "TES3ScriptLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "ScriptUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {

		sol::object ScriptContext::index(std::string key) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			TES3::ScriptVariables* vars = getScriptVariables();

			unsigned int index = 0;
			char type = script->getLocalVarIndexAndType(key.c_str(), &index);
			switch (type) {
			case 's':
				return sol::make_object(state, vars->shortVarValues[index]);
			case 'l':
				return sol::make_object(state, vars->longVarValues[index]);
			case 'f':
				return sol::make_object(state, vars->floatVarValues[index]);
			}

			return sol::nil;
		}

		void ScriptContext::new_index(std::string key, sol::stack_object value) {
			if (!value.is<double>()) {
				return;
			}

			unsigned int index = 0;
			char type = script->getLocalVarIndexAndType(key.c_str(), &index);
			if (!type) {
				return;
			}

			TES3::ScriptVariables* vars = getScriptVariables();

			switch (type) {
			case 's':
				vars->shortVarValues[index] = value.as<double>();
				break;
			case 'l':
				vars->longVarValues[index] = value.as<double>();
				break;
			case 'f':
				vars->floatVarValues[index] = value.as<double>();
				break;
			}
		}

		size_t ScriptContext::length() {
			return (script->shortCount + script->longCount + script->floatCount);
		}

		TES3::ScriptVariables* ScriptContext::getScriptVariables() {
			// First, if we have an explicit variable set, use that.
			if (variables != NULL) {
				return variables;
			}

			// If we have a local script context, use that.
			TES3::ScriptVariables* localVariables = mwse::mwscript::getLocalScriptVariables();
			if (localVariables) {
				return localVariables;
			}

			// Otherwise, we resort to the script itself.
			return &script->varValues;
		}

		//
		//
		//

		sol::object getLocalVarData(TES3::Script* script, const char* name, bool useLocals = true) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			unsigned int index = 0;
			char type = script->getLocalVarIndexAndType(name, &index);
			switch (type) {
			case 's':
				return state.create_table_with("type", type, "index", index, "value", script->getShortValue(index, useLocals));
			case 'l':
				return state.create_table_with("type", type, "index", index, "value", script->getLongValue(index, useLocals));
			case 'f':
				return state.create_table_with("type", type, "index", index, "value", script->getFloatValue(index, useLocals));
			}
			return sol::nil;
		}

		sol::object getLocalVarData(TES3::Script* script, bool useLocals = true) {
			if (script->shortCount == 0 && script->longCount == 0 && script->floatCount == 0) {
				return sol::nil;
			}

			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			sol::table results = state.create_table();

			// Append any short variables.
			for (int i = 0; i < script->shortCount; i++) {
				const char* varName = script->shortVarNamePointers[i];
				results[varName] = getLocalVarData(script, varName, useLocals);
			}

			// Append any long variables.
			for (int i = 0; i < script->longCount; i++) {
				const char* varName = script->longVarNamePointers[i];
				results[varName] = getLocalVarData(script, varName, useLocals);
			}

			// Append any float variables.
			for (int i = 0; i < script->floatCount; i++) {
				const char* varName = script->floatVarNamePointers[i];
				results[varName] = getLocalVarData(script, varName, useLocals);
			}

			return results;
		}

		void bindTES3Script() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for ScriptContext.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<ScriptContext>("tes3scriptContext");
				usertypeDefinition["new"] = sol::no_constructor;

				usertypeDefinition[sol::meta_function::length] = &ScriptContext::length;

				// Allow variables to be get/set using their variable name.
				usertypeDefinition[sol::meta_function::index] = &ScriptContext::index;
				usertypeDefinition[sol::meta_function::new_index] = &ScriptContext::new_index;
			}

			// Binding for TES3::GlobalScript
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::GlobalScript>("tes3globalScript");
				usertypeDefinition["new"] = sol::no_constructor;

				// Access to other objects that need to be packaged.
				usertypeDefinition["reference"] = sol::readonly_property([](TES3::GlobalScript& self) { return self.reference; });
				usertypeDefinition["script"] = sol::readonly_property([](TES3::GlobalScript& self) { return self.script; });

				// Allow a special context to be exposed for reading variables.
				usertypeDefinition["context"] = sol::readonly_property([](TES3::GlobalScript& self) {
					TES3::ItemData * itemData = self.reference->getAttachedItemData();
					return std::shared_ptr<ScriptContext>(new ScriptContext(self.script, itemData ? itemData->scriptData : NULL));
				});
			}

			// Binding for TES3::StartScript
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::StartScript>("tes3startScript");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForBaseObject(usertypeDefinition);

				// Access to other objects that need to be packaged.
				usertypeDefinition["script"] = sol::readonly_property([](TES3::StartScript& self) { return self.script; });
			}

			// Binding for TES3::Script.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Script>("tes3script");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForBaseObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["shortVariableCount"] = sol::readonly_property(&TES3::Script::shortCount);
				usertypeDefinition["longVariableCount"] = sol::readonly_property(&TES3::Script::longCount);
				usertypeDefinition["floatVariableCount"] = sol::readonly_property(&TES3::Script::floatCount);

				// Basic function binding.
				usertypeDefinition["getVariableData"] = [](TES3::Script& self, sol::optional<bool> useLocals) { return getLocalVarData(&self, useLocals.value_or(true)); };

				// Allow a special context to be exposed for reading variables.
				usertypeDefinition["context"] = sol::readonly_property([](TES3::Script& self) { return std::shared_ptr<ScriptContext>(new ScriptContext(&self, &self.varValues)); });
			}
		}
	}
}
