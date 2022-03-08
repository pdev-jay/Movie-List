//
//  movie_review_dll.h
//  c_0223_002
//
//  Created by Junhyuek Kim on 2022/02/23.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RATING 10
#define MAX_MOVIE_NAME 30
#define MAX_DIRECTOR_NAME 20
#define MAX_GENRE_NAME 10

//enum for genre
enum GENRE{
    THRILLER = 1, COMEDY, ROMANCE, DRAMA, ACTION, FANTASY
}genre;

//Options when edit movie info
enum UPDATE{
    CANCEL, TITLE, DIRECTOR, GENRE
}update;

typedef struct MOVIE{
    char name[MAX_MOVIE_NAME];
    char director[MAX_DIRECTOR_NAME];
    char genre[MAX_GENRE_NAME];
    double ratingAvr;
    int participants;
    int ratingTotal;
    
    struct MOVIE *prev;
    struct MOVIE *next;
}movie_t;

void initMovieList(void); //Create head nod
void appendNode(movie_t* data); //Add movie info into memory
void printMovieList(void); //Print whole movies
void printCurrentMovie(movie_t *current); //Print a movie that current indicates
movie_t *newMovie(void); //Save movie info user entered in memory
void insertMovie(movie_t *data); //Insert saved movie info into movie list
void updateCurrentMovie(movie_t *current); //Change movie name, director name or genre
void ratingMovie(movie_t *current); //Give a rating to the cureent movie
movie_t* searchMovie(void); //Return the movie info address user searched by name
void deleteMovie(void); //Delete the movie info that current indicating

void sortingMovie(void); //Sort movies by rating average
int nameCheck(char *name); //Check if there is the same name
void removeBlank(char *name); //Remove blank in movie name or director name

movie_t *head = NULL;
movie_t *current = NULL;

//Create head node
void initMovieList(){
    head = (movie_t*)calloc(1, sizeof(movie_t));
    if (head == NULL) {
        printf("\nFailed to load\n");
        getchar();
    }
    return;
}

//Save data from file to linked list
void appendNode(movie_t* data){
    movie_t* p = head;
    
    //Make p indicate the last node
    while (p -> next != NULL) {
        p = p -> next;
    }
    //Link the last node and a new data
    p -> next = data;
    
    if (p != head) {
        data -> prev = p;
    }
    
    return;
}

void printMovieList(){
    movie_t* p = head;
    
    puts("----------------------------------------------------");//52
    printf("%-30s %-10s     %-s\n", "Title", "Genre", "Rating");
    puts("----------------------------------------------------");
    
    if (p -> next == NULL) {
        printf("%28s\n", "Empty");
        puts("----------------------------------------------------");
        return;
    }
    
    while (p -> next != NULL) {
        p = p -> next;
        printf("\n%-30s %-10s     %6.1lf\n\n%s %-20s %19d\n", p -> name, p -> genre, p -> ratingAvr, "Directed by", p -> director, p -> participants);
        
        puts("\n----------------------------------------------------");
        
    }
    
    return;
}

void printCurrentMovie(movie_t *current){
    if (current == NULL) {
        puts("----------------------------------------------------");
        printf("%28s\n", "Empty");
        puts("----------------------------------------------------");
    } else {
        puts("----------------------------------------------------");
        printf("%-10s %s\n","Title", current -> name);
        printf("\n%-10s %s\n","Director", current -> director);
        printf("\n%-10s %s\n","Genre", current -> genre);
        printf("\n%-10s %.1lf\n","Rating", current -> ratingAvr);
        printf("\n%-10s %d\n", "Votes", current -> participants);
        puts("----------------------------------------------------\n");

    }
    return;
}

//Compare name user entered with names in movie list
//If there is the same name, return 1
int nameCheck(char *name){
    int result = 0;
    movie_t *p = head;
    
    while (p -> next != NULL) {
        p = p -> next;
        
        if (!strcmp(p -> name, name)){
            result = 1;
            break;
        }
    }
    return result;
}

