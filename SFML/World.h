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
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include "CommandQueue.h"
#include "SoundPlayer.h"
#include "Player.h"
#include "Shelf.h"
#include "Villager.h"

#include <memory>
#include <iostream>

namespace sf
{
	class RenderTarget;
}
namespace GEX {


	class World
	{
	public:
		World(sf::RenderTarget& outputTarget, SoundPlayer& sounds, Player& player);
		void								update(sf::Time dt,CommandQueue& commands);
		void								draw();

		CommandQueue&						getCommandQueue();
		bool								hasAlivePlayer() const;
		bool								hasPlayerReachedEnd() const;
		bool								atDoor();
		bool								OutOfMoney();
		bool								shelfEvent(const sf::Event& event);
		bool								dialogueEvent(const sf::Event& event);
		void CollectShelves();
		void								updateVillagers(sf::Time dt);
		bool								villagerBuying();//checks all villagers and has the buy command pushed

	private:
		void								loadTextures();
		void								buildScene();
		void								adaptPlayerPosition();
		void								adaptPlayerVelocity();
		sf::Vector2f						adaptVillagerPosition(sf::Vector2f movement, Villager* v);

		void								addShelves();
		void								createShelves(float relX, float relY, float rotation);

		void								addVillagers();

		void								moveVillager(sf::Vector2f movement, Villager* v);

		sf::FloatRect						getViewBounds() const;
		sf::FloatRect						getBattlefieldBounds() const;

		void								handleCollisions();

		void								destroyEntitiesOutOfView();
		void								updateSounds();


	private: 
		enum Layer
		{
			Background = 0,
			LowerAir,
			UpperAir,
			LayerCount
		};


	


	private:
		sf::RenderTarget&					_target;//the order that they are declared in the .h is the order they are created in the initialization list
		sf::RenderTexture					_sceneTexture;
		sf::View                            _worldView;
		TextureManager                      _textures;
		SoundPlayer&						_sounds;
		std::vector<Shelf*>					_shelves;

		SceneNode                           _sceneGraph;
		std::vector<SceneNode*>				_sceneLayers;

		CommandQueue						_commandQueue;
		sf::FloatRect                       _worldBounds;
		sf::Vector2f                        _spawnPosition;
		float                               _scrollSpeed;

		Player*								_player;

		std::vector<Villager*>				_activeVillagers;

		sf::Time							_spawnTimer;
		const sf::Time						DEFAULT_TIME = sf::seconds(30);//30
	};

}