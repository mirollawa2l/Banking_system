#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LEN             400
#define MAX_LINES           100
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ACCOUNTS        500
#define MAX_ACCOUNT_LENGTH  50
#define MAX_Keyword         50
typedef struct
{
    int month;
    int year;
} date;
typedef struct
{
    char account_no[MAX_ACCOUNT_LENGTH];
    char name[100];
    char mail[100];
    double balance;
    char mobile[15];
    date d_open;
} user;

void add();
void advancedSearch();
void askMenu();
int askSave();
int checkName(char*name,int i);
int checkNumber(char* number);
int dateCmp(date a, date b);
void deleteacc();
void DEPOSIT();
int loadAccounts();
int login();
void MENU();
void modify ();
const char* monthName(int month);
void print();
void printer(user a);
void QUERY();
int quit();
void report ();
void save();
void sortByBalance(user *a,int z);
void sortByDate(user *a,int z);
void sortByName(user *a, int z);
void TRANSFER();
char* validateAccountNumber(char *printvalue,int *i);
char* validateBalance();
int validateBalance100000(char* balance);
int validateBalanceExistance(char *balance,int i);
char* validateDuplication(char *printvalue,int i);
char* validateEmail();
char* validateMobile();
char* validateName();
void WITHDRAW();


user accounts[MAX_ACCOUNTS];        //accounts data
int count = 0;                      //accounts number

int loadAccounts()
{
    count =0;
    const char* filename="accounts.txt";
    FILE* file = fopen(filename, "r+");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }
    char line[400];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char* token;
        token = strtok(line, ",");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }
        strcpy(accounts[count].account_no,token);

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }
        strcpy(accounts[count].name, token);

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }
        strcpy(accounts[count].mail, token);

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }
        accounts[count].balance = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }
        strcpy(accounts[count].mobile, token);

        token = strtok(NULL, "-");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }

        accounts[count].d_open.month=atoi(token);
        token = strtok(NULL, " ");
        if (token == NULL)
        {
            printf("Invalid file format: %s\n", filename);
            exit(1);
        }
        accounts[count].d_open.year = atoi(token);
        count++;
        if (count >= MAX_ACCOUNTS)
        {
            printf("Maximum account limit reached: %d\n", MAX_ACCOUNTS);
            exit(1);
        }
    }

    fclose(file);
}
int login()
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int check_valid=0;
    printf("Enter username: ");
    scanf("%s", username);
    fflush(stdin);

    printf("Enter password: ");
    scanf("%s", password);
    fflush(stdin);

    FILE* file = fopen("users.txt", "r+");
    if (file == NULL)
    {
        printf("Error opening file: users.txt\n");
        exit(1);
    }

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];  // +2 for space and null

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char storedUsername[MAX_USERNAME_LENGTH];
        char storedPassword[MAX_PASSWORD_LENGTH];

        sscanf(line, "%s %s", storedUsername, storedPassword);

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
        {
            fclose(file);
            system("cls");
            printf("Welcome to the system!\n");
            loadAccounts();
            check_valid=1;
            return 1;  // Successful login
        }

    }
    if(!check_valid)
    {
        system("cls");
        printf("Invalid username or password\n");
        fclose(file);
        return 0;

    }
}

int quit()
{
    printf("Are you sure that you want to exit?\n1)Yes\n2)No\n");
    char val[30];
    scanf("%s",val);
    fflush(stdin);
    if(!checkNumber(val))
        quit();
    else if(atoi(val)==1)
        exit(1);
    else if(atoi(val)==2)
    {
        system("cls");
        return 0;
    }
    else
    {
        system("cls");
        printf("The Number you entered is not in range\nTRY AGAIN\n");
        quit();
    }
}

int checkNumber(char* number)
{
    while(*number!='\0')
    {
        if(!((*number>='0')&& (*number<='9')||(*number=='.')))
        {
            printf("Error: Invalid Input number must consist of integers only\n");
            return 0;
        }
        else number++;
    }
    return 1;
}

