#include<iostream>
#include<conio.h>
using namespace std;

struct node{
	int data;
	node *next;
};
class ADTstack
{
	private:
		node *top;
	public:
		ADTstack()
		{
			top = NULL;
		}
		
		int empty()
		{
			if(top == NULL)
				return 1;
			else
				return 0;
		} 
		void push(int num)
		{
			node *temp;
			temp = new node;
			temp->data = num;
			if (top == NULL)
			{
				top = temp;
				temp->next = NULL;
			}
			else
			{
				temp->next = top;
				top = temp;
			}
		}
		int pop()
		{
			int num;
			node *temp;
			if(!empty())
			{
				num = top->data;
				temp = top;
				top = top->next;
				delete temp;
				return num;
			}
			else
			{
				cout<<"Stack is Empty"<<endl;
				return 0;
			}
		}
};
void main()
{
	ADTstack st;
	st.push(23);
	st.push(46);
	st.push(37);
	cout<<st.pop()<<endl;
	cout<<st.pop()<<endl;
	cout<<st.pop()<<endl;
	cout<<st.pop()<<endl;
}