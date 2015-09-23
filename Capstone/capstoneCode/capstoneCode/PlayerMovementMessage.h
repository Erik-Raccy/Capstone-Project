#pragma once

/*
* These messages are constructed by the server in the player controller classes and
* are sent to all clients to send all node positional data (not used atm)
*/

#include "C4Messages.h" // needed for sending messages between classes + multiplayer
#include "C4Character.h"
#include "C4Physics.h"
#include "C4World.h"

//My headers
#include "Game.h"
#include "MyPlayerController.h"

namespace C4
{
	class NodeStateMessage : public ControllerMessage
	{
		friend class CharacterController;

	private:
		Point3D		initialPosition;
		Vector3D	initialVelocity;

	public:
		//Constructors
		NodeStateMessage(ControllerMessageType type, const int32 controllerIndex);
		NodeStateMessage(ControllerMessageType type, const int32 controllerIndex, const Point3D& position, const Vector3D& velocity);
		~NodeStateMessage();

		//Accessors
		const Point3D& GetInitialPosition(void) const
		{
			return (initialPosition);
		}

		const Vector3D& GetInitialVelocity(void) const
		{
			return (initialVelocity);
		}

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};


	//Class contains all the movement information for character controllers, sent from server to clients
	class PlayerMovementMessage : public NodeStateMessage
	{
		friend class MyPlayerController;
		friend class MyRespawnController;

	private:
		unsigned_int32	movementFlag;
		//Constructor
		PlayerMovementMessage(ControllerMessageType type, const int32 controllerIndex);

	public:
		//Constructor
		PlayerMovementMessage(ControllerMessageType type, const int32 controllerIndex, const Point3D& position, const Vector3D& velocity, unsigned_int32 flag);
		~PlayerMovementMessage();

		//Accessor
		unsigned_int32 GetMovementFlag(void) const
		{
			return (movementFlag);
		}

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};

	//Class contains all the movement information for AI
	class AIMovementMessage : public NodeStateMessage
	{
		friend class MyAIController;

	private:
		//Constructor
		AIMovementMessage(ControllerMessageType type, const int32 controllerIndex);

	public:
		//Constructor
		AIMovementMessage(ControllerMessageType type, const int32 controllerIndex, const Point3D& position, const Vector3D& velocity);
		~AIMovementMessage();

		//Methods
		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};
}