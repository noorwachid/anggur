#pragma once

#include <Anggur/Core/Node.h>

namespace Anggur 
{
	class Tree 
	{
	public:
		void Initialize();
		void Terminate();

		Node* GetRootNode();
		void SetRootNode(Node* node);

		void Update();
		void Emit(Event& event);

	private:
		void TraverseAndInitialize(Node* node);
		void TraverseAndTerminate(Node* node);

		void TraverseAndUpdate(Node* node);
		void TraverseAndEmit(Node* node, Event& event);

	private:
		Node* rootNode;
	};
}