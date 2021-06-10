#include "pch.h"
#include "loader.h"
#include "CE_proc.h"
#include "CE_mem.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "mhw_Template.h"
#include "mhw_console.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace loader;

nlohmann::json ConfigFile;

// You need also change the name in mhw_console.cpp
const std::string PLUGIN_NAME = "Template";
const std::string PLUGIN_KEY = "temp";

uintptr_t moduleBase;
uintptr_t mhwPreStage;
uintptr_t mhwEntStage;

int currentStage;

void LoadConfig()
{
	std::ifstream file("nativePC/plugins/" + PLUGIN_NAME + "-config.json");
	ConfigFile = nlohmann::json::object();
	if (!file.fail()) {
		file >> ConfigFile;
		LOG(INFO) <<"[" << PLUGIN_NAME << "] Config found";
		file.close();
	}
	else
	{
		file.close();
		LOG(INFO) <<"[" << PLUGIN_NAME << "] Config not found, generating...";

		// Generate your config here

		std::ofstream ls("nativePC/plugins/" + PLUGIN_NAME + "-config.json");
		ls << ConfigFile;
		ls.close();
	}

	// Load config here

}

bool CheckStage()
{
	// PTR UPDATE
	uintptr_t mhwStageAddr = moduleBase + 0x506D270;
	std::vector<DWORD_PTR> mhwbasePtr = { 0x80 };

	uintptr_t mhwPtrInit = mem::dFindDMAAddy(mhwStageAddr, mhwbasePtr);

	if (*(int*)mhwPtrInit == 0) return false;

	mhwEntStage = mem::dFindDMAAddy(mhwStageAddr, { 0x80, 0xEEC0, 0x118 });

	// PTR UPDATE
	mhwPreStage = mem::dFindDMAAddy(moduleBase + 0x52247F8, { 0xD320 });

	currentStage = *(int*)mhwPreStage;
	if (currentStage > 100 && currentStage < 900) return true;
	else return false;
}

void CheckPointers()
{
	// Scan your pointer
}

void TemplateMemory()
{
	LOG(INFO) <<"[" << PLUGIN_NAME << "] Waiting for you to enter a stage...";

	moduleBase = (uintptr_t)GetModuleHandle(L"MonsterHunterWorld.exe");
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	ConsoleEnable = TCPConnect();

	// Config loaded
	LoadConfig();

	// While Game is loading check when the player enters a stage
	std::this_thread::sleep_for(std::chrono::milliseconds(40000));
	while (!CheckStage())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}  

	LOG(INFO) <<"[" << PLUGIN_NAME << "] Ready...";
	bool globalOn = true;

	// As most of the offset pointer loads after entering a scene, you can scan it after entering a stage
	CheckPointers();

	// Next loop while in game
	char buffer[1024] = { 0 };

	while (true)
	{
		// Start receive console command
		memset(buffer, 0, sizeof(buffer));
		recv(s, buffer, 1024, 0);

		// "temp" is the keyword when typing in chat bar
		//   it also should be the name of the helper file in \Monster Hunter World\nativePC\plugins\ConsoleHelp
		//   as for this "tempHelp.json"
		if (strncmp(buffer, PLUGIN_KEY + " ", strlen(PLUGIN_KEY)) == 0 && strlen(buffer) >= strlen(PLUGIN_KEY)+1)
		{
			char* pch;
			pch = strtok(buffer, " ");
			pch = strtok(NULL, " ");

			// Globally turn on/off plugin, mostly for keyboard keys
			if (strncmp(pch, "on", 2) == 0)
			{
				globalOn = true;
				LognSend("[" + PLUGIN_NAME + "] [Console] Plugin On");
				continue;
			}
			if (globalOn)
			{
				if (strncmp(pch, "off", 3) == 0)
				{
					globalOn = true;
					LognSend("[" + PLUGIN_NAME + "] [Console] Plugin Off");
				}
				else if (strncmp(pch, "reload", 6) == 0)
				{
					// Reload plugin
					LognSend("[" + PLUGIN_NAME + "] [Console] Reoload Plugin");
					globalOn = true;
					LoadConfig();
					CheckStage();
					CheckPointers();
				}
				// option for console command

				Sleep(50);
			}
		}

		if (globalOn)
		{
			// Your code here

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			Sleep(500);
		}
	}
}