#include <iostream>

using namespace std;
// Nodes are used like blocks, in this case to represent a list via a Stack and Queue
class Node
{
private:
	Node* next;
	Node* prev;
	int priority;
	int value;

public:
	Node(int value, Node* nextptr = NULL, Node* prevptr = NULL, int currentpriority = 0);
	int getVal(void);
	Node* getNext(void);
	Node* getPrev(void);
	void setVal(int value);
	void setPrev(Node* prevptr);
	void setNext(Node* nextptr);
	int getPriority(void);
	void setPriority(int priority);
};

// Stack is a container of objects that are inserted and removed according to Last-In-First-Out (LIFO)
// using 'Push' to push the item into the stack and 'Pop' to remove the item out of the stack
class Stack
{
private:
	Node* top;

public:
	Stack(void) {};
	~Stack(void) {};
	void Push(int value);
	Node* NodePop(void);
	int Pop(void);
};


// Queue is similar to Stack but is open at both ends ('back' and front')
// One end is used to insert data (enqueue)
// Other end is used to remove data (dequeue)
// Queue uses the First-In-First-Out (FIFO) principle
class Queue
{
private:
	virtual Node* NodeDequeue(void);

protected:
	Node* back;
	Node* front;

public:
	Queue(void) {};
	~Queue(void) {};
	void Enqueue(int i, int priority = 0);
	int Dequeue(void);
};

// Scheduler inherits from Queue in order to implement a priority queue
// removes items from the Queue based on its priority
// Once the item selected is removed, the Scheduler will relink the nodes required
// to ensure there are no "holes" left
class Scheduler : public Queue
{
private:
	Node* NodeDequeue(void);
};

// initialises the node object with specified parameters
Node::Node(int value, Node* nextptr, Node* prevptr, int currentpriority)
{
	prev = prevptr; // prevptr 
	next = nextptr;
	this->value = value;
	this->priority = currentpriority;
}

// returns the value stored within 'value' of a given Node object e.g. object->getVal();
int Node::getVal(void)
{
	return value;
}

// returns the values stored within the next object
Node* Node::getNext(void)
{
	return next;
}

// returns the values stored within the previous object
Node* Node::getPrev(void)
{
	return prev;
}

// returns the value stored within 'priority' of a given Node object e.g. object->getPriority();
int Node::getPriority(void)
{
	return priority;
}

// sets the value within the parameter to the specified object e.g. object->setVal(number);
void Node::setVal(int value)
{
	value = value;
}

// sets the given Object to the previous object e.g. object->setPrev(nullptr)
void Node::setPrev(Node* prevptr)
{
	prev = prevptr;
}

// sets the given Object to the next object e.g. object->setNext(nullptr)
void Node::setNext(Node* nextptr)
{
	next = nextptr;
}

// sets the value within the parameter to the specified object e.g.object->setPriority(priority);
void Node::setPriority(int priority)
{
	priority = priority;
}

// Push puts a value onto the top of the stack by initialising a new Node named 'tmp'
// and assigning it the values specified e.g. object.Push(20);
// top is then assigned the values within the object 'tmp' in order to place it on the top
void Stack::Push(int value)
{
	Node* tmp = new Node(value, top);
	top = tmp;
}

// NodePop deletes the current top Node by passing the top node to 'tmp' then top obtains the 'next' top node via getNext()
// before returning the object back to the 'Pop' method
Node* Stack::NodePop(void)
{
	Node* tmp = top;
	if (!(top == nullptr)) top = top->getNext();
	return tmp;
}

// Pop removes the topmost Node in the stack and returns the value of that Node
int Stack::Pop(void)
{
	// if the top is empty, it throws an exception to prevent crashing
	if (top == nullptr)
	{
		throw "Stack empty!";
	}
	int ret = top->getVal();

	Node* tmp = NodePop();

	delete tmp;	// deletes the object as it's no longer needed
	return ret;
}

// Enqueue inserts a new Node at the back of the queue with a value
// and/or a priority if using the Scheduler class e.g. object.Enqueue(20, 5)
void Queue::Enqueue(int i, int priority)
{
	// Creates a new Node named 'tmp' which the 'back' Node will be assigned to
	Node* tmp = new Node(i, back, nullptr, priority);
	back = tmp;

	// if the front-most Node is nullptr, then the front is assigned the value of the back-most Node
	// else 'tmp' is assigned the value next to back-most Node setting the previous Node
	// as the back-most Node
	if (front == nullptr) front = back;
	else
	{
		tmp = back->getNext();
		tmp->setPrev(back);
	}
}


