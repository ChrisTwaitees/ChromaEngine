#include "ParticleSystem.h"
#include "common/PrecompiledHeader.h"
#include "jobsystem/JobSystem.h"
#include "time/Time.h"
#include "render/Render.h"


namespace Chroma
{

	std::vector<ParticleEmitter> ParticleSystem::s_Emitters;

	void ParticleSystem::OnUpdate()
	{
		for (ParticleEmitter& emitter : s_Emitters)
		{
			emitter.transform = glm::translate(glm::mat4(1.0), glm::vec3(glm::sin(Time::GetGameTime()), 0.0, glm::sin(Time::GetGameTime())));
			for (Particle* particle : emitter.particles)
			{
				float deltaTime = Time::GetDeltaTime();
				// reset if particle reached lifespan
				if (particle->timeAlive + deltaTime > particle->lifeSpan)
				{
					particle->position = Math::GetTranslation(emitter.transform);
					particle->timeAlive = 0.0f;
					particle->velocity = emitter.settings.initialVelocity;
				}

				particle->timeAlive += deltaTime;
				// gravity
				//particle.velocity = glm::max(particle.velocity - glm::vec3(0.0f, -9.8f * deltaTime, 0.0f), glm::vec3(0.0f, -9.8f, 0.0f));
				particle->velocity = glm::vec3(0.0f, -9.8f * deltaTime, 0.0f);
				particle->position += particle->velocity;

				glm::mat4 pos;
				Render::GetDebugBuffer()->DrawCross(particle->position, 0.1f, glm::vec3(1.0));
			}

			/*JobSystem::Execute(
				[&emitter]() {
					for (Particle& particle : emitter)
					{
						particle.velocity.y += 9.8f * Time::GetDeltaTime();
						glm::mat4 pos;
						Render::GetDebugBuffer()->DrawOverlayCoordinates(pos, 3.0f);
					}
				}
			);*/
		}
	}

	ParticleEmitter* ParticleSystem::CreateEmitter(const glm::mat4& transform, const ParticleSettings& settings,
		const ParticleTexture& texture)
	{
		srand((unsigned)time(NULL));

		// create and apply new emitter
		ParticleEmitter emitter;
		emitter.transform = transform;
		emitter.settings = settings;
		emitter.texture = texture;
		for (unsigned int i = 0; i < settings.numParticles; ++i)
		{
			Particle* particle = new Particle(); 
			particle->lifeSpan = particle->lifeSpan * ((float)rand()/RAND_MAX);
			particle->velocity = settings.initialVelocity;
			particle->position = Math::GetTranslation(emitter.transform);
			emitter.particles.push_back(particle);
		}

		// add to global emitters
		s_Emitters.push_back(emitter);

		// return instance
		return &emitter;
	}
}