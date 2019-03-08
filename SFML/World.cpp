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
#include "Pickup.h"
#include "Projectile.h"
#include "ParticleNode.h"
#include "PostEffect.h"
#include "BloomEffect.h"
#include "SoundNode.h"
#include "State.h"


namespace GEX {
	World::World(sf::RenderTarget& outputTarget,SoundPlayer& sounds, Player& player) :
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
		_enemySpawnPoints(),
		_activeEnemies(),
		_commandQueue(),
		_bloomEffect()
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

		//spawnEnemies();
		updateSounds();
	}

	void World::adaptPlayerVelocity()
	{
		_player->setVelocity(sf::Vector2f(0.f, 0.f));
	}

	void World::addShelves()
	{
		//left wall shelves
		createShelves(-400.f,30.f,270.f);
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
			//move the enemy to the scenegraph
			_sceneLayers[UpperAir]->attachChild(std::move(shelf));
	}

	void World::addEnemies()
	{
		addEnemy(AircraftType::Raptor, 0.f, 200.f);
		addEnemy(AircraftType::Raptor, 250.f, 200.f);
		addEnemy(AircraftType::Raptor, -250.f, 200.f);
		
		addEnemy(AircraftType::Avenger, -70.f, 400.f);//these start at end of screen
		addEnemy(AircraftType::Avenger, 70.f, 400.f);

		addEnemy(AircraftType::Avenger, -170.f, 1000.f);
		addEnemy(AircraftType::Avenger, 170.f, 1000.f);

		addEnemy(AircraftType::Avenger, -140.f, 600.f);
		addEnemy(AircraftType::Avenger, 140.f, 600.f);

		//additional enemies
		addEnemy(AircraftType::Raptor, 0.f, 1200.f);
		addEnemy(AircraftType::Raptor, 250.f, 1200.f);
		addEnemy(AircraftType::Raptor, -250.f, 1200.f);

		addEnemy(AircraftType::Avenger, -70.f, 1400.f);//these start at end of screen
		addEnemy(AircraftType::Avenger, 70.f, 1400.f);

		addEnemy(AircraftType::Avenger, -170.f, 1800.f);
		addEnemy(AircraftType::Avenger, 170.f, 1800.f);

		addEnemy(AircraftType::Avenger, -140.f, 1600.f);
		addEnemy(AircraftType::Avenger, 140.f, 1600.f);

		//sort the planes based on spawn point location 
		std::sort(_enemySpawnPoints.begin(), _enemySpawnPoints.end(),
			[](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
	}

	void World::addEnemy(AircraftType type, float relX, float relY)
	{
		//relY and relX stand for relative x and y
		SpawnPoint spawnPoint(type, _spawnPosition.x + relX, _spawnPosition.y - relY);
		_enemySpawnPoints.push_back(spawnPoint);
	}

	void World::spawnEnemies()
	{
		//while there are enemies to spawn and the enemy is located within the battlefield
		while (!_enemySpawnPoints.empty() && _enemySpawnPoints.back().y > getBattlefieldBounds().top)
		{
			auto spawnPoint = _enemySpawnPoints.back();

			std::unique_ptr<Aircraft> enemy(new Aircraft(spawnPoint.type, _textures));
			enemy->setPosition(spawnPoint.x, spawnPoint.y);
			enemy->setRotation(180);
			//move the enemy to the scenegraph
			_sceneLayers[UpperAir]->attachChild(std::move(enemy));
			//remove the spawned enemy from the list
			_enemySpawnPoints.pop_back();
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

	void World::guideMissiles()
	{
		
		//build list of active enemies
		Command enemyCollector;
		enemyCollector.category = Category::Type::EnemyAircraft;//everything with category enemy with execute this command
		enemyCollector.action = derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time dt) 
		{
			if (!enemy.isDestroyed())//dont add destroyed enemies to list
			{
				_activeEnemies.push_back(&enemy);
			}
		});

		Command missileGuider;
		missileGuider.category = Category::Type::AlliedProjectile;//the derived action is used for safe downcasts
		missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		{
			//ignore bullets only guide missiles
			if (!missile.isGuided())
			{
				return;//if it is a bullet return
			}

			float minDistance = std::numeric_limits<float>::max();
			//start the min distance as the max float and when you find a smaller value set that as minimum distance
			Aircraft* closestEnemy = nullptr;

			for (auto* e : _activeEnemies)
			{
				auto d = distance(missile, *e);
				if (d < minDistance)
				{
					minDistance = d;
					closestEnemy = e;
				}
			}
			if (closestEnemy)
			{
				missile.guidedTowards(closestEnemy->getWorldPosition());
			}
		});

		_commandQueue.push(enemyCollector);
		_commandQueue.push(missileGuider);
		_activeEnemies.clear();
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
			//check for matching categories for the player and enemy two enemies colliding does nothing
			if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyAircraft))
			{
				//if there is a player and enemy colliding then cast them into variables
				auto& player = static_cast<Aircraft&>(*(pair.first));
				auto& enemy = static_cast<Aircraft&>(*(pair.second));

				//when a collsion occurs the player loses health amounting to enemy health
				player.damage(enemy.getHitPoints());
				enemy.destroy();
			}
			//BELOW STOPS COLLISION VERTICALLY BUT IS FLAWED WHEN MOVING HORIZONTALLY AND STOPS SHELF INTERACTION
			//else if (matchesCategories(pair, Category::Type::Player, Category::Type::Shelf))
			//{
			//	auto& player = static_cast<Player&>(*pair.first);
			//	auto& shelf = static_cast<Shelf&>(*pair.second);

			//	// Apply pickup effect to player, destroy projectile

			//	auto pos = player.getPosition();

			//	if (!(shelf.getBoundingBox().intersects(sf::FloatRect(player.getWorldPosition().x - 1, player.getWorldPosition().y, 32, 32))))
			//	{
			//		player.setPosition(pos.x - 1, pos.y);
			//	}
			//	if (!(shelf.getBoundingBox().intersects(sf::FloatRect(player.getWorldPosition().x + 1, player.getWorldPosition().y, 32, 32))))
			//	{
			//		player.setPosition(pos.x + 1, pos.y);
			//	}
			//	if (!(shelf.getBoundingBox().intersects(sf::FloatRect(player.getWorldPosition().x, player.getWorldPosition().y - 1, 32, 32))))
			//	{
			//		player.setPosition(pos.x, pos.y - 1);
			//	}
			//	if (!(shelf.getBoundingBox().intersects(sf::FloatRect(player.getWorldPosition().x, player.getWorldPosition().y + 1, 32, 32))))
			//	{
			//		player.setPosition(pos.x, pos.y + 1);
			//	}
			//	//play collision sound
			//	//player.playLocalSound(_commandQueue, SoundEffectID::CollectPickup);
			//}
			else if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::Pickup))
			{
				auto& player = static_cast<Aircraft&>(*pair.first);
				auto& pickup = static_cast<Pickup&>(*pair.second);

				// Apply pickup effect to player, destroy projectile
				pickup.apply(player);
				pickup.destroy();

				player.playLocalSound(_commandQueue, SoundEffectID::CollectPickup);
			}
			else if ((matchesCategories(pair, Category::Type::EnemyAircraft, Category::Type::AlliedProjectile))
				|| (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyProjectile)))
			{
				auto& aircraft = static_cast<Aircraft&>(*pair.first);
				auto& projectile = static_cast<Projectile&>(*pair.second);

				//projectile damages aircraft and then is destroyed
				aircraft.damage(projectile.getDamage());
				projectile.destroy();
			}
		}
	}

	void World::destroyEntitiesOutOfView()
	{
		Command command;
		command.category = Category::Type::Projectile | Category::Type::EnemyAircraft | Category::Type::Pickup;
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
			bool villagerInteract = false;
			//check if near villager or shelf and move to correct method
			

			Shelf* tmp = nullptr;
			for (auto s : _shelves)
			{
				//if it is intersecting player at all 
				if (s->getBoundingBox().intersects(sf::FloatRect((_player->getWorldPosition().x + 20.f), (_player->getWorldPosition().y + 20.f),32,32))||
					s->getBoundingBox().intersects(sf::FloatRect((_player->getWorldPosition().x - 20.f), (_player->getWorldPosition().y - 20.f), 32, 32)))
				{
  					tmp = s;
					shelfInteract = true;
				}
			}
			if (shelfInteract && !tmp->isOccupied())
			{
				return true;
			}
			else if(shelfInteract && tmp->isOccupied())
			{
				Item item = tmp->removeItemOnShelf();
				_player->addToInventory(&item);
				return false;
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
		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE+yBorder);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - 40.f);

		_player->setPosition(position);
	}

	void World::draw()
	{
		if (PostEffect::isSupported())
		{
			//apply the effects
			_sceneTexture.clear();
			_sceneTexture.setView(_worldView);
			_sceneTexture.draw(_sceneGraph);
			_sceneTexture.display();
			_bloomEffect.apply(_sceneTexture, _target);
		}
		else {
			_target.setView(_worldView);
			_target.draw(_sceneGraph);
		}
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
			return !_worldBounds.contains(sf::Vector2f(_player->getPosition().x + BORDER_DISTANCE,
				_player->getPosition().y + BORDER_DISTANCE));
		}
		else
		{
			return false;
		}
			

	}

	void World::loadTextures()
	{
		_textures.load(GEX::TextureID::Entities, "Media/Textures/Entities.png");
		_textures.load(GEX::TextureID::Jungle, "Media/Textures/Jungle.png");
		_textures.load(GEX::TextureID::Particle, "Media/Textures/Particle.png");
		_textures.load(GEX::TextureID::Explosion, "Media/Textures/Explosion.png");
		_textures.load(GEX::TextureID::FinishLine, "Media/Textures/FinishLine.png");
		_textures.load(GEX::TextureID::Shop, "Media/Textures/shopBackground2.png");
		_textures.load(GEX::TextureID::Shelf, "Media/Textures/shelf.png");
		_textures.load(GEX::TextureID::Items, "Media/Textures/items.png");

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

		//Particle systems
		std::unique_ptr<ParticleNode> smoke(new ParticleNode(Particle::Type::Smoke, _textures));
		_sceneLayers[LowerAir]->attachChild(std::move(smoke));
		std::unique_ptr<ParticleNode> fire(new ParticleNode(Particle::Type::Propellant, _textures));
		_sceneLayers[LowerAir]->attachChild(std::move(fire));

		//add a finish line
		//sf::Texture& finishTexture = _textures.get(TextureID::FinishLine);
		//std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
		//finishSprite->setPosition(0.f, -76.f);
		//_sceneLayers[Background]->attachChild(std::move(finishSprite));

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

		////add player aircraft and game objects
		//std::unique_ptr<Player> leader(new Player(Player::PlayerType::Player, _textures));
		//leader->setPosition(_spawnPosition);
		//leader->setVelocity(0.f, 0.f);
		//_player = leader.get();
		//_sceneLayers[UpperAir]->attachChild(std::move(leader));

		

		addEnemies();

		addShelves();

		//add player aircraft and game objects
		std::unique_ptr<Player> player(_player);
		player->setPosition(_spawnPosition+sf::Vector2f(0,50));
		_sceneLayers[UpperAir]->attachChild(std::move(player));

		CollectShelves();
	}



}