char* validateAccountNumber(char *printvalue,int *i)
{
    int flag_exist=0,flag_no=0;
    char* accountNumber = (char*)malloc(MAX_ACCOUNT_LENGTH* sizeof(char));
    do
    {
        flag_exist=0,flag_no=0;
        printf("%s",printvalue);
        scanf("%s", accountNumber);
        fflush(stdin);
        while(!checkNumber(accountNumber))
        {
            printf("%s",printvalue);
            scanf("%s", accountNumber);
            fflush(stdin);
        }
        for(*i=0; *i<count; (*i)++)
        {
            if(!strcmp(accountNumber,accounts[*i].account_no))
            {
                flag_exist=1;
                break;
            }
            else continue;
        }
        if(strlen(accountNumber)!=10)
        {
            printf("Error: Account Number must consist of 10 numbers\n");
            flag_no=1;
        }
        else if (!flag_exist)
        {
            printf("Error: Account Number doesn't exist\n");
            askMenu();
        }
    }
    while (!flag_exist||flag_no);
    return accountNumber;
}

char* validateDuplication(char *printvalue,int i)
{
    int flag_exist=0,flag_no=0;
    char* accountNumber = (char*)malloc(MAX_ACCOUNT_LENGTH* sizeof(char));
    do
    {
        flag_exist=0,flag_no=0;
        printf("%s",printvalue);
        scanf("%s", accountNumber);
        fflush(stdin);
        while(!checkNumber(accountNumber))
        {
            printf("%s",printvalue);
            scanf("%s", accountNumber);
            fflush(stdin);
        }
        for(i=0; i<count; i++)
        {
            if(!strcmp(accountNumber,accounts[i].account_no))
            {
                flag_exist=1;
                break;
            }
            else continue;
        }
        if(strlen(accountNumber)!=10)
        {
            printf("Error: Account Number must consist of 10 numbers\n");
            flag_no=1;
        }
        else if (flag_exist)
        {
            printf("Error: Account Number already exist\n");
        }
    }
    while (flag_exist||flag_no);
    return accountNumber;
}

int checkName(char*name,int i)
{
    if(name[i]>='a'&& name[i]<= 'z'||name[i] >='A'&& name[i] <='Z')
        return 1;
    else return 0;
}

char* validateName()
{
    int flag_name = 1,countSpace=0,i;
    char* name = (char*)malloc(100* sizeof(char));
    do
    {
        countSpace=0;
        printf("Enter your name: ");
        gets(name);
        for (i=0; i<strlen(name); i++)
        {
            flag_name = 1;
            if (!(checkName(name,i)||(name[i] == ' ')))
            {
                printf("Error: Name must only consist of characters\n");
                flag_name = 0;
                break;
            }
            if(name[i]==' '&&checkName(name,i+1))
            {
                countSpace++;
            }
        }
        if(countSpace<1)
        {
            printf("Error: Name must consist of first name and last name\n");
        }
    }
    while (!flag_name||countSpace<1);
    return name;
}

char* validateEmail()
{
    char* email = (char*)malloc(100* sizeof(char));
    int i,j,flag_dot = 0, flag_at = 0;


    printf("Enter your email: ");
    scanf("%s",email);
    fflush(stdin);

    for (i = 0; email[i] != '\0'; i++)
    {
        if(email[i] == '@')
        {
            flag_at = 1;
            if(checkName(email,i-1) && checkName(email,i+1))
            {
                for(j=i+2; email[j]!='\0'; j++)
                {
                    if(email[j] == '.')
                    {
                        flag_dot = 1;
                        if(checkName(email,j+1 )&&checkName(email,j-1))
                        {
                            return email ;
                        }
                        else
                        {
                            printf("Error: Invalid Email format\n");
                            validateEmail();
                        }
                    }

                }
                if(flag_dot == 0)
                {
                    printf("Error: Invalid Email format\n");
                    validateEmail();
                }
            }
            else
            {
                printf("Error: Invalid Email format\n");
                validateEmail();
            }
        }

    }
    if(flag_at == 0)
    {
        printf("Error: Invalid Email format\n");
        validateEmail();
    }
}


char* validateBalance()
{
    char* balance = (char*)malloc(100* sizeof(char));
    do
    {
        printf("Enter your balance: ");
        scanf("%s", balance);
        fflush(stdin);
    }
    while (!checkNumber(balance));
    return balance;
}

char* validateMobile()
{
    char* mobileNumber = (char*)malloc(15* sizeof(char));
    int flag_mobile=0;
    do
    {
        flag_mobile=0;
        do
        {
            printf("Enter the mobile number: ");
            scanf("%s", mobileNumber);
            fflush(stdin);
        }
        while(!checkNumber(mobileNumber));
        if(strlen(mobileNumber)!=11)
        {
            printf("Error: Mobile number must consist of 11 numbers\n");
            flag_mobile=1;
        }
    }
    while(flag_mobile);
    return mobileNumber;
}

