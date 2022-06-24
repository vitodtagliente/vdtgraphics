/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <list>

#include <vdtmath/transform.h>

#include "color.h"
#include "renderer.h"
#include "texture.h"
#include "texture_rect.h"

namespace graphics
{
	class ParticleSystem
	{
	public:

		struct Particle
		{
			math::transform transform;
			math::vec3 direction;
			float lifetime;
			float speed;
			Color color;
			float resizeRatio;
		};

		ParticleSystem();

		math::vec3 position;
		math::vec3 direction;
		float duration;
		int initialParticles;
		int maxParticles;
		float spawnTime;
		std::pair<int, int> spawnAmountRange;
		std::pair<Color, Color> particleColorRange;
		std::pair<float, float> particleLifetimeRange;
		std::pair<float, float> particleSizeRange;
		bool particleResizeWithTime;
		std::pair<float, float> particleResizeRange;
		std::pair<float, float> particleSpeedRange;
		TexturePtr particleTexture;
		TextureRect particleTextureRect;
		PolygonType particlePolygonType;
		PolygonStyle particlePolygonStyle;

		void update(float deltaTime);
		void render(Renderer& renderer);

		void play();
		void pause();
		void resume();
		void stop();

		bool isPlaying() const { return m_isPlaying; }

	private:
		void spawnParticle();

		bool m_isPlaying;
		float m_lifetime;
		std::list<Particle> m_particles;
		float m_spawnTimer;
	};
}