movie_t *newMovie(){
    movie_t *data = (movie_t*)calloc(1, sizeof(movie_t));
    
    int flag = 0;
    int secondFlag = 0;
    char name[MAX_MOVIE_NAME] = "";
    int check = 0;
    //Check memory allocating
    if (data == NULL){
        puts("Failed to add a new movie");
        getchar();
    }
    //Enter movie name
    while (!flag) {
        printf("\nEnter movie title : ");
        scanf("%[^\n]s", name);
        
        removeBlank(name);
        check = nameCheck(name);
        
        if (check == 1) {
            printf("\n[%s] already exists", name);
            continue;
        }
        
        strcpy(data -> name, name);
        flag = 1;
    }
    fflush(stdin);
    //Enter director name
    printf("\nEnter director's name : ");
    scanf("%[^\n]s", data -> director);
    fflush(stdin);
    removeBlank(data -> director);
    
    //Choose genre
    while (!secondFlag) {
        puts("\n1. THRILLER \t2. COMEDY \t3. ROMANCE \t4. DRAMA \t5. ACTION \t6. FANTASY");
        printf("\nChoose genre : ");
        scanf("%d", &genre);
        fflush(stdin);
        
        switch (genre) {
            case THRILLER:
                strcpy(data -> genre, "Thriller");
                break;
            case COMEDY:
                strcpy(data -> genre, "Comedy");
                break;
            case ROMANCE:
                strcpy(data -> genre, "Romance");
                break;
            case DRAMA:
                strcpy(data -> genre, "Drama");
                break;
            case ACTION:
                strcpy(data -> genre, "Action");
                break;
            case FANTASY:
                strcpy(data -> genre, "Fantasy");
                break;
            default:
                puts("Please choose again");
                getchar();
                continue;
        }
        secondFlag = 1;
    }
    //Rating
    ratingMovie(data);

    return data;
}

void insertMovie(movie_t *data){
    
    //Add into the first node when no data
    if (head -> next == NULL) {
        head -> next = data;
        current = data;
        return;
    }
    
    if (current -> prev == NULL) { //Insert at the first node (head -> next)
        data -> next = head -> next;
        data -> next -> prev = data;
        head -> next = data;
    } else if (current -> next == NULL){ //Add next to the last node (current(last)) -> nexe)
        data -> prev = current;
        current -> next = data;
    } else if (current -> next != NULL && current -> prev != NULL){ //Insert next to current(current -> next) if current is in the middle of list
        data -> next = current -> next;
        data -> next -> prev = data;
        current -> next = data;
        data -> prev = current;
    } else {
        puts("Failed to add a new movie");
        getchar();
    }
    return;
}

void updateCurrentMovie(movie_t *current){
    int flag = 0;
    int secondFlag = 0;
    int check = 0;
    char name[MAX_MOVIE_NAME] = "";
    
    if (current == NULL) {
        puts("\nNo data exists");
        getchar();
        return;
    }
    
    while (!flag) {
        system("clear");
        puts("----------------------------------------------------");
        printf("%32s\n", "Movie Info");
        printCurrentMovie(current);
        printf("%-15s %-15s %-15s\n", "1. Title", "2. Director", "3. Genre");
        printf("\n0. Back\n");
        
        printf("\nChoose one to edit : ");
        scanf("%d", &update);
        fflush(stdin);
        //Select what to edit
        switch (update) {
            case TITLE:
                printf("\nChange [%s] to : ", current -> name);
                scanf("%[^\n]s", name);
                fflush(stdin);
                removeBlank(name);
                check = 0;
                check = nameCheck(name);
                if (check == 1) {
                    printf("\n[%s] already exists", name);
                } else {
                    strcpy(current -> name, name);
                }
                
                break;
                
            case DIRECTOR:
                printf("\nChange [%s] to : ", current -> director);
                scanf("%[^\n]s", current -> director);
                fflush(stdin);
                removeBlank(current -> director);
                
                break;
                
            case GENRE:
                while (!secondFlag) {
                    
                    puts("\n1. THRILLER \t2. COMEDY \t3. ROMANCE \t4. DRAMA \t5. ACTION \t6. FANTASY");
                    printf("\nChange [%s] to : ", current -> genre);
                    scanf("%d", &genre);
                    fflush(stdin);
                    
                    switch (genre) {
                        case THRILLER:
                            strcpy(current -> genre, "Thriller");
                            break;
                        case COMEDY:
                            strcpy(current -> genre, "Comedy");
                            break;
                        case ROMANCE:
                            strcpy(current -> genre, "Romance");
                            break;
                        case DRAMA:
                            strcpy(current -> genre, "Drama");
                            break;
                        case ACTION:
                            strcpy(current -> genre, "Action");
                            break;
                        case FANTASY:
                            strcpy(current -> genre, "Fantasy");
                            break;
                        default:
                            puts("\nPlease choose again");
                            getchar();
                            continue;
                    }
                    secondFlag = 1;
                }
                break;
                
            case CANCEL:
                flag = 1;
                break;
        }
    }
    return;
}

