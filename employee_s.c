#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "project_h.h"

int employee_login()
{
    FILE* fp = fopen("employee.csv","r");
    employee* head = NULL;
    char buffer[1024];
    employee* tail = head;
    fgets(buffer,1024,fp);
    while(fgets(buffer,1024,fp))
    {
        employee* temp = (employee*)malloc(sizeof(employee));
        char *name = strtok(buffer,",");
        char *role = strtok(NULL,",");
        char *empid = strtok(NULL,",");
        char *password = strtok(NULL,",");
        char *branch = strtok(NULL,"\n");
        strcpy(temp->ed.name,name);
        strcpy(temp->ed.role,role);
        strcpy(temp->ed.ID,empid);
        strcpy(temp->ed.password,password);
        strcpy(temp->ed.branch,branch);
        temp->node = NULL;
        if(head == NULL)
            head = temp;
        else
            tail->node = temp;
        tail = temp;
        temp = NULL;
    }
    tail = NULL;
    fclose(fp);
    printf("***********This is Employee login space******************\n");
    printf("\nEnter the Employee ID: ");
    char emp_id[20];
    scanf("%s",emp_id);
    printf("Enter your password: ");
    char password[20];
    scanf("%s",password);
    employee *temp = head;
    int flag = 0;
    while(temp != NULL)
    {
        if((!strcmp(temp->ed.ID,emp_id)) && (!strcmp(temp->ed.password,password)))
        {
            flag = 1;
            break;
        }
        temp = temp->node;
    }
    if(!flag)
    {
        printf("Invalid Login Credentials\n");
        return 1;
    }
    printf("\n\nLogin Successful!!\n");
    printf("Welcome %s\n",temp->ed.name);
    printf("***********************************************\n");
    if(strcmp(temp->ed.role,"Division Manager") == 0)
        division_manager(head);
    else if(strcmp(temp->ed.role,"Manager") == 0)
        manager(temp->ed.branch);
    else
       cashier(temp->ed.branch) ;
    return 0;
}


