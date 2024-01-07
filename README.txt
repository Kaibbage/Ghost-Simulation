Author : Kai Sundararaj
Student Number : 101240325
Program Description : Run a simulation with 5 threads that simulates a ghost and 4 hunters moving around rooms in a building and leaving and collecting evidence with the ghost trying to get rid of all the hunters and the hunters trying to outlast the ghost and collect evidece.
		      
List of files : defs.h, main.c, ghost.c, hunter.c, room.c, building.c, evidence.c, Makefile, README.txt, all of these are stored inside Assignment5.tar

Instructions for compiling and launching:
	The left is what it's doing, the right is what to type into the command line 
	
	     untarring:      tar -xvf Assignment5.tar 
		
      	     object files:   make all
      	     
      	     compiling:      make a5
      	     
      	     launching:      ./a5
      	  
      	     

Note* you can skip making the object files using make all if you want, this is automatically done in make a5
Note* there is a warning when making the object files and compiling about usleep(), I don't know why. I had to use usleep() because sleep() uses whole seconds and this takes way too long. usleep() is supposed to be included in unistd.h so i really don't know why this happens






Extra notes : - I tried to follow the specifications as closely as I could
	      - I did not run into any deadlock problems so I did not implement the deadlock solution in the announcements
	      - To make freeing the data easier and avoid double freeing I created a universal evidence collection in the building that holds every piece of evidence created inside the building
	      - There is a warning about usleep when compiling, but the code still runs fine
	      - I didn't implement part of part 4.4 which said that the hunters win if any hunters had fear < 100, however they should not win because they left out of boredom and did not outlast the ghost.
	        Also in the project demo video, it showed that the hunters did not win unless they outlasted the ghost, so I followed the output design that I saw in the project demo video.
	        
	        IMPORTANT NOTE : Surviving in my simulation is defined as : 
	        								Either
	        								- outlasting the ghost in the building and leaving
	        								- being the hero and gathering 3 different pieces of ghostly evidence
	        								
	        								IMPORTANT DISTINCTION
	        								- leaving out of boredom while the ghost is still there does not count as surviving
	        								- leaving out of boredom after the ghost has left does count as surviving
	        								
	        								- obviously leaving out of fear does not count as surviving
