#include <stdio.h>
#include <string.h>
#include <stdlib.h>
long *parents;
long *children;
long relays_number;

/* long propagate(long current_point, long previous_point, long *farthest_p)
   returns maximum depth from the current point,
   sets farthest point by use of a pointer, because in C I can return only one primitive type.
   Alternatively I could make an array with two values (depth and farthest_point) and return the pointer to it...*/
long propagate(long current_point, long previous_point, long *farthest_p) {
    long *current_children = NULL;
    current_children = (long*) malloc(relays_number*sizeof(long));
    long current_children_number=0;
    for (long i=0; i<relays_number; i++) {
        if (parents[i]==current_point) {
            if (children[i]!=previous_point) {
                current_children[current_children_number]=children[i];
                current_children_number++;
            }
        } else if (children[i]==current_point){
            if (parents[i]!=previous_point) {
                current_children[current_children_number]=parents[i];
                current_children_number++;
            }
        }
    }
    long max_depth=1;
    long farthest_point_from_this=current_point;
    for (long i = 0; i<current_children_number; i++){
            farthest_point_from_this=current_children[i];
            long child_depth=propagate(current_children[i],current_point,&farthest_point_from_this);
            if (child_depth>=max_depth){
                max_depth=child_depth+1;
                *farthest_p=farthest_point_from_this;
            }
    }
    free(current_children);
    return max_depth;
}

int main (int argc, char* argv[]) {
    /************************************************************
     Reading data from file
    ************************************************************/
    if (argc < 2) {
        printf("no path argument");
		return 0;
    }
    printf("%s %s\n", argv[0], argv[1]);
	char *fname = argv[1];
	char result_string[256];
	FILE *file;
	file = fopen(fname, "r");
	if(file == NULL) {
		printf("can't open file %s'", fname);
		return 0;
	}
    char *pch;
	fgets(result_string, sizeof(result_string), file);
    pch = strtok (result_string, " ");
    if (pch == NULL) {
		printf("invalid line 1");
		return 0;
    }
    relays_number = atol(pch);
    printf("relays_number: %ld", relays_number);
    long parents_[relays_number];
    long children_[relays_number];
    parents=parents_;
    children=children_;
    long index = 0;
	while(fgets(result_string, sizeof(result_string), file)) {
        pch = strtok(result_string, " ");
        if (pch == NULL) {
            printf("invalid parameter 1 at line %ld", index+1);
            return 0;
        }
        long parent = atol(pch);
        pch = strtok (NULL, " ");
        if (pch == NULL) {
            printf("invalid parameter 2 at line %ld", index+1);
            return 0;
        }
        long child = atol(pch);
        parents[index] = parent;
        children[index] = child;
        index++;
	}
	fclose(file);
	/*************************************************************************
	 Finding the viral potential
    **************************************************************************/
    //start from any point (for instance 0) and find the farthest point
    long farthest=parents[0];
    long *farthest_p=&farthest;
    propagate(parents[0], -1, farthest_p);
    //start from the farthest point and find the depth
    long depth = propagate(farthest, -1, farthest_p);
    long fastest_propagation = depth / 2;
    printf("\nFastest propagation: %ld", fastest_propagation);
    return 0;
}
