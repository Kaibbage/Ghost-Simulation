#include "defs.h"

/*
 Function:   initEvidence
  Purpose:   initialize a piece of evidence
       in:   the type of the evidence
       in:   the value of the evidence
   in/out:   evidence to be initialized
   return:   nothing, is void 
*/
void initEvidence(EvidenceClassType evType, float value, EvidenceType* evidence){
	evidence->type = evType;
	evidence->value = value;
}

/*
 Function:   initEvidenceList
  Purpose:   initialize an evidence list
   in/out:   evidence list to be initialized
   return:   nothing, is void 
*/
void initEvidenceList(EvidenceListType* evidenceList){
	evidenceList->head = NULL;
	evidenceList->tail = NULL;
	evidenceList->size = 0;
}

/*
 Function:   appendEvidence
  Purpose:   append evidence to the end of an evidence list
   in/out:   the evidence list to be added to
       in:   the evidence node to be added to the evidence list
   return:   nothing, is void 
*/
void appendEvidence(EvidenceListType* evidenceList, EvidenceNodeType* evidenceNode){
	EvidenceNodeType* curr = evidenceList->head;
	EvidenceNodeType* prev = NULL;
	EvidenceNodeType* new = NULL;
	
	
	while(curr != NULL){
		prev = curr;
		curr = curr->next;
	}
	
	
	
	new = evidenceNode;
	
	if(prev == NULL){
		evidenceList->head = new;
		evidenceList->tail = new;	
	}
	else{
		prev->next = new;
		evidenceList->tail = new;
	}
	
	new->next = NULL;
	
	evidenceList->size++;
}

/*
 Function:   removeEvidence
  Purpose:   remove evidence from the evidence list
   in/out:   the evidence list to be removed from
       in:   the index of the evidence node to be removed
   return:   nothing, is void 
*/
void removeEvidence(EvidenceListType* evidenceList, int counted) {
	EvidenceNodeType* curr = evidenceList->head;
	EvidenceNodeType* prev = NULL;
	
	
	
	for(int i = 0; i < counted; i++){
		prev = curr;
		curr = curr->next;
	}
		
	
	
	if(prev == NULL && curr == NULL){
		//nothing to change
	}
	else if(prev != NULL && curr == NULL){
		//not found
	}
	else if(prev == NULL && curr != NULL){
		//broke on head : remove head
		if(evidenceList->size > 1){
			evidenceList->head = curr->next;
		}
		else{
			evidenceList->head = NULL;
			evidenceList->tail = NULL;
		}

		
	}
	else{
		prev->next = curr->next;
		if(curr->next == NULL){
			evidenceList->tail = prev;
		}
		
	}
	
	
	evidenceList->size--;
	
	free(curr);
	
}


/*
 Function:   findIfEvTypeThere
  Purpose:   find if there is any evidence with this type in the evidence list
       in:   the evidence list to be looked at
       in:   the evidence type to be looked for
   return:   the index of the evidence node that matches that type if successful, -1 if failure 
*/
int findIfEvTypeThere(EvidenceListType* evidenceList, EvidenceClassType e){
	EvidenceNodeType* curr = evidenceList->head;
	int counter = 0;
	
	while(curr != NULL){
		if(curr->data->type == e){
			break;
		}
		curr = curr->next;
		counter++;
	}
	
	if(curr == NULL){
		counter = -1;
		return counter;
	}
	
	
	return counter;
	
}

/*
 Function:   checkForDuplicateEvidence
  Purpose:   check if the given evidence is already in this evidence list
       in:   evidence list to be checked in
       in:   the evidence node with the evidence to be checked
   return:   1 if the evidence is already in the list, 0 if it isn't 
*/
int checkForDuplicateEvidence(EvidenceListType* evidenceList, EvidenceNodeType* evidenceNode){
	EvidenceNodeType* curr = evidenceList->head;
	
	int isThere = 0;
	
	while(curr != NULL){
		if(curr->data->value == evidenceNode->data->value && curr->data->type == evidenceNode->data->type){
			isThere = 1;
			break;
		}
		curr = curr->next;
	}
	
	return isThere;
	
	

}

/*
 Function:   checkForGhostlyEvidence
  Purpose:   check if the given evidence is ghostly
       in:   evidence node with the evidence to be checked
   return:   1 if the evidence is ghostly, 0 if it isn't 
*/
int checkForGhostlyEvidence(EvidenceNodeType* evidenceNode){
	int isGhostly = 0;
	float a = 1;
	
	if(evidenceNode->data->type == EMF && evidenceNode->data->value >= 4.9){
		isGhostly = 1;
	}
	else if(evidenceNode->data->type == TEMPERATURE && evidenceNode->data->value <= 0){
		isGhostly = 1;
	}
	else if(evidenceNode->data->type == FINGERPRINTS && evidenceNode->data->value == a){
		isGhostly = 1;
	}
	else if(evidenceNode->data->type == SOUND && evidenceNode->data->value >= 70){
		isGhostly = 1;
	}
	
	return isGhostly;
}

/*
 Function:   returnValueAtI
  Purpose:   return the value of the evidence at node I
       in:   evidence list to be searched
       in:   the index of the evidence to be searched for
   return:   the value of the evidence at index I
*/
float returnValueAtI(EvidenceListType* evidenceList, int i){
	EvidenceNodeType* curr = evidenceList->head;
	
	for(int f = 0; f < i; f++){
		curr = curr->next;
	}
	
	return curr->data->value;
}

/*
 Function:   freeEvidenceData
  Purpose:   free all the evidence data in the given evidence list
   in/out:   evidence list with data to be freed
   return:   nothing, is void
*/
void freeEvidenceData(EvidenceListType* evidenceList){
	EvidenceNodeType* curr = evidenceList->head;
	EvidenceNodeType* next = NULL;
	
	
	while(curr != NULL){
		next = curr->next;
		free(curr->data);
		curr = next;
	}
}

/*
 Function:   freeEvidenceNodes
  Purpose:   free all the evidence data in the given evidence list
   in/out:   evidence list with data to be freed
   return:   nothing, is void
*/
void freeEvidenceNodes(EvidenceListType* evidenceList){
	EvidenceNodeType* curr = evidenceList->head;
	EvidenceNodeType* next = NULL;
	

	
	while(curr != NULL){
		next = curr->next;
		free(curr);
		curr = next;
	}
}

