class Node {
public:
	Vector2 position;
	Vector2 size;
	Vector4 color;
	std::vector<Node*> children;

	virtual initialize() {
	}

	virtual update() {
	}

	virtual render() {
	}
};

class VSplit: public Node {
	void render() override {
		Vector2 overN = size / children.size();

		for (Node* child: children) {
			child.size = overN;
		}
	}
};

class RootNode: public VSplit {
public:
	void initialize() override {
		one = new Node();
		one.color = Vector4::red;

		two = new Node();
		two.color = Vector4::blue;

		children = {
			one,
			two,
		};
	}

public:
	Node* one;
	Node* two;
};