void ratingMovie(movie_t *current){
    int flag = 0;
    int rating = 0;
    
    if (current == NULL) {
        puts("\nNo data exists");
        getchar();
        return;
    }
    
    while (!flag) {
        printf("\nGive a rating (0~ 10) : ");
        scanf("%d", &rating);
        fflush(stdin);
        if (rating < 0 || rating > MAX_RATING) {
            puts("\nPlease enter a number between 0 and 10");
            continue;
        }
        current -> ratingTotal += rating;
        current -> participants++;
        current -> ratingAvr = (double)current -> ratingTotal / current -> participants;
        flag = 1;
    }
    
    
}

void removeBlank(char *name){
    int i = 0;
    for (i = 0; i < strlen(name); i++) {
        if (name[i] == ' ') {
            name[i] = '_';
        }
    }
    return;
}

movie_t* searchMovie(){
    movie_t* p = head;
    movie_t* data = NULL;
    
    char name[MAX_MOVIE_NAME] = "";
    
    printf("\nPlease enter movie title (case and space sensitive) : ");
    scanf("%[^\n]s", name);
    
    removeBlank(name);
    //Search through list
    while (p -> next != NULL) {
        p = p -> next;
        if (!strcmp(p -> name, name)) {
            data = p;
            break;
        }
    }
    
    if (data == NULL) {
        printf("\nCould not find [%s]\n", name);
        getchar();
    }
    
    return data;
}

void deleteMovie(){
    //
    movie_t *p = current;
    
    //The address current is pointing will be deleted
    //Make a copy of current and move current
    if (current -> next != NULL) {
        current = current -> next;
    } else if (current -> next == NULL && current -> prev != NULL){// When current is the last node
        current = current -> prev;
    } else {
        current = NULL;
    }
    
    if (p -> next != NULL && p -> prev == NULL) {//When p is first node
        p -> next -> prev = NULL;
        head -> next = p -> next;
    } else if (p -> next != NULL && p -> prev != NULL){//When p is in the middle of list
        p -> next -> prev = p -> prev;
        p -> prev -> next = p -> next;
    } else if (p -> next == NULL && p -> prev != NULL){//When p is the last node
        p -> prev -> next = NULL;
    } else {//When p is the first node and the only one
        head -> next = NULL;
    }
    
    free(p);//Deallocate p
    return;
}

void sortingMovie(){
    movie_t *p1 = head;//For selection sort
    movie_t *p2 = NULL;//For selection sort
    movie_t temp = {0,};//For swap
    
    if (p1 -> next == NULL){
        puts("\nThe list is empty");
        getchar();
        return;
    }
    while (p1 -> next != NULL) {
        //From the first node to the end
        p1 = p1 -> next;
        
        if (p1 -> next == NULL){//Break when p1 reaches the last node
            break;
        }
        
        p2 = p1;

        while (p2 -> next != NULL) {
            //From p1 + 1(node) to the end
            p2 = p2 -> next;
            
            if (p1 -> ratingAvr < p2 -> ratingAvr){//Descending order
                //Swap name
                strcpy(temp.name, p1 -> name);
                strcpy(p1 -> name, p2 -> name);
                strcpy(p2 -> name, temp.name);
                //Swap director name
                strcpy(temp.director, p1 -> director);
                strcpy(p1 -> director, p2 -> director);
                strcpy(p2 -> director, temp.director);
                //Swap genre
                strcpy(temp.genre, p1 -> genre);
                strcpy(p1 -> genre, p2 -> genre);
                strcpy(p2 -> genre, temp.genre);
                //Swap rating average
                temp.ratingAvr = p1 -> ratingAvr;
                p1 -> ratingAvr = p2 -> ratingAvr;
                p2 -> ratingAvr = temp.ratingAvr;
                //Swap number of participants
                temp.participants = p1 -> participants;
                p1 -> participants = p2 -> participants;
                p2 -> participants = temp.participants;
                //Swap total rating
                temp.ratingTotal = p1 -> ratingTotal;
                p1 -> ratingTotal = p2 -> ratingTotal;
                p2 -> ratingTotal = temp.ratingTotal;
            }
        }
    }
}
