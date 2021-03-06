/**
* @file
* @author Jeff Ottar-
* @version 1.0
*
*
* @section DESCRIPTION
* <  >
*
*
* @section LICENSE
*
*
* Copyright 2017
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/
#pragma once
#include "TextureManager.h"
#include "Item.h"
#include "Player.h"
#include "Shelf.h"
#include "Villager.h"

#include <map>
#include <vector>
#include <string>

namespace GEX
{
	
	struct ItemData
	{
		double						price;
		TextureID					texture;
		sf::IntRect		textureRect;
		
	};

	struct ParticleData
	{
		sf::Color		color;
		sf::Time		lifetime;
	};

	struct PlayerData//data for the player
	{
		TextureID								texture;
		std::map<Player::State, Animation2>		animations;
		int										hitpoints;
	};
	struct ShelfData {

		int											hitpoints;
		TextureID									texture;
		sf::IntRect									textureRect;
		bool										occupied;
	};
	struct VillagerData//data for the player
	{
		TextureID								texture;
		std::map<Villager::State, Animation2>		animations;
		int										hitpoints;
		std::map<Item::Type, int>				value;
		std::vector<std::string>				dialogue;
	};


	std::map<Item::Type, ItemData> initializeItemData();

	std::map<Player::PlayerType, PlayerData> initializePlayerData();

	std::map<Shelf::ShelfType, ShelfData>			initializeShelfData();

	std::map<Villager::Type, VillagerData>			initializeVillagerData();
}
