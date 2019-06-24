#pragma once

#include <fun.h>

#define ORDER 4     //B树的阶数

typedef int KeyType;  
typedef struct BTNode
{  
    int keynum;                        // 结点中关键字的个数，keynum <= ORDER-1  
    KeyType key[ORDER-1];                // 关键字数组，长度=阶数-1  
    struct BTNode* child[ORDER];        // 孩子指针数组，长度=阶数  
    struct BTNode* parent;              //指向双亲的指针
    int isLeaf;                    // 是否是叶子节点的标志  

}BTNode;  

typedef BTNode* BTree;  //定义BTree 

//创建一个空的B树
BTree BTree_init();
//给定数据集data,创建BTree。  
void BTree_create(BTree* tree, const KeyType* data, int length);  
//分裂一个满的子树
void split_tree(BTree *tree);
//销毁BTree，释放内存空间。  
void BTree_destroy(BTree* tree);  
//判断一个节点是否是满节点
int isfull(BTNode *node);
//在BTree中插入关键字key。  
void BTree_insert(BTree* tree, KeyType key);  

//在BTree中移除关键字key。  
void BTree_remove(BTree* tree, KeyType key);  

//深度遍历BTree打印各层结点信息。  
void BTree_print(const BTree tree);  

/*在BTree中查找关键字 key，  
成功时返回找到的节点的地址及 key 在其中的位置 *pos  
失败时返回 NULL 及查找失败时扫描到的节点位置 *pos*/  
BTNode* BTree_search(const BTree tree, int key, int* pos);

