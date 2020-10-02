#ifndef CHROMA_PARTICLE_SYSTEM_H
#define CHROMA_PARTICLE_SYSTEM_H

#include <vector>
#include "glm/glm.hpp"

namespace Chroma
{

	struct Particle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		float lifeSpan{ 3.5f };
		float scale{ 1.0f };
		float timeAlive{ 0.0f };
	};

	template<size_t capacity>
	class ParticleBuffer 
	{
		inline bool push_back(const Particle& particle)
		{
			size_t next = (head + 1) % capacity;
			if (next != tail)
			{
				particles[head] = particle;
				head = next;
				return true;
			}
			return false;
		}

		inline bool pop_front(Particle& particle)
		{
			if (tail != head)
			{
				particle = particles[tail];
				tail = (tail + 1) % capacity;
				return true;
			}
			return false;
		}

		size_t size() const { return sizeof(particles) / sizeof(Particle); }
		Particle& operator[](int index) { return particles[index]; }

	private:
		Particle particles[capacity];
		size_t head = 0;
		size_t tail = 0;
	};

	struct ParticleTexture
	{
		unsigned int texture;
		glm::vec3 colorTint{ 1.0f };
	};

	struct ParticleSettings
	{
		float rate{ 1.0f };
		glm::vec3 initialVelocity{0.0f, 150.0f, 0.0f};
		uint32_t numParticles{ 500 };
	};

	struct ParticleEmitter
	{
		glm::mat4 transform;
		ParticleSettings settings;
		ParticleTexture texture;
		std::vector<Particle*> particles;
		std::vector<Particle*>::iterator begin() { return particles.begin(); }
		std::vector<Particle*>::iterator end() { return particles.end(); }
	};

	class ParticleSystem
	{
	public:
		static void OnUpdate();
		static ParticleEmitter* CreateEmitter(const glm::mat4& transform, const ParticleSettings& settings, const ParticleTexture& texture);

	private:
		static std::vector<ParticleEmitter> s_Emitters;

	};
}

#endif //CHROMA_PARTICLE_SYSTEM_H