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

	Villager::Villager(Villager::Type type, GEX::TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, textures_(textures)
		, state_(State::Up)
	{
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

	void Villager::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//updateStates();

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