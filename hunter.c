#include "defs.h"

/*
 Function:   initHunter
  Purpose:   initialize a hunter
   in/out:   the room that the hunter starts in
       in:   the evidence type that the hunter can collect
       in:   the name of the hunter
   in/out:   the hunter to be initialized
   in/out:   the building that the hunter is in
   return:   nothing, is void
*/
void initHunter(RoomType* startRoom, EvidenceClassType specialty, char* name, HunterType* hunter, BuildingType *building){
	addHunterToRoom(hunter, startRoom);
	hunter->mySpecialty = specialty;
	strcpy(hunter->name, name);
	initEvidenceList(&hunter->myEvidence);
	hunter->fear = 0;
	hunter->timer = BOREDOM_MAX;
	hunter->hero = 0;
	hunter->bored = 0;

	addHunterToBuilding(hunter, building);
}


/*
 Function:   addHunterToRoom
  Purpose:   add a hunter to a room
   in/out:   the hunter to be added to the room
   in/out:   the room for the hunter to be added to
   return:   nothing, is void
*/
void addHunterToRoom(HunterType* hunter, RoomType* room){
	sem_wait(&room->mutex);
	
	room->myHunters[room->hunterCount] = hunter;
	room->hunterCount++;
	hunter->room = room;
	
	sem_post(&room->mutex);
}

/*
 Function:   addHunterToBuilding
  Purpose:   add a hunter to a Building
   in/out:   the hunter to be added to the building
   in/out:   the building for the hunter to be added to
   return:   nothing, is void
*/
void addHunterToBuilding(HunterType* hunter, BuildingType* building){
	building->myHunters[building->hunterCount] = hunter;
	building->hunterCount++;
	
	building->allHunters[building->allHunterCount] = hunter;
	building->allHunterCount++;
	
	hunter->building = building;
}

/*
 Function:   removeHunterFromRoom
  Purpose:   remove a hunter from a room
   in/out:   the hunter to be removed from the room
   in/out:   the room for the hunter to be removed from
   return:   nothing, is void
*/
void removeHunterFromRoom(HunterType* hunter, RoomType* room){
	sem_wait(&room->mutex);
	
	int found = 0;
	for(int i = 0; i < room->hunterCount; i++){
		if(found == 1){
			room->myHunters[i-1] = room->myHunters[i];
			room->myHunters[i] = NULL;
		}
		if(room->myHunters[i]->name == hunter->name){
			room->myHunters[i] = NULL;
			found = 1;
		}
	}
	room->hunterCount--;
	hunter->room = NULL;
	
	sem_post(&room->mutex);
}


/*
 Function:   removeHunterFromBuilding
  Purpose:   remove a hunter from a Building
   in/out:   the hunter to be removed from the building
   in/out:   the building for the hunter to be removed from
   return:   nothing, is void
*/
void removeHunterFromBuilding(HunterType* hunter, BuildingType* building){
	int found = 0;
	for(int i = 0; i < building->hunterCount; i++){
		if(found == 1){
			building->myHunters[i-1] = building->myHunters[i];
			building->myHunters[i] = NULL;
		}
		if(building->myHunters[i]->name == hunter->name){
			building->myHunters[i] = NULL;
			found = 1;
		}
	
	}
	building->hunterCount--;
	hunter->building = NULL;
}

/*
 Function:   chooseHunterAction
  Purpose:   choose the hunter's action and call that function
   in/out:   the hunter whose action is being chosen
   in/out:   the building that the hunter is in
   return:   nothing, is void
*/
void chooseHunterAction(HunterType* hunter, BuildingType* building){
	if(hunter->room->hunterCount > 1){
		int choice = randInt(0, 2);
		if(choice == 0){
			readEvidenceHunter(hunter, building);
			hunter->timer--;
		}
		else if(choice == 1){
			communicateHunters(hunter);
			hunter->timer--;
		}
		else{
			moveRoomHunter(hunter);
			hunter->timer--;
		}
	}
	else{
		int choice = randInt(0, 1);
		if(choice == 0){
			readEvidenceHunter(hunter, building);
			hunter->timer--;
		}
		else{
			moveRoomHunter(hunter);
			hunter->timer--;
		}
	}
	
	if(hunter->room->myGhost != NULL){
		hunter->fear+= FEAR_RATE;
		hunter->timer = BOREDOM_MAX;
	}
	
	
}

