/* Kristin Chen, 118035390, kchen14 */
#include <stdio.h>
#include <math.h>
#define MAX_NUM_ASSIGNMENTS 50

double get_mean(int assignment_score[], int assignment_days_late[], int penalty_pts, int na);
double get_std_dev(int assignment_score[], int assignment_days_late[], int penalty_pts, int na);

/*main method. takes user inputs, calculates numeric score and prints*/
int main(){
    int penalty_pts, nd, na, idx, weight_sum = 0, assignments_dropped = 0;
    double numeric_score = 0;
    char gen_stat;
    /*temp arrays are copies of original to calculate numeric score if assignments are dropped*/
    int assignment_number[MAX_NUM_ASSIGNMENTS], assignment_score[MAX_NUM_ASSIGNMENTS],
            assignment_weight[MAX_NUM_ASSIGNMENTS], assignment_days_late[MAX_NUM_ASSIGNMENTS],
            temp_scores[MAX_NUM_ASSIGNMENTS], temp_weights[MAX_NUM_ASSIGNMENTS],
            temp_values[MAX_NUM_ASSIGNMENTS], temp_days_late[MAX_NUM_ASSIGNMENTS];

    scanf("%d %d %c\n %d", &penalty_pts, &nd, &gen_stat, &na);
    for (idx = 0; idx < na; idx++){
        int num, score, weight, days_late;

        scanf("\n%d, %d, %d, %d", &num, &score, &weight, &days_late);
        /*assigns an array with user inputs*/
        assignment_number[num - 1] = num;
        assignment_score[num - 1] = score;
        assignment_weight[num - 1] = weight;
        assignment_days_late[num - 1] = days_late;
        temp_scores[num - 1] = score;
        temp_weights[num - 1] = weight;
    }

    /*assign temp array to calculate numeric value if assignments are dropped*/
    if (nd != 0){
        int spot, i, highest_value, days_late, total_days_late = 0, wt; /*wt = weight*/
        /*assignment dropped is set to true, used to check if sum weights later*/
        assignments_dropped = 1; 
        /*only leaves na - nd spots for array*/
        for (spot = 0; spot < na - nd; spot++){
            highest_value = 0; days_late = 0;
            /*loops thru assignments to find ones with highest assignment values
                and the corresponding number for days late*/
            for (i = 0; i < na; i++){
                /*if numeric score is larger than highest value (0), numeric score
                    will now be the highest, along w/ corresponding with days late and wt*/
                if (temp_scores[i] * temp_weights[i] > highest_value){
                    highest_value = temp_scores[i] * temp_weights[i];
                    days_late = assignment_days_late[i];
                    wt = assignment_weight[i];
                }
            }
            /*sets prev temp score to 0 so code doesn't repeat*/
            for (i = 0; i < na; i++){
                if (highest_value == temp_scores[i] * temp_weights[i]){
                    temp_scores[i] = 0;
                }
            }
            /*sets temp arrays with values from above*/
            temp_values[spot] = highest_value;
            temp_days_late[spot] = days_late;
            temp_weights[spot] = wt;
        } 
        /*calculating numeric score*/
        for (spot = 0; spot < na - nd; spot++){
            if (temp_days_late[spot] != 0){
                numeric_score += temp_values[spot] - (temp_weights[spot] * 
                    temp_days_late[spot] * penalty_pts);
            } else{
                numeric_score += temp_values[spot];
            }
            total_days_late +=  temp_days_late[spot];
            weight_sum += temp_weights[spot];
        }
    } else{
        /*calculates numeric score w/o drops*/
        for (idx = 0; idx < na; idx++){
            if (assignment_days_late[idx] != 0){
                numeric_score += (assignment_score[idx] - 
                    (assignment_days_late[idx] * penalty_pts)) * assignment_weight[idx];
                weight_sum += assignment_weight[idx];
            } else{
                numeric_score += assignment_score[idx] * assignment_weight[idx];
                weight_sum += assignment_weight[idx];
            }
        }
    }

    /*if sum weight != 100, terminate. otherwise continue to print assignment info*/
    if (weight_sum != 100 && assignments_dropped == 0){
        printf("ERROR: Invalid values provided.\n");
    } else{
        if (numeric_score < 0){
        numeric_score = 0;
        } else{
            numeric_score /= weight_sum;
        }
        printf("Numeric Score: %5.4f\n", numeric_score);
        printf("Points Penalty Per Day Late: %d\n", penalty_pts);
        printf("Number of Assignments Dropped: %d\n", nd);
        printf("Values Provided:\n");
        printf("Assignment, Score, Weight, Days Late\n");

        for (idx = 0; idx < na; idx++){
            printf("%d, %d, %d, %d\n", assignment_number[idx], assignment_score[idx],
                assignment_weight[idx], assignment_days_late[idx]);
        }

        /*if user wants to generate stat*/
        if (gen_stat == 'Y' || gen_stat == 'y'){
            printf("Mean: %5.4f, ", get_mean(assignment_score, assignment_days_late, 
                penalty_pts, na));
            printf("Standard Deviation: %5.4f\n", get_std_dev(assignment_score, 
                assignment_days_late, penalty_pts, na));
        }
    }
    
    return 0;
}

/*determines the mean score of the assignments, taking into acc of days late and penalty pts*/
double get_mean(int assignment_score[], int assignment_days_late[], int penalty_pts, int na){
    float mean = 0;
    int i;

    for (i = 0; i < na; i++){
        if (assignment_days_late[i] != 0){
            mean += assignment_score[i];
            mean -= (penalty_pts * assignment_days_late[i]);
        } else{
            mean += assignment_score[i];
        }
    }
    mean /= na;

    return mean;
} 

/*determines std deviation of assignments, taking into acc of days late and penalty pts*/
double get_std_dev(int assignment_score[], int assignment_days_late[], int penalty_pts, int na){
    float mean = get_mean(assignment_score, assignment_days_late, penalty_pts, na), sd = 0;
    int i;

    for (i = 0; i < na; i++){
        if (assignment_days_late[i] != 0){
            sd += pow(assignment_score[i] - (penalty_pts * assignment_days_late[i]) - mean, 2);
        } else{
            sd += pow(assignment_score[i] - mean, 2);
        } 
    }
    sd /= na;

    return sqrt(sd);
}
