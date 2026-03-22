#include <stdio.h>//输出功能和输入功能 
#include <stdlib.h>//实现申请内存功能 、释放内存的功能。 
#include <string.h>//用字符串类似的东西 。 

// 定义图书结构体（单链表节点）
typedef struct Book {
    int id;                 // 图书编号（唯一标识）
    char name[50];          // 图书名称
    char author[30];        // 作者
    struct Book* next;      // 指向下一个节点的指针
} BookNode;

// 函数声明
BookNode* initLibrary();    // 创建头节点（初始化链表）
void addBook(BookNode* head); // 添加图书（尾部插入）
void deleteBook(BookNode* head); // 删除图书（按编号删除）
void modifyBook(BookNode* head); // 修改图书信息（按编号修改）
void queryBook(BookNode* head);  // 支持重复查询的图书查询功能
void showAllBooks(BookNode* head); // 显示所有图书
void freeLibrary(BookNode* head); // 释放链表内存

int main() {
    int choice;
    // 初始化图书馆（创建头节点，头节点不存储实际图书信息）
    BookNode* library = initLibrary();
    
    if (library == NULL) {
        printf("初始化图书馆失败！\n");
        return -1;
    }

    // 主菜单循环
    while (1) {
        printf("\n======= 图书馆图书管理系统 =======\n");
        printf("1. 添加图书\n");
        printf("2. 删除图书\n");
        printf("3. 修改图书信息\n");
        printf("4. 查询图书\n");
        printf("5. 显示所有图书\n");
        printf("0. 退出系统\n");
        printf("===================================\n");
        printf("请输入操作序号：");
        scanf("%d", &choice);
        getchar(); // 吸收换行符，避免影响后续字符串输入

        switch (choice) {
            case 1:
                addBook(library);
                break;
            case 2:
                deleteBook(library);
                break;
            case 3:
                modifyBook(library);
                break;
            case 4:
                queryBook(library); // 进入查询子菜单（支持重复查询）
                break;
            case 5:
                showAllBooks(library);
                break;
            case 0:
                freeLibrary(library);
                printf("已退出系统，内存已释放！\n");
                return 0;
            default:
                printf("输入错误，请重新选择！\n");
        }
    }
}

// 初始化链表：创建空的头节点
BookNode* initLibrary() {
    BookNode* head = (BookNode*)malloc(sizeof(BookNode));
    if (head == NULL) {
        return NULL;
    }
    head->next = NULL; // 头节点指针置空，表示链表为空
    return head;
}

// 添加图书：尾部插入（保证图书顺序添加）
void addBook(BookNode* head) {
    BookNode* newBook = (BookNode*)malloc(sizeof(BookNode));
    if (newBook == NULL) {
        printf("内存分配失败，添加图书失败！\n");
        return;
    }

    // 输入图书信息
    printf("请输入图书编号：");
    scanf("%d", &newBook->id);
    getchar(); // 吸收换行符
    printf("请输入图书名称：");
    
    /*从键盘（stdin）读取用户输入的书名，
	存到 newBook 这个图书结点的 name 成员里，
	最多读 49 个字符（留 1 个存字符串结束符 \0）。*/
    fgets(newBook->name, 50, stdin);
    newBook->name[strcspn(newBook->name, "\n")] = '\0'; // 去掉换行符
    printf("请输入图书作者：");
    fgets(newBook->author, 30, stdin);
    newBook->author[strcspn(newBook->author, "\n")] = '\0';
    newBook->next = NULL;

    // 找到链表尾部，检查编号是否重复
    BookNode* p = head;
    while (p->next != NULL) {
        if (p->next->id == newBook->id) {
            printf("图书编号%d已存在，添加失败！\n", newBook->id);
            free(newBook);/*把刚才为这本新书 newBook
			 申请的内存空间还给操作系统，
			避免内存泄漏。*/ 
            return;
        }
        p = p->next;
    }

    p->next = newBook;
    printf("图书添加成功！\n");
}

// 删除图书：按编号删除
void deleteBook(BookNode* head) {
    if (head->next == NULL) {
        printf("图书馆暂无图书，无需删除！\n");
        return;
    }

    int delId;
    printf("请输入要删除的图书编号：");
    scanf("%d", &delId);

    BookNode* pre = head;
    BookNode* p = head->next;
    while (p != NULL) {
        if (p->id == delId) {
            pre->next = p->next;
            free(p);
            printf("图书编号%d删除成功！\n", delId);
            return;
        }
        pre = p;
        p = p->next;
    }

    printf("未找到编号为%d的图书，删除失败！\n", delId);
}