int validateBalanceExistance(char *balance,int i)
{
    if(atof(balance)>accounts[i].balance)
    {
        printf("Error: You don't have enough balance\nYour balance: %f\n",accounts[i].balance);
        return 0;
    }
    else return 1;
}

int validateBalance100000(char* balance)
{
    if(atof(balance)>10000.0)
    {
        printf("Error: Max limit per transaction is $10,000\n");
        return 0;
    }
    else return 1;
}

void add()
{
    char printvalue[]="Enter account number: ";
    count++;
    int i=count-1;
    char*ptr;
    user temp;
    ptr=validateDuplication(printvalue,i);
    strcpy(temp.account_no,ptr);
    free(ptr);
    ptr=validateName();
    strcpy(temp.name,ptr);
    free(ptr);
    ptr=validateMobile();
    strcpy(temp.mobile,ptr);
    free(ptr);
    ptr=validateEmail();
    strcpy(temp.mail,ptr);
    free(ptr);
    ptr=validateBalance();
    temp.balance=atof(ptr);
    free(ptr);
    time_t t; //struct to store in seconds
    time(&t); //function to store current time in seconds in the struct created before
    struct tm *tm_info = localtime(&t); //function to localize time in seconds
    temp.d_open.month = tm_info->tm_mon + 1; //add one as the month is counted from 0 in this fn and out months start from 1
    temp.d_open.year = tm_info->tm_year + 1900; // add 1900 because the fn start counting the seconds from year 1900AD
    if(askSave())
    {
        accounts[i]=temp;
        char filename[30] ;
        sprintf(filename,"%s.txt",accounts[i].account_no);
        FILE *file=fopen(filename, "w");
        if (file == NULL)
        {
            printf("Error opening file\n");
            exit(1);
        }
        fclose(file);
        save();
    }
    else MENU();
}

void DEPOSIT()
{
    int i;
    char*ptr;
    char printvalue[]="Enter the account number: ";
    char accountNumber[MAX_ACCOUNT_LENGTH];
    char  depositAmount[100];
    ptr=validateAccountNumber(printvalue,&i);
    strcpy(accountNumber,ptr);
    free(ptr);
    do
    {
        ptr=validateBalance();
        strcpy(depositAmount,ptr);
        free(ptr);
    }
    while(!validateBalance100000(depositAmount));
    if(askSave)
    {
        accounts[i].balance+=atof(depositAmount);
        printf("Deposit successful\nNew balance: %f\n",accounts[i].balance);
        double deposit=atof(depositAmount);
        FILE* file = fopen(strcat(accountNumber,".txt"), "a");
        if (file == NULL)
        {
            printf("Error opening file");
            exit(1);
        }
        fprintf(file, "Depositted amount to the account: %f New balance: %f\n", deposit,accounts[i].balance);
        fclose(file);
        save();
    }
    else MENU();

}

void TRANSFER()
{
    int i,j,flag_existance=1,flag_10000=1;
    char*ptr;
    char printvalue1[]="Enter the account number to transfer money from: ";
    char accountNumber1[MAX_ACCOUNT_LENGTH];
    ptr=validateAccountNumber(printvalue1,&i);
    strcpy(accountNumber1,ptr);
    free(ptr);
    char printvalue2[]="Enter the account number to transfer money to: ";
    char accountNumber2[MAX_ACCOUNT_LENGTH];

    ptr=validateAccountNumber(printvalue2,&j);
    strcpy(accountNumber2,ptr);
    free(ptr);
    while(atof(accountNumber1)==atof(accountNumber2))
    {
        printf("Error: You can't transfer money to the same account\n");
        ptr=validateAccountNumber(printvalue2,&j);
        strcpy(accountNumber2,ptr);
        free(ptr);
    }
    char  transferAmount[100];
    do
    {
        flag_existance=1,flag_10000=1;
        ptr=validateBalance();
        strcpy(transferAmount,ptr);
        free(ptr);
        if(!validateBalance100000(transferAmount))
            flag_10000=0;
       else if(!validateBalanceExistance(transferAmount,i))
        {
            askMenu();
            flag_existance=0;
        }
    }
    while(!flag_10000||!flag_existance);
    if(askSave)
    {
        accounts[i].balance-=atof(transferAmount);
        accounts[j].balance+=atof(transferAmount);  // Update the balances of the source and destination accounts
        printf("Transfer Successful\nNew balance of The source account: %f\nNew balance of The destination account: %f\n",accounts[i].balance,accounts[j].balance);
        double transfer=atof(transferAmount);
        FILE* file1 = fopen(strcat(accountNumber1,".txt"), "a");
        if (file1 == NULL)
        {
            printf("Error opening file");
            exit(1);
        }
        fprintf(file1, "Transferred amount from the account: %f New Balance: %f\n",transfer,accounts[i].balance);
        FILE* file2 = fopen(strcat(accountNumber2,".txt"), "a");
        if (file2 == NULL)
        {
            printf("Error opening file");
            exit(1);
        }
        fprintf(file2, "Transferred amount to the account: %f New Balance: %f\n",transfer,accounts[j].balance);
        fclose(file1);
        fclose(file2);
        save();
    }
    else MENU();
}

