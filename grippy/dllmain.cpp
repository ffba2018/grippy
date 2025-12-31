#include <windows.h>
#include "pch.h"
#include <thread>
#include <chrono>

uintptr_t GetVehicleBase() {
	uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
	if (!base) return 0;

	uintptr_t p1 = *(uintptr_t*)(base + 0x00CF5F48);
	if (!p1) return 0;
	uintptr_t p2 = *(uintptr_t*)(p1 + 0xD0);
	if (!p2) return 0;
	uintptr_t p3 = *(uintptr_t*)(p2 + 0x28);
	if (!p3) return 0;
	uintptr_t p4 = *(uintptr_t*)(p3 + 0x44);
	if (!p4) return 0;
	return *(uintptr_t*)(p4 + 0x28);
}

void ApplyGripValues(uintptr_t vBase) {
	uintptr_t attrib24 = *(uintptr_t*)(vBase + 0x24);
	uintptr_t attrib34 = *(uintptr_t*)(vBase + 0x34);
	uintptr_t attrib2C = *(uintptr_t*)(vBase + 0x2C);

	if (!attrib24 || !attrib34 || !attrib2C) return;

	*(float*)(attrib34 + 0x10) = -1.0f;
	*(float*)(attrib34 + 0x1C) = 500.0f;
	*(float*)(attrib34 + 0x24) = 0.0f;
	*(float*)(attrib34 + 0x28) = 0.0f;
	*(float*)(attrib2C + 0x28) = 0.0f;
	*(float*)(attrib24 + 0x13C) = 1000.0f;
	*(float*)(attrib34 + 0x20) = 100.0f;
	*(float*)(attrib34 + 0x2C) = 100.0f;
	*(float*)(attrib34 + 0x44) = 0.0f;
	*(float*)(attrib34 + 0x64) = 0.0f;
	*(float*)(attrib24 + 0x3C) = 0.0f;
	*(float*)(attrib24 + 0x44) = 100.0f;
}

void ModMainLoop() {
	while (true) {
		uintptr_t vehicleBase = GetVehicleBase();
		if (vehicleBase) {
			ApplyGripValues(vehicleBase);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(ModMainLoop).detach();
	}
	return TRUE;
}