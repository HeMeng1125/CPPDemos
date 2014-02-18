#include <stdlib.h>
#include <string.h>
#include "BinomialHeap.h"

#define MAX 1000

//创建一个空的二项堆
pBinHeap CreateBinHeap()
{
	pBinHeap newHeap = (pBinHeap)malloc(sizeof(BinNode)); 
	memset(newHeap, 0, sizeof(BinNode)); 
	return newHeap;
}

//寻找最小关键字
pBinNode SearchMinKey(pBinHeap pHeap)
{ 
	pBinNode y = NULL; 
	pBinNode x = pHeap; 
	int min = MAX; 
	while(x) 
	{
		if (x->key<min)
		{
			min = x->key;
			y = x;
		}
		x = x->sibling;
	}
	return y;
}

//连接两个二项堆
void Link(pBinHeap pH1, pBinHeap pH2)
{
	pBinNode pH1Parent = pH1->parent;
	pH1->sibling = pH1Parent->child;
	pH1Parent->child = pH1;
	++(pH1Parent->degree);
}

//将H1, H2的根表合并成一个按度数的单调递增次序排列的链表
pBinHeap Merge(pBinHeap pH1, pBinHeap pH2)
{    
	//heap->堆的首地址，H3为指向新堆根结点    
	pBinHeap heap = NULL, firstHeap = NULL, secondHeap = NULL, pre_H3 = NULL, H3 = NULL; 
	if (pH1 != NULL && pH2 != NULL)
	{      
		firstHeap = pH1; 
		secondHeap = pH2;
		//整个while，firstHeap, secondHeap, pre_H3, H3都在往后顺移        
		while (firstHeap != NULL && secondHeap != NULL) 
		{           
			if (firstHeap->degree <= secondHeap->degree) 
			{               
				H3 = firstHeap;  
				firstHeap = firstHeap->sibling; 
			} 
			else
			{   
				H3 = secondHeap;
				secondHeap = secondHeap->sibling;  
			}  

			if (pre_H3 == NULL)
			{             
				pre_H3 = H3; 
				heap = H3;  
			} 
			else
			{    
				pre_H3->sibling = H3;
				pre_H3 = H3;    
			}  

			if (firstHeap != NULL)
				H3->sibling = firstHeap; 
			else 
				H3->sibling = secondHeap; 
		}//while    
	} 
	else if (pH1 != NULL) 
		heap = pH1;   
	else 
		heap = pH2; 
	pH1 = pH2 = NULL;
	return heap;
}

//合并两个二项堆
pBinHeap Union(pBinHeap pH1, pBinHeap pH2)
{
	pBinHeap pre_x = NULL, x = NULL, next_x = NULL;
	pBinHeap newH= CreateBinHeap();
	newH = Merge(pH1,pH2);
	if(newH == NULL) return newH;

	x = newH;
	next_x = x->sibling;

	while (next_x)
	{
		if (x->degree != next_x->degree || ( next_x->sibling != NULL && next_x->sibling->degree == x->degree))
		{
			pre_x = x;
			x = next_x;
		}
		else if (x->key <= next_x->key)
		{
			x->sibling = next_x->sibling;
			Link(next_x,x);
		}
		else 
		{
			if (pre_x == NULL)
				newH = next_x;
			else
				pre_x->sibling = next_x;
			Link(x, next_x);
			x = next_x;
		}
		next_x = x->sibling;
	}
	return newH;
}

//插入一个节点到二项堆中
void Inerst(pBinHeap pH, BinNode node)
{
	pBinHeap newH = CreateBinHeap();
	node.parent = NULL;
	node.child = NULL;
	node.sibling = NULL;
	node.degree = 0;
	newH = &node;
	pH = Union(pH,newH);
}

//抽取有最小关键字的结点
pBinHeap ExtractMin(pBinHeap heap)
{   
	pBinHeap pre_y = NULL, y = NULL, x = heap;
	int min = INT_MAX;   
	while (x != NULL) 
	{       
		if (x->key < min) 
		{          
			min = x->key; 
			pre_y = y;   
			y = x;      
		}       
		x = x->sibling;
	}    
	if (y == NULL) 
	{       
		return NULL; 
	}    
	if (pre_y == NULL) 
	{       
		heap = heap->sibling;
	}
	else
	{   
		pre_y->sibling = y->sibling;
	}     
	//将y的子结点指针reverse  
	pBinHeap H2 = NULL, p = NULL; 
	x = y->child; 
	while (x != NULL)
	{      
		p = x;    
		x = x->sibling; 
		p->sibling = H2; 
		H2 = p;     
		p->parent = NULL;   
	}     
	heap = Union(heap, H2);  
	return y;
}

//减小关键字的值
void Decrease(pBinHeap pH, pBinNode x, int k)
{

	pBinNode y,z;
	if (k>x->key)
		return;
	x->key = k;
	y = x;
	z = y->parent;
	while (z && y->key<z->key)
	{
		int tem = y->key;
		y->key = z->key;
		z->key = tem;

		y = z;
		z = y->parent;
	}
}

//删除一个关键字
void Delete(pBinNode pH, pBinNode x)
{
	Decrease(pH,x,-MAX);
	ExtractMin(pH);

}