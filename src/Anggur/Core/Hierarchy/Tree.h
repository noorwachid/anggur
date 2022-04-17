#pragma once

#include <Anggur/Core/Hierarchy/Node.h>

namespace Anggur {
	class Tree {
	public:
		static void initialize();
		static void terminate();

		static Node* getRoot();
		static void setRoot(Node* scene);

		static void update();
		static void emit(Event& event);

	private:
		static void traverseAndInitialize(Node* entity);
		static void traverseAndTerminate(Node* entity);

		static void traverseAndUpdate(Node* entity);
		static void traverseAndEmit(Node* entity, Event& event);

	private:
		static Node* root;
	};
}