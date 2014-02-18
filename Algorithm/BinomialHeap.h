/*
 *描述      :  二项堆实现
 *作者      :  何猛
 *创建时间   :  2013/6/5
*/
#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

typedef struct _bin_node
{
	struct _bin_node* parent;
	struct _bin_node* sibling;
	struct _bin_node* child;
	unsigned int degree;
	int key;
} BinNode, *pBinNode;

typedef pBinNode pBinHeap;  //二项堆

pBinHeap CreateBinHeap();

pBinNode SearchMinKey(pBinHeap pHeap);

void Link(pBinHeap pH1, pBinHeap pH2);

pBinHeap Merge(pBinHeap pH1, pBinHeap pH2);

pBinHeap Union(pBinHeap pH1, pBinHeap pH2);

void Inerst(pBinHeap pH, BinNode node);

pBinHeap ExtractMin(pBinHeap heap);

void Decrease(pBinHeap pH, pBinNode x, int k);

void Delete(pBinNode pH, pBinNode x);

#endif // !BINOMIALHEAP_H