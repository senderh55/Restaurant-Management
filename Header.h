#ifndef _main
#define _main
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Product {   //products stracture
	char* ProductName;
	int Price;
	int Quantity;
	int popular;
	struct Product* next;
}Product;

typedef struct Manot {    //kitchen manager structure
	Product* head, *tail;  
	int num_of_products;
}Manot, * PManot;



typedef struct ordered { // Table reservation Structure
	char* dish_name;
	int num_of_dishes;
	struct ordered * next,*prev;
	int	price;
}ordered;

typedef struct tables_manager // tables manager structure
{
	ordered* head;
	int bill;
	int num_of_tables;
}tables_manager, * Ptables_manager;


void CreateProducts(FILE* in, FILE* out, PManot m, Ptables_manager arr);
//Option 1 - CreateProducts: scan from the file the dishes for the kitchen
void AddItems(char* st, int qu, PManot M, FILE* out);
// Option 2 :addItems - Add inventory to an existing dish in the kitchen
void OrderItem(int TableNumber, char* ProductName, int qu, FILE* in, FILE* out, PManot M, Ptables_manager pr);
/*Option 3:OrderItem function - Get order from the table , checks whether all received inputs are valid*/
void RemoveItem(int TableNumber, char* ProductName, int qu, Ptables_manager pr, FILE* out);
//Option 4 RemoveItem function: Partially cancel order of items 
void RemoveTable(int TableNumber, Ptables_manager pr, FILE* out, PManot m);
//option 5 : RemoveTable - Close a table
void deletetable(Ptables_manager pr, int table); //free only table 
void Error_Msg(char* msg, FILE* out);// print error messege and exit the program
void DeleteKitchen(PManot m); // delete kitchen memory alloaction
int duplicate_check(PManot m, char* st); //check if there is duplication of string
void product_list_update(Product* p, PManot m); // add product to the list from last
#endif 