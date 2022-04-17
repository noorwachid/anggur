#include <Anggur/Core/Hierarchy/Tree.h>

namespace Anggur {
	// public:
	void Tree::initialize() {
		traverseAndInitialize(root);
	}

	void Tree::terminate() {
		traverseAndTerminate(root);
	}

	Node* Tree::getRoot() {
		return root;
	}

	void Tree::setRoot(Node* root) {
		terminate();

		Tree::root = root;

		initialize();
	}

	void Tree::update() {
		traverseAndUpdate(root);
	}

	void Tree::emit(Event& event) {
		traverseAndEmit(root, event);
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

	// private:
	Node* Tree::root = nullptr;
}