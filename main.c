#include "defs.h"

/*
 Function:   main
  Purpose:   Run main control flow
   return:   0 for success
*/
int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));
    
    
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);
    
    GhostType ghost;
    makeRandGhost(&ghost, &building);
    
    
    char hunter1Name[MAX_STR] = "a";
    char hunter2Name[MAX_STR] = "a";
    char hunter3Name[MAX_STR] = "a";
    char hunter4Name[MAX_STR] = "a";
    
    while(strcmp(hunter1Name, hunter2Name) == 0 || strcmp(hunter1Name, hunter3Name) == 0 || strcmp(hunter1Name, hunter4Name) == 0|| strcmp(hunter2Name, hunter3Name) == 0 || strcmp(hunter2Name, hunter4Name) == 0 || strcmp(hunter3Name, hunter4Name) == 0){
    	printf("The hunter names must all be different\n");
    	printf("What is the name of hunter 1:");
    	scanf("%s", hunter1Name);
    	printf("What is the name of hunter 2:");
    	scanf("%s", hunter2Name);
    	printf("What is the name of hunter 3:");
    	scanf("%s", hunter3Name);
    	printf("What is the name of hunter 4:");
    	scanf("%s", hunter4Name);
    }
    
    
    
    HunterType hunter1;
    HunterType hunter2;
    HunterType hunter3;
    HunterType hunter4;
    
    initHunter(building.rooms.head->data, EMF, hunter1Name, &hunter1, &building);
    initHunter(building.rooms.head->data, TEMPERATURE, hunter2Name, &hunter2, &building);
    initHunter(building.rooms.head->data, FINGERPRINTS, hunter3Name, &hunter3, &building);
    initHunter(building.rooms.head->data, SOUND, hunter4Name, &hunter4, &building);
    
    pthread_t gt, ht1, ht2, ht3, ht4; 
    
    pthread_create(&gt, NULL, ghostThread, &ghost);
    pthread_create(&ht1, NULL, hunterThread, &hunter1);
    pthread_create(&ht2, NULL, hunterThread, &hunter2);
    pthread_create(&ht3, NULL, hunterThread, &hunter3);
    pthread_create(&ht4, NULL, hunterThread, &hunter4);
    
    pthread_join(gt, NULL);
    pthread_join(ht1, NULL);
    pthread_join(ht2, NULL);
    pthread_join(ht3, NULL);
    pthread_join(ht4, NULL);
    
    
    
    printResults(&building, &ghost);
    

    
    freeBuilding(&building);


    return 0;
}


/*
 Function:   randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

/*
 Function:   randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}


/*
 Function:   ghostThread
  Purpose:   run the ghost thread
   in/out:   void pointer to our ghost
   return:   nothing, is void
*/
void *ghostThread(void *ghosty){
	GhostType* ghost = ((GhostType*) ghosty);
	while(ghost->boredomTimer > 0){
		chooseGhostAction(ghost, ghost->building);
		usleep(WAIT_TIME);
	}
	
	
	removeGhostFromRoom(ghost, ghost->room);
	removeGhostFromBuilding(ghost, ghost->building);
	

	
	printf("*   The Ghost has exited the building\n");
}


/*
 Function:   hunterThread
  Purpose:   run a hunter thread
   in/out:   void pointer to the hunter
   return:   nothing, is void 
*/
void *hunterThread(void *huntery){
	HunterType* hunter = ((HunterType*) huntery);
	
	int EMFthere = 0;
    	int TEMPERATUREthere = 0;
    	int FINGERPRINTSthere = 0;
    	int SOUNDthere = 0;
    	int total;
    	
    	
	
	while(hunter->timer > 0 && hunter->fear < MAX_FEAR){
		chooseHunterAction(hunter, hunter->building);
		usleep(WAIT_TIME);
		
		found3GhostlyEvidence(&EMFthere, &TEMPERATUREthere, &FINGERPRINTSthere, &SOUNDthere, hunter);
		total = EMFthere + TEMPERATUREthere + FINGERPRINTSthere + SOUNDthere;
		
		if(total >= 3){
			break;
		}
		
	}
	
	removeHunterFromRoom(hunter, hunter->room);
	
	if(hunter->building->ghost != NULL && total < 3){
		removeHunterFromBuilding(hunter, hunter->building);
	}
	else if(total >= 3){
		hunter->hero = 1;
	}
	else{
		hunter->bored = 1;
	}
	
	
	printf("*   %s has exited the building\n", hunter->name);
}

