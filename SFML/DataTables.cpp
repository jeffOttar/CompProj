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
#include "DataTables.h"
#include "JsonFrameParser.h"

namespace GEX {
	std::map<AircraftType, AircraftData> initializeAircraftData()
	{
		std::map<AircraftType, AircraftData> data;
		
		//player aircraft
		data[AircraftType::Eagle].hitpoints = 100;
		data[AircraftType::Eagle].speed = 200.f;
		data[AircraftType::Eagle].fireInterval = sf::seconds(1);
		data[AircraftType::Eagle].texture = TextureID::Entities;
		data[AircraftType::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
		data[AircraftType::Eagle].hasRollAnimation = true;

		//non shooting enemy
		data[AircraftType::Raptor].hitpoints = 20;
		data[AircraftType::Raptor].speed = 80.f;
		data[AircraftType::Raptor].texture = TextureID::Entities;
		data[AircraftType::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
		data[AircraftType::Raptor].directions.emplace_back(Direction(45.f, 80.f));
		data[AircraftType::Raptor].directions.emplace_back(Direction(-45.f, 160.f));
		data[AircraftType::Raptor].directions.emplace_back(Direction(45.f, 80.f));
		data[AircraftType::Raptor].fireInterval = sf::Time::Zero;//never fires
		data[AircraftType::Raptor].hasRollAnimation = false;

		//shooting enemy
		data[AircraftType::Avenger].hitpoints = 40;
		data[AircraftType::Avenger].speed = 50.f;
		data[AircraftType::Avenger].texture = TextureID::Entities;
		data[AircraftType::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
		data[AircraftType::Avenger].directions.emplace_back(Direction(45.f, 50.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(0.f, 50.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(-45.f, 100.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(0.f, 50.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(45.f, 50.f));
		data[AircraftType::Avenger].fireInterval = sf::seconds(2);
		data[AircraftType::Avenger].hasRollAnimation = false;

		return data;
	}
	std::map<Item::Type, ItemData> initializeItemData()
	{
		std::map<Item::Type, ItemData> data;

		data[Item::Type::BlackCoat].texture = TextureID::Items;
		data[Item::Type::BlackCoat].price = 250;
		data[Item::Type::BlackCoat].textureRect= sf::IntRect(0,320,32,32);

		data[Item::Type::Bread].texture = TextureID::Items;
		data[Item::Type::Bread].price = 20;
		data[Item::Type::Bread].textureRect = sf::IntRect(0, 960, 32, 32);

		data[Item::Type::OldSword].texture = TextureID::Items;
		data[Item::Type::OldSword].price = 100;
		data[Item::Type::OldSword].textureRect = sf::IntRect(32, 0, 32, 32);
		

		return data;
	}
	std::map<Projectile::Type, ProjectileData> initializeProjectileData()
	{
		std::map<Projectile::Type, ProjectileData> data;

		data[Projectile::Type::AlliedBullet].damage = 10;
		data[Projectile::Type::AlliedBullet].speed = 300.f;
		data[Projectile::Type::AlliedBullet].texture = TextureID::Entities;
		data[Projectile::Type::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

		data[Projectile::Type::EnemyBullet].damage = 10;
		data[Projectile::Type::EnemyBullet].speed = 300.f;
		data[Projectile::Type::EnemyBullet].texture = TextureID::Entities;
		data[Projectile::Type::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);


		data[Projectile::Type::Missile].damage = 200;
		//data[Projectile::Type::Missile].speed = 150.f;
		data[Projectile::Type::Missile].speed = 200.f;
		data[Projectile::Type::Missile].texture = TextureID::Entities;
		data[Projectile::Type::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

		return data;
	}
	std::map<Pickup::Type, PickupData> initializePickupData()
	{
		std::map<Pickup::Type, PickupData> data;

		data[Pickup::Type::HealthRefill].texture = TextureID::Entities;
		data[Pickup::Type::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
		data[Pickup::Type::HealthRefill].action = [](Aircraft& a) {a.repair(25); };

		data[Pickup::Type::MissileRefill].texture = TextureID::Entities;
		data[Pickup::Type::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
		data[Pickup::Type::MissileRefill].action = [](Aircraft& a) {a.collectMissiles(3); };

		data[Pickup::Type::FireSpread].texture = TextureID::Entities;
		data[Pickup::Type::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
		data[Pickup::Type::FireSpread].action = [](Aircraft& a) {a.increaceFireSpread(); };

		data[Pickup::Type::FireRate].texture = TextureID::Entities;
		data[Pickup::Type::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
		data[Pickup::Type::FireRate].action = [](Aircraft& a) {a.increaceFireRate(); };

		return data;
	}

	std::map<Particle::Type, ParticleData> initializeParticleData()
	{
		std::map<Particle::Type, ParticleData> data;

		//fire part has short lifetime and is composed of these colors
		data[Particle::Type::Propellant].color = sf::Color(255, 255, 50);
		//data[Particle::Type::Propellant].color = sf::Color(0, 0, 255);
		data[Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

		//smoke is grey and lasts 4 seconds
		data[Particle::Type::Smoke].color = sf::Color(50, 50, 50);
		//data[Particle::Type::Smoke].color = sf::Color(197, 179, 88);
		data[Particle::Type::Smoke].lifetime = sf::seconds(4.f);

		return data;
	}
	std::map<Player::PlayerType, PlayerData> initializePlayerData()
	{
		std::map<Player::PlayerType, PlayerData> data;

		data[Player::PlayerType::Player].hitpoints = 100;
		data[Player::PlayerType::Player].texture = TextureID::Characters;
		//data[Player::PlayerType::Player].textureRect = sf::IntRect(0, 384, 32, 32);//first 2 numbers are location in sprite sheet

		JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

		data[Player::PlayerType::Player].animations[Player::State::Up].addFrameSet(frames.getFramesFor("pinkHairUp"));
		data[Player::PlayerType::Player].animations[Player::State::Up].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Up].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::Down].addFrameSet(frames.getFramesFor("pinkHairDown"));
		data[Player::PlayerType::Player].animations[Player::State::Down].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Down].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::Right].addFrameSet(frames.getFramesFor("pinkHairRight"));
		data[Player::PlayerType::Player].animations[Player::State::Right].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Right].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::Left].addFrameSet(frames.getFramesFor("pinkHairLeft "));
		data[Player::PlayerType::Player].animations[Player::State::Left].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Left].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveUp].addFrameSet(frames.getFramesFor("pinkHairUp"));
		data[Player::PlayerType::Player].animations[Player::State::MoveUp].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveUp].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveDown].addFrameSet(frames.getFramesFor("pinkHairDown"));
		data[Player::PlayerType::Player].animations[Player::State::MoveDown].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveDown].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveRight].addFrameSet(frames.getFramesFor("pinkHairRight"));
		data[Player::PlayerType::Player].animations[Player::State::MoveRight].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveRight].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveLeft].addFrameSet(frames.getFramesFor("pinkHairLeft"));
		data[Player::PlayerType::Player].animations[Player::State::MoveLeft].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveLeft].setRepeating(false);
		

