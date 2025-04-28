#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct IceCream
{
    char id[3];
    char productName[21];
    int weight;
    float pricePerKg;
} IceCream;

void printArr(IceCream* iceCreams, int n)
{
    printf("\n=====All IceCreams=====\n");
    for (int i = 0; i < n; i++)
    {
        printf("%s; %s; %d; %0.2f;\n", iceCreams[i].id, iceCreams[i].productName, iceCreams[i].weight, iceCreams[i].pricePerKg);
    }
    printf("\n=======================\n");
}

int inputNumber(int lower, int upper, char expression[])
{
    int n;
    printf(expression);
    scanf("%d", &n);

    while (n < lower || n > upper)
    {
        printf("Incorrect input!\n");
        printf(expression);
        scanf("%d", &n);
    }
    return n;
}

char* inputId(char expression[])
{
    char* id;
    id = (char*)malloc(sizeof(char)*2);
    printf(expression);
    scanf("%s", id);

    while ((id[0] < 65 || id[0] > 90) 
    || (id[1]-'0'< 0 || id[1]-'0'> 9) 
    || strlen(id) > 2)
    {
        fflush(stdin);
        printf("Incorrect input!\n");
        printf(expression);
        scanf("%s", id);
    }
    fflush(stdin);
    return id;
}

IceCream* mallocArr(int n)
{
    IceCream* iceCreams = (IceCream*)malloc(n * sizeof(IceCream));
    if (iceCreams == NULL)
    {
        printf("Couldn't allocate memory!\n");
        exit(1);
    }
    return iceCreams;
}

IceCream createIceCream(int number)
{
    printf("======%d======\n", number);
    char* id = inputId("Id (combination of uppercase letter and a digit ex. (A3)): ");
    fflush(stdin);
    printf("Product name: ");
    char name[20];
    fflush(stdin);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    fflush(stdin);
    int weight;
    printf("Weight (in Kg): ");
    scanf("%d", &weight);
    float pricePerKg;
    printf("Price per Kg: ");
    scanf("%f", &pricePerKg);

    IceCream iceCream;
    strcpy(iceCream.id, id);
    strcpy(iceCream.productName, name);
    iceCream.weight = weight;
    iceCream.pricePerKg = pricePerKg;

    printf("ID: %s\n", iceCream.id);

    return iceCream;
}

IceCream* fillArr(IceCream* iceCreams, int n)
{
    for (int i = 0; i < n; i++)
    {
        iceCreams[i] = createIceCream(i+1);
    }
    return iceCreams;
}

float sumPrice(IceCream* iceCreams, int n, char letter)
{
    float price = 0;
    for (int i = 0; i < n; i++)
    {
        if (iceCreams[i].productName[0] == letter)
        {
            price += iceCreams[i].weight * iceCreams[i].pricePerKg;
        }
    }
    return price;
}

int saveToTxtFile(IceCream* iceCreams, int n, float price, int weight)
{
    FILE* filePtr = fopen("d:\\C++CProgramming\\Exam3\\info.txt", "w");
    if (filePtr == NULL)
    {
        perror("Wrong path!\n");
        exit(1);
    }

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (iceCreams[i].pricePerKg < price && iceCreams[i].weight > weight)
        {
            fprintf(filePtr, "%s;%s;%d;%0.2fleva\n", iceCreams[i].id, iceCreams[i].productName, iceCreams[i].weight, iceCreams[i].pricePerKg);
            count += 1;
        }
    }
    
    fclose(filePtr);
    return count;
}

void saveToBinFile(IceCream* iceCreams, int n)
{
    char path[] = "d:\\C++CProgramming\\Exam3\\icecream.bin";
    FILE* filePtr = fopen(path, "wb");

    if (filePtr == NULL)
    {
        perror("Wrong path!");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        int productNameLen = strlen(iceCreams[i].productName);
        
        fwrite(&iceCreams[i].id, sizeof(char), 2, filePtr);
        fwrite(&productNameLen, sizeof(int), 1, filePtr);
        fwrite(iceCreams[i].productName, sizeof(char), productNameLen, filePtr);
        fwrite(&iceCreams[i].weight, sizeof(int), 1, filePtr);
        fwrite(&iceCreams[i].pricePerKg, sizeof(float), 1, filePtr);
        fputc('\n', filePtr);
    }
    fclose(filePtr);
}

void printIceCream(IceCream iceCream)
{
    printf("================\n");
    printf("IceCream - %s\n", iceCream.productName);
    printf("Price - %0.2f\n", iceCream.pricePerKg);
    printf("================\n");
}

void readBinFile(char* idToSearch)
{
    char path[] = "d:\\C++CProgramming\\Exam3\\icecream.bin";
    FILE* filePtr = fopen(path, "rb");

    if (filePtr == NULL)
    {
        perror("Wrong path!");
        exit(1);
    }

    int flag = 0;
    while (!feof(filePtr))
    {
        char* id;
        char* productName;
        int productNameLen;
        int weight;
        float pricePerKg;

        id = (char*)malloc(sizeof(char) * 3);
        fread(id, sizeof(char), 2, filePtr);
        id[2] = '\0';
        fread(&productNameLen, sizeof(int), 1, filePtr);
        productName = (char*)malloc(sizeof(char) * productNameLen+1);
        productName[productNameLen] = '\0';
        fread(productName, sizeof(char), productNameLen, filePtr);
        fread(&weight, sizeof(int), 1, filePtr);
        fread(&pricePerKg, sizeof(float), 1, filePtr);

        
        if (strcmp(id, idToSearch) == 0)
        {
            IceCream iceCream;
            strcpy(iceCream.id, id);
            strcpy(iceCream.productName, productName);
            iceCream.weight = weight;
            iceCream.pricePerKg = pricePerKg;

            printIceCream(iceCream);
            flag = 1;
        }
        
        free(id);
        free(productName);
        fgetc(filePtr);
    }
    
    if (flag == 0)
    {
        printf("Person not found!\n");
    }
    fclose(filePtr);
}

int main()
{
    int n = inputNumber(3, 15, "Enter number of iceCream products (integer between 3 and 15): ");
    IceCream* iceCreams = mallocArr(n);
    iceCreams = fillArr(iceCreams, n);

    printArr(iceCreams, n);

    fflush(stdin);
    char firstLetter;
    printf("Enter first letter of a product: ");
    scanf("%c", &firstLetter);
    printf("Price: %0.2f\n", sumPrice(iceCreams, n, firstLetter));

    printf("Saved products in txt file: %d\n", saveToTxtFile(iceCreams, n, 3, 2));

    saveToBinFile(iceCreams, n);

    char* idToSearch = inputId("Input id to search in binary file (format 'A3'): ");
    readBinFile(idToSearch);

    return 0;
}