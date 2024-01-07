#include "defs.h"

/*
 Function:   initGhost
  Purpose:   initialize the given ghost
       in:   the type of ghost
   in/out:   the room the ghost starts in
   in/out:   the ghost to be initialized
   in/out:   the building the ghost starts in
   return:   nothing, is void
*/
void initGhost(GhostClassType type, RoomType* thisRoom, GhostType* ghost, BuildingType* building){
	ghost->ghostClass = type;
	addGhostToRoom(ghost, thisRoom);
	ghost->boredomTimer = BOREDOM_MAX;
	addGhostToBuilding(ghost, building);
}

/*
 Function:   makeRandGhost
  Purpose:   generate a random room and a random ghost type and call a function to initialize the ghost
       in:   the ghost
       in:   the building
   return:   nothing, is void
*/
void makeRandGhost(GhostType* ghost, BuildingType* building){
	int randType = randInt(0, 3);
	
	GhostClassType ghostClass;
	
	if(randType == 0){
		ghostClass = POLTERGEIST;
	}
	else if(randType == 1){
		ghostClass = BANSHEE;
	}
	else if(randType == 2){
		ghostClass = BULLIES;
	}
	else{
		ghostClass = PHANTOM;
	}
	
	int randRoom = randInt(0, building->rooms.size-2);
	
	
	RoomNodeType* theRoom = building->rooms.head->next;
	
	for(int i = 0; i < randRoom; i++){
		theRoom = theRoom->next;
	}
	
	
	
	initGhost(ghostClass, theRoom->data, ghost, building);
}

/*
 Function:   addGhostToRoom
  Purpose:   add a ghost to a room
   in/out:   the ghost to be added to the room
   in/out:   the room for the ghost to be added to
   return:   nothing, is void
*/
void addGhostToRoom(GhostType* ghost, RoomType* room){
	sem_wait(&room->mutex);	
	
	room->myGhost = ghost;
	ghost->room = room;
	
	sem_post(&room->mutex);
}

/*
 Function:   removeGhostFromRoom
  Purpose:   remove a ghost from a room
   in/out:   the ghost to be removed from the room
   in/out:   the room for the ghost to be removed from
   return:   nothing, is void
*/
void removeGhostFromRoom(GhostType* ghost, RoomType* room){
	sem_wait(&room->mutex);
	
	room->myGhost = NULL;
	ghost->room = NULL;
	
	sem_post(&room->mutex);
}

/*
 Function:   addGhostToBuilding
  Purpose:   add a ghost to a Building
   in/out:   the ghost to be added to the building
   in/out:   the building for the ghost to be added to
   return:   nothing, is void
*/
void addGhostToBuilding(GhostType* ghost, BuildingType* building){
	building->ghost = ghost;
	ghost->building = building;
}

/*
 Function:   removeGhostFromBuilding
  Purpose:   remove a ghost from a Building
   in/out:   the ghost to be removed from the building
   in/out:   the building for the ghost to be removed from
   return:   nothing, is void
*/
void removeGhostFromBuilding(GhostType* ghost, BuildingType* building){
	building->ghost = NULL;
	ghost->building = NULL;
}

/*
 Function:   chooseGhostAction
  Purpose:   choose the ghost's action and call that function
   in/out:   the ghost whose action is being chosen
   in/out:   the building that the ghost is in
   return:   nothing, is void
*/
void chooseGhostAction(GhostType* ghost, BuildingType* building){
	//Hunter is there
	if(ghost->room->hunterCount > 0){ 
		int choice = randInt(0, 1);
		if(choice == 0){
			generateGhostEvidence(ghost, building);
		}
		else if(choice == 1){
			stayStillGhost(ghost);
		}
	
		ghost->boredomTimer = BOREDOM_MAX;
		
		
	}
	//Hunter isn't there
	else{
		int choice = randInt(0, 2);
		if(choice == 0){
			generateGhostEvidence(ghost, building);
		}
		else if(choice == 1){
			stayStillGhost(ghost);
		}
		else{
			moveRoomGhost(ghost);
		}
		
		ghost->boredomTimer--;
		
		
	}
	
	
	
}


