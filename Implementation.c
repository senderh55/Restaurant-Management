#include "Header.h"
void CreateProducts(FILE* in, FILE* out, PManot m, Ptables_manager arr)
//Option 1 - CreateProducts: scan from the file the dishes for the kitchen
{
	Product* p;
	int x1, x2;
	char temp[51];
	FILE* stack;
	stack = fopen("Manot.txt", "r");// list of stock in kitchen
	if (stack == NULL) // check if the text opened successfully
	{
		fclose(in); /* closing files */
		fclose(stack);
		free(arr); /* free array of manager table */
		Error_Msg("Failed opening the file.Exiting!", out); /* finish program */
	}

	while (fscanf(stack, "%s", temp) != EOF)
	{
		if (duplicate_check(m, temp))// if duplicate check == 1 , scan next product
		{
			fprintf(out, "The product %s is alredy exist!!\n", temp);
			fscanf(stack, "%d %d", &x1, &x2);
		}
		else
		{
			fscanf(stack, "%d %d", &x1, &x2); // check if price and quantity are positive
			if (x1 <= 0 || x2 <= 0)
				fprintf(out, "%s\n", "Price and Quantity must be positive");
			else
			{
				p = (Product*)malloc(sizeof(Product)); /* memory sallocation */
				if (p == NULL)
				{
					fclose(in);
					fclose(stack);
					DeleteKitchen(m); /* free linked list of stock kitchen */
					free(arr);
					Error_Msg("Memory error!!", out); /* finish program */
				}
				if (!(p->ProductName = (char*)malloc(strlen(temp) + 1 * sizeof(char)))) /* memory allocation for product name */
				{
					fclose(in);
					fclose(stack);
					DeleteKitchen(m);
					free(arr);
					Error_Msg("Memory error!!", out);
				}
				strcpy(p->ProductName, temp); /* put data in the node struct */
				p->Quantity = x1;
				p->Price = x2;
				p->popular = 0;
				product_list_update(p, m); // add the product to the list from tail
				m->num_of_products++; /* add count in number of products */
			}

		}
	}
	if (!(m->num_of_products))  fprintf(out, "%s\n", "The kitchen wasn't created. The stock is empty!");
	else fprintf(out, "%s\n", "The kitchen was created");
	fclose(stack); /* closing file to read */
}

void AddItems(char* st, int qu, PManot M, FILE* out)
// Option 2 :addItems - Add inventory to an existing dish in the kitchen
{
	Product* temp = M->head;
	int i;
	if (qu <= 0) /* if quantity is negative */
	{
		fprintf(out, "%s\n", "quntity must be positive");
		return;
	}
	else
	{
		for (i = 1; i < M->num_of_products; i++) /* Goes over the products on the list */
		{
			if (!strcmp(temp->ProductName, st)) //the product exist in current place in the list
			{
				temp->Quantity += qu;
				fprintf(out, "%d %s were added to the kitchen\n", qu, st);
				return;
			}
			else temp = temp->next; //go to next product in the kitchen
		}
	}
	fprintf(out, "%s weren't in kitchen list\n", st); // if the product isn't exist/
}


void OrderItem(int TableNumber, char* ProductName, int qu, FILE* in, FILE* out, PManot M, Ptables_manager pr)
/*Option 3:OrderItem function - Get order from the table , checks whether all received inputs are valid*/
{
	ordered* new;
	Product* dish = M->head;
	int i, dish_price = 0;

	if (TableNumber < 1 || TableNumber > pr->num_of_tables) //Checks if the table exists
	{
		fprintf(out, "Table number %d doesn't exist\n", TableNumber);
		return;
	}
	if (qu <= 0)   //Checks whether the quantity input is correct
	{
		fprintf(out, "%s\n", "quntity must be positive");
		return;
	}
	if (!duplicate_check(M, ProductName))    //Checks whether the product exists
	{
		fprintf(out, "We don't have %s, sorry!\n", ProductName);
		return;
	}

	for (i = 0; i < M->num_of_products; i++) // loop for the product
	{
		if (!strcmp(dish->ProductName, ProductName)) // the product were found
		{
			if (dish->Quantity < qu) // Checks whether the amount of the requested product exists in the kitchen
			{
				fprintf(out, "We dont have enough %s in our kitchen right now!\n", dish->ProductName);
				return;
			}
			dish->Quantity -= qu; // Reduce the quantity ordered from the existing quantity in the kitchen
			dish->popular += qu; //Calculates the total amount of the ordered product
			new = (ordered*)malloc(sizeof(ordered)); // Memory allocation for the new order
			if (new == NULL)
			{
				fclose(in);
				DeleteKitchen(M);
				for (i = 1; i < pr->num_of_tables; i++)
					deletetable(pr, i);
				Error_Msg("Memory error!!", out);
			}
			if (!(new->dish_name = (char*)malloc(strlen(ProductName) + 1 * sizeof(char))))
				// Memory allocation for the new order name
			{
				fclose(in);
				DeleteKitchen(M);
				for (i = 1; i < pr->num_of_tables; i++)
					deletetable(pr, i);
				Error_Msg("Memory error!!", out);
			}
			new->num_of_dishes = qu; // update the new order quntity
			strcpy(new->dish_name, ProductName); // update the new order dish name

			//link the node to the linked list 
			new->next = pr[TableNumber - 1].head;
			new->prev = NULL;
			if (pr[TableNumber - 1].head != NULL)
				pr[TableNumber - 1].head->prev = new;
			pr[TableNumber - 1].head = new;

			fprintf(out, "%d %s were added to the table number %d\n", qu, ProductName, TableNumber);
			pr[TableNumber - 1].head->price = dish->Price; // update the price of the dish
			pr[TableNumber - 1].bill += dish->Price * qu;  //update the bill of the table	
			return;
		}
		else dish = dish->next; //continue to look go to next product in the kitchen
	}
}