		return data;

		/***VILLAGER AND ITEM WILL USE JSON LIKE THIS

		data[Ghost::GhostType::Ghost].texture = TextureID::Atlas;
		data[Ghost::GhostType::Ghost].hitpoints = 5;
		data[Ghost::GhostType::Ghost].textureRect = sf::IntRect(43, 379, 40, 40);

		JsonFrameParser frames = JsonFrameParser("Media/Textures/pacManAtlas.json");

		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Up].addFrameSet(frames.getFramesFor("red up"));//ghost up images

		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Up].setDuration(sf::Time(sf::seconds(1.f)));
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Up].setRepeating(true);

		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Down].addFrameSet(frames.getFramesFor("red down"));//ghost down images
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Down].setDuration(sf::Time(sf::seconds(1.f)));
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Down].setRepeating(true);

		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Left].addFrameSet(frames.getFramesFor("red left"));//ghost left images
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Left].setDuration(sf::Time(sf::seconds(1.f)));
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Left].setRepeating(true);

		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Right].addFrameSet(frames.getFramesFor("red right"));//ghost right images
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Right].setDuration(sf::Time(sf::seconds(1.f)));
		data[Ghost::GhostType::Ghost].animations[Ghost::GhostState::Right].setRepeating(true);
		*/
	}
	std::map<Shelf::ShelfType, ShelfData> initializeShelfData()
	{
		std::map<Shelf::ShelfType, ShelfData> data;

		data[Shelf::ShelfType::Shelf].texture = TextureID::Shelf;
		data[Shelf::ShelfType::Shelf].hitpoints = 10;
		data[Shelf::ShelfType::Shelf].textureRect = sf::IntRect(0, 0, 75, 100);

		return data;
	}
	std::map<Villager::Type, VillagerData> initializeVillagerData()
	{
		std::map<Villager::Type, VillagerData> data;

		data[Villager::Type::Courtney].hitpoints = 100;
		data[Villager::Type::Courtney].texture = TextureID::Characters;
		//data[Player::PlayerType::Player].textureRect = sf::IntRect(0, 384, 32, 32);//first 2 numbers are location in sprite sheet

		JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

		data[Villager::Type::Courtney].animations[Villager::State::Up].addFrameSet(frames.getFramesFor("pinkHairUp"));
		data[Villager::Type::Courtney].animations[Villager::State::Up].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Up].setRepeating(false);
	
		data[Villager::Type::Courtney].animations[Villager::State::Down].addFrameSet(frames.getFramesFor("pinkHairDown"));
		data[Villager::Type::Courtney].animations[Villager::State::Down].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Down].setRepeating(false);
		
		data[Villager::Type::Courtney].animations[Villager::State::Right].addFrameSet(frames.getFramesFor("pinkHairRight"));
		data[Villager::Type::Courtney].animations[Villager::State::Right].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Right].setRepeating(false);
				
		data[Villager::Type::Courtney].animations[Villager::State::Left].addFrameSet(frames.getFramesFor("pinkHairLeft "));
		data[Villager::Type::Courtney].animations[Villager::State::Left].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Left].setRepeating(false);
					
		data[Villager::Type::Courtney].animations[Villager::State::MoveUp].addFrameSet(frames.getFramesFor("pinkHairUp"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveUp].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveUp].setRepeating(false);
							
		data[Villager::Type::Courtney].animations[Villager::State::MoveDown].addFrameSet(frames.getFramesFor("pinkHairDown"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveDown].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveDown].setRepeating(false);
							
		data[Villager::Type::Courtney].animations[Villager::State::MoveRight].addFrameSet(frames.getFramesFor("pinkHairRight"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveRight].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveRight].setRepeating(false);
							
		data[Villager::Type::Courtney].animations[Villager::State::MoveLeft].addFrameSet(frames.getFramesFor("pinkHairLeft"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveLeft].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveLeft].setRepeating(false);


		return data;
	}
}