/*
 Function:   generateGhostEvidence
  Purpose:   generate some evidence for the ghost and leave it in the room the ghost is in and the building, tell the user what the ghost is doing
       in:   the ghost who is leaving evidence
   in/out:   the building that the ghost is in
   return:   nothing, is void
*/
void generateGhostEvidence(GhostType* ghost, BuildingType* building){
	int done = 0;
	EvidenceClassType leftEvType;
	float value;
	int randEvType;
	
	while(done == 0){
		 randEvType = randInt(0, 3);
		
		
		if(randEvType == 0){
			leftEvType = EMF;
			value = randFloat(4.70, 5.00);
			
			if(ghost->ghostClass != PHANTOM){
				done = 1;
			
			}
		}
		else if(randEvType == 1){
			leftEvType = TEMPERATURE;
			value = randFloat(-10.00, 1.00);
			
			if(ghost->ghostClass != BULLIES){
				done = 1;
			}
		}
		else if(randEvType == 2){
			leftEvType = FINGERPRINTS;
			value = randInt(1, 1);
			
			if(ghost->ghostClass != BANSHEE){
				done = 1;	
			}
		}
		else{
			leftEvType = SOUND;
			value = randFloat(65.00, 75.00);
			
			if(ghost->ghostClass != POLTERGEIST){
				done = 1;	
			}
		}
		
	}
	
	
	
	if (sem_wait(&ghost->room->mutex) < 0) {
      		printf("semaphore wait error\n");
      		exit(1);
    	}
    	
    	
	EvidenceType* e = calloc(1, sizeof(EvidenceType));
	initEvidence(leftEvType, value, e);
	
	EvidenceNodeType* newEvidenceR = calloc(1, sizeof(EvidenceNodeType));
	newEvidenceR->data = e;
	
	EvidenceNodeType* newEvidenceB = calloc(1, sizeof(EvidenceNodeType));
	newEvidenceB->data = e;
	
	appendEvidence(&ghost->room->roomEvidence, newEvidenceR);
	appendEvidence(&building->allEvidence, newEvidenceB);
	
	if(leftEvType == EMF){
		if(value > 4.9){
			printf("The Ghost has left some GHOSTLY EMF evidence");
		}
		else{
			printf("The Ghost has left some STANDARD EMF evidence");
		}
	}
	else if(leftEvType == TEMPERATURE){		
		if(value < 0){
			printf("The Ghost has left some GHOSTLY TEMPERATURE evidence");
		}
		else{
			printf("The Ghost has left some STANDARD TEMPERATURE evidence");
		}
	}
	else if(leftEvType == FINGERPRINTS){
		if(value == 1){
			printf("The Ghost has left some GHOSTLY FINGERPRINT evidence");
		}
		else{
			printf("The Ghost has left some STANDARD FINGERPRINT evidence");
		}
	}
	else{
		if(value > 70){
			printf("The Ghost has left some GHOSTLY SOUND evidence");
		}
		else{
			printf("The Ghost has left some STANDARD SOUND evidence");
		}
	}
	printf(" in the %s\n", ghost->room->name);
	
	
	if (sem_post(&ghost->room->mutex) < 0) {
      		printf("semaphore post error\n");
      		exit(1);
    	}
    	
    	
    	
	
}

/*
 Function:   stayStillGhost
  Purpose:   do nothing, tell the user what the ghost is doing
       in:   the ghost who is staying still
   return:   nothing, is void
*/
void stayStillGhost(GhostType* ghost){
	//The ghost doesn't do anything lol
	printf("The Ghost stayed still in the %s\n", ghost->room->name);
}


/*
 Function:   moveRoomGhost
  Purpose:   move the ghost to a new adjacent room, tell the user what the ghost is doing
   in/out:   the ghost who is moving rooms
   return:   nothing, is void
*/
void moveRoomGhost(GhostType* ghost){
	int randRoom = randInt(0, ghost->room->connectedRooms.size-1);
	
	RoomNodeType* theRoom = ghost->room->connectedRooms.head;
	
	for(int i = 0; i < randRoom; i++){
		theRoom = theRoom->next;
	}
	

	
	removeGhostFromRoom(ghost, ghost->room);
	addGhostToRoom(ghost, theRoom->data);
	
	printf("The Ghost moved to the %s\n", ghost->room->name);
	

    	
	
}