/*
 Function:   readEvidenceHunter, tell the user what the hunter is doing
  Purpose:   if there is any evidence in the room that the hunter can read, then read it, if not then generate some standard evidence and read it
       in:   the hunter who is reading evidence
   in/out:   the building that the hunter is in
   return:   nothing, is void
*/
void readEvidenceHunter(HunterType* hunter, BuildingType* building){

	int a = findIfEvTypeThere(&hunter->room->roomEvidence, hunter->mySpecialty);
	float value;
	EvidenceClassType type;
	
	if(a == -1){
		if(hunter->mySpecialty == EMF){
			type = EMF;
			value = randFloat(0, 4.9);
			printf("%s found some RANDOM EMF evidence\n", hunter->name);
		}
		else if(hunter->mySpecialty == TEMPERATURE){
			type = TEMPERATURE;
			value = randFloat(0, 27);
			printf("%s found some RANDOM TEMPERATURE evidence\n", hunter->name);
		}
		else if(hunter->mySpecialty == FINGERPRINTS){
			type = FINGERPRINTS;
			value = randInt(0, 0);
			printf("%s found some RANDOM FINGERPRINTS evidence\n", hunter->name);
		}
		else{
			type = SOUND;
			value = randFloat(40, 70);
			printf("%s found some RANDOM SOUND evidence\n", hunter->name);
		}
		
		EvidenceType* e = calloc(1, sizeof(EvidenceType));
		initEvidence(type, value, e);
		
		EvidenceNodeType* newEvidenceH = calloc(1, sizeof(EvidenceNodeType));
		newEvidenceH->data = e;
		
		EvidenceNodeType* newEvidenceB = calloc(1, sizeof(EvidenceNodeType));
		newEvidenceB->data = e;
		
		appendEvidence(&hunter->myEvidence, newEvidenceH);
		appendEvidence(&building->allEvidence, newEvidenceB);
		

	}
	else{
	
		
		if (sem_wait(&hunter->room->mutex) < 0) {
      			printf("semaphore wait error\n");
      			exit(1);
    		}
    		
		
		float eValue = returnValueAtI(&hunter->room->roomEvidence, a);
		EvidenceType* e = calloc(1, sizeof(EvidenceType));
		initEvidence(hunter->mySpecialty, eValue, e);
		
		EvidenceNodeType* movedEvidenceH = calloc(1, sizeof(EvidenceNodeType));
		movedEvidenceH->data = e;
		
		EvidenceNodeType* movedEvidenceB = calloc(1, sizeof(EvidenceNodeType));
		movedEvidenceB->data = e;
		
		
		appendEvidence(&hunter->myEvidence, movedEvidenceH);
		removeEvidence(&hunter->room->roomEvidence, a);
		
		appendEvidence(&building->allEvidence, movedEvidenceB);
		
		if(checkForGhostlyEvidence(movedEvidenceB) == 1){
			hunter->timer = BOREDOM_MAX;
		}
		
		//printf("value: %.2f\n", e->value);
		
		if(hunter->mySpecialty == EMF){
			printf("%s found some UNKNOWN EMF evidence\n", hunter->name);
		}
		else if(hunter->mySpecialty == TEMPERATURE){
			printf("%s found some UNKNOWN TEMPERATURE evidence\n", hunter->name);
		}
		else if(hunter->mySpecialty == FINGERPRINTS){
			printf("%s found some UNKNOWN FINGERPRINTS evidence\n", hunter->name);
		}
		else{
			printf("%s found some UNKNOWN SOUND evidence\n", hunter->name);
		}
		
		
		if (sem_post(&hunter->room->mutex) < 0) {
      			printf("semaphore post error\n");
      			exit(1);
    		}
    		

		
		
	}
	
	
}

/*
 Function:   communicateHunters
  Purpose:   communicate by appending new ghostly evidence from the given hunter's evidence list into another random hunter in the room's evidence list, tell the user what the hunter is doing
       in:   the hunter who is sharing their evidence
   return:   nothing, is void
*/
void communicateHunters(HunterType* hunter){
	HunterType *comHunter = hunter;
	int randHunter;
	
	
	while(comHunter == hunter){
		randHunter = randInt(0, hunter->room->hunterCount-1);
		comHunter = hunter->room->myHunters[randHunter];
	}
	
	EvidenceNodeType* curr = hunter->myEvidence.head;
	
	while(curr != NULL){
		if(checkForDuplicateEvidence(&comHunter->myEvidence, curr) == 0 && checkForGhostlyEvidence(curr) == 1){
			EvidenceNodeType* eNode = calloc(1, sizeof(EvidenceNodeType));
			eNode->data = curr->data;
			appendEvidence(&comHunter->myEvidence, eNode);
			
			//printf("val:%.2f\n", curr->data->value);
			
			
		}
		curr = curr->next;
	}
	
	printf("%s communicated with %s\n", hunter->name, comHunter->name);
}


/*
 Function:   moveRoomHunter
  Purpose:   move the hunter to a new adjacent room, tell the user what the hunter is doing
   in/out:   the hunter who is moving rooms
   return:   nothing, is void
*/
void moveRoomHunter(HunterType* hunter){
	int randRoom = randInt(0, hunter->room->connectedRooms.size-1);
	
	RoomNodeType* theRoom = hunter->room->connectedRooms.head;
	
	for(int i = 0; i < randRoom; i++){
		theRoom = theRoom->next;
	}
	
	
	removeHunterFromRoom(hunter, hunter->room);
	addHunterToRoom(hunter, theRoom->data);
	
	
	printf("%s moved to the %s\n", hunter->name, hunter->room->name);



    	

}

/*
 Function:   found3GhostlyEvidence
  Purpose:   change variables to represent which types of ghostly evidence were found
      out:   pointer to value that represents if ghostly EMF value is there
      out:   pointer to value that represents if ghostly TEMPERATURE value is there
      out:   pointer to value that represents if ghostly FINGERPRINTS value is there
      out:   pointer to value that represents if ghostly SOUND value is there
       in:   the hunter that whose evidence we are checking
   return:   nothing, is void 
*/
void found3GhostlyEvidence(int *EMFthere, int *TEMPERATUREthere, int *FINGERPRINTSthere, int *SOUNDthere, HunterType* survivor){
	EvidenceNodeType* curr = survivor->myEvidence.head;
    	
    	while(curr != NULL){
    		if(checkForGhostlyEvidence(curr) == 1){
    			if(curr->data->type == EMF){
    				*EMFthere = 1;
    			}
    			else if(curr->data->type == TEMPERATURE){
    				*TEMPERATUREthere = 1;
    			}
    			else if(curr->data->type == FINGERPRINTS){
    				*FINGERPRINTSthere = 1;
    			}
    			else{
    				*SOUNDthere = 1;
    			}
    		}
    		curr = curr->next;
    	}
}