/*
 Function:   printResults
  Purpose:   print the results of our simulation
       in:   pointer to the building
       in:   pointer to the ghost
   return:   nothing, is void 
*/
void printResults(BuildingType* building, GhostType* ghost){

    printf("\n");
    
    printf("\n Let's see the results:\n");

    
    for(int i = 0; i < building->allHunterCount; i++){

    	if(building->allHunters[i]->fear >= 100){
    		printf("*   %s ran away out of fear", building->allHunters[i]->name);
    	}
    	else if(building->allHunters[i]->timer <= 0){
    		printf("*   %s left from boredom", building->allHunters[i]->name);
    	}
    	else{
    		printf("*   %s survived and uncovered the ghost", building->allHunters[i]->name);
    	}
    	printf("\n");

    	
    }
    
    printf("\n");

    
    if(building->hunterCount < 1){
    	int someBored = 0;
    	int someAfraid = 0;
    	for(int i = 0; i < building->allHunterCount; i++){
    		if(building->allHunters[i]->fear >= 100){
    			someAfraid = 1;
    		}
    		else if(building->allHunters[i]->timer <= 0){
    			someBored = 1;
    		}
    	}
    	if(someBored == 1 && someAfraid == 1){
    		printf("The Ghost has gotten rid of all the hunters using fear and boredom!");
    	}
    	else if(someBored == 1 && someAfraid == 0){
    		printf("The Ghost has gotten rid of all the hunters using boredom!");
    	}
    	else if(someBored == 0 && someAfraid == 1){
    		printf("The Ghost has gotten rid of all the hunters using fear!");
    	}
    	printf("\nThe Ghost has won!\n");
    }
    else{
    	HunterType* survivor;
    	for(int i = 0; i < building->hunterCount; i++){
    		if(building->myHunters[i]->hero == 1){
    			survivor = building->myHunters[i];
    		}
    	}
    	
    	
    	int EMFthere = 0;
    	int TEMPERATUREthere = 0;
    	int FINGERPRINTSthere = 0;
    	int SOUNDthere = 0;
    	
    	found3GhostlyEvidence(&EMFthere, &TEMPERATUREthere, &FINGERPRINTSthere, &SOUNDthere, survivor);
    	
    	
    	printf("Found GHOSTLY Evidence:\n");
    	if(EMFthere == 1){
    		printf("*   EMF\n");
    	}
    	if(TEMPERATUREthere == 1){
    		printf("*   EVIDENCE\n");
    	}
    	if(FINGERPRINTSthere == 1){
    		printf("*   FINGERPRINTS\n");
    	}
    	if(SOUNDthere == 1){
    		printf("*   SOUND\n");
    	}
    	
    	printf("\n");
    	
    	EvidenceClassType hunterAns;
    	
    	if(EMFthere == 0){
    		printf("Speculated ghost type : PHANTOM\n");
    		hunterAns = PHANTOM;
    	}
    	else if(TEMPERATUREthere == 0){
    		printf("Speculated ghost type : BULLIES\n");
    		hunterAns = BULLIES;
    	}
    	else if(FINGERPRINTSthere == 0){
    		printf("Speculated ghost type : BANSHEE\n");
    		hunterAns = BANSHEE;
    	}
    	else{
    		printf("Speculated ghost type : POLTERGEIST\n");
    		hunterAns = POLTERGEIST;
    	}
    	
    	if(ghost->ghostClass == POLTERGEIST){
    		printf("Answer : POLTERGEIST\n");
    	}
    	else if(ghost->ghostClass == BANSHEE){
    		printf("Answer : BANSHEE\n");
    	}
    	else if(ghost->ghostClass == BULLIES){
    		printf("Answer : BULLIES\n");
    	}
    	else{
    		printf("Answer : PHANTOM\n");
    	}
    	
    	printf("\n");
    	
    	if(hunterAns == ghost->ghostClass){
    		printf("The Hunters win!\n");
    		
    	}
    	else{
    		printf("The Hunters' guess was wrong, the Ghost wins\n");
    	}
    	
    	for(int i = 0; i < building->hunterCount; i++){
    		printf("*   %s survived!\n", building->myHunters[i]->name);
    	}
    	
    }
}

