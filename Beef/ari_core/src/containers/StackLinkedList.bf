namespace ari.core
{
	struct StackLinkedList<T>
	{
	    public struct Node
		{
	        public T data;
	        public Node* next;
	    };
	    
	    Node* head;

	    public void Push(Node * newNode) mut
		{
			newNode.next = head;
			head = newNode;
		}

	    public Node* Pop() mut
		{
			Node * top = head;
			head = head.next;
			return top;
		}
	}
}
