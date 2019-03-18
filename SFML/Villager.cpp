#include "Villager.h"
#include "DataTables.h"
#include "Category.h"
#include "Utility.h"

namespace GEX {
	namespace
	{
		//const is all uppercase and this is for all the aircraft data (it had to be initialized when declared because its const
		const std::map<Villager::Type, VillagerData> TABLE = initializeVillagerData();
	}
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 100);
	auto randomNumber = std::bind(distribution, generator);

	Villager::Villager(Villager::Type type, GEX::TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, textures_(textures)
		, state_(State::Left)
		, _leaveTime(sf::seconds(300))
		, _buyTime(defaultBuyTime)
		, _moveTime(defaultMoveTime)
		, _buying(false)
	{ 
		sprite_.setTexture(textures.get(TABLE.at(type).texture));
		sprite_.scale(5.f, 5.f);
		for (auto a : TABLE.at(type).animations)
		{
			animations_[a.first] = a.second;
		}
		sprite_.setTextureRect(sf::IntRect());
		centerOrigin(sprite_);
	}	  

	unsigned int Villager::getCategory() const
	{
		switch (type_)
		{
		case Type::Courtney:
			return Category::Courtney;
			break;

		/*case ObstacleType::RaceCar:
			return Category::RaceCar;
			break;*/

		}
	}

	sf::FloatRect Villager::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
		box.width -= 35; // tighten up bounding box for more realistic collisions
		box.left += 10;
		box.height -= 25;
		box.top += 20;
		return box;
	}

	void Villager::accelerate(sf::Vector2f velocity)
	{
	}

	bool Villager::isMarkedForRemoval() const
	{
		return isDestroyed() && finishedAnimation();
	}

	bool Villager::isBuying()
	{
		return _buying;
	}

	void Villager::checkBuy()//random check to set variable
	{
		if (randomNumber() <= 85)
		{
			_buying = true;
		}
		else if (randomNumber() > 85)
		{
			_buying = false;
		}
	}

	sf::Vector2f Villager::randomMove()//random direction movement
	{
		if (randomNumber() <= 30)
		{
			return sf::Vector2f(-5,0);
		}
		else if (randomNumber() > 30 && randomNumber() < 50)
		{
			return sf::Vector2f(5, 0);
		}
		else if (randomNumber() >= 55 && randomNumber() < 75)
		{
			return sf::Vector2f(0, 5);
		}
		else if (randomNumber() >= 75 && randomNumber() < 95)
		{
			return sf::Vector2f(0, -5);
		}
		else
		{
			return sf::Vector2f();
		}

	}

	Villager::Type Villager::getType()
	{
		return type_;
	}

	void Villager::decrementBuyTime(sf::Time time)
	{
		_buyTime -= time;
	}

	sf::Time Villager::getBuyTime()
	{
		return _buyTime;
	}

	void Villager::decrementMoveTime(sf::Time time)
	{
		_moveTime -= time;
	}

	sf::Time Villager::getMoveTime()
	{
		return _moveTime;
	}

	void Villager::decrementLeaveTime(sf::Time time)
	{
		_leaveTime -= time;
	}

	sf::Time Villager::getLeaveTime()
	{
		return _leaveTime;
	}

	void Villager::resetBuyTime()
	{
		_buyTime = defaultBuyTime;
	}

	void Villager::resetMoveTime()
	{
		_moveTime = defaultMoveTime;
	}

	void Villager::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		updateStates();

		auto rec = animations_.at(state_).update(dt);

		sprite_.setTextureRect(rec);

		centerOrigin(sprite_);

		Entity::updateCurrent(dt, commands);
	}

	void Villager::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	void Villager::updateStates()
	{
		if (state_ == State::MoveUp && animations_[state_].isFinished())
			setState(State::Up);
		else if (state_ == State::MoveDown && animations_[state_].isFinished())
			setState(State::Down);
		else if (state_ == State::MoveLeft && animations_[state_].isFinished())
			setState(State::Left);
		else if (state_ == State::MoveRight && animations_[state_].isFinished())
			setState(State::Right);
	}

	bool Villager::finishedAnimation() const
	{
		return animations_[state_].isFinished();
	}

	bool Villager::finishedAnimation(State state) const
	{
		if (state_ == state) {
			return animations_[state_].isFinished();
		}

		return false;
	}

	void Villager::setState(State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

}