void WITHDRAW()
{
    int i,flag_existance=1,flag_10000=1;
    char printvalue[]="Enter the account number: ";
    char accountNumber[MAX_ACCOUNT_LENGTH];
    char* ptr;
    ptr=validateAccountNumber(printvalue,&i);
    strcpy(accountNumber,ptr);
    free(ptr);
    char  withdrawnAmount[100];
    do
    {
        flag_existance=1,flag_10000=1;
        ptr=validateBalance();
        strcpy(withdrawnAmount,ptr);
        free(ptr);
        if(!validateBalance100000(withdrawnAmount))

            {
                flag_10000=0;
            }
       else if(!validateBalanceExistance(withdrawnAmount,i))
        {
            askMenu();
            flag_existance=0;
        }
    }
    while(!flag_10000||!flag_existance);
    if(askSave)
    {
        accounts[i].balance-=atof(withdrawnAmount);
        printf("Transaction succeded\nNew Balance:%f\n",accounts[i].balance);
        double withdraw=atof(withdrawnAmount);
        FILE* file = fopen(strcat(accountNumber,".txt"), "a");
        if (file == NULL)
        {
            printf("Error opening file");
            exit(1);
        }
        fprintf(file, "Withdrawn amount from the account: %f New Balance: %f\n",withdraw,accounts[i].balance);

        fclose(file);
        save();
    }
    else MENU();

}

void advancedSearch()
{
    int i,j,k,keylen,namelen,check1,check2,good = 0,matchedlen = -1,hold ;
    char keyword[MAX_Keyword] = {};
    user matched[MAX_ACCOUNTS];
    printf("Enter Keyword: ");
    gets(keyword);
    keylen = strlen(keyword);
    for(i = 0; i < count ; i++) //i = index of accounts
    {
        good = 0;
        namelen = strlen(accounts[i].name);
        for(j = 0 ; j < namelen  ; j++  ) // j = index of chars in name array of the account with index i
        {
            good = 0;

            check1 = (accounts[i].name[j] == keyword[0] || (accounts[i].name[j] + 32) == keyword[0] || (accounts[i].name[j] - 32) == keyword[0]); //not case sensitive
            if(check1)
            {
                good = 0;

                for(k = j ; k-j <= keylen ; k++ ) //k = index of character that matched the first character of keyword and k-j is used to pass on array of the keyword
                {
                    if(isspace(accounts[i].name[k]) && accounts[i].name[k] != keyword[k-j]) //jump any space in the name array except if keyword[] has a space at the same instance
                    {
                        k++; //to jump the space
                        j++; //to keep the original k-j
                    }
                    check2 = (accounts[i].name[k] == keyword[k-j] || (accounts[i].name[k] + 32) == keyword[k-j] || (accounts[i].name[k] - 32) == keyword[k-j])  ; //not case sensitive

                    if(check2)
                    {
                        good++; //used to check that all consecutive matched letters are = the length of the original keyword
                    }
                    if(good == keylen && hold != i) //if good = keyword len and you never addded this before please add it
                    {
                        hold = i;  //hold used here because the same name could have several instances of the same keyword while it should be stored only once
                        matchedlen++; //used as a flag to check if any matches was found
                        matched[matchedlen] = accounts[i]; //add the account that had the keyword to the array that will be added
                    }
                }
            }
        }
    }

    printf("\n\nSearch results:\n\n");
    if(matchedlen != -1)
    {
        for(i = 0 ; i <= matchedlen ; i ++) //prints all matched accounts.
        {
            printer(matched[i]);
            printf("\n");
        }
    }
    else
    {
        printf("Keyword not found\n");
    }
}