void RemoveItem(int TableNumber, char* ProductName, int qu, Ptables_manager pr, FILE* out)
//Option 4 RemoveItem function: Partially cancel order of items 
{
	ordered* temp;
	if (TableNumber< 1 || TableNumber > pr->num_of_tables) //Checks if the table exists
	{
		fprintf(out, "The table %d doesn't exist\n", TableNumber);
		return;
	}
	if (pr[TableNumber - 1].head == NULL) //Checks for open orders at the table
	{
		fprintf(out, "There are no open orders at table number %d\n", TableNumber);
		return;
	}
	temp = pr[TableNumber - 1].head;
	while (strcmp(temp->dish_name, ProductName))
		//Checks whether there is an order for the specific dish at the table
	{
		temp = temp->next;
		if (temp == NULL)
		{
			fprintf(out, "The dish %s doesn't exist in the table\n", ProductName);
			return;
		}
	}

	if (temp->num_of_dishes < qu) // Checks if try to return more dishes than the existing in table
	{
		fprintf(out, "There's no %d of %s on table number %d.\n", qu, ProductName, TableNumber);
		return;
	}

	pr[TableNumber - 1].bill -= (temp->price) * qu; //Refund according to the dishes returned
	temp->num_of_dishes -= qu; //Delete the relavent number of dishes from the table
	fprintf(out, "%d %s was returned to the kitchen from table number %d\n", qu, ProductName, TableNumber);

	if (temp->num_of_dishes == 0) // If there are no more dishes of this kind at the table, free the order memory
	{
		if (temp->prev == NULL && temp->next == NULL) /* only one recived item in the table */
		{
			pr[TableNumber - 1].head = NULL;
			free(temp->dish_name);
			free(temp);
		}
		else
		{
			if (temp->next != NULL) /* if not in the end in list item of the table */
				temp->next->prev = temp->prev;
			if (temp->prev == NULL) /* if the first item */
			{
				free(temp->dish_name);
				free(temp);
			}
			else
			{
				temp->prev->next = temp->next;
				free(temp->dish_name);
				free(temp);
			}
		}
	}

}
void RemoveTable(int TableNumber, Ptables_manager pr, FILE* out, PManot m)
//option 5 : RemoveTable - Close a table
{
	Product* p = m->head;
	char best_dish_name[51];
	int max = 0, i, opentables = 0;
	ordered* temp;
	if (TableNumber< 1 || TableNumber > pr->num_of_tables) /* invalid table number */
	{
		fprintf(out, "Table number %d doesn't exist!\n", TableNumber);
		return;
	}
	temp = pr[TableNumber - 1].head;
	if (pr[TableNumber - 1].head == NULL) //Table has no order
	{
		fprintf(out, "Table number %d is not ordered yet\n", TableNumber);
		return;
	}

	while (p != NULL) /* most popular dish */
	{
		if (p->popular > max)
		{
			max = p->popular;
			strcpy(best_dish_name, p->ProductName);
		}
		p = p->next;
	}

	while (temp != NULL)
	{
		fprintf(out, "%d %s. ", temp->num_of_dishes, temp->dish_name); /* prints invited products of table */
		temp = temp->next;
	}

	fprintf(out, "The bill for table number %d is %d nis, please!\n", TableNumber, pr[TableNumber - 1].bill); /* prints bill to table */
	for (i = 0; i < pr->num_of_tables; i++)
		if (pr[i].bill) opentables++;
	if (opentables == 1) /* if last table */
		fprintf(out, "The most ordered dish is %s! (was ordered %d times)\n", best_dish_name, max);
	deletetable(pr, TableNumber); // free table 
	pr[TableNumber - 1].bill = 0; /* reset bill to zero */
	opentables--; /* Reducing the number of tables */

}

void deletetable(Ptables_manager pr, int table) /*free only table */
{
	ordered* todel;
	while (pr[table - 1].head != NULL)
	{
		free(pr[table - 1].head->dish_name); /* free string */
		todel = pr[table - 1].head;
		pr[table - 1].head = pr[table - 1].head->next; /* promote in linked list of products that invited */
		if (pr[table - 1].head != NULL)
			pr[table - 1].head->prev = NULL;
		free(todel);
	}
}



void Error_Msg(char* msg, FILE* out)// print error messege and exit the program
{
	fprintf(out, "\n%s", msg);
	fclose(out);
	exit(1);
}

void DeleteKitchen(PManot m) // delete kitchen memory alloaction
{
	Product* temp;
	while (m->head != NULL)
	{

		temp = m->head;
		m->head = m->head->next; /* promote in linked list */
		free(temp->ProductName); /* free string */
		free(temp); /* free node */

	}

}
int duplicate_check(PManot m, char* st) //check if there is duplication of string
{
	Product* temp = m->head;
	while (temp != NULL)
	{
		if (strcmp(st, temp->ProductName)) /* 1 = if strings are not equal */
			temp = temp->next; /* promote in linked list */
		else
			return 1; /* if strings equal */
	}
	return 0; /* if strings are not equal */
}


void product_list_update(Product* p, PManot m) // add product to the list from last
{
	if (m->head == NULL) // if list is empty ,set head to this new node
		m->head = p;
	else
		m->tail->next = p; // link in the new mode to the end of the list
	m->tail = p; // set t the last node
	p->next = NULL; // set NULL for the end of the list
}

