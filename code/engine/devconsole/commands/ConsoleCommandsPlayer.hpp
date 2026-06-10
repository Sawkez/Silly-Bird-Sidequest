#pragma once

#include <string>
#include <vector>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/world/WorldManager.hpp"
#include "game/player/Player.hpp"

namespace ConsoleCommandsPlayer {
void giveUpgrade(const std::vector<std::string>& args) {
	Player& player = WorldManager::GetLevel().GetPlayer();

	bool gave = false;

	for (const auto& upgrade : args) {
		if (upgrade == "0" || upgrade == "dive") {
			player.GiveUpgrade(Player::UPGRADE_DIVE);
			gave = true;
		}

		else if (upgrade == "1" || upgrade == "dash") {
			player.GiveUpgrade(Player::UPGRADE_DASH);
			gave = true;
		}

		else if (upgrade == "2" || upgrade == "slide") {
			player.GiveUpgrade(Player::UPGRADE_SLIDE);
			gave = true;
		}

		else if (upgrade == "3" || upgrade == "diveboost") {
			player.GiveUpgrade(Player::UPGRADE_DIVEBOOST);
			gave = true;
		}

		else if (upgrade == "4" || upgrade == "rejuvenator") {
			player.GiveUpgrade(Player::UPGRADE_REJUVENATOR);
			gave = true;
		}

		else if (upgrade == "5" || upgrade == "wallrun") {
			player.GiveUpgrade(Player::UPGRADE_WALLRUN);
			gave = true;
		}

		else if (upgrade == "all") {
			player.SetUpgrades(0b11111111);
			gave = true;
		}

		else {
			dc::err << "Upgrade not found: " << upgrade << dc::endl;
		}
	}

	if (!gave) {
		dc::msg << "Possible options: dive (0), dash (1), slide (2), diveboost (3), rejuvenator (4), wallrun (5), all" << dc::endl;
	}
}
}  // namespace ConsoleCommandsPlayer