const char* monthName(int month)
{
    switch (month)
    {
    case 1:
        return "January";
        break;
    case 2:
        return "February";
        break;
    case 3:
        return "March";
        break;
    case 4:
        return "April";
        break;
    case 5:
        return "May";
        break;
    case 6:
        return "June";
        break;
    case 7:
        return "July";
        break;
    case 8:
        return "August";
        break;
    case 9:
        return "September";
        break;
    case 10:
        return "October";
        break;
    case 11:
        return "November";
        break;
    case 12:
        return "December";
        break;
    }

}

void printer(user a)
{
    printf("Account number: %s\n",a.account_no);
    printf("Name: %s\n",a.name);
    printf("E-mail: %s\n",a.mail);
    printf("Balance: %.2lf $\n",a.balance);
    printf("Mobile: %s\n",a.mobile);
    printf("Date Opened: %s %d\n",monthName(a.d_open.month),a.d_open.year);
}

void sortByName(user *a, int z)
{
    int i,j, flag = 1,type;
    if(z == 1)
    {
        type = 1;
    }
    else
    {
        type = -1;
    }
    user x;
    for(i = 0 ; i < count-1; i++)
    {
        if(flag == 0) break;
        flag = 0;
        for(j = 0 ; j < count - i -1 ; j++)
        {

            if(strcmp((a+ j)-> name, (a+ j +1)-> name ) == type)
            {
                x =  (*(a+ j));
                (*(a + j)) = (*(a+ j +1));
                (*(a+ j +1)) = x;
                flag = 1;
            }
        }
    }
}

void sortByBalance(user *a,int z)
{
    int i,j, flag = 1;
    user x;
    for(i = 0 ; i < count-1; i++)
    {
        if(flag == 0) break;
        flag = 0;
        for(j = 0 ; j < count - i -1 ; j++)
        {
            if(z == 1)
            {
                if((a+ j)-> balance < (a+ j +1)-> balance)
                {
                    x =  (*(a+ j));
                    (*(a + j)) = (*(a+ j +1));
                    (*(a+ j +1)) = x;
                    flag = 1;
                }
            }
            else
            {
                if((a+ j)-> balance > (a+ j +1)-> balance)
                {
                    x =  (*(a+ j));
                    (*(a + j)) = (*(a+ j +1));
                    (*(a+ j +1)) = x;
                    flag = 1;
                }
            }
        }
    }
}

int dateCmp(date a, date b)
{
    if(a.year > b.year ) return 1;
    if(a.year < b.year ) return -1;
    if(a.year == b.year)
    {
        if(a.month > b.month ) return 1;
        if(a.month < b.month ) return -1;
        if(a.month == b.month ) return 0;
    }

}

void sortByDate(user *a,int z)
{
    int i,j, flag = 1,type;
    if(z == 1)
    {
        type = 1;
    }
    else
    {
        type = -1;
    }
    user x;
    for(i = 0 ; i < count-1; i++)
    {
        if(flag == 0) break;
        flag = 0;
        for(j = 0 ; j < count - i -1 ; j++)
        {

            if(dateCmp((a+ j)-> d_open,(a+ j +1)-> d_open) == type)
            {
                x =  (*(a+ j));
                (*(a + j)) = (*(a+ j +1));
                (*(a+ j +1)) = x;
                flag = 1;
            }
        }
    }
}

