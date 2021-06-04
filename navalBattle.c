// naval battle using the browser as a game board
#include <stdio.h>
#include <stdlib.h>
#define ROW 10 // number of matrix rows
#define COLUMN 10 // number of matriz columns
#define MAXSHIPS 14 // total amount of "squares" of ships

// prototype of functions
void create_html_opponent(FILE *cfgo, FILE *opponent, char **tab_opponent_aux);
void create_html_player(FILE *cfgp, FILE *player, char **tab_player_aux);
void write_html_opponent(FILE *cfgo, FILE *opponent, char **tab_opponent_aux);
void write_html_player(FILE *cfgp, FILE *player, char **tab_player_aux);
void convert_opponent(char **tab_opponent_aux, int **tab_opponent);
void convert_player(char **tab_player_aux, int **tab_player);
int *opponent_shoot(FILE *cfgp, FILE *player, int **tab_player, int *shot_opponent);
int *player_shoot(FILE *cfgo, FILE *opponent, int **tab_opponent, int *shot_player);
int opponent_check_shot(FILE *cfgp, FILE *player, int **tab_player, int *shot_opponent);
int player_check_shot(FILE *cfgo, FILE *opponent, int **tab_opponent, int *shot_player);
void update_tab_opponent(FILE *cfgo, FILE *opponent, int **tab_opponent);
void update_tab_player(FILE *cfgp, FILE *player, int **tab_player);

int main(){
	int i, j, choice;
	char **tab_player_aux, **tab_opponent_aux; // matrix to read of the settings
	int **tab_opponent, **tab_player; // matrix to manipulate the board
	int *shot_player, *shot_opponent; // shooting position
	int hits_player = 0, hits_opponent = 0; // return of function check_shot
	int count_hits_player = 0, count_hits_opponent = 0; // hit counter
	
	FILE *player, *opponent; // html file for console
	FILE *cfgp, *cfgo; // txt file for configuration
	
	// main vector allocation
	shot_player = malloc(2 * sizeof(int *));
	shot_opponent = malloc(2 * sizeof(int *));
	
	tab_opponent = malloc(ROW * sizeof(int *));
	tab_player = malloc(ROW * sizeof(int *));
	tab_player_aux = malloc(ROW * sizeof(char *));
	tab_opponent_aux = malloc(ROW * sizeof(char *));
	
	// matrix column allocation
	for(i = 1; i <= COLUMN; i++){
		tab_opponent[i] = malloc(COLUMN * sizeof(int));
		tab_player[i] = malloc(COLUMN * sizeof(int));
		tab_player_aux[i] = malloc(COLUMN * sizeof(int));
		tab_opponent_aux[i] = malloc(COLUMN * sizeof(int));
	}
	// menu
	printf("############## naval battle ##############");
	printf("\n[1] - start new game.");
	printf("\n[2] - view rules and specifications.");
	printf("\n-----> ");
	scanf("%d", &choice);
	printf("\n###########################################\n");
	
	if(choice == 1){
		create_html_player(cfgp, player, tab_player_aux);
		create_html_opponent(cfgo, opponent, tab_opponent_aux);
		convert_player(tab_player_aux, tab_player);
		convert_opponent(tab_opponent_aux, tab_opponent);
		
		do{	
			// opponent's move
			opponent_shoot(cfgp, player, tab_player, shot_opponent);
			hits_opponent = opponent_check_shot(cfgp, player, tab_player, shot_opponent);
			update_tab_player(cfgp, player, tab_player); 
			if(hits_opponent == 1)
				count_hits_opponent++;
								
			// player's move
			player_shoot(cfgo, opponent, tab_opponent, shot_player);
			hits_player = player_check_shot(cfgo, opponent, tab_opponent, shot_player); 
			update_tab_opponent(cfgo, opponent, tab_opponent);
			if(hits_player == 1)
				count_hits_player++;
					
			}while((count_hits_player < MAXSHIPS) && (count_hits_opponent < MAXSHIPS));
			
			// checks number of hits
			if(count_hits_opponent == MAXSHIPS)
				printf("\n----------------------------------------\n\nopponent won!\n");
		
			if(count_hits_player == MAXSHIPS)
				printf("\n----------------------------------------\n\nplayer won!\n");
					
			return 0;
	}
	
	if(choice == 2){
		printf("\nrules and specifications:");
		printf("\nthere are four types of ships, each of which only exists once;");
		printf("\n\t-> aircraft carrier (5);\n\t-> tanker (4);\n\t-> counter torpedoes (3);\n\t-> submarine (2);");
		printf("\nfor 'squares' there are no ships represented by '0' and in html by '~';");			
		printf("\ncomputer starts playing -> random row and column shooting;");	
		printf("\nplayer picks the row and column (1 to 10) he wants to shoot;");	
		printf("\n\t-> was correct and notified with the model of the ship;\n\t-> if you made a mistake and were notified that you were unsuccessful;");
		printf("\nwhen a 'square' of the ship is hit, only the 'square' is destroyed and not the entire ship;");
		printf("\nthe game ends when the number of hits equals the maximum number of ships (14).\n\n");	
		main();
	}
	
	else
		printf("\ninvalid option! ending naval battle ...\n");
}

