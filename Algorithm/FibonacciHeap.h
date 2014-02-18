#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H
//쳲��������ADT
struct FibonacciHeapNode {
	int key;       //���
	int degree;    //��
	FibonacciHeapNode * left;  //���ֵ�
	FibonacciHeapNode * right; //���ֵ�
	FibonacciHeapNode * parent; //�����
	FibonacciHeapNode * child;  //��һ�����ӽ��
	bool marked;           //�Ƿ�ɾ����1������
};

typedef FibonacciHeapNode FibNode;

//쳲�������ADT
struct FibonacciHeap {
	int keyNum;   //���н�����
	FibonacciHeapNode * min;//��С�ѣ������
	int maxNumOfDegree;   //����
	FibonacciHeapNode * * cons;//ָ�����ȵ��ڴ�����
};

typedef FibonacciHeap FibHeap;

/*****************��������*************************/
//��x��˫�����Ƴ�
inline void FibNodeRemove(FibNode * x);

//��x�ѽ�����y���֮ǰ(ѭ��������)
void FibNodeAdd(FibNode * x, FibNode * y);

//��ʼ��һ���յ�Fibonacci Heap
FibHeap * FibHeapMake() ;

//��ʼ�����x
FibNode * FibHeapNodeMake();

//�ѽ��x����fibonacci heap��
void FibHeapInsert(FibHeap * heap, FibNode * x);

//�������ڵ�ֵ����Fibonacci Heap
void FibHeapInsertKeys(FibHeap * heap, int keys[], int keyNum);

//��ֵ����Fibonacci Heap
static void FibHeapInsertKey(FibHeap * heap, int key);

//��ȡ��С���
FibNode * FibHeapExtractMin(FibHeap * heap);

//�ϲ�������ͬ�����Ķ�����
void FibHeapConsolidate(FibHeap * heap);

//��x��������ӵ�y�����
void FibHeapLink(FibHeap * heap, FibNode * x, FibNode *y);

//����FibHeapConsolidate������ϣ���ÿռ�
static void FibHeapConsMake(FibHeap * heap);

//���ѵ���С����Ƴ�����ָ�������ֵ�
static FibNode *FibHeapMinRemove(FibHeap * heap);

//��Сһ���ؼ���
void FibHeapDecrease(FibHeap * heap, FibNode * x, int key);

//�ж�x�븸�ڵ�y֮������ӣ�ʹx��Ϊһ����
static void FibHeapCut(FibHeap * heap, FibNode * x, FibNode * y);

//��������
static void FibHeapCascadingCut(FibHeap * heap, FibNode * y);

//�޸Ķ���
void renewDegree(FibNode * parent, int degree);

//ɾ�����
void FibHeapDelete(FibHeap * heap, FibNode * x);

//���������ؼ���
FibNode * FibHeapSearch(FibHeap * heap, int key);

//��FibHeapSearch����
static FibNode * FibNodeSearch(FibNode * x, int key);

//���ٶ�
void FibHeapDestory(FibHeap * heap);

//��FibHeapDestory����
static void FibNodeDestory(FibNode * x);

//�����ӡ��
static void FibHeapPrint(FibHeap * heap);

//��FibHeapPrint����
static void FibNodePrint(FibNode * x);
#endif // !FIBONACCIHEAP_H
