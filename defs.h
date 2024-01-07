#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99
#define WAIT_TIME          500 //wait time for each thread

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;


//Making my structs, they're named obviously
typedef struct EvidenceNode{
	struct Evidence *data;
	struct EvidenceNode *next;
} EvidenceNodeType;

typedef struct RoomNode{
	struct Room *data;
	struct RoomNode *next;
} RoomNodeType;

typedef struct RoomList {
	RoomNodeType *head;
	RoomNodeType *tail; 
	int size;
} RoomListType;

typedef struct EvidenceList {
	EvidenceNodeType *head;
	EvidenceNodeType *tail; 
	int size;
} EvidenceListType;


typedef struct Room {
  char name[MAX_STR];
  RoomListType connectedRooms;
  EvidenceListType roomEvidence;
  struct Hunter* myHunters[MAX_HUNTERS];
  struct Ghost *myGhost;
  int hunterCount;
  
  sem_t mutex;
} RoomType;


typedef struct Ghost{
  GhostClassType ghostClass;
  RoomType *room;
  int boredomTimer;
  struct Building* building;
  
} GhostType;

typedef struct Evidence{
  EvidenceClassType type;
  float value;
  
} EvidenceType;


typedef struct Hunter{
  RoomType *room;
  EvidenceClassType mySpecialty;
  EvidenceListType myEvidence;
  char name[MAX_STR];
  int fear;
  int timer;
  struct Building* building;
  int hero;
  int bored;
  
  
} HunterType;

typedef struct Building{
  GhostType* ghost;
  HunterType* myHunters[MAX_HUNTERS];
  HunterType* allHunters[MAX_HUNTERS];
  RoomListType rooms;
  EvidenceListType allEvidence; //Remember to talk about this in README
  int hunterCount;
  int allHunterCount;
  
} BuildingType;








//main funcs
int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters
//threads
void *ghostThread(void *);
void *hunterThread(void *);
//print results
void printResults(BuildingType*, GhostType*);





//Building functions
//Init
void initBuilding(BuildingType*);
void populateRooms(BuildingType*);   // Populates the building with sample data for rooms
//free
void freeBuilding(BuildingType*);



//Ghost functions
//Init
void initGhost(GhostClassType, RoomType*, GhostType*, BuildingType*);
void makeRandGhost(GhostType*, BuildingType*);
//add/remove
void addGhostToRoom(GhostType*, RoomType*);
void addGhostToBuilding(GhostType*, BuildingType*);
void removeGhostFromRoom(GhostType*, RoomType*);
void removeGhostFromBuilding(GhostType*, BuildingType*);
//Actions
void chooseGhostAction(GhostType*, BuildingType*);
void generateGhostEvidence(GhostType*, BuildingType*);
void stayStillGhost(GhostType*);
void moveRoomGhost(GhostType*);



//Room functions
//Init
void initRoomList(RoomListType*);
void initRoom(RoomType*, char*);
//add/check
void appendRoom(RoomListType*, RoomNodeType*);
//free
void freeRoomData(RoomListType*);
void freeRoomNodes(RoomListType*);
//connect
void connectRooms(RoomType*, RoomType*);




//Hunter functions
//Init
void initHunter(RoomType*, EvidenceClassType, char*, HunterType*, BuildingType*);
//add/remove
void addHunterToRoom(HunterType*, RoomType*);
void addHunterToBuilding(HunterType*, BuildingType*);
void removeHunterFromRoom(HunterType*, RoomType*);
void removeHunterFromBuilding(HunterType*, BuildingType*);
//Actions
void chooseHunterAction(HunterType*, BuildingType*);
void readEvidenceHunter(HunterType*, BuildingType*);
void communicateHunters(HunterType*);
void moveRoomHunter(HunterType*);
//Check if have 3 diff type pieces of ghostly evidence
void found3GhostlyEvidence(int*, int*, int*, int*, HunterType*);



//Evidence functions
//init
void initEvidence(EvidenceClassType, float, EvidenceType*);
void initEvidenceList(EvidenceListType*);
//add/remove/check
void appendEvidence(EvidenceListType*, EvidenceNodeType*);
int checkForDuplicateEvidence(EvidenceListType*, EvidenceNodeType*);
int checkForGhostlyEvidence(EvidenceNodeType*);
int findIfEvTypeThere(EvidenceListType*, EvidenceClassType);
void removeEvidence(EvidenceListType*, int);
float returnValueAtI(EvidenceListType*, int);
//free
void freeEvidenceData(EvidenceListType*);
void freeEvidenceNodes(EvidenceListType*);




