#include <Anggur/Core/Tree.h>

namespace Anggur {
	// public:
	void Tree::initialize() {
		traverseAndInitialize(rootNode);
	}

	void Tree::terminate() {
		traverseAndTerminate(rootNode);
	}

	Node* Tree::getRootNode() {
		return rootNode;
	}

	void Tree::setRootNode(Node* node) {
		terminate();

		rootNode = node;

		initialize();
	}

	void Tree::update() {
		traverseAndUpdate(rootNode);
	}

	void Tree::emit(Event& event) {
		traverseAndEmit(rootNode, event);
	}

	void Tree::traverseAndInitialize(Node* node) {
		if (node) {
			// BFS traversal
			node->initialize();

			for (Node* child: node->children) {
				traverseAndInitialize(child);
			}
		}
	}

	void Tree::traverseAndTerminate(Node* node) {
		if (node) {
			// DFS traversal
			for (Node* child: node->children) {
				traverseAndTerminate(child);
			}

			node->terminate();
			delete node;
			node = nullptr;
		}
	}

	void Tree::traverseAndUpdate(Node* node) {
		if (node) {
			// BFS traversal
			node->update();
			node->render();

			for (Node* child: node->children) {
				traverseAndUpdate(child);
			}
		}
	}

	void Tree::traverseAndEmit(Node* node, Event& event) {
		if (node) {
			// DFS traversal
			for (Node* child: node->children) {
				traverseAndUpdate(child);
			}

			node->onEvent(event);
		}
	}
}