#include<iostream>
#include<coonio.h>
using namespace std;

struct node{
	ind data;
	node *temp;
};
class ADTqueue
{
	private:
		node *front, *rear;
	public:
		ADTqueue()
		{
			front = NULL;
			rear = NULL;
		}
		int empty()
		{
			if(front == NULL)
				return 1; 
			else
				return 0;
		}
		void append(int num)
		{
			if (rear != NULL)
			{
				rear->next = new node;
				rear = rear->next;
				rear->data = num;
				rear->next = NULL;
			}
			else
			{
				front = rear = new node;
				front->data = num;
				front->next = NULL;
			}
		}
		int serve()
		{
			int num;
			if(!empty())
			{
				num = front->data;
				node *temp = front;
				front = front->next;
				delete temp;
				if(front == NULL) {
					rear = NULL;
				}
				return num;
			} 
			else
			{
				cout<<"Queue is Empty"<<endl;
				return 0;
			} 
		}
}; 
void main()
{
	ADTqueue q;
	q.append(23);
	q.append (46);
	q.append (37);
	cout<<q.serve()<<endl;
	cout<<q.serve()<<endl;
	cout<<q.serve()<<endl;
	cout<<q.serve()<<endl;
} 