#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define productDB "products.txt"
#define MAXSIZE 10

typedef struct productStruct {
    char name[30];
    int quantity;
    int barcode;
    double price;
}Product;

Product product;


int isNumber(char * str,int i=0){
	if(str[i]=='\0') return 1;
	if(isdigit(str[i]) || str[i]=='.') isNumber(str,i+1);
	else return 0;
}



FILE * databaseOpen(char * database) {
    FILE * productList = fopen(database, "r");
    if (productList == NULL) {
        productList = fopen(database, "w+");
    }
    else {
        fclose(productList);
        productList = fopen(database, "a+");
    }

    if (productList!=NULL)
    return productList;
    else {
        printf("A problem has been occured while reading database");
        exit(0);
    }
}

int isExist(FILE * database, int barcode) {

    int count = 0, check = -1;
    rewind(database);
    while (fscanf(database,"%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
        if (barcode == product.barcode) {
            check = 1;
            break;
        }
        count++;
    }
    if (check != -1) return count;
    return check;
}
int isExist(FILE * database, char * productName){
	int count = 0 , check=-1;
	rewind(database);
	while (fscanf(database,"%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
		if(strcmp(product.name,productName)==0) {
			check=1;
			break;
		}
		count++;
}
	if(check!=-1) return count;
	return check;
}
Product getProductFromDB(FILE * database, int row) {
    int count = 0;
    Product result;
    rewind(database);
    while (fscanf(database, "%d %s %d %lf", &result.barcode, result.name, &result.quantity, &result.price) != EOF) {
        if (count == row) break;
    }
    return result;
}



int createProduct(FILE * database, char * name, int quantity, int barcode, double price) {
    fseek(database, 0, SEEK_END);
    if (fprintf(database, "%d %s %d %.2lf\n", barcode, name, quantity, price)) {
        printf("\n\nSuccess!\n\n");
        return 1;
    }
    else {
        printf("\n\nFailed!\n\n");
        return 0;
    }
}

FILE * deleteProductList(FILE * database, int row) {
    FILE * temp = fopen("temp.txt", "w+");
	rewind(database);
    int count = 0;
    while (fscanf(database, "%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
        if (count != row) fprintf(temp, "%d %s %d %.2lf\n", product.barcode, product.name, product.quantity, product.price);
        count++;
    }

    fclose(temp);
    fclose(database);
    remove(productDB);
    rename("temp.txt", productDB);
    temp = databaseOpen(productDB);
    return temp;
}

FILE * updateProductList(FILE * database, int row, int updatedValue) {
    if(updatedValue==0) {
        database=deleteProductList(database,row);
        return database; 
	}

    FILE * temp = fopen("temp.txt", "w+");
    int count = 0;
    rewind(database);
    while (fscanf(database, "%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
    if (count != row) fprintf(temp, "%d %s %d %.2lf\n", product.barcode, product.name, product.quantity, product.price);
    else fprintf(temp, "%d %s %d %.2lf\n", product.barcode, product.name, updatedValue, product.price);
    count++;
}

    fclose(temp);
    fclose(database);
    remove(productDB);
    rename("temp.txt", productDB);
    temp = databaseOpen(productDB);
    return temp;
}

int numberOfRecord(FILE * database) {
    int count = 0;
    rewind(database);
    Product product;

    while (fscanf(database, "%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
        count++;
    }

    return count;
}

void listAllProducts(FILE * database) {
    rewind(database);
    printf("\n\nBARCODE NUMBER\tPRODUCT NAME\tQUANTITY\tPRICE\n\n");
    printf("-----------------------------------------------------------\n\n");
    while (fscanf(database, "%d %s %d %lf", &product.barcode, product.name, &product.quantity, &product.price) != EOF) {
        printf("%d\t\t%s\t\t%d\t\t%.2lf TL\n\n", product.barcode, product.name, product.quantity, product.price);
        printf("-----------------------------------------------------------\n\n");
    }
    printf("\nWe found %d result(s).\n\n", numberOfRecord(database));
}

int amountComparison(int value, int value2) {
    if (value >= value2) return 1;
    return 0;
}


FILE * sellProduct(FILE * database) {
    int barcode,row,amount;
    Product result;
    char buffer[255];
	
    listAllProducts(database);
	while(1){
       printf("Enter the barcode number of product that you want to buy: ");
        scanf("%s", buffer);
		if(isNumber(buffer)) { sscanf(buffer,"%d",&barcode); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}

    row = isExist(database, barcode);
    if (row==-1) {
        printf("\n\nThe product hasn't been found\n\n");
        return database;
    }

    result = getProductFromDB(database, row);
    while(1){
      printf("Enter the amount of quantity that you want to buy: ");
        scanf("%s", buffer);
		if(isNumber(buffer)) { sscanf(buffer,"%d",&amount); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}

    if (amountComparison(product.quantity, amount)) {
        printf("\n\nTotal Price: %.2lf TL. \n\n Do you want to buy? (Y/N)\n\n",(amount *product.price));
		fflush(stdin);
        scanf("%s",buffer);
		
        if(buffer[0]=='y' || buffer[0]=='Y'){
            database=updateProductList(database,row,product.quantity-amount);
        }
		else if( buffer[0]=='n' || buffer[0]=='N'){fflush(stdin); return database;}

    }
    else {
        printf("\n\nWe don't have enough amount of quantity ( %d products on stock )\n\n",product.quantity);
        return database;
    }
}




bool checkAmountOfRecords(FILE * database) {
    int savedAmount = numberOfRecord(database);

    if (savedAmount >= MAXSIZE) { printf("\n\nYou can't add a new product because you have reached max amount.\n\n"); return 0; }
    else return 1;
}

void printRecord(Product record) {
    printf("\n\nBarcode Number: %d\nProduct Name: %s\nQuantity: %d\nPrice: %.2lf TL\n\n\n", record.barcode, record.name, record.quantity, record.price);
}

void search(FILE * database) {
    int barcode;
    Product result;
	char buffer[255];
   while(1){
        printf("Enter the barcode number: ");
        scanf("%s", buffer);
		if(isNumber(buffer)) { sscanf(buffer,"%d",&barcode); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}
    int row = isExist(database, barcode);

    if (row == -1) {
        printf("\n\nThere is not any product which has the barcode number %d\n\n\n", barcode);
    }
    else {
        printf("\n\n-----------------------------------------------------------");
        result = getProductFromDB(database, row);
        printRecord(result);
        printf("-----------------------------------------------------------\n\n");
    }
}


void addMenu(FILE * database) {

    Product inputs,result;
	char  buffer[255];

	while(1){
        printf("Enter the barcode number: ");
        scanf("%s", buffer);
		if(isNumber(buffer)) { sscanf(buffer,"%d",&inputs.barcode); break; }
		else printf("\n\n You must enter integer value!\n\n");
	}
        int row = isExist(database, inputs.barcode);
        if (row==-1) {
            if (checkAmountOfRecords(database)) {
            printf("Enter the name of the product: ");
            scanf("%s", inputs.name);
			int row2=isExist(database, inputs.name);
			if(row2!=-1)
			{
				printf("\n There is already a product which is named %s on database",inputs.name);
				return;
			}
           while(1){
			printf("Enter the amount of quantity: ");
            scanf("%s", buffer);
			if(isNumber(buffer)) { sscanf(buffer,"%d",&inputs.quantity); break; }
			else printf("\n\n You must enter integer value!\n\n");
		   }
            while(1){
		   printf("Enter the price of the product: ");
            scanf("%s", buffer);
			if(isNumber(buffer)) { sscanf(buffer,"%lf",&inputs.price); break;}
			else printf("\n\n You must enter double value!\n\n");
		   }
            createProduct(database, inputs.name, inputs.quantity, inputs.barcode, inputs.price);

        }
            else printf("\n\nDatabase is full.\n\n");
    }
        else {
            result=getProductFromDB(database,row);
            while(1){
			printf("Enter the amount of quantity: ");
            scanf("%s", buffer);
			if(isNumber(buffer)) { sscanf(buffer,"%d",&inputs.quantity); break;}
			else printf("\n\n You mush enter integer value!\n\n");
			}
            database = updateProductList(database, row, (inputs.quantity + result.quantity) );
        }

}
int mainMenu(FILE * database) {
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
    FILE * database = databaseOpen(productDB);
    int select = -1;
    while (select != 5) {
		fflush(stdin);
        select = mainMenu(database);
        if (select == 1) listAllProducts(database);
        else if (select == 2)addMenu(database);
        else if (select == 3) sellProduct(database);
        else if (select == 4) search(database);
        else if (select == 5) exit(0);
        else fflush(stdin);
    }

    fclose(database);
	
    return 0;
}