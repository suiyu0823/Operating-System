#include "main.h"
#include "stacklist.h"

CStackList::CStackList(void)
{
		initList(&list);
		front=list;
		middle=list;
		back=list;
}

CStackList::~CStackList(void)
{
		destory();
}

int CStackList::initList(LinkList* L)
{
		(*L)=NULL;
		return 0;
}

int CStackList::insert(int nValue)
{
		LinkList p=(LinkList)malloc(sizeof(Node));
		p->nValue=nValue;
		p->next=NULL;

		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		LinkList q;
		if(back!=NULL)
		{
				q=back;
				q->next=p;
				back=p;

				middle=getMidpos();
		}
		else
		{
				back=p;
				front=p;
				middle=p;
				list=p;
		}
		return 0;
}

int CStackList::remove(int pos)
{
		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		int n=0;
		LinkList p=list;	
		while(p && pos-->0)
		{
				n++;
				p=p->next;
		}		

		
		remove(p);
		return 0;
}

int CStackList::remove(PositionNode pos)
{
		LinkList p,q;
		p=list;
		q=p;

		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		while(p)
		{
				if(p==pos)
				{
						q->next=p->next;
						free(p);
						p=NULL;
						return 0;
				}

				q=p;
				p=p->next;
		}
		return -1;
}

int CStackList::countList()
{
		int n=0;
		LinkList p=list;

		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		while(p)
		{
				n++;
				p=p->next;
		}
		return n;
}

void CStackList::destory()
{
		LinkList p,q;
		p=list;

		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		while(p)
		{
				q=p;
				p=p->next;
				free(q);
				q=NULL;
		}
}

int CStackList::getValue(int pos)
{
		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		LinkList p=list;
		while(p && pos-->0)
		{
				p=p->next;
		}
		return p->nValue;
}

int CStackList::setValue(int pos,int nValue)
{
		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		LinkList p=list;
		while(p && pos-->0)
		{
				p=p->next;
		}
		p->nValue=nValue;
		return 0;
}

PositionNode CStackList::getMidpos()
{
		CAutoLock _l(m_lock,MACRO_NODE_LOCK);
		int nCount=countList();
		nCount/=2;
		LinkList p=list;
		while(p && nCount-->0)
		{
				p=p->next;
		}
		return p;
}
