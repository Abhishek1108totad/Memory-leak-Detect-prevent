/*
Mastering Memory Management: Detecting and Preventing Leaks in C

In the world of embedded systems and low-level programming, managing dynamic memory efficiently is not just a skill—it's a necessity. One of the most elusive and detrimental issues we encounter are Memory leaks. These sneaky bugs can slowly degrade performance, leading to unexpected crashes and system failures, problems we definitely want to avoid in critical code.

The Challenge: How can we reliably track memory allocations in C to ensure no allocated memory is left dangling when our program completes?

The solution lies in creating a simple yet effective memory leak detector:

How It Works:
- Each time memory is allocated (using 'malloc'), we log the allocation essentially maintaining a record for every allocated memory block.
- When memory is freed (using 'free'), we remove the block from our record.
- At the program's end, we review the record. If any memory blocks remain unfreed, we’ve identified our leak.

This approach doesn’t just help catch memory leaks—it fosters the practice of responsible memory management. In environments like embedded systems, where every byte is precious, avoiding memory leaks is critical to ensuring systems run efficiently and reliably over time.

Key Takeaways:
- Track Allocations: Keep track of every allocated memory block and ensure its properly freed.
- Leak Detection: At program completion, report any unfreed memory to identify potential leaks.
- Clean Memory Practices: Prevent dangling pointers by cleaning up all allocated memory, ensuring nothing is left behind.

Whether you're developing for resource-constrained environments or high-performance applications, effective memory management is non-negotiable. The better you control your allocations, the fewer issues you'll face in production.

Pro Tip: Implementing this pattern in your workflow can save you countless hours of debugging, ensuring your code remains lean, stable, and efficient.


*/

#include<iostream>
#include<stdlib.h>

using namespace std;

//Node structure for linked list
typedef struct Node{
    void *ptr;        //Pointer to the allocated memory
    size_t size;      //Size of allocated memory
    struct Node *next;//pointer to the next node
}Node;

//Head of the linked list
Node *head = NULL;

//custom malloc function
void *my_malloc(size_t size)
{
    if(size == 0)
    {
        return NULL;//handle zero-sized allocation
    }
    
    //Allocate memory
    void *ptr = malloc(size);
    if(ptr == NULL)
    {
        cout<<"Memory allocation Failed!\n";
        return NULL;
    }
    
    //Creat a new node and add it to the linked list
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node == NULL)
    {
        free(ptr);
        cout<<"Memory allocation for tracking failed!\n";
        return NULL;
    }
    
    new_node->ptr = ptr;
    new_node->size = size;
    new_node->next = head;
    head = new_node;
    
    return ptr; //Return the allocated memory pointer
}

//custom free function
void my_free(void *ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    
    //Traverse the linked list to find the node to free 
    Node *current = head;
    Node *previous = NULL;
    
    while(current != NULL)
    {
        if(current->ptr == ptr)
        {
            if(previous == NULL)
            {
                head = current->next;//Remove the head
            }
            else
            {
                previous->next = current->next;//Remove the node from the list
            }
            free(current->ptr);//free the allocated memory
            free(current);//free the node from the list
            return;
        }
        previous = current;
        current = current->next;
    }
    cout<<"Attempted to free untrack pointer:"<<ptr;
}

//Function to report memory leaks
void Report_memory_leaks()
{
    Node *current = head;
    if(current == NULL)
    {
        cout<<"No memory leaks detected.\n";
        return;
    }
    
    cout<<"Memory leaks detected:\n";
    while(current != NULL)
    {
        cout<<"Leaked memory at: "<<current->ptr<<", Size: "<<current->size<<"bytes\n";
        current = current->next;
    }
}

int main()
{
    char *data1 = (char *) my_malloc(10);
    char *data2 = (char *) my_malloc(20);
    my_free(data1);//freeing data1
    
    //Intentionally not freeing data2 to create a leak
    //Report any memory leaks
    Report_memory_leaks();
    //Clean up remaining allocations
    my_free(data2);
    return 0;
}