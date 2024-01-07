#include "defs.h"

/*
 Function:   initRoomList
  Purpose:   initialize a room list
   in/out:   the room list to be initialized
   return:   nothing, is void
*/
void initRoomList(RoomListType* roomList){
	roomList->head = NULL;
	roomList->tail = NULL;
	roomList->size = 0;
}

/*
 Function:   initRoom
  Purpose:   initialize a room
   in/out:   the room
       in:   the name of the room
   return:   nothing, is void
*/
void initRoom(RoomType* room, char* name){
	strcpy(room->name, name);
	initRoomList(&room->connectedRooms);
	initEvidenceList(&room->roomEvidence);
	room->hunterCount = 0;
	room->myGhost = NULL;
	sem_init(&room->mutex, 0, 1);


	
}

/*
 Function:   appendRoom
  Purpose:   append a room to the end of a room list
   in/out:   the room list to be added to
       in:   the room node to be added to the room list
   return:   nothing, is null 
*/
void appendRoom(RoomListType* roomList, RoomNodeType* roomNode){
	RoomNodeType* curr = roomList->head;
	RoomNodeType* prev = NULL;
	RoomNodeType* new = NULL;
	
	while(curr != NULL){
		prev = curr;
		curr = curr->next;
	}
	
	
	
	new = roomNode;
	
	if(prev == NULL){
		roomList->head = new;
		roomList->tail = new;	
	}
	else{
		prev->next = new;
		roomList->tail = new;
	}
	
	new->next = NULL;
	
	roomList->size++;
}

/*
 Function:   connectRooms
  Purpose:   connect 2 rooms by adding each other to their room list
   in/out:   the first room
   in/out:   the second room
   return:   nothing, is null 
*/
void connectRooms(RoomType* room1, RoomType* room2){
	RoomNodeType* room1Node = calloc(1, sizeof(RoomNodeType));
    	room1Node->data = room1;
    	RoomNodeType* room2Node = calloc(1, sizeof(RoomNodeType));
    	room2Node->data = room2;
    	
	appendRoom(&room1->connectedRooms, room2Node);
	appendRoom(&room2->connectedRooms, room1Node);
}

/*
 Function:   freeRoomData
  Purpose:   free all the room data in the given room list
   in/out:   room list with data to be freed
   return:   nothing, is void
*/
void freeRoomData(RoomListType* roomList){
	RoomNodeType* curr = roomList->head;
	RoomNodeType* next = NULL;

	
	
	while(curr != NULL){
		next = curr->next;
		free(curr->data);
		curr = next;
	}
}

/*
 Function:   freeRoomNodes
  Purpose:   free all the room data in the given room list
   in/out:   room list with data to be freed
   return:   nothing, is void
*/
void freeRoomNodes(RoomListType* roomList){
	RoomNodeType* curr = roomList->head;
	RoomNodeType* next = NULL;
	

	
	while(curr != NULL){
		next = curr->next;
		free(curr);
		curr = next;
	}
}




