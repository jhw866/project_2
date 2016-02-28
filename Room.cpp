#include "Room.h"

Room::Room(Ogre::SceneManager* sceneManager, Simulator *sim) {
	// Build Ground Wall
	
	btScalar mass(0.f);
	btScalar rest(1.f);
	btScalar frict(0.5f);

	walls.push_back(new Wall(Ogre::String("ground"),
					   sceneManager,
					   sim,
					   mass,
					   rest,
					   frict,
					   wallLength,
					   wallLength,
					   Ogre::Real(0),
					   Ogre::Real(0),
					   Ogre::Real(0),
					   Ogre::Real(0),
					   Ogre::Real(0),
					   Ogre::Real(0)));

	walls.push_back(new Wall(Ogre::String("northWall"),
					   sceneManager,
					   sim,
					   mass,
					   rest,
					   frict,
					   wallLength,
					   wallLength,
					   Ogre::Real(0),
					   Ogre::Real(wallLength/2),
					   Ogre::Real(wallLength/2),
					   Ogre::Real(0),
					   Ogre::Real(-90),
					   Ogre::Real(0)));

	walls.push_back(new Wall(Ogre::String("southWall"),
					   sceneManager,
					   sim,
					   mass,
					   rest,
					   frict,
					   wallLength,
					   wallLength,
					   Ogre::Real(0),
					   Ogre::Real(wallLength/2),
					   Ogre::Real(-wallLength/2),
					   Ogre::Real(0),
					   Ogre::Real(90),
					   Ogre::Real(0)));

	walls.push_back(new Wall(Ogre::String("eastWall"),
					   sceneManager,
					   sim,
					   mass,
					   rest,
					   frict,
					   wallLength,
					   wallLength,
					   Ogre::Real(wallLength/2),
					   Ogre::Real(wallLength/2),
					   Ogre::Real(0),
					   Ogre::Real(90),
					   Ogre::Real(0),
					   Ogre::Real(0)));

	walls.push_back(new Wall(Ogre::String("westWall"),
					   sceneManager,
					   sim,
					   mass,
					   rest,
					   frict,
					   wallLength,
					   wallLength,
					   Ogre::Real(-wallLength/2),
					   Ogre::Real(wallLength/2),
					   Ogre::Real(0),
					   Ogre::Real(-90),
					   Ogre::Real(0),
					   Ogre::Real(0)));

	walls.push_back(new Wall(Ogre::String("ceiling"),
					   sceneManager,
					   sim,
					   mass,
					   rest,
					   frict,
					   wallLength,
					   wallLength,
					   Ogre::Real(0),
					   Ogre::Real(wallLength),
					   Ogre::Real(0),
					   Ogre::Real(0),
					   Ogre::Real(180),
					   Ogre::Real(0)));
}

Room::~Room() {

}

/* walls[0] = ground
   walls[1] = northwall
   walls[2] = southwall
   walls[3] = eastwall
   walls[4] = westwall
   walls[5] = ceiling
*/
Ogre::Vector3 Room::checkBoundary(Ogre::SceneNode *node, Ogre::Vector3 move, int index)
{
	Ogre::Vector3 position = node->getPosition();
	Wall* wall = walls.at(index);
	Ogre::Entity* entity = wall->getEntity();
	Ogre::AxisAlignedBox box = entity->getWorldBoundingBox();
	Ogre::Vector3 maxCorner = box.getMaximum();
	Ogre::Real difference = 0;
	int footnode = 0;
	switch(index)
	{
		case 0:
			footnode = 1;
		case 4:
			difference = maxCorner[footnode] - (position[footnode] + move[footnode]);;
			difference = difference < move[footnode]? move[footnode]: difference;
			// clamp
			difference = difference > 0.0? 0.0: difference;
			move[footnode] = difference;
			break;
		case 5:
			footnode = 1;
		case 3:
			difference = maxCorner[footnode] - (position[footnode] + move[footnode]);
			difference = difference > move[footnode]? move[footnode]: difference;
			// clamp
			difference = difference < 0.0? 0.0: difference;
			move[footnode] = difference;
			break;
		default:
			Ogre::LogManager::getSingleton().logMessage ("invalid argument");
	}
	std::cout << move << std::endl;
	return move;

}

