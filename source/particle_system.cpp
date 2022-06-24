#include <vdtgraphics/particle_system.h>

namespace graphics
{
	ParticleSystem::ParticleSystem()
		: position()
		, direction()
		, duration(1.f)
		, initialParticles(0)
		, maxParticles(100)
		, spawnTime(.1f)
		, spawnAmountRange(0, 1)
		, particleColorRange(Color::White, Color::White)
		, particleLifetimeRange(.0f, 1.f)
		, particleSizeRange(1.f, 1.f)
		, particleResizeWithTime(true)
		, particleResizeRange(.1f, .1f)
		, particleSpeedRange(1.f, 1.f)
		, particleTexture()
		, particlePolygonType(PolygonType::circle)
		, particlePolygonStyle(PolygonStyle::fill)
		, m_isPlaying(false)
		, m_lifetime(duration)
		, m_particles()
		, m_spawnTimer(spawnTime)
	{

	}

	void ParticleSystem::update(const float deltaTime)
	{
		if (!m_isPlaying) return;

		m_lifetime -= deltaTime;
		if (m_lifetime <= 0.0f)
		{
			stop();
			return;
		}

		m_spawnTimer -= deltaTime;
		if (m_spawnTimer <= 0.0f)
		{
			int particlesToSpawn = math::random(spawnAmountRange.first, spawnAmountRange.second);
			for (int i = 0; i < particlesToSpawn; ++i)
			{
				spawnParticle();
			}
			m_spawnTimer = spawnTime;
		}

		for (auto it = m_particles.begin(); it != m_particles.end(); )
		{
			Particle& particle = *it;

			particle.lifetime -= deltaTime;
			if (particle.lifetime <= 0.0f)
			{
				it = m_particles.erase(it);
				continue;
			}

			particle.transform.position += particle.direction * particle.speed * deltaTime;
			particle.transform.update();
			++it;
		}
	}

	void ParticleSystem::render(Renderer& renderer)
	{
		for (const Particle& particle : m_particles)
		{
			if (particleTexture)
			{
				renderer.drawTexture(particleTexture.get(), particle.transform.matrix(), particleTextureRect, particle.color);
			}
			else
			{
				PolygonStyle style = renderer.getPolygonStyle();
				renderer.setPolygonStyle(particlePolygonStyle);
				if (particlePolygonType == PolygonType::circle)
				{
					renderer.drawCircle(particle.transform.position, particle.transform.scale.x, particle.color);
				}
				else
				{
					renderer.drawRect(particle.transform.position, particle.transform.scale.x, particle.transform.scale.y, particle.color);
				}
				renderer.setPolygonStyle(style);
			}
		}
	}

	void ParticleSystem::play()
	{
		m_lifetime = duration;
		m_isPlaying = true;
	}

	void ParticleSystem::pause()
	{
		m_isPlaying = false;
	}

	void ParticleSystem::resume()
	{
		m_isPlaying = true;
	}

	void ParticleSystem::stop()
	{
		m_isPlaying = true;
		m_lifetime = 0.0f;
		m_particles.clear();
	}

	void ParticleSystem::spawnParticle()
	{
		if (m_particles.size() + 1 >= maxParticles) return;

		Particle particle;
		particle.color = Color::random(particleColorRange.first, particleColorRange.second);
		particle.direction = direction == math::vec3::zero
			? math::vec3(math::random(-1.f, 1.f), math::random(-1.f, 1.f), 0.f)
			: direction;
		particle.lifetime = math::random(particleLifetimeRange.first, particleLifetimeRange.second);
		particle.speed = math::random(particleSpeedRange.first, particleSpeedRange.second);
		particle.transform.position = position;
		particle.transform.scale.x = particle.transform.scale.y = math::random(particleSizeRange.first, particleSizeRange.second);

		m_particles.push_back(particle);
	}
}