#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct propagation_result {
    long depth;
    long farthest_point;
};

/****************************************************************
   Function propagate(...) is called recursively,
   returns maximum depth
   and the farthest point from the current point
*****************************************************************/
struct propagation_result propagate(long current_point,
                                    long previous_point, //where we come here from, don't go back
                                    long *parents,       //the first column in the input file
                                    long *children,      //the second column in the input file
                                    long relays_number) {
    long depth = 1;
    long farthest_point = current_point;

    long neighbor;
    for (long i = 0; i < relays_number; i++) {
        if (parents[i] != current_point && children[i] != current_point) continue;
        if (parents[i] == previous_point || children[i] == previous_point) continue;
        if (parents[i] == current_point) {
            neighbor = children[i];
        } else { // (children[i] == current_point)
            neighbor = parents[i];
        }
        struct propagation_result neighbor_result = propagate(neighbor,  //recursion for each neighbor
                                                              current_point, //"previous" for the neighbor
                                                              parents,
                                                              children,
                                                              relays_number);
        long child_depth = neighbor_result.depth;
        if (child_depth >= depth) {
            depth = child_depth + 1;
            farthest_point = neighbor_result.farthest_point;
        }
    }
    struct propagation_result result = {depth, farthest_point};
    return result;
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
    long previous_point = -1;
    // Let's find the farthest point
    // Start from any point (for instance from the first)
    struct propagation_result first_propagation_result = propagate(parents[0],
                                                                   previous_point,
                                                                   parents,
                                                                   children,
                                                                   relays_number);
    // Now find the biggest distance on the net
    // Start from the recently found farthest point
    printf("\nfirst_propagation_result.farthest_point = %ld",first_propagation_result.farthest_point);
    struct propagation_result second_propagation_result = propagate(first_propagation_result.farthest_point,
                                                                    previous_point,
                                                                    parents,
                                                                    children,
                                                                    relays_number);
    // The vantage point is in the middle of the longest route
    long depth = second_propagation_result.depth;
    long fastest_propagation = depth / 2;
    printf("\nFastest propagation: %ld", fastest_propagation);
    return 0;
}
