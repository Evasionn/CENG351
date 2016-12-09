#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define productDB "products.txt"


typedef struct productStruct {
	char name[30];
	int quantity;
	int barcode;
	double price;
}Product;

typedef struct {
	Product * database;
	int size;
} ProductList;

Product product; // a global variable


void save(ProductList * db) {
	FILE * database = fopen(productDB, "w+");
	
	for (int i = 0; i < db->size; i++) {
		fprintf(database, "%d %s %d %.2lf\n", db->database[i].barcode, db->database[i].name, db->database[i].quantity, db->database[i].price);
	}
	fclose(database);
}

int isNumber(char * str, int i = 0) {
	if (str[i] == '\0') return 1;
	if (isdigit(str[i]) || str[i] == '.') isNumber(str, i + 1);
	else return 0;
}



void createDatabaseTxt(char * database) {
	FILE * productList = fopen(database, "r");
	if (productList == NULL) {
		productList = fopen(database, "w+");
	}

	fclose(productList);
}


int isExist(ProductList * db, int barcode) {
	int index;
	for (index = 0; index< db->size; index++)
		if (db->database[index].barcode == barcode) return index;
	return -1;
}
int isExist(ProductList * db, char * productName) {
	int index;
	for (index = 0; index< db->size; index++)
		if (strcmp(db->database[index].name, productName) == 0) return index;
	return -1;
}
Product getProductFromDB(ProductList * db, int index) {
	return db->database[index];

}



void createProduct(ProductList * db, char * name, int quantity, int barcode, double price) {
	Product newProduct;
	newProduct.barcode = barcode;
	newProduct.quantity = quantity;
	newProduct.price = price;
	strcpy(newProduct.name, name);
	db->database = (Product *)realloc(db->database, sizeof(Product)*(db->size + 1));
	db->database[db->size] = newProduct;
	db->size++;
}

void deleteProductList(ProductList * db, int index) {
	db->database[index] = db->database[db->size - 1];
	db->database = (Product *)realloc(db->database, sizeof(Product) * (db->size - 1));
	db->size--;
}

void updateProductList(ProductList * db, int index, int updatedValue) {
	if (updatedValue == 0) {
		deleteProductList(db, index);
	}

	db->database[index].quantity = updatedValue;
}


int numberOfRecord(ProductList * db) {
	return db->size;
}

ProductList * createDB() {
	ProductList * db = (ProductList *)malloc(sizeof(ProductList));

	db->database = NULL;
	db->size = 0;
	return db;
}

