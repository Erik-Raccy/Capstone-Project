#include "MyCamera.h"

using namespace C4;

//Smart camera that zooms in and out
MyCamera::MyCamera() : FrustumCamera(2.0f, 1.0f)
{
	cameraZ =5;
}

MyCamera::~MyCamera()
{
}

void MyCamera::Preprocess(void)
{
	FrustumCamera::Preprocess();
}

//When the player uses the mouse to rotate camera

void MyCamera::Move(void)
{
	GamePlayer *player = static_cast<GamePlayer*>(TheMessageMgr->GetLocalPlayer());
	if (player) //If local player
	{
		MyPlayerController* controller = static_cast<MyPlayerController*>(player->GetController());
		if (controller)
		{
			Model *model = controller->GetTargetNode(); 
			if (model!=nullptr) //If the model exists
			{
				CollisionData	data;

				//Get the player's information
				const Point3D& position = model->GetWorldPosition();
				Vector3D forwardVector = model->GetNodeTransform()[0];
			
				//Get the camera's position
				Point3D p2 = Point3D(position.x - 10 * forwardVector.x +3*forwardVector.y, position.y - 10 * forwardVector.y -3*forwardVector.x, position.z + cameraZ);
				
				//Get the point directly behind the player
				Point3D p3 = Point3D(position.x -forwardVector.x, position.y - forwardVector.y, position.z +2);

				//If there is something in the way 
				if (GetWorld()->DetectCollision(p3, p2, 0.3F, kCollisionCamera, &data))
				{
					//Move camera closer to the palyer
					float t = data.param;
					p2 = position * (1.0F - t) + p2 * t;
				}
				
				//`Restrict the height of the camera to be between 2<=cameraHeight<=25
				float pitch = TheInputMgr->GetMouseDeltaY();
				cameraZ += pitch;
				if (cameraZ < 2)	cameraZ = 2;
				if (cameraZ > 25)	cameraZ = 25;
	
				// Set the camera's position and orientation.
				SetNodePosition(Point3D(p2.x,p2.y,p2.z));
				LookAtPoint(Point3D(position.x+3*forwardVector.y, position.y-3*forwardVector.x, position.z +2));
			}
		}
	}

}