int update_interest()
{
    time_t t;
    time(&t);
    struct tm *mytime = localtime(&t);
    FILE* fp = fopen("bank_details.txt","r");
    char feed[1024];
    bank_details bd;
    fgets(feed,1024,fp);
    bd.rate_sb = atof(strtok(feed,","));
    bd.rate_rd = atof(strtok(NULL,","));
    bd.update_count = atoi(strtok(NULL,"\n"));
    fclose(fp);
    if(mytime->tm_hour >= 20 && bd.update_count != mytime->tm_mday)
    {
        printf("%d\n",time(&t));
        fp = fopen("customers.dat","rb");
        customer cd[size];
        customer c;
        int size1 = 0;
        while(fread(&cd[size1],sizeof(customer),1,fp))
        {
            size1++;
        }
        fclose(fp);
        for(int i = 0;i<size1;i++)
        {
            if(cd[i].ac_status == 1)
                cd[i].balance = cd[i].balance + cd[i].balance*(bd.rate_sb/365);
                printf("%.2f\n",cd[i].balance);
        }
        fp = fopen("CUSTOMERS.dat","wb");
        bd.update_count = mytime->tm_mday;
        printf("%d\n",bd.update_count);
        for(int i = 0; i < size1; i++)
        {
            fwrite(&cd[i], sizeof(customer), 1, fp);
        }
        fclose(fp);
        fp = fopen("bank_details.txt","w");
        fprintf(fp,"%.2f,%.2f,%d\n",bd.rate_sb,bd.rate_rd,bd.update_count);
        fclose(fp);
    }
    return 0;
}
int manager(char* branch)
{
    branch_details load[5];
    FILE* fp = fopen("branchdetails.csv","r");
    int loader = 0,flag = 0;
    char feed[1024];
    fgets(feed,1024,fp);
    while(fgets(feed,1024,fp))
    {
        strcpy(load[loader].branch,strtok(feed,","));
        load[loader].total_deposit = atof(strtok(NULL,","));
        load[loader].no_of_accounts = atoi(strtok(NULL,","));
        load[loader].loans_granted = atoi(strtok(NULL,","));
        load[loader].customer_ID = atoi(strtok(NULL,"\n"));
        if(!strcmp(load[loader].branch,branch))
            flag = loader;
        loader++;
    }
    fclose(fp);
    char response = 'c';
    while(1)
    {
        printf("Choose from the following:\n");
        printf("1. View customers\n");
        printf("2. Search/Block/Unblock/Delete Accounts\n");
        printf("3. Verify Customer details\n");
        printf("4. Loan Approvals\n");
        printf("5. View trasaction details\n");
        printf("6. View Feedback\n");
        printf("7. View deposit details\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        int ch, flag = 0;
        scanf("%d",&ch);
        printf("\n******************************************************************************\n");
        switch(ch)
        {
            case 1:
            {
                view_customers(branch);
                update_interest();
                break;
            }
            case 2:
            {
                block(branch,load,flag);
                break;
            }
            case 3:
            {
                verify_applicants(branch,load,flag);
                update_interest();
                break;
            }
            case 4:
            {
                approve_loan_applicants(branch,load,flag);
                update_interest();
                break;
            }
            case 5:
            {
                update_interest();
                view_details('T');
                break;
            }
            case 6:
            {
                update_interest();
                view_feedback();
                break;
            }
            case 7:
            {
                update_interest();
                view_details('D');
                break;
            }
            case 0:
                update_interest();
                flag = 1;
                break;
            default:
                printf("Invalid Choice!!\n\n");
        }
        printf("******************************************************************************\n");
        if(flag == 1)
        {
            break;
        }
        //printf("Enter c to continue or e to exit\n");
        //scanf(" %c", &response);
    }
    return 0;
}
void division_manager(employee* head)
{
    int loop_runner = 1;
    do
    {
    printf("Choose from the following:\n");
    printf("1.Display Employees\n");
    printf("2.Add Employees\n");
    printf("3.Remove employees\n");
    printf("4.Logout\n");
    int ch;
    char name[20],role[20],branch[20],password[20],emp_id[20];
    scanf("%d",&ch);
    switch(ch)
    {
        case 1: display();
                break;
        case 2: printf("\nEnter the following details:\n");
                struct employee_details e;
                do
                {
                    printf("Name: ");
                    scanf(" %[^\n]s",e.name);
                }while(strlen(e.name)>20);
                int ch;
                //Take input of branch and role
                while(1)
                {
                    printf("Select the role of the employee:\n");
                    printf("1.Manager\n2.Cashier\n");
                    scanf("%d",&ch);
                    if(ch == 1) strcpy(e.role,"Manager");
                    else if(ch == 2) strcpy(e.role,"Cashier");
                    else {printf("Invalid Role\n");continue;}
                    printf("Select the branch of the employee:\n");
                    printf("1.A\n2.B\n3.C\n4.D\n5.E\n");
                    scanf("%d",&ch);
                    if(ch == 1) strcpy(e.branch,"A");
                    else if(ch == 2) strcpy(e.branch,"B");
                    else if(ch == 3) strcpy(e.branch,"C");
                    else if(ch == 4) strcpy(e.branch,"D");
                    else if(ch == 5) strcpy(e.branch,"E");
                    else {printf("Invalid Branch\n");continue;};
                    break;
                }
                printf("Password: ");
                fflush(stdin);
                scanf("%s",e.password);
                add_employees(e,head);
                break;
        case 3: printf("\nEnter the following details:\n");
                printf("Employee ID: ");
                scanf("%s",emp_id);
                printf("Branch: ");
                scanf("%s",branch);
                strtok(branch,"\n");
                remove_employee(emp_id,branch,head);
                break;
        case 4: loop_runner = 0;
                break;
        default: printf("Invalid Choice!\n");
    }
    }while(loop_runner);
    employee* temp  = (employee*)malloc(sizeof(employee));
    temp = head;
    while(temp != NULL)
    {
        head = temp->node;
        free(temp);
        temp = head;
    }
}
char* generate_employee_id(int l,int up)
{
    srand(time(0));
    int rand_num = (rand() % (up- l + 1)) + l;
    char emp_id[20];
    return itoa(rand_num,emp_id,10);
}
int add_employees(struct employee_details e,employee* head)
{
    employee* temp = (employee*)malloc(sizeof(employee));
    employee* present = (employee*)malloc(sizeof(employee));
    present = head;
    employee* prev = NULL;
    char* eid = generate_employee_id(10000,100000);
    while(present != NULL)
    {
        if((!strcmp(present->ed.role,e.role)) && (!strcmp(present->ed.branch,e.branch)))
        {
            printf("%s is already appointed to this branch\n",e.role);
            return 1;
        }
        if(!strcmp(present->ed.ID,eid))
        {           
            eid = generate_employee_id(10000,100000);//generate another id and check again
            present = head;
        }
        else
        {
            present = present->node;
        }
    }
    strcpy(e.ID,eid);
    present = head;
    prev = NULL;
    temp->ed = e;
    //Just for orderly arrangement
    while(present && strcmp(temp->ed.branch,present->ed.branch)>0)
    {
        prev = present;
       present = present->node;
    }
    if(present && !strcmp(present->ed.branch,temp->ed.branch))
    {
        if(!strcmp(present->ed.role,"Manager"))
        {
            temp->node = present->node;
            present->node = temp;
        }
        else
        {
            temp->node = present;
            if(present == head) head = temp;
            else prev->node = temp;
        }
    }
    else
    {
        temp->node = present;
        if(present == head) 
        head = temp;
        else 
        prev->node = temp; 
    }  
    FILE* fp = fopen("employee.csv","w");
    fprintf(fp,"Name,Role,Employee ID,password,branch\n");
    temp = head;
    while(temp != NULL)
    {
        fprintf(fp,"%s,%s,%s,%s,%s\n",temp->ed.name,temp->ed.role,temp->ed.ID,temp->ed.password,temp->ed.branch);
        temp = temp->node;
    }  
    fclose(fp); 
    printf("Employee with following details has been added:\n");
    printf("1.Name: %s\n",e.name);
    printf("2.Role: %s\n",e.role);
    printf("3.Employee ID: %s\n",e.ID);
    printf("4.Branch: %s\n",e.branch);  
    int resp = 1;
    while(resp)
    {
        char a[2];
        printf("Enter any digit to continue: ");
        scanf("%s",a);
        if(strlen(a)<=2)
            if(a[0]>'0' && a[0]<'9')
                resp = 0;
    }
}
    
void remove_employee(char* emp_id,char* branch,employee* head)
{
    FILE* fp = fopen("employee.csv","w");
    employee* present = (employee*)malloc(sizeof(employee));
    present = head;
    employee* prev = NULL;
    int flag = 0;
    while(present != NULL)
    {
        if((!strcmp(present->ed.ID,emp_id)) && !strcmpi(branch,"0"))
        {
            flag = 1;
            printf("You are not authorised to delete this employee\n");
            break;
        }
        if((!strcmp(present->ed.ID,emp_id)) && (!strcmpi(present->ed.branch,branch)))
        {
            flag = 1;
            printf("Employee with following details has been deleted:\n");
            printf("1.Name: %s\n",present->ed.name);
            printf("2.Role: %s\n",present->ed.role);
            printf("3.Employee ID: %s\n",present->ed.ID);
            printf("4.Branch: %s\n",present->ed.branch);   
            if(prev == NULL)
            {
                head = present->node;
            }
            else
            {
                prev->node = present->node;
            }
            free(present);
            present = NULL;
            break;
        }
        prev = present;
        present = present->node;
    }
    if(!flag)
    printf("Employee doesnot exist!\n");
 
    employee* temp = (employee*)malloc(sizeof(employee));
    temp = head;
    fprintf(fp,"Name,Role,Employee ID,password,branch\n");
    while(temp != NULL)
    {
        fprintf(fp,"%s,%s,%s,%s,%s\n",temp->ed.name,temp->ed.role,temp->ed.ID,temp->ed.password,temp->ed.branch);
        temp = temp->node;
    }  
    fclose(fp); 
    int resp = 1;
    while(resp)
    {
        char a[2];
        printf("Enter any digit to continue: ");
        scanf("%s",a);
        if(strlen(a)<=2)
            if(a[0]>'0' && a[0]<'9')
                resp = 0;
    }
}
void display()
{
    FILE* fp = fopen("employee.csv","r");
    char buffer[1024];
    while(fgets(buffer,1024,fp))
    {
        char *name = strtok(buffer,",");
        char *role = strtok(NULL,",");
        char *empid = strtok(NULL,",");
        char *password = strtok(NULL,",");
        char *branch = strtok(NULL,",");
        strtok(branch,"\n");
        if(!strcmp(role,"Role"))
        {
               printf("%s\t\t%s\t\t%s\t%s\n",name,role,empid,branch);
               continue;
        }
        if(!strcmp(role,"Division Manager"))
            continue;
        if(strlen(name)>14)
            printf("%s\t%s\t\t%s\t\t%s\n",name,role,empid,branch);
        else 
        printf("%s\t\t%s\t\t%s\t\t%s\n",name,role,empid,branch);
    }
    fclose(fp);
    int resp = 1;
    while(resp)
    {
        char a[2];
        printf("Enter any digit to continue: ");
        scanf("%s",a);
        if(strlen(a)<=2)
            if(a[0]>'0' && a[0]<'9')
                resp = 0;
    }
}
int view_customers(char* man_branch)
{
    int i = 0,j = 0;
    FILE* fptr = fopen("CUSTOMERS.dat"," rb");
    fseek(fptr, 0, SEEK_END);
    long size1 = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    customer cd[size];
    customer c;
    int resp=1,k = 0;
    while(fread(&c,sizeof(customer),1,fptr))
    {
        if(!strcmp(man_branch,c.branch) && c.ac_status != 0)
        {
            cd[k] = c;
            k++;
        }
    }
    fclose(fptr);
    j = 0;
    int res = 1;
    do
    {
        printf("%-20s%-20s%10s\n","Customer No","ID","Account no");
        while(j<k)
        {
            printf("%-20d%-20s%10s\n",j+1,cd[j].customer_personal_details.ID,cd[j].ac_no);
            j++;
        }
        printf("Enter customer number of the customer for detailed information and 0 to return to main menu\n");
        int choice;
        scanf("%d",&choice);
        if(choice<0 || choice>j)
        {
            printf("Invalid Application number!!\n");
        }
        else if(choice == 0)
        {
            return 1;
        }
        else 
        {
            printf("*****************************************************************************\n");
            printf("ID : %s\n",cd[choice-1].customer_personal_details.ID);
            //printf("ID : %s\n",cd[choice - 1].customer_personal_details.ID);
            printf("Branch : %s\n",cd[choice-1].branch);
            printf("Account No: %s\n",cd[choice-1].ac_no);
            printf("IFSC Code: %s\n",cd[choice-1].ifsc_code);
            printf("Balance : %.2f\n",cd[choice-1].balance);
            if(cd[choice-1].loan.loan_status != 0)
                printf("Loan : %.2f\n",cd[choice-1].loan.principal_amount);
        }
        printf("******************************************************************************\n");
        printf("Enter 1 to return to customer view menu or 0 to exit\n");
        scanf("%d",&res);
        j = 0;
    }while(res == 1);
    return 0;
}
int block(char* man_branch,branch_details load[],int flag)
{
    FILE* fp = fopen("customers.dat","rb");
    fseek(fp,0,SEEK_END);
    long size1 = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char field[100];
    printf("Enter the ID/Account number of the customer to be found: ");
    scanf(" %[^\n]s",field);
    customer c;
    customer* found[size];
    customer cd[5*size];
    int i = 0,k = 0;
    while(fread(&c,sizeof(customer),1,fp))
    {
        if(strcmp(c.branch,man_branch))
        {
            cd[k] = c;
            k++;
            continue;
        }
        if((!strcmp(c.ac_no,field) || !strcmp(c.customer_personal_details.ID,field)) && c.ac_status != 0)
        {
                printf("%d\n",c.ac_status);
                cd[k] = c;
                found[i] = &cd[k];
                k++;
                i++;
        }
    }
    fclose(fp);
    if(i != 0)
    {
        int res = 1;
        printf("Customers with matching credentials were found!\n");
        do
        {
            int j = 0,resp = 0;
            printf("%-5s|%-20s|%-20s\n","Sl.No","Name","Account number");
            while(j<i)
            {
                printf("%-5d|%-20s|%-20s\n",j+1,found[j]->customer_personal_details.ID,found[j]->ac_no);
                j++;
            }
            printf("Enter the Serial number to get detailed information and 0 to exit: ");
            scanf("%d",&resp);
            if(resp == 0)
                return 0;
            else if(resp > i)
                printf("Invalid serial number\n");
            else
            {
                printf("*****************************************************************************\n");
                printf("Name : %s\n",found[resp-1]->customer_personal_details.ID);
                printf("Branch : %s\n",found[resp-1]->branch);
                printf("Account No: %s\n",found[resp-1]->ac_no);
                printf("IFSC Code: %s\n",found[resp-1]->ifsc_code);
                printf("Balance : %.2f\n",found[resp-1]->balance);
                if(found[resp-1]->loan.loan_status != 0)
                   printf("Loan : %.2f\n",found[resp-1]->loan.principal_amount);
                if(found[resp-1]->ac_status == 1)
                {    
                    printf("STATUS : ACTIVE\n");
                    printf("********************************************************************************\n");
                    printf("Enter 1 to block this account 2 to delete and any other number to return to search results \n");
                    scanf("%d",&res);
                    if(res == 1)
                    {
                        found[resp - 1]->ac_status = -2;
                        printf("Account blocked!\n");
                    }  
                    else if(res == 2)
                    {
                        found[resp - 1]->ac_status = -1;
                        printf("Balance to be paid: %.2f\n",found[resp - 1]->balance);
                        printf("Enter any number to continue...\n");
                        int ch;
                        scanf("%d",&ch);
                        printf("Account Deleted!\n");
                    }
                }
                else if(found[resp-1]->ac_status == -2)
                {
                    printf("STATUS : BLOCKED\n");
                    printf("********************************************************************************\n");
                    printf("Enter 1 to unblock this account 2 to delete and any other number to return to search results \n");
                    scanf("%d",&res);
                    if(res == 1)
                    {
                        found[resp - 1]->ac_status = 1;
                        printf("Account reactived!\n");
                    }  
                    else if(res == 2)
                    {
                        found[resp - 1]->ac_status = -1;
                        printf("Balance to be paid: %.2f\n",found[resp - 1]->balance);
                        printf("Enter any number to continue...\n");
                        int ch;
                        scanf("%d",&ch);
                        printf("Account Deleted!\n");
                        load[flag].no_of_accounts--;
                        FILE* f = fopen("branchdetails.csv","w");
                        fprintf(f,"Branch,total_deposit,no_of_accounts,loans_granted,customer_ID\n");
                        for(int i = 0; i < 5; i++)
                        {
                            fprintf(f,"%s,%.2f,%d,%d,%d\n",load[i].branch,load[i].total_deposit,load[i].no_of_accounts,load[i].loans_granted,load[i].customer_ID);
                        }
                        fclose(f);
                    }
                }
                FILE* fp = fopen("CUSTOMERS.dat", "wb");
                for(int i = 0; i < size1 / sizeof(customer); i++)
                {
                    if(cd[i].ac_status == -1)
                        continue;
                    fwrite(&cd[i], sizeof(customer), 1, fp);
                }
                fclose(fp);  
            }
        }while(res != 1 && res!=2);
    }
    else
        printf("Not found!\n");
    return 0;
}
int generate_customer_ID(customer* c,branch_details *load)
{
    itoa(load->customer_ID,(*c).customer_personal_details.ID,10);
    load->customer_ID++;
    return 0;
}
int verify_applicants(char* man_branch,branch_details load[],int flag)
{
    int slno;
    FILE* fptr = fopen("CUSTOMERS.dat"," rb");
    fseek(fptr, 0, SEEK_END);
    long size1 = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    customer cd[size];
    customer c;
    int resp=1,k = 0,my_branch = 0;
    while(fread(&c,sizeof(customer),1,fptr))
    {
        if(!strcmp(man_branch,c.branch))
            my_branch++;
        cd[k] = c;
        k++;
    }
    fclose(fptr);
    while(1)
    {
        int i, j, count=0;
        time_t t;
        time(&t);
        printf("As of %s\n",ctime(&t));
        printf("%10s %-20s %-20s\n","Appl.no","Name","Verification status");
        for(i = 0;i<k; i++)
        {
            if(strcmp(cd[i].branch,man_branch))
                continue;
            if(cd[i].ac_status == 1 || cd[i].ac_status == -1)
            {
                count++;
                continue;
            }
            for(j = 0;j < 3; j++)
            {
                if(j == 0)
                    printf("%10d ",i+1);
                else if(j == 1)
                    printf("%-20s ",cd[i].customer_personal_details.ID);
                else
                    printf("%-20s ","PENDING");
            }
            printf("\n");
        }
        if(count == my_branch)
        {
            int ch;
            printf("No pending approvals\n\n");
            printf("Enter 1 to refresh and anyother number to return to main menu: ");
            scanf("%d",&ch);
            if(ch != 1)
                return 0;
            else 
            {
                verify_applicants(man_branch,load,flag);
                return 1;
            }
        }
        else
        {
            printf("\n\nEnter the Appl.no of applicant to get detailed information 0 to refresh and -1 to exit: ");
            scanf("%d", &slno);
            if(slno == -1)
                break;
            else if(slno == 0)
            {
                    verify_applicants(man_branch,load,flag);
                    return 0;
            }
            if(cd[slno-1].ac_status == 1 || cd[slno-1].ac_status == -1 ||slno > k || strcmp(cd[slno-1].branch,man_branch))
            {
             printf("Invalid Application number!\n\n");
             continue;
            }
            else
            {
                printf("*************************************************************************\n");
                printf("Name of the customer: %s\n", cd[slno-1].customer_personal_details.ID);
                printf("\n\nEnter 1 to activate this account, 0 to reject the application and anyother number to exit: ");
                scanf("%d", &resp);
                if(resp == 1)
                {
                    int ch;
                    printf("This account is now activated!\n");
                    //generate customer ID
                    cd[slno - 1].ac_status = 1;
                    load[flag].no_of_accounts++;
                    FILE* f = fopen("branchdetails.csv","w");
                    fprintf(f,"Branch,total_deposit,no_of_accounts,loans_granted,customer_ID\n");
                    for(int i = 0; i < 5; i++)
                    {
                        fprintf(f,"%s,%.2f,%d,%d,%d\n",load[i].branch,load[i].total_deposit,load[i].no_of_accounts,load[i].loans_granted,load[i].customer_ID);
                    }
                    fclose(f);
                    printf("Enter any number to return to main menu: ");
                    scanf("%d",&ch);
                    break;
                }
                else if(resp == 0)
                {
                    int ch;
                    printf("\n\nDo you want to reject this Appication?\nEnter 1 for yes:");
                    scanf("%d", &resp);
                    if(resp != 1)
                        continue;        
                    printf("Application for creation of account has been rejected!\n");
                    cd[slno - 1].ac_status = -1;//-1 if application is rejected
                    printf("Enter any number to return to main menu: ");
                    scanf("%d",&ch);
                    break;
                }
                else continue;
            }
        }
    }
    FILE* fp = fopen("CUSTOMERS.DAT", "wb");
    for(int i = 0; i < size1 / sizeof(customer); i++)
    {
        if(cd[i].ac_status == -1)
            continue;
        fwrite(&cd[i], sizeof(customer), 1, fp);
    }
    fclose(fp);
    return 0;
} 
int approve_loan_applicants(char* man_branch,branch_details load[],int flag)
{
    int slno;
    FILE* fptr = fopen("CUSTOMERS.dat"," rb");
    fseek(fptr, 0, SEEK_END);
    long size1 = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    customer cd[size];
    customer c;
    int resp=1,k = 0,my_branch = 0;
    while(fread(&c,sizeof(customer),1,fptr))
    {
        if(!strcmp(man_branch,c.branch))
            my_branch++;
        cd[k] = c;
        k++;
    }
    fclose(fptr);
    while(1)
    {
        int i, j, count=0;
        printf("%10s %-20s %-20s\n","Appl.no","ID","Approval status");
        for(i = 0;i<k; i++)
        {
            if(strcmp(cd[i].branch,man_branch))
                continue;
            if(cd[i].loan.loan_status != 2)
            {
                count++;
                continue;
            }
            for(j = 0;j < 3; j++)
            {
                if(j == 0)
                    printf("%10d ",i+1);
                else if(j == 1)
                    printf("%-20s ",cd[i].customer_personal_details.ID);
                else
                    printf("%-20s ","PENDING");
            }
            printf("\n");
        }
        if(count == my_branch)
        {
            int ch;
            printf("No pending approvals\n\n");
            printf("Enter 1 to refresh and anyother number to return to main menu: ");
            scanf("%d",&ch);
            if(ch != 1)
                return 0;
            else 
            {
                    approve_loan_applicants(man_branch,load,flag);
                    return 1;
            }
        }
        else
        {
            printf("\n\nEnter the Appl.no of applicant to get detailed information 0 to refresh and -1 to exit: ");
            scanf("%d", &slno);
            if(slno == -1)
                break;
            else if(slno == 0)
            {
                    approve_loan_applicants(man_branch,load,flag);
                    return 0;
            }
            if(cd[slno-1].loan.loan_status != 2 || slno > k)
            {
                printf("Invalid Application number!\n\n");
                continue;
            }
            else
            {
                printf("*************************************************************************\n");
                printf("Name of the customer: %s\n", cd[slno-1].customer_personal_details.ID);
                //printf("Category : %s\n", cd[slno-1].loan.type);
                printf("Duration: %d\n", cd[slno-1].loan.duration);
                printf("Principal amount : %.2f\n", cd[slno-1].loan.principal_amount);
                printf("\n\nEnter 1 to approve the application, 0 to reject and anyother number to exit: ");
                scanf("%d", &resp);
                if(resp == 1)
                {
                    int ch;
                    printf("This loan is now approved!\n");
                    cd[slno-1].balance += cd[slno-1].loan.principal_amount;
                    cd[slno - 1].loan.loan_status = 1;
                    load[flag].loans_granted++;
                    FILE* f = fopen("branchdetails.csv","w");
                    fprintf(f,"Branch,total_deposit,available_fund,net_profit_per_annum,net_profit_per_month,profit_percent,no_of_accounts,loans_granted,customer_ID\n");
                    for(int i = 0; i < 5; i++)
                    {
                        fprintf(f,"%s,%.2f,%d,%d,%d\n",load[i].branch,load[i].total_deposit,load[i].no_of_accounts,load[i].loans_granted,load[i].customer_ID);
                    }
                    fclose(f);
                    printf("Enter any number to return to main menu: ");
                    scanf("%d",&ch);
                    break;
                }
                else if(resp == 0)
                {
                    int ch;
                    printf("\n\nDo you want to reject this Appication?\nEnter 1 for yes:");
                    scanf("%d", &resp);
                    if(resp != 1)
                        continue;        
                    printf("Application for sanction of loan has been rejected!\n");
                    cd[slno - 1].loan.loan_status = -1;//-1 if application is rejected
                    printf("Enter any number to return to main menu: ");
                    scanf("%d",&ch);
                    break;
                }
                else continue;
            }
        }
    }
    FILE *fp;
    fp = fopen("CUSTOMERS.DAT", "wb");
    for(int i = 0; i < size1 / sizeof(customer); i++)
    {
        fwrite(&cd[i], sizeof(customer), 1, fp);
    }
    fclose(fp);
    return 0;
} 
int cashier(char* branch)
{
     do
    {
    branch_details load[5];
    FILE* fp = fopen("branchdetails.csv","r");
    int loader = 0,found = 0;
    char feed[1024];
    fgets(feed,1024,fp);
    while(fgets(feed,1024,fp))
    {
        strcpy(load[loader].branch,strtok(feed,","));
        load[loader].total_deposit = atof(strtok(NULL,","));
        load[loader].no_of_accounts = atoi(strtok(NULL,","));
        load[loader].loans_granted = atoi(strtok(NULL,","));
        load[loader].customer_ID = atoi(strtok(NULL,"\n"));
        if(!strcmp(load[loader].branch,branch))
            found = loader;
        loader++;
    }
    fclose(fp);
    FILE* fptr = fopen("CUSTOMERS.dat"," rb");
    fseek(fptr, 0, SEEK_END);
    long size1 = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    customer cd[5*size];
    customer c;
    int k = 0,my_branch = 0;
    while(fread(&c,sizeof(customer),1,fptr))
    {
        if(!strcmp(branch,c.branch))
            my_branch++;
        cd[k] = c;
        k++;
    }
    fclose(fptr);
    printf("1.Deposit\n");
    printf("2.Withdrawl\n");
    printf("3.Loan payment\n");
    printf("4.Logout\n");
    printf("Enter your choice: ");
    int resp,flag = 0, flag1 = 0;
    scanf("%d",&resp);
    char acc_no[20];
    int i;
    switch(resp)
    {
        case 1: printf("*************************************************************************\n");
                printf("Enter Account Number of the user: ");
                scanf(" %[^\n]s",acc_no);
                for(i = 0; i < k ; i++)
                {
                    if(!strcmpi(acc_no,cd[i].ac_no))
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                {
                    printf("Balance Available: %.2f\n",cd[i].balance);
                    printf("Enter the amount to be deposited: ");
                    float amount;
                    scanf("%f",&amount);
                    cd[i].balance += amount;
                    printf("Amount deposited successfully!\n");
                    /*load[found].available_fund += amount;
                    FILE* f = fopen("branchdetails.csv","w");
                    fprintf(f,"Branch,total_deposit,no_of_accounts,loans_granted,customer_ID\n");
                    for(int i = 0; i < 5; i++)
                    {
                        fprintf(f,"%s,%.2f,%d,%d,%d\n",load[i].branch,load[i].total_deposit,load[i].no_of_accounts,load[i].loans_granted,load[i].customer_ID);
                    }
                    fclose(f);*/
                    printf("Balance: %.2f\n",cd[i].balance);
                }
                else
                    printf("Account number not found...\n");
                break;
        case 2: printf("*************************************************************************\n");
                printf("Enter Account Number of the user: ");
                scanf(" %[^\n]s",acc_no);
                for(i = 0; i < k ; i++)
                {
                    if(!strcmpi(acc_no,cd[i].ac_no))
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                {
                    printf("Balance Available: %.2f\n",cd[i].balance);
                    printf("Enter the amount to be withdrawn: ");
                    float amount;
                    scanf("%f",&amount);
                    if(cd[i].balance - amount >= 500)
                    {
                        cd[i].balance -= amount;
                        printf("Amount withdrawn successfully!\n");
                        printf("Balance: %.2f\n",cd[i].balance);
                    }
                    else
                        printf("Unable to withdraw, minimum balance should be Rs.500/-\n");
                }
                else
                    printf("Account number not found...\n");
            break;
        case 3: printf("*************************************************************************\n");
                printf("Enter Account Number of the user: ");
                scanf(" %[^\n]s",acc_no);
                for(i = 0; i < k ; i++)
                {
                    if(!strcmpi(acc_no,cd[i].ac_no))
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                {
                    if(cd[i].loan.loan_status == 1)
                    {
                        printf("Loan details:\n");
                        printf("Name: %s\n",cd[i].customer_personal_details.ID);
                        printf("Loan amount: %.2f\n",cd[i].loan.principal_amount);
                        //printf("Loan type: %s\n",cd[i].loan.type);
                        float emi = cd[i].loan.principal_amount*(cd[i].loan.rate_of_interest/100)*pow((1+cd[i].loan.rate_of_interest/100),cd[i].loan.duration)/(pow((1+cd[i].loan.rate_of_interest/100),cd[i].loan.duration)-1);
                        printf("EMI payable: %.2f\n",emi);
                        printf("Enter 1 to withdraw the emi: ");
                        int r;
                        scanf("%d",&r);
                        if(r == 1)
                        {
                        if(cd[i].balance - emi >= 500)
                        {
                             cd[i].balance -= emi;
                             printf("Amount withdrawn successfully!\n");
                             printf("Balance: %.2f\n",cd[i].balance);
                             cd[i].loan.principal_amount -= emi - (cd[i].loan.rate_of_interest*cd[i].loan.principal_amount)/1200;
                             printf("Principal Remaining: %.2f\n",cd[i].loan.principal_amount);
                        }
                        else
                            printf("Unable to withdraw, minimum balance should be Rs.500/- after withdrawal\n");
                        }
                    }
                    else if(cd[i].loan.loan_status == 0)
                    {
                        printf("The user has not applied for any loan\n");
                    }
                    else if(cd[i].loan.loan_status == 2)
                    {
                        printf("The customer's loan application is still being processed\n");
                    }
                    else
                    {
                        printf("The customer's loan application has been rejected\n");
                    }
                }
                else
                    printf("Account number not found...\n");
                break;
        case 4:
            flag1 = 1;
            break;
        default:
            printf("Invalid choice!!\n");
    }
    fp = fopen("CUSTOMERS.DAT", "wb");
    for(int i = 0; i < size1 / sizeof(customer); i++)
    {
        fwrite(&cd[i], sizeof(customer), 1, fp);
    }
    fclose(fp);
    if(flag1 == 1)
    {
        break;
    }
    }while(1);
    return 0;
}

void view_details(char key)
{
    FILE *fp;
    if(key == 'T')
    {
        fp = fopen("transaction_details.txt", "r");
    }
    else
    {
        fp = fopen("deposit_records.txt", "r");
    }
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char s[300], temp[300] = "\0";
    while(fgets(s, 300, fp) != NULL)
    {
        s[strlen(s) - 1] = '\0';
        char* temp1 = decrypt(s, temp);
        //temp1[strlen(temp1) - 1] = '\0';
        printf("%s\n", temp1);
    }
    fclose(fp);
}

void view_feedback()
{
    FILE *fp;
    fp = fopen("feedback.txt", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char buffer[500];
    while(fgets(buffer, 500, fp))
    {
        printf("%s", buffer);
    }
    fclose(fp);
}