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
#include "World.h"
#include "SoundNode.h"
#include "State.h"
#include "CurrentShelf.h"
#include "CurrentVillager.h"
#include <cstdlib>

#include <algorithm>

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, (static_cast<int>(GEX::Villager::Type::count) - 1));
auto randomNumber = std::bind(distribution, generator);
namespace GEX {

	World::World(sf::RenderTarget& outputTarget, SoundPlayer& sounds, Player& player) :
		_target(outputTarget),
		_worldView(outputTarget.getDefaultView()),
		_textures(),
		_sounds(sounds),
		_shelves(),
		_sceneGraph(),
		_sceneLayers(),
		_worldBounds(0.f, 0.f, _worldView.getSize().x, _worldView.getSize().y),//left top width height
		_spawnPosition(_worldView.getSize().x / 2.f, _worldBounds.height - _worldView.getSize().y / 2.f),
		_scrollSpeed(-50.f),
		_player(&player),
		_activeVillagers(),
		_commandQueue(),
		_spawnTimer(DEFAULT_TIME)
	{
		_sceneTexture.create(_target.getSize().x, _target.getSize().y);

		loadTextures();

		buildScene();

		//prepare the view
		_worldView.setCenter(_spawnPosition);
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		//scroll the world
		_player->setVelocity(0.f, 0.f);

		destroyEntitiesOutOfView();

		//while the commandQueue is not empty send the next command to the scene graph
		while (!_commandQueue.isEmpty())
		{
			_sceneGraph.onCommand(_commandQueue.pop(), dt);
		}


		handleCollisions();
		_sceneGraph.removeWrecks();

		adaptPlayerVelocity();
		_sceneGraph.update(dt, commands);
		adaptPlayerPosition();

		//clear list before each recollection
		Command villagerCollector;
		villagerCollector.category = Category::Type::Villager;//everything with category villager with execute this command
		villagerCollector.action = derivedAction<Villager>([this](Villager& villager, sf::Time dt)
		{
			auto type = villager.getType();
			bool exists = false;
			for (auto v : _activeVillagers)//if villager doesnt exist then they can be added
			{
				if ((v->getType()) == type)
				{
					exists = true;
				}
			}

			if (!villager.isDestroyed() && !exists)//dont add destroyed villagers to list 
			{
				_activeVillagers.push_back(&villager);
			}
		});
		_commandQueue.push(villagerCollector);
		
		_spawnTimer -= dt;
		if (_spawnTimer <= sf::Time::Zero)//every so many seconds
		{
			_spawnTimer = DEFAULT_TIME;
			addVillagers();
		}
		updateSounds();
	}

	void World::adaptPlayerVelocity()
	{
		_player->setVelocity(sf::Vector2f(0.f, 0.f));
	}

	sf::Vector2f World::adaptVillagerPosition(sf::Vector2f movement, Villager* v)
	{
		const float BORDER_DISTANCE = 195.f;
		const float yBorder = 75.f;//used for the y axis addition

		sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, //get the 0,0 of the bounds by getting center and then half of the height and width
			_worldView.getSize());
		sf::Vector2f position = v->getPosition() + movement;

