#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/****************************************************************
   Function propagate(...) is called recursively,
   returns maximum depth from the current point,
   sets farthest point by use of a pointer parameter,
   because in C I can return only one value of a primitive type.
   Alternatively I could make an array with two values
   (depth and farthest_point) and return the pointer to it...
*****************************************************************/
long propagate(long current_point,
               long previous_point, //where we come here from, don't go back
               long *farthest_p,    //parameter for returning the deepest point
               long *parents,       //the first column in the input file
               long *children,      //the second column in the input file
               long relays_number) {
    long *neighbors = (long*) malloc(relays_number*sizeof(long));
    long neighbors_number=0;
    for (long i = 0; i < relays_number; i++) {
        if (parents[i] == current_point) {
            if (children[i] != previous_point) {
                neighbors[neighbors_number] = children[i];
                neighbors_number++;
            }
        } else if (children[i] == current_point) {
            if (parents[i] != previous_point) {
                neighbors[neighbors_number] = parents[i];
                neighbors_number++;
            }
        }
    }
    long depth_record = 1; //current record of depth, will grow further
    long farthest_point_from_this = current_point;
    for (long i = 0; i < neighbors_number; i++) {
        farthest_point_from_this = neighbors[i];
        long child_depth = propagate(neighbors[i],  //recursion for each neighbor
                                     current_point,           //"previous" for the neighbor
                                     &farthest_point_from_this,//returning parameter
                                     parents,
                                     children,
                                     relays_number);
        if (child_depth >= depth_record) { //record is reached and may be beaten
            depth_record = child_depth + 1;
            *farthest_p = farthest_point_from_this;
        }
    }
    free(neighbors);
    return depth_record;
}

int main (int argc, char* argv[]) {
    /************************************************************
     Reading data from file
    ************************************************************/
    if (argc < 2) {
        printf("No path argument. Example: virality.exe dataset.txt");
		return 1;
    }
    printf("%s %s\n", argv[0], argv[1]);
	char *fname = argv[1];
	char result_string[256];
	FILE *file;
	file = fopen(fname, "r");
	if(file == NULL) {
		printf("Can't open file %s'", fname);
		return 2;
	}
    char *pch;
	fgets(result_string, sizeof(result_string), file);
    pch = strtok(result_string, " ");
    if (pch == NULL) {
		printf("Invalid line 1. Number required.");
		return 3;
    }
    long relays_number = atol(pch);
    printf("Relays number: %ld\n", relays_number);
    long parents[relays_number];
    long children[relays_number];
    long index = 0;
	while (fgets(result_string, sizeof(result_string), file)) {
        pch = strtok(result_string, " ");
        if (pch == NULL) {
            printf("invalid parameter 1 at line %ld. Two space separated numbers required.", index + 1);
            return 4;
        }
        long parent = atol(pch);
        pch = strtok(NULL, " ");
        if (pch == NULL) {
            printf("invalid parameter 2 at line %ld. Two space separated numbers required.", index + 1);
            return 5;
        }
        long child = atol(pch);
        parents[index] = parent;
        children[index] = child;
        index++;
	}
	fclose(file);
    printf("Read rows: %ld", index);

	/*************************************************************************
	 Finding the viral potential
    **************************************************************************/
    // Let's find the farthest point
    long farthest = parents[0];
    long *farthest_p = &farthest;
    propagate(parents[0], // start from any point (for instance 0)
              -1,         // the first point has no "previous"
              farthest_p, // returning parameter, will be used later
              parents,
              children,
              relays_number);
    // Now find the biggest distance on the net
    long depth = propagate(farthest,  // start from the previously found farthest point
                           -1,        // the first point has no "previous"
                           farthest_p,// now plays no role
                           parents,
                           children,
                           relays_number);
    // The vantage point is in the middle of the longest route
    long fastest_propagation = depth / 2;
    printf("\nFastest propagation: %ld", fastest_propagation);
    return 0;
}