void create_html_opponent(FILE *cfgo, FILE *opponent, char **tab_opponent_aux){
	// create opponent's initial html
	opponent = fopen("opponent.html","w+");
	fprintf(opponent, "<!DOCTYPE html><html><head><meta charset='unicode'><link rel='stylesheet' href='./style.css'><meta http-equiv='refresh' content='1'><title>opponent</title></head><body><h1>opponent</h1>");
	write_html_opponent(cfgo, opponent, tab_opponent_aux);
	fprintf(opponent, "</body></html>");
	fclose(opponent);
}

void create_html_player(FILE *cfgp, FILE *player, char **tab_player_aux){
	// create player's initial html
	player = fopen("player.html","w+");
	fprintf(player, "<!DOCTYPE html><html><head><meta charset='unicode'><link rel='stylesheet' href='./style.css'><meta http-equiv='refresh' content='1'><title>player</title></head><body><h1>player</h1>");
	write_html_player(cfgp, player, tab_player_aux);
	fprintf(player, "</body></html>");
	fclose(player);
}

void write_html_opponent(FILE *cfgo, FILE *opponent, char **tab_opponent_aux){
	// write opponent's initial html
	int i, j;
	cfgo = fopen("config_opponent.txt", "r");
	
	// read the matrix in the txt file
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++)
		fscanf(cfgo, " %c", &tab_opponent_aux[i][j]);
		
	}
	
	fprintf(opponent, "<table><tr>");
	for(i = 0; i <= ROW; i++){
		//table top
		fprintf(opponent, "<td>" "%d", i, "</td>");
	}
	fprintf(opponent, "</tr>");
	
	// print matrix in browser
	for(i = 1; i <= ROW; i++){
		fprintf(opponent, "<tr><td>""%d", i, "</td>");
		for(j = 1; j <= COLUMN; j++){
			// hide matrix
			fprintf(opponent, "<td>""~""</td>");
		}
	}
	fprintf(opponent, "</tr></table>");
	fclose(cfgo);
}

void write_html_player(FILE *cfgp, FILE *player, char **tab_player_aux){
	// write player's initial html
	int i, j;
	cfgp = fopen("config_player.txt", "r");
	
	// read the matrix in the txt file
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++)
		fscanf(cfgp, " %c", &tab_player_aux[i][j]);

	}
	
	fprintf(player, "<table><tr>");
	for(i = 0; i <= ROW; i++){
		// table top
		fprintf(player, "<td>" "%d", i, "</td>");
	}
	fprintf(player, "</tr>");
	
	// print matrix in browser
	for(i = 1; i <= ROW; i++){
		fprintf(player, "<tr><td>""%d", i, "</td>");
		for(j = 1; j <= COLUMN; j++){
			// hide matrix
			fprintf(player, "<td>""~""</td>");
		}
	}
	fprintf(player, "</tr></table>");
	fclose(cfgp);
}

void convert_opponent(char **tab_opponent_aux, int **tab_opponent){
	// convert char to integer
	int i, j;
	
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++){
			tab_opponent[i][j] = tab_opponent_aux[i][j];
			if(tab_opponent[i][j] >= '0' && tab_opponent[i][j] <= '5'){
				tab_opponent[i][j] -= '0'; 
			}
		}
	}
}

