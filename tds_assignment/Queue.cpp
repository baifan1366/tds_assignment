#include<iostream>
using namespace std;

class ADTqueue
{
	private:
		int queue[10];
		int head,tail;
	public:
		ADTqueue()
		{
			tail = -1;
			head = 0;
		}
		int empty()
		{
			if(head == tail+1)
				return 1; 
			else
				return 0;
		}
		int full()
		{
			if(tail == 9)
				return 1;
			else
				return 0;
		}
		void append(int num)
		{
			if (!full())
			{
				tail++;
				queue[tail] = num;
			}
			else
			{
				cout<<"Queue is Full"<<endl;
			}
		}
		int serve()
		{
			int num;
			if(!empty())
			{
				num = queue[head];
				head++;
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