#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int *parents;
int *children;
int number_of_links;

int propagate(int current_point,
              int previous_point,
              int *farthest_point) {
    printf("\n> Enter the function with parameters: \n");
    printf("current_point = %d, previous_point = %d, farthest_point = %d\n", current_point, previous_point, farthest_point[0]);

    int children_of_current[number_of_links];
    int number_of_children_of_current=0;
    for (int i=0; i<number_of_links; i++) {
        if (parents[i]==current_point) {
            if (children[i]!=previous_point) {
                children_of_current[number_of_children_of_current]=children[i];
                number_of_children_of_current++;
            }
        } else if (children[i]==current_point){
            if (parents[i]!=previous_point) {
                children_of_current[number_of_children_of_current]=parents[i];
                number_of_children_of_current++;
            }
        }
    }
    printf("number_of_children_of_current = %d\n", number_of_children_of_current);
    int max_depth=1;

    int farthest_point_from_this=current_point;
    for (int i = 0; i<number_of_children_of_current; i++){
            printf("Calling for child %d\n",i);
            farthest_point_from_this=children_of_current[i];
            int depth_of_child=propagate(children_of_current[i],current_point,&farthest_point_from_this);
            printf("depth_of_child=%d, max_depth=%d, farthest_point_from_this=%d\n",depth_of_child, max_depth, farthest_point_from_this);
            if (depth_of_child>=max_depth){
                max_depth=depth_of_child+1;
                farthest_point[0]=farthest_point_from_this;
                printf("***********Updated: max_depth=%d, farthest_point[0]=%d\n",depth_of_child, max_depth, farthest_point[0]);
            }
    }
    printf("< Exit the function with parameter\n");
    printf("farthest_point = %d\n, returned value = %d\n",farthest_point[0],max_depth);
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
    number_of_links = atoi(pch);
    printf("number of links: %d", number_of_links);
    int parents_[number_of_links];
    int children_[number_of_links];
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
