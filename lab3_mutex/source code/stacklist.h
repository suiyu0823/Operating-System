#ifndef _STACKLIST_H_
#define _STACKLIST_H_

struct Node
{
	int nValue;
	Node* next;	
};

typedef Node* LinkList;
typedef Node* PositionNode;

class CStackList
{
public:
	CStackList(void);
	~CStackList(void);

public:
	int initList(LinkList* L);
	int insert(int nValue);
	int remove(PositionNode pos);
	int remove(int pos);
	int countList();
	void destory();

	int getValue(int pos);
	int setValue(int pos,int nValue);

	PositionNode getMidpos();

private:
	mylock m_lock;

	LinkList list;
	LinkList front;
	LinkList middle;
	LinkList back;
};

#endif