void convert_player(char **tab_player_aux, int **tab_player){
	// convert char to integer
	int i, j;

	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++){
			tab_player[i][j] = tab_player_aux[i][j];
			if(tab_player[i][j] >= '0' && tab_player[i][j] <= '5')
				tab_player[i][j] -= '0';
		}
	}
}

int *opponent_shoot(FILE *cfgp, FILE *player, int **tab_player, int *shot_opponent){
	// generate random number for opposing row and column shooting
	srand(time(NULL));
	// row
	shot_opponent[0] = 1+(rand()%10);
 	// column
	shot_opponent[1] = 1+(rand()%10);  
	
	return shot_opponent;
}

int *player_shoot(FILE *cfgo, FILE *opponent, int **tab_opponent, int *shot_player){
	// player tells you where you want to shoot
	// row
	printf("enter the row you want to shoot: ");
	scanf("%d", &shot_player[0]);
	// column
	printf("enter the column you want to shoot: ");
	scanf("%d", &shot_player[1]);

	return shot_player;
}

int opponent_check_shot(FILE *cfgp, FILE *player, int **tab_player, int *shot_opponent){
	// check if the opponent's shot hit any ships -> if yes returns 1, else 0 
	int aux_opponent;
	
	printf("opponent shot the row %d and column: %d\n", shot_opponent[0], shot_opponent[1]);
	
	if(tab_player[shot_opponent[0]][shot_opponent[1]] == 5){
		printf("\nopponent hit the aircraft carrier (5).\n----------------------------------------\n\n");
		aux_opponent = 1;
	}
	if(tab_player[shot_opponent[0]][shot_opponent[1]]  == 4){
		printf("\nopponent hit the tanker (4).\n----------------------------------------\n\n");
		aux_opponent = 1;
	}
	if(tab_player[shot_opponent[0]][shot_opponent[1]] == 3){
		printf("\nopponent hit the counter torpedoes (3).\n----------------------------------------\n\n");
		aux_opponent = 1;
	}
	if(tab_player[shot_opponent[0]][shot_opponent[1]] == 2){
		printf("\nopponent hit the submarine (2).\n----------------------------------------\n\n");
		aux_opponent = 1;
	}
	if(tab_player[shot_opponent[0]][shot_opponent[1]] == 0){
		printf("\nopponent missed the shot.\n----------------------------------------\n\n");
		aux_opponent = 2;
	}
	if(tab_player[shot_opponent[0]][shot_opponent[1]] == 1 || tab_player[shot_opponent[0]][shot_opponent[1]]  == 8)
		printf("\nopponent, this position is repeated!\n----------------------------------------\n\n");

	// change values in settings
	if(aux_opponent == 1){
		tab_player[shot_opponent[0]][shot_opponent[1]] = 1;
		return 1;
	}
	if(aux_opponent == 2){
		tab_player[shot_opponent[0]][shot_opponent[1]] = 8;
		return 0;
	}
}

int player_check_shot(FILE *cfgo, FILE *opponent, int **tab_opponent, int *shot_player){
	// check if the player's shot hit any ships -> if yes returns 1, else 0 
	int aux_player;
	
	printf("\nplayer shot the row: %d and column: %d\n", shot_player[0], shot_player[1]);
	
	if(tab_opponent[shot_player[0]][shot_player[1]] == 5){
		printf("\nplayer hit the aircraft carrier (5).\n----------------------------------------\n\n");
		aux_player = 1;
	}
	if(tab_opponent[shot_player[0]][shot_player[1]] == 4){
		printf("\nplayer hit the tanker (4).\n----------------------------------------\n\n");
		aux_player = 1;
	}
	if(tab_opponent[shot_player[0]][shot_player[1]] == 3){
		printf("\nplayer hit the counter torpedoes (3).\n----------------------------------------\n\n");
		aux_player = 1;
	}
	if(tab_opponent[shot_player[0]][shot_player[1]] == 2){
		printf("\nplayer hit the submarine (2).\n----------------------------------------\n\n");
		aux_player = 1;
	}
	if(tab_opponent[shot_player[0]][shot_player[1]] == 0){
		printf("\nplayer missed the shot.\n----------------------------------------\n\n");
		aux_player = 2;
	}
	if(tab_opponent[shot_player[0]][shot_player[1]] == 1 || tab_opponent[0][shot_player[1]] == 8)
		printf("\nplayer, this position is repeated!\n----------------------------------------\n\n");
	
	// change values in settings
	if(aux_player == 1){
		tab_opponent[shot_player[0]][shot_player[1]] = 1;
		return 1;
	}
	if(aux_player == 2){
		tab_opponent[shot_player[0]][shot_player[1]] = 8;
		return 0;
	}
}

