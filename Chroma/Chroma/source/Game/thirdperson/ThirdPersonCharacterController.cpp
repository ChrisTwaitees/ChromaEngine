#include "ThirdPersonCharacterController.h"
#include <input/Input.h>
#include <entity/Entity.h>
#include <render/Render.h>

namespace Chroma
{
	void ThirdPersonCharacterController::ProcessPreviousFrame()
	{
		// VELOCITY
		m_Velocity = m_Position - m_PreviousPosition;
		m_PreviousPosition = m_Position;
		m_CurrentSpeed = glm::length(m_Velocity);

	}

	void ThirdPersonCharacterController::ProcessCollisions()
	{
		// clear collision force from previous frame
		m_CollisionForce = glm::vec3(0.0);

		// normalize collision data
		glm::vec3 averageColAPos = glm::vec3(0.0);
		glm::vec3 averageColBPos = glm::vec3(0.0);
		glm::vec3 averageColBNormal = glm::vec3(0.0);
		float numCollisions = (float)GetPhysicsComponent()->GetRigidBodyCollisionData().size();

		// Collect Collision Data
		for (CollisionData const& colData : GetPhysicsComponent()->GetRigidBodyCollisionData())
		{
			// Add valid collision data
			if (glm::dot(-m_GravityDirection, colData.m_ColliderBContactNormal) < 0.1) // filter out ground collisions - angle depended on max slope and collision normal
			{
				// Mask dynamic objects
				if (colData.m_ColliderBState == Dynamic) 
					continue;

				// Collect collisions
				averageColAPos += colData.m_ColliderAContactPoint;
				averageColBPos += colData.m_ColliderBContactPoint;
				averageColBNormal += colData.m_ColliderBContactNormal;
			
				// Debug
				// positions
				Chroma::Render::GetDebugBuffer()->DrawOverlayLine(colData.m_ColliderAContactPoint, colData.m_ColliderBContactPoint, glm::vec3(1.0, 0.0, 0.5));
				// normals
				Chroma::Render::GetDebugBuffer()->DrawOverlayLine(colData.m_ColliderBContactPoint, colData.m_ColliderBContactPoint + colData.m_ColliderBContactNormal, glm::vec3(1.0, 0.0, 0.5));
			}
		}
	
		// Add collision force
		if (numCollisions > 0.0) // check collisions occurred
		{
			averageColAPos /= glm::vec3(numCollisions);
			averageColBPos /= glm::vec3(numCollisions);
			averageColBNormal /= glm::vec3(numCollisions);
			m_CollisionForce += glm::length(averageColAPos - averageColBPos) * averageColBNormal;
		}

		// GROUND Check
		glm::vec3 rayStart = m_Position + ( - glm::normalize(m_GravityDirection) * GetParentEntity()->GetHeight()/glm::vec3(2.0));
		glm::vec3 rayEnd = m_Position ;
		RayHitData groundRay = Chroma::Physics::GetRayHitData(rayStart, rayEnd);
		m_IsOnGround = groundRay.m_Hit;

		// Debug
		if (m_IsOnGround)
		{
			// shift up to ground pos
			m_CollisionForce += groundRay.m_RayHitPosition - m_Position;
			// hitpos
			//Chroma::Render::GetDebugBuffer()->DrawOverlayLine(rayStart, rayEnd, glm::vec3(0.0, 1.0, 0.0));
			// normal
			//Chroma::Render::GetDebugBuffer()->DrawOverlayLine( groundRay.m_RayHitPosition, groundRay.m_RayHitPosition + groundRay.m_RayHitNormal, glm::vec3(0.0, 1.0, 0.0));
		}

		// GRAVITY
		CalculateGravity();
	}

	void ThirdPersonCharacterController::CalculateGravity()
	{
		// Apply Gravity
		if (!m_IsOnGround) // if in air, apply gravity
		{
			// using dot product check if gravity has reached max
			float gravitySpeed = (float)glm::max(glm::dot(m_GravityDirection, glm::normalize(m_GravityForce)), 0.0f);
			gravitySpeed = gravitySpeed > 0.0f ? gravitySpeed * glm::length(m_GravityForce) : 0.0f; // weird hack since float kept returning nan(-ind)		
			if (gravitySpeed < m_GravityMax)
			{
				m_GravityCurrent += 0.5f * m_GravityMax * glm::pow(DELTATIME, 2.0) * m_GravityStrength;
				m_GravityForce += m_GravityDirection * glm::vec3(m_GravityCurrent);
			}
		}
		else
		{
			m_GravityCurrent = 0.0;
			m_GravityForce = glm::vec3(0.0f);
		}
	}

