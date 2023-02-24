#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "project_h.h"

int main()
{
    int c;
    do
    {
        LINE
        printf("\n1. Create Account\n2. Login\n0. Exit\n");
        scanf("%d", &c);
        switch(c)
        {
            case 1:
                sign_up();
                break;
            case 2:
                login();
                break;
            case 0:
                break;
            default:
                printf("Invalid Input\n");
                break;
        }
    }while(c != 0);
    return 0;
}

void login()
{
    int ch;
    do
    {
        LINE
        printf("\n1. Login as customer\n2. Login as employee\n0. Go Back\n");
        scanf("%d", &ch);
        switch (ch)
        {
            case 1:
                customer_login();
                break;
            case 2:
                employee_login();
                break;
            case 0:
                break;
            default:
                printf("Invalid Input\n");
                break;
        }
    }while(ch != 0);
}

void sign_up()
{
    customer c;
    char temp[20];
    do
    {
        printf("\nEnter your ID: ");
        scanf("%s", c.customer_personal_details.ID);
        if(ID_exists(c.customer_personal_details.ID) == 1)
        {    
            printf("Sorry, ID already exists, please try again\n");
            continue;
        }
        printf("\nEnter your password: ");
        scanf("%s", c.customer_personal_details.password);
        printf("\nConfirm Password: ");
        scanf("%s", temp);
        if(strcmp(temp, c.customer_personal_details.password))
        {
            printf("\nPasswords do not match, try again\n");
        }
    }while(strcmp(temp, c.customer_personal_details.password));
    encrypt(c.customer_personal_details.password);
    create_upi_handler(&c);
    c.transactions.transaction_count = 0;
    c.balance = 0;
    generate_ac_no(&c);
    strcpy(c.ifsc_code, "MYBANK");
    printf("\nEnter the type of account you intend to create: ");
    scanf("%s", c.type);
    c.loan.loan_status = 0;
    c.ac_status = 0;
    int valid_branch = 0;
    while(!valid_branch)
    {
        printf("\nEnter the branch in which you wish to create your account: ");
        scanf("%s",c.branch);
        if(!strcmpi(c.branch,"A")||!strcmpi(c.branch,"B")||!strcmpi(c.branch,"C")||!strcmpi(c.branch,"D")||!strcmpi(c.branch,"E"))
        {
            valid_branch = 1;
        }
        else
        {
            strcpy(c.branch,"");
        }
    }
    // writing into file
    FILE *fp;
    fp = fopen("CUSTOMERS.DAT", "ab");
    c.customer_no = (ftell(fp) / sizeof(customer));
    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);
    printf("\nYour account creation application has been submitted.\n");
}