void update_tab_player(FILE *cfgp, FILE *player, int **tab_player){
	// board upgrade, if opponent's shot hit ship changes to 1, else changes to 8
	int i, j;
	cfgp = fopen("config_player.txt", "r+");
	player = fopen("player.html", "r+");
	
	// saves the accurate shot in the opponent's configuration file
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++)
			fprintf(cfgp, "%d", tab_player[i][j]);
		fprintf(cfgp, "\n");
	}
	fprintf(player, "<!DOCTYPE html><html><head><meta charset='unicode'><link rel='stylesheet' href='./style.css'><meta http-equiv='refresh' content='1'><title>player</title></head><body><h1>player</h1>");
    
    // read matrix in txt file
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++)
			fscanf(cfgp, " %d", &tab_player[i][j]);
	}
	
	fprintf(player, "<table><tr>");
	for(i = 0; i <= ROW; i++){
		// table top
		fprintf(player, "<td>%d", i, "</td>");
	}
	fprintf(player, "</tr>");
	
	// print matrix in browser
	for(i = 1; i <= ROW; i++){
		fprintf(player, "<tr><td>%d", i, "</td>");
		for(j = 1; j <= COLUMN; j++){
			// hide the matrix when 0
			if(tab_player[i][j] != 8 && tab_player[i][j] != 1)
				fprintf(player, "<td>~</td>");

			// if opponent's shot hit a ship, change to 1, else change to 8
			if(tab_player[i][j] == 8 || tab_player[i][j] == 1)
				fprintf(player, "<td>%d</td>", tab_player[i][j]);
		}
	}
	fprintf(player, "</tr></table></body></html>");
	fclose(cfgp);
	fclose(player);
}

void update_tab_opponent(FILE *cfgo, FILE *opponent, int **tab_opponent){
	// board upgrade, if player's shot hit ship changes to 1, else changes to 8
	int i, j;
	cfgo = fopen("config_opponent.txt", "r+");
	opponent = fopen("opponent.html", "r+");
	
	// saves the accurate shot in the player's configuration file
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++)
			fprintf(cfgo, "%d", tab_opponent[i][j]);
		fprintf(cfgo, "\n");
	}
	fprintf(opponent, "<!DOCTYPE html><html><head><meta charset='unicode'><link rel='stylesheet' href='./style.css'><meta http-equiv='refresh' content='1'><title>opponent</title></head><body><h1>opponent</h1>");
    
    // read matrix in txt file
	for(i = 1; i <= ROW; i++){
		for(j = 1; j <= COLUMN; j++)
			fscanf(cfgo, " %d", &tab_opponent[i][j]);
	}
	
	fprintf(opponent, "<table><tr>");
	for(i = 0; i <= ROW; i++){
		// table top
		fprintf(opponent, "<td>%d", i, "</td>");
	}
	fprintf(opponent, "</tr>");
	
	// print matrix in browser
	for(i = 1; i <= ROW; i++){
		fprintf(opponent, "<tr><td>%d", i, "</td>");
		for(j = 1; j <= COLUMN; j++){
			// hide the matrix when 0
			if(tab_opponent[i][j] != 8 && tab_opponent[i][j] != 1)
				fprintf(opponent, "<td>~</td>");

			// if player's shot hit a ship, change to 1, else change to 8
			if(tab_opponent[i][j] == 8 || tab_opponent[i][j] == 1)
				fprintf(opponent, "<td>%d</td>", tab_opponent[i][j]);
		}
	}
	fprintf(opponent, "</tr></table></body></html>");
	fclose(cfgo);
	fclose(opponent);
}