	void ThirdPersonCharacterController::ProcessMovement()
	{
		// JUMPING
		ProcessJump();

		// 0 out Controller Force
		m_ControllerForce = glm::vec3(0.0);

		// AIM ANGLE
		m_CamFacingAngle = Chroma::Math::GetFacingAngleEuler(m_Position, m_CamPosition);

		glm::vec3 toPlayer = glm::normalize(m_Position - m_CamPosition);
		glm::vec3 sidePlayer = glm::normalize(glm::cross(toPlayer, -m_GravityDirection));

		// removing y and normalizing
		toPlayer.y = 0.0f;
		sidePlayer.y = 0.0f;
		normalize(toPlayer);
		normalize(sidePlayer);

		// Set Heading
		if (glm::abs(Chroma::Input::GetAxis("Horizontal")) > m_InputMin || glm::abs(Chroma::Input::GetAxis("Vertical")) > m_InputMin)
		{
			glm::vec3 playerHeadingUnClamped = (sidePlayer * Chroma::Input::GetAxis("Horizontal")) + (toPlayer * -Chroma::Input::GetAxis("Vertical"));
			m_PlayerHeading = glm::normalize(playerHeadingUnClamped);
			// TODO: Implement Event System Hook up
			/*if (Chroma::Input::IsPressed(Chroma::Input::L3))
				m_ControllerForce += m_PlayerHeading * glm::min(glm::length(playerHeadingUnClamped), 0.75f) * m_WalkSpeed * m_SprintSpeedMultiplier * glm::vec3(DELTATIME);
			else
				m_ControllerForce += m_PlayerHeading * glm::min(glm::length(playerHeadingUnClamped), 0.75f) * m_WalkSpeed * glm::vec3(DELTATIME);*/
		}

		// Orientation
		float degreesRotated = Chroma::Math::DegreesBetweenVectors2D(CHROMA_FORWARD, m_PlayerHeading);
		m_Rotation = glm::angleAxis(-degreesRotated, CHROMA_UP);

		// Add FORCES
		m_Position += m_CollisionForce + m_GravityForce + m_ControllerForce;

		// prevent falling into the abyss
		m_Position.y = glm::max(m_Position.y, .0f);

		// DEBUG
		// Forces
		Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_GravityForce, glm::vec3(0.227, 0.466, 0.992));
		Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position + -m_GravityDirection, m_Position + (m_ControllerForce * glm::vec3(8.0)) + -m_GravityDirection, glm::vec3(0.992, 0.666, 0.227));
		Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position + -(m_GravityDirection * glm::vec3(2.0)), m_Position + (m_CollisionForce * glm::vec3(2.0)) + -(m_GravityDirection * glm::vec3(2.0)), glm::vec3(0.725, 0.074, 0.701));
		// Player Heading
		Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_PlayerHeading, glm::vec3(0.1, 0.9, 0.8));
	}

	void ThirdPersonCharacterController::ProcessIKs()
	{

	}

	void ThirdPersonCharacterController::ProcessJump()
	{
		// JUMPING
		// Calculate jump vector
		glm::vec3 rotateJumpVectorAxis = glm::normalize(glm::cross(CHROMA_UP, m_PlayerHeading));
		glm::quat rotateJumpVector = glm::angleAxis(m_CurrentSpeed * m_JumpHeadingBias, rotateJumpVectorAxis);
		m_JumpVector = rotateJumpVector * m_JumpVectorStationary;


		// TODO: Implement Event System Hookup
		// Double Jump : if jumped once
		//if (!m_IsOnGround && !m_HasDoubleJumped && ((Chroma::Input::IsPressed(Chroma::Input::CROSS) || Chroma::Input::IsPressed(Chroma::Input::SPACEBAR))))
		//{
		//	CHROMA_INFO("Double Jump triggered!");
		//	float jumpIntertia = Chroma::Math::InertiaForHeight(m_GravityMax, m_JumpHeight) * DELTATIME;
		//	m_GravityForce += m_JumpVector * glm::vec3(jumpIntertia);
		//	m_HasDoubleJumped = true;
		//}

		//// Jump : if on ground and button pressed
		//if (m_IsOnGround && (Chroma::Input::IsPressed(Chroma::Input::CROSS) || Chroma::Input::IsPressed(Chroma::Input::SPACEBAR)))
		//{
		//	CHROMA_INFO("Jump triggered!");
		//	float jumpIntertia = Chroma::Math::InertiaForHeight(m_GravityMax, m_JumpHeight) * DELTATIME;
		//	m_GravityForce += m_JumpVector * glm::vec3(jumpIntertia);
		//	m_HasDoubleJumped = false;
		//}


		// JumpVector
		//Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + (m_JumpVector * glm::vec3(m_JumpHeight)) + glm::vec3(0.1, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0));
	}

	void ThirdPersonCharacterController::ProcessCamera()
	{
		glm::vec3 toCam = glm::normalize(m_CamPosition - m_Position);
		// POSITION
		// check if camera within radius 
		if (glm::length(m_Position - m_CamPosition) > m_FollowDistMax)
		{
			// pull back within follow radius
			m_CamPosition = m_Position + (toCam * m_FollowDistMax);
		}
		else if (glm::length(m_Position - m_CamPosition) < m_FollowDistMin)
		{
			// push back within follow radius
			m_CamPosition = m_Position + (toCam * m_FollowDistMin);
		}
		else // follow movement
		{
			m_CamPosition += m_Velocity * m_CamDrag;
		}
		// ROTATION
		glm::vec3 horizontalAxis = -m_GravityDirection;
		glm::vec3 verticalAxis = glm::normalize(glm::cross(toCam, horizontalAxis));

		// vertical rotation
		if (glm::abs(Chroma::Input::GetControllerRightHorizontal()) > m_InputMin)
		{
				glm::quat horizontalRotation = glm::angleAxis((float)(Chroma::Input::GetControllerRightHorizontal() * m_CamRotationSpeed * Chroma::Time::GetDeltaTime()), horizontalAxis);
				m_CamPosition = Chroma::Math::RotateAroundPivot(m_CamPosition, m_Position, horizontalRotation);
		}
		// horizontal rotation
		if (glm::abs(Chroma::Input::GetControllerRightVertical()) > m_InputMin)
		{
			glm::quat verticalRotation = glm::angleAxis((float)(Chroma::Input::GetControllerRightVertical() * m_CamRotationSpeed * Chroma::Time::GetDeltaTime()), verticalAxis);
			glm::vec3 nextVerticalPosition = Chroma::Math::RotateAroundPivot(m_CamPosition, m_Position, verticalRotation);
			if (glm::abs(Chroma::Math::GetFacingAngleEuler(m_Position, nextVerticalPosition)) < m_CamRotMax)
				m_CamPosition = nextVerticalPosition;
		}

		// DIRECTION - Aim At Player
		m_CamLookAtPos = (m_Position + (GetParentEntity()->GetHeight() * 0.8f * -m_GravityDirection));

		m_CamDirection = glm::normalize(m_CamLookAtPos - m_CamPosition);
		// UP
		m_CamUp = -m_GravityDirection;


	}


	void ThirdPersonCharacterController::ProcessTransforms()
	{
		// Calculate and apply transform to parent entity
		CalculateTransform();
		GetParentEntity()->SetTransform(m_Transform);
		// Apply transforms to camera controller
		m_CameraController->m_CamPos = m_CamPosition;
		m_CameraController->m_CamDir = m_CamDirection;
		m_CameraController->m_CamUp = m_CamUp;
		// Debug
		//Chroma::Render::GetDebugBuffer()->DrawOverlayCoordinates(m_Transform, 5.0f);
	}

	void ThirdPersonCharacterController::ProcessInput()
	{
		// camera context switching
		// switch between editor cam and player cam
		// TODO: Implement Event System Hook up 
		//if (Chroma::Input::IsPressed(Chroma::Input::R1) && Chroma::Input::IsPressed(Chroma::Input::L1)) 
		//{
		//	CHROMA_WARN("Switching Camera Context...");
		//	Chroma::Input::ToggleCursorEnabled();
		//	Chroma::Scene::GetRenderCamera()->SetCameraMode(CameraMode::Custom);
		//}

		//if (Chroma::Input::IsPressed(Chroma::Input::R3) && Chroma::Input::IsPressed(Chroma::Input::L3))
		//{
		//	CHROMA_WARN("Toggling Physics Rendering");
		//	Chroma::Physics::ToggleDrawDebug();
		//}
	}

	void ThirdPersonCharacterController::Update()
	{
		CHROMA_INFO_UNDERLINE;

		// Calculate from Previous Frame
		ProcessPreviousFrame();

		// Input
		ProcessInput();

		// Collisions
		ProcessCollisions();

		// Movement
		ProcessMovement();

		// Camera
		ProcessCamera();

		// Transfroms
		ProcessTransforms();

		CHROMA_INFO_UNDERLINE;
	}
}
