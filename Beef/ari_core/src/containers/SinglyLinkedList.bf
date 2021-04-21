namespace ari.core
{
	public struct SinglyLinkedList<T>
	{
		public struct Node
		{
			public T data;
			public Node* next;
		}

		public Node* head = null;

		public this()
		{

		}

		public void Insert(Node* previousNode, Node* newNode) mut
		{
    		if (previousNode == null)
			{
		        // Is the first node
		        if (head != null)
				{
		            // The list has more elements
		            newNode.next = head;           
		        }
				else
				{
		            newNode.next = null;
		        }
		        head = newNode;
		    }
			else
			{
		        if (previousNode.next == null)
				{
		            // Is the last node
		            previousNode.next = newNode;
		            newNode.next = null;
		        }
				else
				{
		            // Is a middle node
		            newNode.next = previousNode.next;
		            previousNode.next = newNode;
		        }
		    }
		}
		
		public void Remove(Node* previousNode, Node* deleteNode) mut
		{
		    if (previousNode == null)
			{
		        // Is the first node
		        if (deleteNode.next == null)
				{
		            // List only has one element
		            head = null;            
		        }
				else
				{
		            // List has more elements
		            head = deleteNode.next;
		        }
		    }
			else
			{
		        previousNode.next = deleteNode.next;
		    }
		}
	}
}