void txtToDB(ProductList * db) {
	FILE * database = fopen(productDB, "r");
	while (fscanf(database, "%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
		db->database = (Product*)realloc(db->database, sizeof(Product)*(db->size + 1));
		db->database[db->size].barcode = product.barcode;
		db->database[db->size].quantity = product.quantity;
		db->database[db->size].price = product.price;
		strcpy(db->database[db->size].name, product.name);
		db->size++;
	}
	fclose(database);
}



void listAllProducts(ProductList * db) {
	printf("\n\nBARCODE NUMBER\tPRODUCT NAME\tQUANTITY\tPRICE\n\n");
	printf("-----------------------------------------------------------\n\n");
	for (int i = 0; i < db->size; i++) {
		printf("%d\t\t%s\t\t%d\t\t%.2lf TL\n\n", db->database[i].barcode, db->database[i].name, db->database[i].quantity, db->database[i].price);
		printf("-----------------------------------------------------------\n\n");
	}
	printf("\nWe found %d result(s).\n\n", numberOfRecord(db));

}
int amountComparison(int value, int value2) {
	if (value >= value2) return 1;
	return 0;
}


void sellProduct(ProductList * db) {
	int barcode, row, amount;
	Product result;
	char buffer[255];

	listAllProducts(db);
	while (1) {
		printf("Enter the barcode number of product that you want to buy: ");
		scanf("%s", buffer);
		if (isNumber(buffer)) { sscanf(buffer, "%d", &barcode); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}

	row = isExist(db, barcode);
	if (row == -1) {
		printf("\n\nThe product hasn't been found\n\n");
		return;
	}

	result = getProductFromDB(db, row);
	while (1) {
		printf("Enter the amount of quantity that you want to buy: ");
		scanf("%s", buffer);
		if (isNumber(buffer)) { sscanf(buffer, "%d", &amount); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}

	if (amountComparison(result.quantity, amount)) {
		printf("\n\nTotal Price: %.2lf TL. \n\n Do you want to buy? (Y/N)\n\n", (amount *result.price));
		fflush(stdin);
		scanf("%s", buffer);

		if (buffer[0] == 'y' || buffer[0] == 'Y') {
			updateProductList(db, row, result.quantity - amount);
			save(db);
		}
		else if (buffer[0] == 'n' || buffer[0] == 'N') { fflush(stdin); return; }

	}
	else {
		printf("\n\nWe don't have enough amount of quantity ( %d products on stock )\n\n", result.quantity);

	}
}



void printRecord(Product record) {
	printf("\n\nBarcode Number: %d\nProduct Name: %s\nQuantity: %d\nPrice: %.2lf TL\n\n\n", record.barcode, record.name, record.quantity, record.price);
}

void search(ProductList * db) {
	int barcode;
	Product result;
	char buffer[255];
	while (1) {
		printf("Enter the barcode number: ");
		scanf("%s", buffer);
		if (isNumber(buffer)) { sscanf(buffer, "%d", &barcode); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}
	int row = isExist(db, barcode);

	if (row == -1) {
		printf("\n\nThere is not any product which has the barcode number %d\n\n\n", barcode);
	}
	else {
		printf("\n\n-----------------------------------------------------------");
		result = getProductFromDB(db, row);
		printRecord(result);
		printf("-----------------------------------------------------------\n\n");
	}
}




void addMenu(ProductList * db) {
	Product inputs, result;
	char  buffer[255];

	while (1) {
		printf("Enter the barcode number: ");
		scanf("%s", buffer);
		if (isNumber(buffer)) { sscanf(buffer, "%d", &inputs.barcode); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}
	int row = isExist(db, inputs.barcode);
	if (row == -1) {

		printf("Enter the name of the product: ");
		scanf("%s", inputs.name);
		int row2 = isExist(db, inputs.name);
		if (row2 != -1)
		{
			printf("\n There is already a product which is named %s on database", inputs.name);
			return;
		}
		while (1) {
			printf("Enter the amount of quantity: ");
			scanf("%s", buffer);
			if (isNumber(buffer)) { sscanf(buffer, "%d", &inputs.quantity); break; }
			else printf("\n\n You must enter integer value!\n\n");
		}
		while (1) {
			printf("Enter the price of the product: ");
			scanf("%s", buffer);
			if (isNumber(buffer)) { sscanf(buffer, "%lf", &inputs.price); break; }
			else printf("\n\n You must enter double value!\n\n");
		}
		createProduct(db, inputs.name, inputs.quantity, inputs.barcode, inputs.price);
		save(db);

	}
	else {
		result = getProductFromDB(db, row);
		while (1) {
			printf("Enter the amount of quantity: ");
			scanf("%s", buffer);
			if (isNumber(buffer)) { sscanf(buffer, "%d", &inputs.quantity); break; }
			else printf("\n\n You mush enter integer value!\n\n");
		}
		updateProductList(db, row, (inputs.quantity + result.quantity));
		save(db);
	}
}
int mainMenu(ProductList * db) {
	char buffer[255];
	int choice;
	puts("1. List all product");
	puts("2. Add a new product");
	puts("3. Sell the product");
	puts("4. Search the product according to barcode number");
	puts("5. Exit");
	printf("Your choice is: ");

	scanf("%s", buffer);
	if (isNumber(buffer))
		sscanf(buffer, "%d", &choice);
	else return -1;


	return choice;

}


int main(void) {
	ProductList * DB = createDB();
	createDatabaseTxt(productDB);

	txtToDB(DB);

	int select = -1;
	while (select != 5) {
		fflush(stdin);
		select = mainMenu(DB);
		if (select == 1) listAllProducts(DB);
		else if (select == 2)addMenu(DB);
		else if (select == 3) sellProduct(DB);
		else if (select == 4) search(DB);
		else if (select == 5) exit(0);
		else fflush(stdin);
	}



	return 0;
}