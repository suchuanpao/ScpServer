/*************************************************************************
	> File Name: rbt.c
	> Author: Chuanpao Su
	> Mail: 291657954@qq.com
	> Created Time: Fri 22 Jul 2016 07:22:33 AM PDT
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//数据的操作函数
typedef struct __DataOpt {
    int32_t (*Compare)(void *cond, void * data);//数据对比方法，第一个参数为对比的条件
    int32_t (*AddCompare)(void *cond, void * data);//数据对比方法，第一个参数为对比的条件
    int32_t (*FindCompare)(void *cond, void * data);//数据对比方法，第一个参数为对比的条件
    int32_t (*Edit)(void *data);//如果不需要重新排列返回0，否则返回1
    int32_t (*EditCompare)(void *cond, void * data);//如果不需要重新排列返回0，否则返回1
    void * (*Copy)(void * data);//数据拷贝方法
    int32_t (*Value)(void * data);//获取该数据的加权值
    int32_t (*Release)(void * data);//删除数据
    int32_t (*ReleaseCompare)(void * data);//删除数据
}DataOpt;

//形容数据的结构体，内部包含数据操作函数，以及以便实现数据默认处理方式的数据一些相关信息
typedef struct __DataDesc {
    int32_t data_size;//数据长度
    int32_t data_type;//数据类型
    DataOpt * opt;//数据操作函数
} DataDesc;

typedef enum __DATABASE_TYPE
{
    RBTREE = 0,
}DATABASE_TYPE;
typedef enum __ABT_COLOR {
    BLACK_NODE = 0,
    RED_NODE = 1,
}ABT_COLOR;

typedef struct __DataBaseHead DataBaseHead;
//数据库的增删改查的方式，不同的增删改查，意味着不同的数据结构
typedef struct __DataBaseOpt
{
    int32_t (*Delete)(DataBaseHead * head, void * cond);//删
    int32_t (*Add)(DataBaseHead * head, void * data);//增
    void * (*Find)(DataBaseHead * head, void * cond);//查,会拷贝
    int32_t (*Edit)(DataBaseHead * head, void * cond);//改
    int32_t (*Push)(DataBaseHead * head, void * cond);//拿出来，从数据结构中删除
    int32_t (*Pop)(DataBaseHead * head, void * cond);//拿出来，从数据结构中删除
}DataBaseOpt;

//数据库头节点，管理整个数据库

struct __DataBaseHead
{
    void * head;//头指针
    int n_counts;//节点个数
    int max_deep;//树深度
    DataDesc data_desc;//数据类型
    DataBaseOpt * opt;//数据库的增删改查的方式，意味着不同的数据结构
};

typedef struct __Rbt {
    struct __Rbt * left;//左子
    struct __Rbt * right;//右子
    struct __Rbt * parent;//父
    ABT_COLOR cl;//颜色
    void * data;//数据
}Rbt;

int32_t RbtDelete(DataBaseHead * head, void * cond);//删除符合条件cond的节点，通过设置head->data_desc.opt->Compare方法设置判断规则
int32_t RbtAdd(DataBaseHead * head, void * data);//添加一个节点,通过设置head->data_desc.opt->Copy方法设置拷贝规则
void * RbtFind(DataBaseHead * head, void * cond);//查找符合条件cond的节点，通过设置Rbthead->data_desc.opt->Compare方法设置判断规则
int32_t RbtEdit(DataBaseHead * head, void * cond);//编辑符合条件cond的节点，通过设置head->data_desc.opt->Compare方法设置判断规则


DataBaseOpt Rbtopt = {
    .Add = RbtAdd,
    .Delete = RbtDelete,
    .Find = RbtFind,
    .Edit = RbtEdit
};


int32_t InitDataBaseHead(DataBaseHead * head, DATABASE_TYPE type, DataDesc * data_desc)
{
    assert(head);
    memset(head,0,sizeof(*head));//清空
    memcpy(&(head->data_desc), data_desc, sizeof(head->data_desc));//获取数据类型以及数据操作方式
    switch(type)
    {
    case RBTREE:
        head->opt = &Rbtopt;
        break;
    }
}

//增加
/*
*   param@1:    头节点，用于管理整个红黑树
*   param@2:    添加的数据，节点类型的数据
*   use:必须实现head->data_desc->opt.Copy函数与head->data_desc.opt->Compare函数用于
* */
int RbtAdd(DataBaseHead * head, void * data)
{
    assert(head);
    assert(head->data_desc.opt);
    assert(head->data_desc.opt->Compare);

    Rbt * new_node = NULL;
    Rbt** insert_node = NULL;

    void * new_data = NULL;
    int ret = 0;

    DataDesc * data_desc = &head->data_desc;
    DataOpt * data_opt = data_desc->opt;


    //new node
    new_node = (Rbt *)malloc(sizeof(Rbt));
    if (!new_node) {
        perror("RbtAdd malloc new_node failed");
        goto add_node_err;
    }
    memset(new_node, 0, sizeof(*new_node));

    //new data
    if (data_opt->Copy) {
        new_data = data_opt->Copy(data);
    } else {
        new_data = malloc(data_desc->data_size);
    }
    if (!new_data) {
        perror("RbtAdd cpy data failed");
        goto add_node_err;
    }
    new_node->data = new_data;

    //Find insert node
    //lock rbt
    insert_node = (Rbt**)&(head->head);
    while (*insert_node) {
        if (data_opt->Compare)
            ret = data_opt->EditCompare(data, (*insert_node)->data);
        else
            ret = memcmp(data, (*insert_node)->data, data_desc->data_size);

        if (ret == 0) {
            perror("there is a same data in this rbt!");
            goto add_node_err;
        } 
        else if (ret > 0)
            *insert_node = (*insert_node)->right;
        else if (ret < 0)
            *insert_node = (*insert_node)->left;
    }
    *insert_node = new_node;
    //unlock rbt
    return 0;
    
    //error
add_node_err:
    if (new_data) {
        if (data_opt->Release) {
            data_opt->Release(new_data);
        } else {
            free(new_data);
        }
    }
    if (new_node)
        free(new_node);
    return -1;
}
//删除
int32_t RbtDelete(DataBaseHead * head, void * cond)
{
    assert(head);
    assert(head->data_desc.opt);
    assert(head->data_desc.opt->Compare);

    Rbt** delete_node = NULL;
    int ret = 0;

    DataDesc * data_desc = &head->data_desc;
    DataOpt * data_opt = data_desc->opt;

    //lock rbt
    delete_node = (Rbt**)&(head->head);
    while (*delete_node) {
        if (data_opt->Compare)
            ret = data_opt->Compare(cond, (*delete_node)->data);
        else
            ret = memcmp(cond, (*delete_node)->data, data_desc->data_size);

        if (ret == 0)
            break;
        else if (ret > 0)
            *delete_node = (*delete_node)->right;
        else if (ret < 0)
            *delete_node = (*delete_node)->left;
    }
    //delete_node
    if (data_opt->Release) {
        data_opt->Release((*delete_node)->data);
    } else {
        free((*delete_node)->data);
    }
    free(*delete_node);
    *delete_node = NULL;
    return 0;
}
//查找
void * RbtFind(DataBaseHead * head, void * cond)
{
    assert(head);
    assert(head->data_desc.opt);
    assert(head->data_desc.opt->Compare);

    Rbt ** find_node = NULL;
    int ret = 0;
    DataDesc * data_desc = &head->data_desc;
    DataOpt * data_opt = data_desc->opt;

    //lock rbt
    find_node = (Rbt**)&(head->head);
    while (*find_node) {
        if (data_opt->Compare)
            ret = data_opt->Compare(cond, (*find_node)->data);
        else
            ret = memcmp(cond, (*find_node)->data, data_desc->data_size);

        if (ret == 0) {
            if (data_opt->Copy) {
                return data_opt->Copy((*find_node)->data);
            }
            else {
                void * tmp = malloc(data_desc->data_size * sizeof(char));
                memcpy(tmp, (*find_node)->data, data_desc->data_size * sizeof(char));
                return tmp;
            }
        }
        else if (ret > 0)
            *find_node = (*find_node)->right;
        else if (ret < 0)
            *find_node = (*find_node)->left;
    }
    return NULL;
}
//修改
int32_t RbtEdit(DataBaseHead * head, void * cond)
{
    assert(head);
    assert(head->data_desc.opt);
    assert(head->data_desc.opt->Compare);

    Rbt ** edit_node = NULL;
    int ret = 0;
    int err = 0;
    DataDesc * data_desc = &head->data_desc;
    DataOpt * data_opt = data_desc->opt;

    //lock rbt
    edit_node = (Rbt**)&(head->head);
    while (*edit_node) {
        if (data_opt->Compare)
            ret = data_opt->Compare(cond, (*edit_node)->data);
        else
            ret = memcmp(cond, (*edit_node)->data, data_desc->data_size);

        if (ret == 0) {
            err = data_opt->Edit((*edit_node)->data);
            if (err != 0) {
                
            }
        }
        else if (ret > 0)
            *edit_node = (*edit_node)->right;
        else if (ret < 0)
            *edit_node = (*edit_node)->left;
    
    }
}


int main()
{
    Rbt *head = malloc(sizeof(Rbt));
    if (!head) {
        perror("error!");
    }
    memset(&head,0,sizeof(head));
//    RbtAdd(&head);
    return 0;
}