// 修改图书信息：按编号修改
void modifyBook(BookNode* head) {
    if (head->next == NULL) {
        printf("图书馆暂无图书，无需修改！\n");
        return;
    }

    int modId;
    printf("请输入要修改的图书编号：");
    scanf("%d", &modId);
    getchar();/*“吃掉” scanf 留下的换行符 **，
	避免后面用 fgets 读字符串时出问题。*/

    BookNode* p = head->next;
    while (p != NULL) {
        if (p->id == modId) {
            printf("找到图书编号%d，开始修改：\n", modId);
            printf("请输入新的图书名称（原：%s）：", p->name);
            fgets(p->name, 50, stdin);
            p->name[strcspn(p->name, "\n")] = '\0';// 去掉换行符
            printf("请输入新的图书作者（原：%s）：", p->author);
            fgets(p->author, 30, stdin);
            p->author[strcspn(p->author, "\n")] = '\0';
            printf("图书信息修改成功！\n");
            return;
        }
        p = p->next;
    }

    printf("未找到编号为%d的图书，修改失败！\n", modId);
}

// 查询图书：支持重复查询（子菜单循环）
void queryBook(BookNode* head) {
    if (head->next == NULL) {
        printf("图书馆暂无图书！\n");
        return;
    }

    int subChoice;
    // 查询子菜单循环（支持重复查询）
    while (1) {
        printf("\n======= 图书查询子菜单 =======\n");
        printf("1. 按编号查询\n");
        printf("2. 按名称查询\n");
        printf("3. 按作者查询\n");
        printf("0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入查询方式序号：");
        scanf("%d", &subChoice);
        getchar(); // 吸收换行符

        char key[50];
        int keyId;
        int findFlag = 0;
        BookNode* p = head->next;

        switch (subChoice) {
            case 1: // 按编号查询
                printf("请输入查询的图书编号：");
                scanf("%d", &keyId);
                while (p != NULL) {
                    if (p->id == keyId) {
                        printf("\n===== 找到图书 =====\n");
                        printf("编号：%d\n", p->id);
                        printf("名称：%s\n", p->name);
                        printf("作者：%s\n", p->author);
                        findFlag = 1;/*是一个标记变量赋值，
						用来记录「已经找到目标图书」这件事。*/
                        break;
                    }
                    p = p->next;
                }
                break;

            case 2: // 按名称查询
                printf("请输入查询的图书名称：");
                fgets(key, 50, stdin);
                key[strcspn(key, "\n")] = '\0';
                while (p != NULL) {
                    if (strcmp(p->name, key) == 0) {
                        printf("\n===== 找到图书 =====\n");
                        printf("编号：%d\n", p->id);
                        printf("名称：%s\n", p->name);
                        printf("作者：%s\n", p->author);
                        findFlag = 1;
                        break;
                    }
                    p = p->next;
                }
                break;

            case 3: // 按作者查询
                printf("请输入查询的图书作者：");
                fgets(key, 30, stdin);
                key[strcspn(key, "\n")] = '\0';
                while (p != NULL) {
                    if (strcmp(p->author, key) == 0) {
                        printf("\n===== 找到图书 =====\n");
                        printf("编号：%d\n", p->id);
                        printf("名称：%s\n", p->name);
                        printf("作者：%s\n", p->author);
                        findFlag = 1;
                        break;
                    }
                    p = p->next;
                }
                break;

            case 0: // 返回主菜单
                printf("退出查询，返回主菜单...\n");
                return; // 退出查询子循环，回到主菜单

            default:
                printf("查询方式选择错误，请重新输入！\n");
                continue; // 重新显示查询子菜单
        }

        // 提示是否找到图书
        if (subChoice >= 1 && subChoice <= 3 && !findFlag) {
            printf("未找到符合条件的图书！\n");
        }

        // 每次查询后换行，提升可读性
        printf("\n");
    }
}

// 显示所有图书
void showAllBooks(BookNode* head) {
    if (head->next == NULL) {
        printf("图书馆暂无图书！\n");
        return;
    }

    printf("\n======= 图书馆所有图书 =======\n");
    BookNode* p = head->next;
    int count = 0;
    while (p != NULL) {
        count++;
        printf("第%d本：\n", count);
        printf("编号：%d\n", p->id);
        printf("名称：%s\n", p->name);
        printf("作者：%s\n", p->author);
        printf("------------------------\n");
        p = p->next;
    }
    printf("共查询到%d本图书\n", count);
}

// 释放链表所有内存
void freeLibrary(BookNode* head) {
    BookNode* p = head;
    BookNode* q = NULL;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
}
