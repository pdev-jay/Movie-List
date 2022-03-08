//
//  main.c
//  c_0223_002
//
//  Created by Junhyuek Kim on 2022/02/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "movie_review_dll.h"
#define FILE_NAME "movie_review.txt"

//Main menu select options
enum MAIN_MENU{
    END, MANAGE, SORTING, WRITE
}mainMenu;

//Edit menu select options
enum EDIT_MENU{
    BACK, PREV, NEXT, RATING, INSERT, MODIFY, DELETE, SEARCH
}editSelect;

void readMovieList(void); //Read movie list file
void saveMovieList(void); //Save movie list to file
void manageMovie(void); //Manage movies (navigate, rating, add, delete, update, search)

int main(int argc, const char * argv[]) {
    int flag = 0;
    
    initMovieList();
    readMovieList();
    
    while (!flag) {
        system("clear");
        
        puts("----------------------------------------------------");
        printf("%31s\n", "Movie list");
        printMovieList();
        printf("\n");
        puts("1. Manage movie list\n");
        puts("2. Sort by rating\n");
        puts("3. Save\n");
        puts("0. Exit");
        printf("\n>> ");
        scanf("%d", &mainMenu);
        getchar();
        
        switch (mainMenu) {
            case MANAGE:
                manageMovie(); //Go to management page
                break;
            case SORTING:
                sortingMovie(); //Sorting movie list by rating
                break;
            case WRITE:
                saveMovieList(); //Save movie list to file
                break;
            case END:
                flag = 1;
                break;
            default:
                puts("\nPlease choose again");
                getchar();
                
                break;
        }
    }
    return 0;
}


void readMovieList(){
    movie_t *data = NULL;
    
    FILE* file = NULL;
    int flag = 0;
    
    file = fopen(FILE_NAME, "rt"); //Open file in rt(read, text) mode
    
    if (file == NULL) {
        printf("\nFailed to load %s file\n", FILE_NAME);
        return;
    }
    
    while (!flag) {
        data = (movie_t*)calloc(1, sizeof(movie_t)); //Allocating memory for data read from file
        
        int result = fscanf(file, "%s %s %s %lf %d %d", data -> name, data -> genre, data -> director, &data -> ratingAvr, &data -> ratingTotal, &data -> participants); //Read data from file by its element
        //fscan reads a set of data and moves cursor at the end of the set
        //Returns -1(EOF) when file ends
        
        if (result == EOF){
            flag = 1;
            continue;
        }
        appendNode(data);
    }
    
    fclose(file);
    return;
}

void saveMovieList(){
    movie_t* p = head;
    FILE* file = NULL;
    int count = 0;
    
    
    file = fopen(FILE_NAME, "wt"); //Open file in wt(write, text) mode
    
    if (file == NULL) {
        printf("\nFailed to save %s file\n", FILE_NAME);
        
        return;
    }
    
    while (p -> next != NULL) {
        p = p -> next;
        int result = fprintf(file, "%s %s %s %.1lf %d %d ", p -> name, p -> genre, p -> director, p -> ratingAvr, p -> ratingTotal, p -> participants);
        //Write data from memory to file
        if (result == EOF){
            break;
        }
        count++;
    }
    
    printf("\nSuccessfully saved %d data\n", count);
    
    fclose(file);
    getchar();
    
}

void manageMovie(){
    int flag = 0;
    movie_t *data = NULL;
    current = head -> next; // First location where data exist
    
    while (!flag && head != NULL) {
        system("clear");
        puts("----------------------------------------------------");
        printf("%32s\n", "Movie review");

        
        printCurrentMovie(current);
        
        printf("%-30s", "1. Previous");
        printf("2. Next\n");
        printf("\n%-30s", "3. Give ratings");
        printf("4. Add new movie\n");
        printf("\n%-30s", "5. Edit movie info");
        printf("6. Delete\n");
        printf("\n%-30s", "7. Search");
        printf("\n\n0. Back\n");
        printf("\n>> ");
        scanf("%d", &editSelect);
        getchar();
        
        //Disable PREV, NEXT, RATING, MODIFY, DELETE, SEARCH when no data
        if (current == NULL) {
            switch (editSelect) {
                case PREV:
                case NEXT:
                case RATING:
                case MODIFY:
                case DELETE:
                case SEARCH:
                    continue;
            }
        }
        
        switch (editSelect) {
            case PREV:
                if (current -> prev != NULL) {
                    current = current -> prev; //Move to the previous data
                }
                break;
            case NEXT:
                if (current -> next != NULL) {
                    current = current -> next; //Move to the next data
                }
                break;
            case RATING:
                ratingMovie(current); //Give a rating to the current data(movie)
                break;
            case INSERT:
                data = newMovie(); //Get new movie info from user and save to memory
                insertMovie(data); //Insert new movie in movie list
                
                break;
            case MODIFY:
                
                updateCurrentMovie(current); //Change movie name, director name or genre
                
                break;
            case DELETE:
                deleteMovie(); //Delete current movie info
                
                break;
            case SEARCH:
                data = searchMovie(); //Find movie by name and return address
                if (data != NULL) {
                    current = data; //current pointing at the adress -> printCurrentMovie will show searched movie
                }
                break;
            case BACK:
                flag = 1;
                break;
                
            default:
                puts("Please choose again");
                getchar();
                break;
        }
    }
    
}