		//adjust position so it stays within the left and right bounds
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		//adjust position so it stays within the bounds
		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE + yBorder);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - 40.f);

		return position;
	}

	void World::addShelves()
	{
		//left wall shelves
		createShelves(-400.f, 30.f, 270.f);
		createShelves(-410.f, -70.f, 270.f);
		createShelves(-420.f, -170.f, 270.f);
		createShelves(-430.f, -270.f, 270.f);

		//upper shelves
		createShelves(-200.f, 100.f, 0.f);
		createShelves(-100.f, 100.f, 0.f);
		createShelves(-00.f, 100.f, 0.f);
		createShelves(100.f, 100.f, 0.f);
	}

	void World::createShelves(float relX, float relY, float rotation)
	{

		std::unique_ptr<Shelf> shelf(new Shelf(Shelf::ShelfType::Shelf, _textures));
		shelf->setPosition(_spawnPosition.x + relX, _spawnPosition.y - relY);
		shelf->setRotation(rotation);
		_sceneLayers[UpperAir]->attachChild(std::move(shelf));
	}

	void World::addVillagers()
	{
		int randomNum = randomNumber();
		auto type = static_cast<Villager::Type>(randomNum);
		bool exists = false;

		//loop through all active villagers and check if new type exists
		for (auto v : _activeVillagers)
		{
			if ((v->getType()) == type)
			{
				exists = true;
			}
		}
		if (!exists)//if new villager doesnt exist then spawn them
		{
			switch (type)
			{
			case Villager::Type::Courtney:
			{
				std::unique_ptr<Villager> villager(new Villager(type, _textures));
				villager->setPosition(_spawnPosition.x + 150.f, _spawnPosition.y);
				//shelf->setRotation(rotation);
				_sceneLayers[UpperAir]->attachChild(std::move(villager));
				break;
			}
			case Villager::Type::Greg:
			{
				std::unique_ptr<Villager> villager(new Villager(type, _textures));
				villager->setPosition(_spawnPosition.x + 150.f, _spawnPosition.y);
				//shelf->setRotation(rotation);
				_sceneLayers[UpperAir]->attachChild(std::move(villager));
				break;
			}
			case Villager::Type::Jhon:
			{
				std::unique_ptr<Villager> villager(new Villager(type, _textures));
				villager->setPosition(_spawnPosition.x + 150.f, _spawnPosition.y);
				//shelf->setRotation(rotation);
				_sceneLayers[UpperAir]->attachChild(std::move(villager));
				break;
			}
			}
		}

	}

	/**
	move the villager and update their state
	*/
	void World::moveVillager(sf::Vector2f movement, Villager* v)
	{
		v->move(movement);
		if (movement != sf::Vector2f(0, 0))
		{
			if (movement.x < 0)
			{
				if (v->finishedAnimation())
					v->setState(Villager::State::MoveLeft);
			}
			else if (movement.x > 0)
			{
				if (v->finishedAnimation())
					v->setState(Villager::State::MoveRight);
			}
			else if (movement.y < 0)
			{
				if (v->finishedAnimation())
					v->setState(Villager::State::MoveUp);

			}
			else if (movement.y > 0)
			{
				if (v->finishedAnimation())
					v->setState(Villager::State::MoveDown);

			}
		}

	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		//get the view bounds and expand the height by 100 pixels and reduce the top by 100 pixels(+y goes down)
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100.f;
		bounds.height += 100.f;
		return bounds;
	}


	/**
	*check if they have matching categories or if they match but are in the wrong order
	*
	*/
	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		// check if they match categories
		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1)//check if they match but in the wrong order
		{
			std::swap(colliders.first, colliders.second);//swap them the right way
			return true;
		}
		else
		{
			return false;
		}
	}

	void World::handleCollisions()
	{
		//build list of colliding pairs
		std::set<SceneNode::Pair> collisionPairs;
		_sceneGraph.checkSceneCollision(_sceneGraph, collisionPairs);


		for (SceneNode::Pair pair : collisionPairs)
		{
			 if (matchesCategories(pair, Category::Type::Player, Category::Type::Shelf))
			{
				auto& player = static_cast<Player&>(*pair.first);
				auto& shelf = static_cast<Shelf&>(*pair.second);

				auto pos = player.getWorldPosition();
				auto pos2 = shelf.getWorldPosition();

				if (!(player.getBoundingBox().contains(shelf.getWorldPosition())))//bounce player back the difference between positions
				{
					auto diff = pos - pos2;
					player.setPosition((pos + (diff / 2.f)));
				}

				
			
				//play collision sound
				player.playLocalSound(_commandQueue, SoundEffectID::Bump);
			}
			else if (matchesCategories(pair, Category::Type::Player, Category::Type::Villager))
			{
				auto& player = static_cast<Player&>(*pair.first);
				auto& villager = static_cast<Villager&>(*pair.second);

				auto pos = player.getWorldPosition();
				auto pos2 = villager.getWorldPosition();

				//bounce player back the difference between positions
				if (!(player.getBoundingBox().contains(villager.getWorldPosition())))
				{
					auto diff = pos - pos2;
					player.setPosition((pos + (diff/2.f)));
				}

				
				//play collision sound
				player.playLocalSound(_commandQueue, SoundEffectID::Bump);
			}
			else if (matchesCategories(pair, Category::Type::Villager, Category::Type::Shelf))
			{
				auto& villager = static_cast<Villager&>(*pair.first);
				auto& shelf = static_cast<Shelf&>(*pair.second);

				// Apply pickup effect to player, destroy projectile

				auto pos = villager.getPosition();
				auto pos2 = shelf.getWorldPosition();

				//bounce villager back the difference between positions
				if (!(villager.getBoundingBox().contains(shelf.getWorldPosition())))
				{
					auto diff = pos - pos2;
					villager.setPosition((pos + (diff / 2.f)));
				}
			}
			
		}
	}

	void World::destroyEntitiesOutOfView()
	{
		Command command;
		command.category = Category::Type::Villager;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
		{
			//destroy if it is not in the area
			if (!getBattlefieldBounds().intersects(e.getBoundingBox())) {
				e.destroy();
			}
		});

		_commandQueue.push(command);
	}

	void World::updateSounds()
	{
		_sounds.setListenerPosition(_player->getWorldPosition());
		_sounds.removeStoppedSounds();
	}

	bool World::shelfEvent(const sf::Event & event)
	{
		if (event.key.code == sf::Keyboard::Space)//if key press is return
		{
			bool shelfInteract = false;


			Shelf* tmp = nullptr;
			for (auto s : _shelves)//if interacting with shelf then it is a shelf event
			{
				//if it is intersecting player at all 
				if (s->getBoundingBox().intersects(sf::FloatRect((_player->getWorldPosition().x + 40.f), (_player->getWorldPosition().y + 40.f), 32, 32)) ||
					s->getBoundingBox().intersects(sf::FloatRect((_player->getWorldPosition().x - 40.f), (_player->getWorldPosition().y - 40.f), 32, 32)))
				{
					//tmp = s;
					GEX::CurrentShelf::getInstance().setCurrentShelf(s);
					shelfInteract = true;
				}
			}
			tmp = GEX::CurrentShelf::getInstance().getCurrentShelf();
			if (GEX::CurrentShelf::getInstance().getCurrentShelf() != nullptr) {
				//if it occupied the remove item else change state and add item
				if (shelfInteract && !tmp->isOccupied())
				{
					_player->playLocalSound(_commandQueue, SoundEffectID::Interact);
					return true;
				}
				if (shelfInteract && tmp->isOccupied())
				{
					_player->playLocalSound(_commandQueue, SoundEffectID::Interact);
					Item item = tmp->removeItemOnShelf();
					_player->addToInventory(&item);
					return false;
				}
			}
		}
		return false;
	}

	bool World::dialogueEvent(const sf::Event & event)
	{
		if (event.key.code == sf::Keyboard::Space)//if key press is return
		{
			bool villagerInteract = false;

			Villager* tmp = nullptr;
			for (auto v : _activeVillagers)
			{
				//if it is intersecting player at all 
				if (v->getBoundingBox().intersects(sf::FloatRect((_player->getWorldPosition().x + 40.f), (_player->getWorldPosition().y + 40.f), 32, 32)) ||
					v->getBoundingBox().intersects(sf::FloatRect((_player->getWorldPosition().x - 40.f), (_player->getWorldPosition().y - 40.f), 32, 32)))
				{
					GEX::CurrentVillager::getInstance().setCurrentVillager(v);
					villagerInteract = true;
				}
			}
			if (GEX::CurrentVillager::getInstance().getCurrentVillager() != nullptr) {
				if (villagerInteract)
				{
					return true;
				}
			}
		}
		return false;
	}

	void World::CollectShelves()
	{
		Command shelfCollector;
		shelfCollector.category = Category::Type::Shelf;//everything with category enemy with execute this command
		shelfCollector.action = derivedAction<Shelf>([this](Shelf& shelf, sf::Time dt)
		{
			_shelves.push_back(&shelf);
		});

		_commandQueue.push(shelfCollector);
	}

	void World::updateVillagers(sf::Time dt)
	{
		for (auto v : _activeVillagers)//update all active villagers
		{
			v->decrementBuyTime(dt);
			v->decrementMoveTime(dt);
			v->decrementLeaveTime(dt);
			if (!(v->getLeaveTime() <= sf::Time::Zero))//if time to do action then do that action
			{
				if (v->getMoveTime() <= sf::Time::Zero)
				{
					sf::Vector2f movement = v->randomMove();
					//check if movement is out of bounds
					adaptVillagerPosition(movement, v);
					v->resetMoveTime();
					moveVillager(movement, v);
				}
				if (v->getBuyTime() <= sf::Time::Zero)
				{
					v->checkBuy();
					v->resetBuyTime();
				}
			}
			else
			{
				auto it = std::find(_activeVillagers.begin(), _activeVillagers.end(), v);
				if (it != _activeVillagers.end())//extra safety
				{
					_activeVillagers.erase(it);
				}
				v->destroy();
			}
		}
	}

	bool World::villagerBuying()
	{
		for (auto v : _activeVillagers)
		{
			if (v->isBuying()) {
				for (auto s : _shelves)
				{
					//if it is close at all 
					if (s->getBoundingBox().intersects(sf::FloatRect((v->getWorldPosition().x + 20.f), (v->getWorldPosition().y + 20.f), 320, 320)) ||
						s->getBoundingBox().intersects(sf::FloatRect((v->getWorldPosition().x - 20.f), (v->getWorldPosition().y - 20.f), 320, 320)))
					{
						GEX::CurrentShelf::getInstance().setCurrentShelf(s);
					}
				}
				if (GEX::CurrentShelf::getInstance().getCurrentShelf() != nullptr)
				{
					bool occupied = GEX::CurrentShelf::getInstance().getCurrentShelf()->isOccupied();
					if (occupied)//if they can buy then return true to that item
					{
						v->setBuy(false);
						GEX::CurrentVillager::getInstance().setCurrentVillager(v);
						return true;
					}
				}
				v->setBuy(false);
			}
		}
		return false;
	}




	void World::adaptPlayerPosition()
	{
		const float BORDER_DISTANCE = 195.f;
		const float yBorder = 75.f;//used for the y axis addition

		sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, //get the 0,0 of the bounds by getting center and then half of the height and width
			_worldView.getSize());
		sf::Vector2f position = _player->getPosition();

		//adjust position so it stays within the left and right bounds
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		//adjust position so it stays within the bounds
		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE + yBorder);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - 40.f);

		_player->setPosition(position);
	}

	void World::draw()
	{
		
			_sceneTexture.clear();
			_sceneTexture.setView(_worldView);
			_sceneTexture.draw(_sceneGraph);
			_sceneTexture.display();
		
			_target.setView(_worldView);
			_target.draw(_sceneGraph);
		
	}

	CommandQueue & World::getCommandQueue()
	{
		return _commandQueue;
	}

	bool World::hasAlivePlayer() const
	{
		return !_player->isDestroyed();
	}

	bool World::hasPlayerReachedEnd() const
	{
		return !_worldBounds.contains(_player->getPosition());
	}

	bool World::atDoor()
	{
		const float BORDER_DISTANCE = 200.f;

		/*return !_worldBounds.contains(sf::Vector2f(_player->getPosition().x + BORDER_DISTANCE,
			_player->getPosition().y + BORDER_DISTANCE));*/

		if (_player->getPosition().y < 480.f && _player->getPosition().y > 240.f)
		{
			_player->playLocalSound(_commandQueue, SoundEffectID::Interact);
			//set the player positition back some after this
			return !_worldBounds.contains(sf::Vector2f(_player->getPosition().x + BORDER_DISTANCE,
				_player->getPosition().y + BORDER_DISTANCE));
		}
		else
		{
			return false;
		}


	}

	bool World::OutOfMoney()
	{
		if (_player->getInventory().empty())//if there are items then they can make money
		{
			double money = _player->getMoney();
			double itemsValue;
			bool hasShelfItem;
			for (auto shelfItem : _shelves)
			{
				hasShelfItem = shelfItem->isOccupied();
				if (hasShelfItem == true)
				{
					break;
				}
			}
			if ((!hasShelfItem) && (money) < 20)//if the money on hand is less than 20 and no items
			{
				return true;
			}
		}

		return false;
	}

	void World::loadTextures()
	{
		_textures.load(GEX::TextureID::Villagers, "Media/Textures/characters.png");
		_textures.load(GEX::TextureID::Shop, "Media/Textures/shopBackground2.png");
		_textures.load(GEX::TextureID::Shelf, "Media/Textures/shelf.png");
		_textures.load(GEX::TextureID::Items, "Media/Textures/items.png");


		_textures.load(GEX::TextureID::Happy, "Media/Textures/happyMarco.png");
		_textures.load(GEX::TextureID::Indifferent, "Media/Textures/indifferentMarco.png");
		_textures.load(GEX::TextureID::Unhappy, "Media/Textures/unhappyMarco.png");

	}

	void World::buildScene()
	{
		//initialize layers
		for (int i = 0; i < LayerCount; i++) {
			auto category = (i == UpperAir) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			_sceneLayers.push_back(layer.get());
			_sceneGraph.attachChild(std::move(layer));
		}



		//sound effects
		std::unique_ptr<SoundNode> sNode(new SoundNode(_sounds));
		_sceneGraph.attachChild(std::move(sNode));

		//draw background
		sf::Texture& texture = _textures.get(TextureID::Shop);//get reference to texture
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(false);//repeat the texture

		//add sprite
		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));//create sprite node using the texture and bounds
		backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);//make texture bounds line up with world bounds
		_sceneLayers[Background]->attachChild(std::move(backgroundSprite));//

		addShelves();

		//add player and game objects
		std::unique_ptr<Player> player(_player);
		player->setPosition(_spawnPosition + sf::Vector2f(0, 50));
		_sceneLayers[UpperAir]->attachChild(std::move(player));

		CollectShelves();
	}



}