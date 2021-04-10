#ifndef FILE_FILE_H
#define FILE_FILE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char name[100];

typedef struct The_users
{
    char id[11];
    char pwd[20];
} users;


void create_file()
{
    FILE *fp;
    if ((fp = fopen("users.txt", "rb")) == NULL) /*如果此文件不存在*/
    {
        if ((fp = fopen("users.txt", "wb+")) == NULL)
        {
            printf("创建文件失败");
        }
    }
}

void login()
{
    //定义结构体The_users别名
    users a, b; 

    FILE *fp;

    fp = fopen("users.txt", "r");

    //读入一个结构体字符块 写入b
    fread(&b, sizeof(struct The_users), 1, fp); 

    printf("--- 登录 ---\n");
    printf("请输入用户名：");
    scanf("%s", a.id);

    while (1)
    {
        //如果有此用户名
        if (strcmp(a.id, b.id) == 0) 
        {
            break;
        }
        else
        {
            //如果文件没有读完
            if (!feof(fp)) 

            {
                fread(&b, sizeof(struct The_users), 1, fp);
                continue;
            }
            else
            {
                printf("用户不存在!\n");
                return;
            }
        }
    }
    while (1)
    {
        printf("请输入密码:");
        scanf("%s", a.pwd);
        if (strcmp(a.pwd, b.pwd) == 0) 

        {

            fclose(fp);
            printf("登陆成功!\n");
            break;
        }
        else
        {
            printf("密码错误!\n");
            continue;
        }
    }
    strcpy(name, a.id);
}

/* 按结构体的大小，一次写入或者读入1个结构体大小 */
void registered() 
{
    users a, b; 
    FILE *fp;
    fp = fopen("users.txt", "r");

    fread(&b, sizeof(struct The_users), 1, fp); 

    printf("--- 注册 ---\n");
    printf("请输入注册用户名：");
    scanf("%s", a.id);

    while (1)
    {
        if (strcmp(a.id, b.id) != 0) 
        {
            if (!feof(fp)) 
            {
                fread(&b, sizeof(struct The_users), 1, fp);
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            printf("需要注册的用户已存在\n");
            login();
        }
    }

    printf("请输入密码:");
    scanf("%s", a.pwd);

    fp = fopen("users.txt", "a+");
    fwrite(&a, sizeof(struct The_users), 1, fp);
    printf("用户注册成功!\n");
    fclose(fp);
    login();
    
}
#endif //FILE_FILE_H
