// Simple automation test to verify the Aura module loads correctly.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAuraModuleLoadTest, "Aura.Module.Load", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAuraModuleLoadTest::RunTest(const FString& Parameters)
{
    FModuleManager& ModuleManager = FModuleManager::Get();
    ModuleManager.LoadModule(TEXT("Aura"));

    const bool bIsLoaded = ModuleManager.IsModuleLoaded(TEXT("Aura"));
    TestTrue(TEXT("Aura module should be loaded and valid."), bIsLoaded);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