void print()
{
    user sorted[MAX_ACCOUNTS] ;
    int i,flag=1,flag_no=1,flag_type = 1;
    char way[30],type[30];
    for(i=0 ; i < count ; i++)
    {
        sorted[i] = accounts[i]; //array to change its order without changing the original array
    }
    do
    {
        flag=1,flag_no=1;
        printf("How would you like the accounts to be sorted?\n");
        printf("1) By Name\n2) By Balance\n3) By Date Opened\n");
        scanf("%s",way);
        fflush(stdin);
        if(!checkNumber(way))
            flag_no=0;
        else
            switch(atoi(way))
            {
            case 1:

                printf("Please choose the type:\n");
                printf("1) A-Z\n2) Z-A\n");
                do
                {
                    flag_type = 1;
                    scanf("%s",type);
                    fflush(stdin);
                    if(!checkNumber(type))
                        flag_type=0;
                    if(!(atoi(type)>0 && atoi(type)<3)) printf("The Number you entered is not in range\nTRY AGAIN\n");
                }
                while(!(atoi(type)>0 && atoi(type)<3 && flag_type == 1));
                sortByName(sorted,atoi(type));
                break;
            case 2:
                printf("Please choose the type:\n");
                printf("1) Highest to Lowest\n2) Lowest to Highest\n");
                do
                {
                    flag_type = 1;
                    scanf("%s",type);
                    fflush(stdin);
                    if(!checkNumber(type))
                        flag_type=0;
                    if(!(atoi(type)>0 && atoi(type)<3)) printf("The Number you entered is not in range\nTRY AGAIN\n");
                }
                while(!(atoi(type)>0 && atoi(type)<3 && flag_type == 1));
                sortByBalance(sorted,atoi(type));
                break;
            case 3:
                printf("Please choose the type:\n");
                printf("1) Old to New\n2) New to Old\n");
                do
                {
                    flag_type = 1;
                    scanf("%s",type);
                    fflush(stdin);
                    if(!checkNumber(type))
                        flag_type=0;
                    if(!(atoi(type)>0 && atoi(type)<3)) printf("The Number you entered is not in range\nTRY AGAIN\n");
                }
                while(!(atoi(type)>0 && atoi(type)<3 && flag_type == 1));
                sortByDate(sorted,atoi(type));
                break;
            default:
                printf("The Number you entered is not in range\nTRY AGAIN\n");
                flag=0;
            }
    }
    while(!flag||!flag_no);
    printf("\n\n");
    for(i=0 ; i < count ; i++) //print sorted array
    {
        printer(sorted[i]);
        printf("\n");
    }

}

void QUERY()
{
    int i;
    char*ptr;
    char printvalue[]="Enter account number: ";
    ptr=validateAccountNumber(printvalue,&i);
    free(ptr);
    printer(accounts[i]);

}

void deleteacc()
{
    int i,flag=0,err;
    char filename[11];
    char*ptr;
    char printvalue[]="Enter account number: ";

    ptr=validateAccountNumber(printvalue,&i);
    free(ptr);
    if(accounts[i].balance!=0)
    {
        printf("Balance not equal to zero, Account cannot be deleted\n");
        flag=1;
    }
    if(!flag)
    {
        if(askSave())
        {
            sprintf(filename,"%s.txt",accounts[i].account_no);
            err = remove(filename);
            if(err == 1)
            {
                printf("File has been deleted successfully\n");
            }
            else
            {
                printf("File not deleted\n");
            }
            for( i; i<count-1; i++)
            {
                accounts[i]=accounts[i+1];
            }
            count--;
            printf("The account has been deleted\n");
            save();

        }
        else MENU();
    }
}

