#include "btree.h"
int main()
{
    KeyType data[10] = {15,2,3,45,6,7,8,9,78,34};
    BTree bt;
    /*printf("请输入十个数字：\n");
    for(int i =0;i<10;i++)
    {
        scanf("%d",data+i);
    }*/
    BTree_create(&bt,data,10);
    printf("得到的b树如下：\n");
    BTree_print(bt);
    return 0;
}

BTree BTree_init()
{
    BTree bt =(BTree)calloc(1,sizeof(BTNode));
    bt->isLeaf = 1;
    bt->keynum = 0;
    bt->parent = NULL;
    return bt;
}
void BTree_create(BTree *tree,const KeyType *data,int length)
{
    *tree = BTree_init();
    for(int i = 0;i<length;i++)
    {
        BTree_insert(tree,data[i]);
    }
}
int isfull(BTNode *node)
{
    if(node == NULL)
    {
        return 0;
    }
    if(node->keynum < ORDER-1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void BTree_insert(BTree *tree,KeyType key)
{
    
    BTNode *bnp = *tree;
    if(tree ==NULL)
    {
        printf("NULL key =%d\n",key);
    }
    if(!bnp->isLeaf)
        printf("insert key =%d,bnp->key[0]=%d,bnp->child[0]->key[0]=%d\n",key,bnp->key[0],bnp->child[0]->key[0]);
    sleep(1);
    //根节点满，直接进行分裂
    if(isfull(*tree))
    {
        split_tree(tree);
        BTree_insert(tree,key);
        return;
    }
    if(!bnp)
    printf("bnp=null");
    //是叶子节点且不满，直接插入
    if(bnp->isLeaf && !isfull(bnp))
    {
        printf("insert key =%d,bnp->key[0]=%d\n",key,bnp->key[0]);
        //比最大的关键字都大，直接插在末尾
        if(key>=bnp->key[bnp->keynum -1])
        {
            bnp->keynum++;
            bnp->key[bnp->keynum-1] = key;
        }
        else
        {
            for(int i = 0;i<ORDER-1;i++)
            {
                //找到一个比待插入关键字大的关键字，则直接插入
                if(key< bnp->key[i] )
                {
                    KeyType  temp= bnp->key[i];
                    bnp->key[i] = key;
                    bnp->keynum++;
                    for(int j = i + 1;j< bnp->keynum;j++)
                    {
                        key = bnp->key[j];
                        bnp->key[j] = temp;
                        temp = key;
                    }

                    break;
                }
            }
        }
        return;
    }
    if(!bnp->isLeaf)
    {
        printf("in:3 keynum=%d\n",bnp->keynum);
        for(int i = 0;i < bnp ->keynum;i++)
        {
            printf("key[%d]=%d\t",i,bnp->key[i]);
            if(key<bnp->key[i])
            {
                printf("<insert i =%d\t>\t",bnp->child[i]->key[0]);
                BTree_insert(&(bnp->child[i]),key);
                return;
            }
        }
        printf("\n");
        BTree_insert(&(bnp->child[bnp->keynum]),key);
    }
}

void split_tree(BTree *tree)
{
    BTree bnp1 = *tree;
    BTree bnp2 = BTree_init();
    BTree bp;
    int num = bnp1->keynum;
    int split = num/2;
    if(bnp1->parent == NULL)
    {
         bp = BTree_init();
         bp->parent = NULL;
         bp->keynum = 1;
         bp->isLeaf = 0;
         bp->key[0] = bnp1->key[split];
         bp->child[0] = bnp1;
         bp->child[1] = bnp2;
    }
    else
    {
        bp = bnp1->parent;
        bp->isLeaf = 0;
        bp->keynum++;
        KeyType temp1, temp2;
        BTNode *tcp1, *tcp2;
        for(int i = 0;i < bp->keynum;i++)
        {
            //新关键字插到末尾
            if(i == bp->keynum-1)
            {
                bp->key[i] = bnp1->key[split];
                bp->child[i] = bnp1;
                bp->child[i+1] = bnp2;
                break;
            }
            //新关键字插到中间
            if(bp->key[i]>bnp1->key[split])
            {
                tcp2 = bnp2;
                temp2 = bnp1->key[split];
                for(int k = i;k<bp->keynum;k++)
                {
                    //关键字后移
                    temp1 = bp->key[k];
                    bp->key[k] = temp2;
                    temp2 = temp1;
                    //子树指针后移
                    tcp1 = bp->child[k+1];
                    bp->child[k+1] = tcp2;
                    tcp2 = tcp1;
                }
            }

        }
    }
    
    bnp1->keynum = split;
    bnp2->keynum = num - split -1;
    for(int i = 0, j = split+1;j<num;j++)
    {
        bnp2->key[i]=bnp1->key[j];
        bnp2->child[i]=bnp1->child[j];
    }
    bnp2->child[bnp2->keynum] = NULL;
    bnp2->isLeaf = bnp1->isLeaf;
    bnp2->parent = bp;
    bnp1->parent = bp;
    
    (*tree) = bp;
}

void BTree_print(const BTree tree)
{
    static int i = 0;
    i++;
    if(i >=10)
    {
        return;
    }
    if(tree ==NULL)
    {
        return;
    }
    else
    {
        for(int i = 0;i< tree->keynum;i++)
        {
            printf("%d\n",tree->key[i]);
        }
        if(!tree->isLeaf)
        {
            for(int i = 0;i<=tree->keynum;i++)
            {
                BTree_print(tree->child[i]);
            }
        }
    }
}
