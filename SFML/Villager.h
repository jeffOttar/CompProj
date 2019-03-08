#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include "TextureManager.h"
#include "Animation2.h"

namespace GEX {
	class Villager :
		public Entity
	{
	public:
		enum class Type { Courtney, count };

		enum class State { Up, Down, Left, Right, MoveUp, MoveDown, MoveLeft, MoveRight, count };

		Villager(Villager::Type type, GEX::TextureManager& textures);
		~Villager() = default;

		unsigned int							getCategory() const override;
		sf::FloatRect							getBoundingBox() const override;
		void									accelerate(sf::Vector2f velocity) override;
		bool									isMarkedForRemoval() const override;

	private:
		void									updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void									drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void									updateStates();

		bool finishedAnimation() const;

		bool finishedAnimation(State state) const;

		void setState(State state);

	private:
		Type									type_;
		State									state_;
		mutable sf::Sprite						sprite_;
		mutable std::map<State, Animation2>		animations_;
		const TextureManager&					textures_;
	};

}