void modify ()
{
    int i;
    char printvalue[]="Enter account number: ";

    char accountNumber[MAX_ACCOUNT_LENGTH];
    char*ptr;
    ptr=validateAccountNumber(printvalue,&i);
    strcpy(accountNumber,ptr);
    free(ptr);
    int flag=1,flag_no=1;
    char temp[MAX_ACCOUNT_LENGTH];
    printer(accounts[i]);
    do
    {
        flag=1,flag_no=1;
        printf("What do you want to modify\n1)Name\n2)Mobile\n3)Email\n4)Return to Menu\n");
        char choice[30];
        scanf("%s", choice);
        fflush(stdin);
        if(!checkNumber(choice))
            flag_no=0;
        else
            switch (atoi(choice))
            {

            case 1:
            {
                ptr=validateName();
                strcpy(temp,ptr);
                free(ptr);
                if(askSave())
                {
                    strcpy(accounts[i].name,temp);
                    save();
                    break;
                }
                else MENU();
            }

            case 2:
            {
                ptr=validateMobile();
                strcpy(temp,ptr);
                free(ptr);
                if(askSave())
                {
                    strcpy(accounts[i].mobile,temp);
                    save();
                    break;
                }
                else MENU();
            }

            case 3:
            {

                ptr=validateEmail();
                strcpy(temp,ptr);
                free(ptr);
                if(askSave())
                {
                    strcpy(accounts[i].mail,temp);
                    save();
                    break;
                }
                else MENU();
            }
            case 4:
                return;

            default:
            {
                printf("The Number you entered is not in range\nTRY AGAIN\n");
                flag = 0;
            }
            }
    }
    while(!flag||!flag_no);
}
void report ()
{
    int i;
    char printvalue[]="Enter account number: ";
    char*ptr;
    ptr=validateAccountNumber(printvalue,&i);
    strcpy(accounts[i].account_no,ptr);
    free(ptr);
    char filename[30];
    sprintf(filename,"%s.txt",accounts[i].account_no);
    FILE *file=fopen(filename, "a+");

    if (file== NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    int no_line=0;
    char buffer[MAX_LINES][MAX_LEN];
    while(!feof(file))
        if(fgets(buffer[no_line],MAX_LEN,file)!=NULL)no_line++;
    if(no_line==0)
        printf("No transactions happened on this account\n");

    else if(no_line>=5)
    {
        for(int i=0; i<5; i++)
        {
            printf("%s\n",buffer[no_line-i-1]);
        }
    }
    else
    {
        for(int i=0; i<no_line; i++)
        {
            printf("%s\n",buffer[no_line-i-1]);
        }
    }

    fclose(file);
}

void save()
{
    const char* filename="accounts.txt";
    int i ;
    FILE* file = fopen(filename, "w+");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    for(i=0; i<count; i++)
    {

        fprintf(file,"%s,%s,%s,%0.2f,%s,%d-%d \n",accounts[i].account_no,accounts[i].name,accounts[i].mail,accounts[i].balance,accounts[i].mobile,accounts[i].d_open.month,accounts[i].d_open.year);

    }
    printf("\nSaved Successfully\n");
    fclose(file);
}

void askMenu()
{
    char choice[30];
    printf("What do you want\n1)Continue\n2)Return to Menu\n");
    scanf("%s",choice);
    fflush(stdin);
    if(!checkNumber(choice))askMenu();
    else
        switch(atoi(choice))
        {
        case 1:
            break;
        case 2:
            MENU();
        default:
            printf("The Number you entered is not in range\nTry Again\n");
            askMenu();
        }
}

int askSave()
{
    char choice[30];
    printf("Do you want to save the changes?\n1)Yes\n2)No\n");
    scanf("%s",choice);
    fflush(stdin);
    if(!checkNumber(choice)) askSave();
    else

        switch(atoi(choice))
        {
        case 1:
            return 1;
        case 2:
            return 0;
        default:
            printf("The Number you entered is not in range\nTry Again");
            askSave();
        }

}

int flag_login = 0;
void MENU()
{
    char choice[30];
    system("cls");
    while (!flag_login)

    {
        printf("Enter (1 or 2)\n1.LOGIN\n2.QUIT\n");
        scanf("%s", choice);
        fflush(stdin);
        if(!checkNumber(choice));
        else if(atoi(choice)!=1&&atoi(choice)!=2 )
        {
            system("cls");
            printf("The Number you entered is not in range\nTRY AGAIN\n");
        }
        else
        {
            if (atoi(choice) == 2) quit();
            if (atoi(choice) == 1 && login() == 1)  flag_login++;
        }

    }


    do
    {

        printf("Enter a number from (1 to 11)\n1.ADD\n2.DELETE\n3.MODIFY\n4.WITHDRAW\n5.TRANSFER\n6.DEPOSIT\n7.REPORT\n8.QUERY\n9.ADVANCED SEARCH\n10.PRINT\n11.QUIT\n");
        scanf("%s", choice);
        fflush(stdin);
        if(!checkNumber(choice));
        else
            switch (atoi(choice))
            {
            case 1:
                add();
                break;
            case 2:
                deleteacc();
                break;
            case 3:
                modify();
                break;
            case 4:
                WITHDRAW();
                break;
            case 5:
                TRANSFER();
                break;
            case 6:
                DEPOSIT();
                break;
            case 7:
                report();
                break;
            case 8:
                QUERY();
                break;
            case 9:
                advancedSearch();
                break;
            case 10:
                print();
                break;
            case 11:
                quit();
                break;
            default:
                system("cls");
                printf("The Number you entered is not in range\nTRY AGAIN\n");
            }
    }
    while(1);
}

int main()
{
    MENU();
    return 0;
}
