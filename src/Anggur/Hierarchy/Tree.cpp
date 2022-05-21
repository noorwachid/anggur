#include <Anggur/Core/Tree.h>

namespace Anggur 
{
	// public:
	
	void Tree::Initialize() {
		TraverseAndInitialize(rootNode);
	}

	void Tree::Terminate() 
	{
		TraverseAndTerminate(rootNode);
	}

	Node* Tree::GetRootNode() 
	{
		return rootNode;
	}

	void Tree::GetRootNode(Node* node) 
	{
		Terminate();

		rootNode = node;

		Initialize();
	}

	void Tree::Update() 
	{
		TraverseAndUpdate(rootNode);
	}

	void Tree::Emit(Event& event) 
	{
		TraverseAndEmit(rootNode, event);
	}

	void Tree::TraverseAndInitialize(Node* node) 
	{
		if (node) 
		{
			// BFS traversal
			node->initialize();

			for (Node* child: node->children) 
			{
				TraverseAndInitialize(child);
			}
		}
	}

	void Tree::TraverseAndTerminate(Node* node) 
	{
		if (node) {
			// DFS traversal
			for (Node* child: node->children) 
			{
				TraverseAndTerminate(child);
			}

			node->terminate();
			delete node;
			node = nullptr;
		}
	}

	void Tree::TraverseAndUpdate(Node* node) 
	{
		if (node) 
		{
			// BFS traversal
			node->update();
			node->render();

			for (Node* child: node->children) 
			{
				TraverseAndUpdate(child);
			}
		}
	}

	void Tree::TraverseAndEmit(Node* node, Event& event) 
	{
		if (node) 
		{
			// DFS traversal
			for (Node* child: node->children) 
			{
				TraverseAndUpdate(child);
			}

			node->onEvent(event);
		}
	}
}