// NodeDequeue obtains the Node at the front of the Queue and returns it to the 'Dequeue' method
// in order for it to be removed from the queue
Node* Queue::NodeDequeue(void)
{
	// Creates a new Node 'tmp' which is assigned the data within the 'front' Node
	// if front is not nullptr, then front is assigned the data from the previous Node
	// and then the next Node after 'front' is set to nullptr
	Node* tmp = front;
	if (front != nullptr)
	{
		front = front->getPrev();
		if (front != nullptr) front->setNext(nullptr);
	}
	return tmp;	// returns tmp back to the 'Dequeue' method
}


// Dequeue removes the selected Node from the queue from the 'NodeDequeue' method
// and also stores the value contained within the selected Node in order to be displayed at runtime
int Queue::Dequeue(void)
{
	Node* tmp = NodeDequeue();	// Creates a new Node 'tmp' which stores the data within the method 'NodeDequeue'
	int ret = 0;

	// if 'tmp' Node isn't nullptr, then ret is assigned the value stored with the Node
	// else the program will exit to prevent a crash when the queue is empty
	if (tmp != nullptr)
	{
		ret = tmp->getVal();
	}
	else
	{
		cout << "\nQueue is empty!!" << endl;
		cout << "Terminating program..." << endl;

		exit(-1);
	}
	if (front == nullptr) back = front;	//if front is nullptr, then the front is assigned the value of the back-most Node
	delete tmp;
	return ret;
}

// NodeDequeue from the Scheduler class inherits two methods from Queue 'Enqueue' and 'Dequeue' in order to
// manipulate the Node selected to be removed by Dequeue based on priority where the higher priority (10)
// will be removed before the lower priority (0)
// If there are multiple Nodes with the same priority, then it is chosen on a First-Come-First-Served (FCFS) basis
Node* Scheduler::NodeDequeue(void)
{
	// Checks if the queue is empty and exits program
	if (front == nullptr)
	{
		cout << "\nQueue is empty!!" << endl;
		cout << "Terminating program..." << endl;

		exit(-1);
	}
	// findpriority is created in order to find the Node to select for removal
	Node* findpriority = front; // findpriority is assigned the values within the front node
	Node* tmp = findpriority; // tmp is assigned the values within findpriority

	// randomly selects a search method based on probability
	// by using a seed obtained by casting a pointer address as an int
	int random = reinterpret_cast<int>(&front);
	srand(random);
	int randompriority = rand() % 10;	// obtains a random priority between 0-10
	int probability = rand() % 100;		// obtains a random percentage between 0-100

	// if probability is higher than 40, the normal priority queue will take place
	if (probability >= 40)
	{
		// while loop that runs until it either finds a Node with the priority '10'
		// or until it reaches the end of the queue by searching each node for a higher
		// priority Node
		while (findpriority != nullptr)
		{
			// if the priority is 10, there is no need to search the entire list
			// therefore assigning the Node from 'findpriority' to 'tmp' in order to be removed from the list
			if (findpriority->getPriority() == 10)
			{
				tmp = findpriority;
				break;	// breaks from the while loop as the highest priority has been found
			}
			// checks if the previous Node's priority in the queue is greater than the one currently selected to be removed
			else if (findpriority->getPriority() > tmp->getPriority())
			{
				tmp = findpriority;	// tmp is assigned the data stored within findpriority
			}

			findpriority = findpriority->getPrev();	// findpriority obtains the previous Node in the list
		}
	}
	// else if the probability is less than 40, a random priority is chosen to be removed
	// in order to prevent blocking
	else if (probability < 40)
	{
		do
		{
			randompriority = rand() % 10;	// randomly chooses a priority value

			if (findpriority->getPriority() == randompriority)
			{
				tmp = findpriority;
				break;
			}
			
			findpriority = findpriority->getPrev();
		} while (findpriority != nullptr);

	}

	//if the front node is selected to be removed, then front will be set to the previous node
  	if (tmp == front)
	{
		front = front->getPrev();
		if (front != nullptr) front->setNext(nullptr);
	}
	//if the back node is selected to be removed, then back will be set to the next node
	else if (tmp == back)
	{
		back = back->getNext();
		if (back != nullptr) back->setPrev(nullptr);
	}
	//if the node selected is from the middle (has a node at prev and next), then previous
	//node will be set to next and next node will be set to previous
	else
	{
		tmp->getPrev()->setNext(tmp->getNext());
		tmp->getNext()->setPrev(tmp->getPrev());
	}
	return tmp;
}