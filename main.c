#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int *parents;
int *children;
int relays_number;

/* int propagate(int current_point, int previous_point, int *farthest_p)
   returns maximum depth from the current point,
   sets farthest point by use of a pointer, because in C I can return only one primitive type.
   Alternatively I could make an array with two values (depth and farthest_point) and return the pointer to it...*/
int propagate(int current_point, int previous_point, int *farthest_p) {
    int current_children[relays_number];
    int current_children_number=0;
    for (int i=0; i<relays_number; i++) {
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
    int max_depth=1;
    int farthest_point_from_this=current_point;
    for (int i = 0; i<current_children_number; i++){
            farthest_point_from_this=current_children[i];
            int child_depth=propagate(current_children[i],current_point,&farthest_point_from_this);
            if (child_depth>=max_depth){
                max_depth=child_depth+1;
                *farthest_p=farthest_point_from_this;
            }
    }
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
    relays_number = atoi(pch);
    printf("relays_number: %d", relays_number);
    int parents_[relays_number];
    int children_[relays_number];
    parents=parents_;
    children=children_;
    int index = 0;
	while(fgets(result_string, sizeof(result_string), file)) {
        int pch = strtok(result_string, " ");
        if (pch == NULL) {
            printf("invalid parameter 1 at line %d", index+1);
            return 0;
        }
        int parent = atoi(pch);
        pch = strtok (NULL, " ");
        if (pch == NULL) {
            printf("invalid parameter 2 at line %d", index+1);
            return 0;
        }
        int child = atoi(pch);
        parents[index] = parent;
        children[index] = child;
        index++;
	}
	fclose(file);
	/*************************************************************************
	 Finding the viral potential
    **************************************************************************/
    //start from any point (for instance 0) and find the farthest point
    int farthest=parents[0];
    int *farthest_p=&farthest;
    propagate(parents[0], NULL, farthest_p);
    //start from the farthest point and find the depth
    int depth = propagate(farthest, NULL, farthest_p);
    int fastest_propagation = depth / 2;
    printf("\nFastest propagation: %d", fastest_propagation);
    return 0;
}
