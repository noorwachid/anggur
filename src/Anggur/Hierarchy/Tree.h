#pragma once

#include <Anggur/Core/Node.h>

namespace Anggur {
	class Tree {
	public:
		void initialize();
		void terminate();

		Node* getRootNode();
		void setRootNode(Node* node);

		void update();
		void emit(Event& event);

	private:
		void traverseAndInitialize(Node* node);
		void traverseAndTerminate(Node* node);

		void traverseAndUpdate(Node* node);
		void traverseAndEmit(Node* node, Event& event);

	private:
		Node* rootNode;
	};
}