#include "Header.h"
void main()
{
	Manot kitchen;
	int tableNum = 0, numOfTables, option = 0, qun = 0, i;
	tables_manager* TableRep;
	ordered* temp;
	char dish_name[51];
	FILE* in, * out;
	in = fopen("Instructions.txt", "r"); // open file to read
	if (in == NULL) // if failed in openning file
	{
		printf("Failed to open instructions.txt \n");
		exit(1); /*finish program*/
	}
	out = fopen("output.txt", "w"); /*open file to write*/
	if (out == NULL) // if failed in openning file
	{
		printf("Failed to open output.txt \n");
		fclose(in);
		exit(1);
	}
	fscanf(in, "%d", &numOfTables); /*scan number of tables*/
	if (numOfTables <= 0)
	{
		fclose(in);
		Error_Msg("Wrong input , the number of tables must be positive !\n", out);
	}
	if (!(TableRep = (tables_manager*)malloc(sizeof(tables_manager) * numOfTables)))  /*memory allocation for array of manager struct*/
	{
		fclose(in);
		Error_Msg("Not enough memory!!", out);
	}
	TableRep->num_of_tables = numOfTables;
	for (i = 0; i < TableRep->num_of_tables; i++) /*reset heads and bills in the array of manager struct */
	{
		TableRep[i].head = NULL;
		TableRep[i].bill = 0;
	}

	kitchen.head = NULL;  /*reset  head*/
	kitchen.tail = NULL;   /*reset tail*/
	kitchen.num_of_products = 0;  /*reset number of products in the kitchen*/

	while (fscanf(in, "%d", &option) != EOF)    /*while the instruction isn't finished*/
	{
		if (option == 1)
			CreateProducts(in, out, &kitchen, TableRep);
		if (option == 2)
		{
			fscanf(in, "%s %d", dish_name, &qun);
			AddItems(dish_name, qun, &kitchen, out);
		}
		if (option == 3)
		{
			fscanf(in, "%d %s %d", &tableNum, dish_name, &qun);
			OrderItem(tableNum, dish_name, qun, in, out, &kitchen, TableRep);
		}
		if (option == 4)
		{
			fscanf(in, "%d %s %d", &tableNum, dish_name, &qun);
			RemoveItem(tableNum, dish_name, qun, TableRep, out);
		}
		if (option == 5)
		{
			fscanf(in, "%d", &tableNum);
			RemoveTable(tableNum, TableRep, out, &kitchen);
		}
	}

	for (i = 1; i <= numOfTables; i++)  /*free tables  (the invited products to table)*/
		deletetable(TableRep, i);
	free(TableRep);   /*free array*/
	DeleteKitchen(&kitchen);   /*free linked list of stock in the kitchen*/

	fclose(in);
	fclose(out);
}



