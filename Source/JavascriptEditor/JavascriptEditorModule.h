﻿#pragma once

#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

struct IEditorExtension
{
#if WITH_EDITOR
	virtual void Register() = 0;
	virtual void Unregister() = 0;
#endif
};

/**
 * The public interface to this module
 */
class IJavascriptEditorModule : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IJavascriptEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked< IJavascriptEditorModule >("JavascriptEditor");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "JavascriptEditor" );
	}

	virtual class UJavascriptContext* GetJavascriptContext() { return nullptr; }

	virtual void AddExtension(IEditorExtension* Extension) = 0;
	virtual void RemoveExtension(IEditorExtension* Extension) = 0;
	virtual class UJavascriptEditorObjectManager* GetEditorObjectManager() = 0;
};

#if WITH_EDITOR
static void PatchReimportRule()
{
	FAutoReimportWildcard WildcardToInject;
	WildcardToInject.Wildcard = TEXT("Scripts/**.json");
	WildcardToInject.bInclude = false;

	auto Default = GetMutableDefault<UEditorLoadingSavingSettings>();
	bool bHasChanged = false;
	for (auto& Setting : Default->AutoReimportDirectorySettings)
	{
		bool bFound = false;
		for (const auto& Wildcard : Setting.Wildcards)
		{
			if (Wildcard.Wildcard == WildcardToInject.Wildcard)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			Setting.Wildcards.Add(WildcardToInject);
			bHasChanged = true;
		}
	}
	if (bHasChanged)
	{
		Default->PostEditChange();
	}
}
#endif
