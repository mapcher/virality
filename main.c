#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int *parents;
int *children;
int relays_number;

int propagate(int current_point,
              int previous_point,
              int *farthest_point) {
    printf("\n> Enter the function with parameters:");
    printf("\ncurrent_point = %d, previous_point = %d, farthest_point = %d", current_point, previous_point, farthest_point[0]);

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
    printf("\ncurrent_children_number= %d", current_children_number);
    int max_depth=1;

    int farthest_point_from_this=current_point;
    for (int i = 0; i<current_children_number; i++){
            printf("\nCalling child %d",i);
            farthest_point_from_this=current_children[i];
            int child_depth=propagate(current_children[i],current_point,&farthest_point_from_this);
            printf("\nchild_depth=%d, max_depth=%d, farthest_point_from_this=%d",child_depth, max_depth, farthest_point_from_this);
            if (child_depth>=max_depth){
                max_depth=child_depth+1;
                farthest_point[0]=farthest_point_from_this;
                printf("\n***********Updated: max_depth=%d, farthest_point[0]=%d",child_depth, max_depth, farthest_point[0]);
            }
    }
    printf("\n< Exit the function with parameter");
    printf("\nfarthest_point = %d\n, returned value = %d",farthest_point[0],max_depth);
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
        printf("\n%d. %d - %d", index, parent, child);
        parents[index] = parent;
        children[index] = child;
        index++;
	}
	fclose(file);
	/*************************************************************************
	 Finding the viral potential
    **************************************************************************/
    //start from any point and find the farthest point
    int starting_point = parents[0];
    int previous_point = -1;
    int farthest_point[1];
    farthest_point[0] = starting_point;
    int depth=propagate(starting_point, previous_point, farthest_point);
    printf("depth = %d, farthest_point = %d",depth,farthest_point[0]);
    //start from the farthest point and find the depth
    int new_starting_point = farthest_point[0];
    previous_point = -1;
    farthest_point[0] = new_starting_point;
    depth = propagate(new_starting_point, previous_point, farthest_point);

    int fastest_propagation = depth / 2;
    printf("\nFastest propagation: %d", fastest_propagation);
    return 0;
}
