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
	
	/***
	set the item values for each item
	***/
	std::map<Item::Type, ItemData> initializeItemData()
	{
		std::map<Item::Type, ItemData> data;

		data[Item::Type::BlackCoat].texture = TextureID::Items;
		data[Item::Type::BlackCoat].price = 250;
		data[Item::Type::BlackCoat].textureRect= sf::IntRect(0,290,32,32);

		data[Item::Type::Bread].texture = TextureID::Items;
		data[Item::Type::Bread].price = 20;
		data[Item::Type::Bread].textureRect = sf::IntRect(0, 960, 32, 32);

		data[Item::Type::OldSword].texture = TextureID::Items;
		data[Item::Type::OldSword].price = 100;
		data[Item::Type::OldSword].textureRect = sf::IntRect(32, 0, 32, 32);

		data[Item::Type::Hat].texture = TextureID::Items;
		data[Item::Type::Hat].price = 50;
		data[Item::Type::Hat].textureRect = sf::IntRect(64, 448, 32, 32);

		data[Item::Type::JeweledBangle].texture = TextureID::Items;
		data[Item::Type::JeweledBangle].price = 1000;
		data[Item::Type::JeweledBangle].textureRect = sf::IntRect(0, 416, 32, 32);
		

		return data;
	}
	
	/***
	set the player values for each element
	***/
	std::map<Player::PlayerType, PlayerData> initializePlayerData()
	{
		std::map<Player::PlayerType, PlayerData> data;

		data[Player::PlayerType::Player].hitpoints = 100;
		data[Player::PlayerType::Player].texture = TextureID::Characters;
		
		JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

		data[Player::PlayerType::Player].animations[Player::State::Up].addFrameSet(frames.getFramesFor("playerUpIdle"));
		data[Player::PlayerType::Player].animations[Player::State::Up].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Up].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::Down].addFrameSet(frames.getFramesFor("playerDownIdle"));
		data[Player::PlayerType::Player].animations[Player::State::Down].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Down].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::Right].addFrameSet(frames.getFramesFor("playerRightIdle"));
		data[Player::PlayerType::Player].animations[Player::State::Right].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Right].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::Left].addFrameSet(frames.getFramesFor("playerLeftIdle"));
		data[Player::PlayerType::Player].animations[Player::State::Left].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::Left].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveUp].addFrameSet(frames.getFramesFor("playerUpMove"));
		data[Player::PlayerType::Player].animations[Player::State::MoveUp].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveUp].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveDown].addFrameSet(frames.getFramesFor("playerDownMove"));
		data[Player::PlayerType::Player].animations[Player::State::MoveDown].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveDown].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveRight].addFrameSet(frames.getFramesFor("playerRightMove"));
		data[Player::PlayerType::Player].animations[Player::State::MoveRight].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveRight].setRepeating(false);

		data[Player::PlayerType::Player].animations[Player::State::MoveLeft].addFrameSet(frames.getFramesFor("playerLeftMove"));
		data[Player::PlayerType::Player].animations[Player::State::MoveLeft].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Player::PlayerType::Player].animations[Player::State::MoveLeft].setRepeating(false);
		

		return data;

	}

	/***
	set the values for shelf
	***/
	std::map<Shelf::ShelfType, ShelfData> initializeShelfData()
	{
		std::map<Shelf::ShelfType, ShelfData> data;

		data[Shelf::ShelfType::Shelf].texture = TextureID::Shelf;
		data[Shelf::ShelfType::Shelf].hitpoints = 10;
		data[Shelf::ShelfType::Shelf].textureRect = sf::IntRect(0, 0, 75, 100);

		return data;
	}

	/***
	set the values for villagers
	***/
	std::map<Villager::Type, VillagerData> initializeVillagerData()
	{
		std::map<Villager::Type, VillagerData> data;

		/*Courtney Villager*/
		data[Villager::Type::Courtney].hitpoints = 100;
		data[Villager::Type::Courtney].texture = TextureID::Villagers;
		
		JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

		data[Villager::Type::Courtney].animations[Villager::State::Up].addFrameSet(frames.getFramesFor("pinkHairUpIdle"));
		data[Villager::Type::Courtney].animations[Villager::State::Up].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Up].setRepeating(false);

		data[Villager::Type::Courtney].animations[Villager::State::Down].addFrameSet(frames.getFramesFor("pinkHairDownIdle"));
		data[Villager::Type::Courtney].animations[Villager::State::Down].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Down].setRepeating(false);

		data[Villager::Type::Courtney].animations[Villager::State::Right].addFrameSet(frames.getFramesFor("pinkHairRightIdle"));
		data[Villager::Type::Courtney].animations[Villager::State::Right].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Right].setRepeating(false);

		data[Villager::Type::Courtney].animations[Villager::State::Left].addFrameSet(frames.getFramesFor("pinkHairLeftIdle"));
		data[Villager::Type::Courtney].animations[Villager::State::Left].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::Left].setRepeating(false);
					
		data[Villager::Type::Courtney].animations[Villager::State::MoveUp].addFrameSet(frames.getFramesFor("pinkHairUpMove"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveUp].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveUp].setRepeating(false);
							
		data[Villager::Type::Courtney].animations[Villager::State::MoveDown].addFrameSet(frames.getFramesFor("pinkHairDownMove"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveDown].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveDown].setRepeating(false);
							
		data[Villager::Type::Courtney].animations[Villager::State::MoveRight].addFrameSet(frames.getFramesFor("pinkHairRightMove"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveRight].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveRight].setRepeating(false);
							
		data[Villager::Type::Courtney].animations[Villager::State::MoveLeft].addFrameSet(frames.getFramesFor("pinkHairLeftMove"));
		data[Villager::Type::Courtney].animations[Villager::State::MoveLeft].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Courtney].animations[Villager::State::MoveLeft].setRepeating(false);


		data[Villager::Type::Courtney].value[Item::Type::BlackCoat] = 500;
		data[Villager::Type::Courtney].value[Item::Type::Bread] = 22;
		data[Villager::Type::Courtney].value[Item::Type::JeweledBangle] = 2000;
		data[Villager::Type::Courtney].value[Item::Type::Hat] = 10;
		data[Villager::Type::Courtney].value[Item::Type::OldSword] = 25;

		data[Villager::Type::Courtney].dialogue.push_back("I love the feeling of riches");
		data[Villager::Type::Courtney].dialogue.push_back("My coat collection could use some improvement/maybe I could use a darker color this time");
		data[Villager::Type::Courtney].dialogue.push_back("Hats are the most worthless thing ever they always/ruin my hair");
		data[Villager::Type::Courtney].dialogue.push_back("I hate seeing low quality weaponry./ Just looking at a weak sword is pitiful/you need something strong to kill your enemies");
		data[Villager::Type::Courtney].dialogue.push_back("I don't just like bread, I knead bread");
		data[Villager::Type::Courtney].dialogue.push_back("I love being surrounded by my wealth and minions");
		data[Villager::Type::Courtney].dialogue.push_back("Praise the great being, the savior of all/Marco's greatness shall enlighten you and put you/ on the path of salvation");
		data[Villager::Type::Courtney].dialogue.push_back("Praise Marco");
		data[Villager::Type::Courtney].dialogue.push_back("Marco is kind, even I can't hate him./He is like a magical hat that will never mess up your/hair. He almost makes me want to be good");


		/*Greg Villager*/
		////////////////////////////////////////////////
		data[Villager::Type::Greg].hitpoints = 100;
		data[Villager::Type::Greg].texture = TextureID::Villagers;
		
		data[Villager::Type::Greg].animations[Villager::State::Up].addFrameSet(frames.getFramesFor("oldManUpIdle"));
		data[Villager::Type::Greg].animations[Villager::State::Up].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::Up].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::Down].addFrameSet(frames.getFramesFor("oldManDownIdle"));
		data[Villager::Type::Greg].animations[Villager::State::Down].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::Down].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::Right].addFrameSet(frames.getFramesFor("oldManRightIdle"));
		data[Villager::Type::Greg].animations[Villager::State::Right].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::Right].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::Left].addFrameSet(frames.getFramesFor("oldManLeftIdle"));
		data[Villager::Type::Greg].animations[Villager::State::Left].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::Left].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::MoveUp].addFrameSet(frames.getFramesFor("oldManUpMove"));
		data[Villager::Type::Greg].animations[Villager::State::MoveUp].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::MoveUp].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::MoveDown].addFrameSet(frames.getFramesFor("oldManDownMove"));
		data[Villager::Type::Greg].animations[Villager::State::MoveDown].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::MoveDown].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::MoveRight].addFrameSet(frames.getFramesFor("oldManRightMove"));
		data[Villager::Type::Greg].animations[Villager::State::MoveRight].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::MoveRight].setRepeating(false);

		data[Villager::Type::Greg].animations[Villager::State::MoveLeft].addFrameSet(frames.getFramesFor("oldManLeftMove"));
		data[Villager::Type::Greg].animations[Villager::State::MoveLeft].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Greg].animations[Villager::State::MoveLeft].setRepeating(false);

		data[Villager::Type::Greg].value[Item::Type::BlackCoat] = 50;
		data[Villager::Type::Greg].value[Item::Type::Bread] = 50;
		data[Villager::Type::Greg].value[Item::Type::JeweledBangle] = 125;
		data[Villager::Type::Greg].value[Item::Type::Hat] = 100;
		data[Villager::Type::Greg].value[Item::Type::OldSword] = 250;

		data[Villager::Type::Greg].dialogue.push_back("I don't need a black coat my current outfit is good/ enough harumph");
		data[Villager::Type::Greg].dialogue.push_back("Jewelry is useless, if I can't eat it, it has no value");
		data[Villager::Type::Greg].dialogue.push_back("fufufu a hat would go well with my scarf");
		data[Villager::Type::Greg].dialogue.push_back("Bread is a must for anyone, it can be used to/make many things.");
		data[Villager::Type::Greg].dialogue.push_back("swords are cool, I was going to be the strongest/adventurer until I took out a loan I couldn't repay");
		data[Villager::Type::Greg].dialogue.push_back("Marcoooooooooooo");
		data[Villager::Type::Greg].dialogue.push_back("Praise Marco, the greatest man around");
		data[Villager::Type::Greg].dialogue.push_back("Do you know of Marco's greatness?");
		


		/*Jhon Villager*/
		///////////////////////////////////////////////////
		data[Villager::Type::Jhon].hitpoints = 100;
		data[Villager::Type::Jhon].texture = TextureID::Villagers;

		data[Villager::Type::Jhon].animations[Villager::State::Up].addFrameSet(frames.getFramesFor("yellowHairUpIdle"));
		data[Villager::Type::Jhon].animations[Villager::State::Up].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::Up].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::Down].addFrameSet(frames.getFramesFor("yellowHairDownIdle"));
		data[Villager::Type::Jhon].animations[Villager::State::Down].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::Down].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::Right].addFrameSet(frames.getFramesFor("yellowHairRightIdle"));
		data[Villager::Type::Jhon].animations[Villager::State::Right].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::Right].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::Left].addFrameSet(frames.getFramesFor("yellowHairLeftIdle"));
		data[Villager::Type::Jhon].animations[Villager::State::Left].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::Left].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::MoveUp].addFrameSet(frames.getFramesFor("yellowHairUpMove"));
		data[Villager::Type::Jhon].animations[Villager::State::MoveUp].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::MoveUp].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::MoveDown].addFrameSet(frames.getFramesFor("yellowHairDownMove"));
		data[Villager::Type::Jhon].animations[Villager::State::MoveDown].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::MoveDown].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::MoveRight].addFrameSet(frames.getFramesFor("yellowHairRightMove"));
		data[Villager::Type::Jhon].animations[Villager::State::MoveRight].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::MoveRight].setRepeating(false);

		data[Villager::Type::Jhon].animations[Villager::State::MoveLeft].addFrameSet(frames.getFramesFor("yellowHairLeftMove"));
		data[Villager::Type::Jhon].animations[Villager::State::MoveLeft].setDuration(sf::Time(sf::seconds(0.75f)));
		data[Villager::Type::Jhon].animations[Villager::State::MoveLeft].setRepeating(false);

		data[Villager::Type::Jhon].value[Item::Type::BlackCoat] = 250;
		data[Villager::Type::Jhon].value[Item::Type::Bread] = 20;
		data[Villager::Type::Jhon].value[Item::Type::JeweledBangle] = 800;
		data[Villager::Type::Jhon].value[Item::Type::Hat] = 100;
		data[Villager::Type::Jhon].value[Item::Type::OldSword] = 150;

		
		data[Villager::Type::Jhon].dialogue.push_back("Bread is an essential food. It has been eaten for /centuries.It is prepared from a dough of flour/and water");
		data[Villager::Type::Jhon].dialogue.push_back("Coats are good for winter and they come in /different colours");
		data[Villager::Type::Jhon].dialogue.push_back("Marco has the experience and the kindness that/makes him unique");
		data[Villager::Type::Jhon].dialogue.push_back("Marco has the answer for all the doubts");
		data[Villager::Type::Jhon].dialogue.push_back("Hats can help keep you warm");
		data[Villager::Type::Jhon].dialogue.push_back("Lavish jewelery is an unnecessary expense");
		data[Villager::Type::Jhon].dialogue.push_back("Be careful of Greg he is crazy");
		data[Villager::Type::Jhon].dialogue.push_back("Courtney seems to be very secretive and has/money that has no source");
		data[Villager::Type::Jhon].dialogue.push_back("Praise Marco");
		

		return data;
	}
}