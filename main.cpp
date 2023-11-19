#include <iostream>
#include <climits>
#include <cstdint>

struct Node {
  Node* parent = nullptr;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
  size_t height = 1;
  int64_t value = -1;
  Node(Node* left_child, Node* right_child, int64_t value) {
    this->left_child = left_child;
    this->right_child = right_child;
    this->value = value;
  }
};

class TreeAVL {
  Node* root_tree_ = nullptr;
  size_t size_ = 0;
  Node* mid_node = nullptr;
  Node* Insert(Node* node, int64_t value) {
    if (node == nullptr) {
      return new Node(nullptr, nullptr, value);
    }
    if (value < node->value) {
      node->left_child = Insert(node->left_child, value);
    }
    if (value >= node->value) {
      node->right_child = Insert(node->right_child, value);
    }
    return Balance(node);
  }

  int64_t BalanceFactor(Node* node) {
    int left_height = node->left_child != nullptr ? static_cast<int>(node->left_child->height) : 0;
    int right_height = node->right_child != nullptr ? static_cast<int>(node->right_child->height) : 0;
    return right_height - left_height;
  }

  void FixHeight(Node* node) {
    int left_height = node->left_child != nullptr ? static_cast<int>(node->left_child->height) : 0;
    int right_height = node->right_child != nullptr ? static_cast<int>(node->right_child->height) : 0;
    node->height = std::max(left_height, right_height) + 1;
  }

  Node* RotateRight(Node* right_node) {
    Node* left_node = right_node->left_child;
    right_node->left_child = left_node->right_child;
    left_node->right_child = right_node;
    FixHeight(right_node);
    FixHeight(left_node);
    return left_node;
  }

  Node* RotateLeft(Node* left_node) {
    Node* right_node = left_node->right_child;
    left_node->right_child = right_node->left_child;
    right_node->left_child = left_node;
    FixHeight(left_node);
    FixHeight(right_node);
    return right_node;
  }

  Node* Balance(Node* node) {
    FixHeight(node);
    if (BalanceFactor(node) == 2) {
      if (BalanceFactor(node->right_child) < 0) {
        node->right_child = RotateRight(node->right_child);
      }
      return RotateLeft(node);
    }
    if (BalanceFactor(node) == -2) {
      if (BalanceFactor(node->left_child) > 0) {
        node->left_child = RotateLeft(node->left_child);
      }
      return RotateRight(node);
    }
    return node;
  }

  int64_t Search(Node* node, int64_t val) {
    if (node == nullptr) {
      return INT_MAX;
    }
    if (val < node->value) {
      return std::min(Search(node->left_child, val), node->value);
    }
    if (val > node->value) {
      return Search(node->right_child, val);
    }
    return node->value;
  }

  Node* DeleteMax(Node* node) {
    if (node->right_child == nullptr) {
      std::cout << node->value << '\n';
      Node* left_node = node->left_child;
      delete node;
      return left_node;
    }
    node->right_child = DeleteMax(node->right_child);
    return Balance(node);
  }

  Node* DeleteMin(Node* node) {
    if (node->left_child == nullptr) {
      std::cout << node->value << '\n';
      Node* right_node = node->right_child;
      delete node;
      return right_node;
    }
    node->left_child = DeleteMin(node->left_child);
    return Balance(node);
  }

  Node* DeleteMaxWithoutWriting(Node* node) {
    if (node->right_child == nullptr) {
      Node* left_node = node->left_child;
      delete node;
      return left_node;
    }
    node->right_child = DeleteMaxWithoutWriting(node->right_child);
    return Balance(node);
  }

  int64_t GetMin(Node* node) {
    Node* cur_node = node;
    while (cur_node->left_child != nullptr) {
      cur_node = cur_node->left_child;
    }
    return cur_node->value;
  }

  int64_t GetMax(Node* node) {
    Node* cur_node = node;
    while (cur_node->right_child != nullptr) {
      cur_node = cur_node->right_child;
    }
    return cur_node->value;
  }

  void FullClear(Node* node) {
    if (node == nullptr) {
      return;
    }
    FullClear(node->left_child);
    FullClear(node->right_child);
    delete node;
    node = nullptr;
  }

 public:
  void Insert(int64_t value) {
    std::cout << "ok\n";
    ++size_;
    if (size_ == 1) {
      if (root_tree_ == nullptr) {
        root_tree_ = new Node(nullptr, nullptr, value);
      } else {
        root_tree_->value = value;
        root_tree_->left_child = nullptr;
        root_tree_->right_child = nullptr;
        root_tree_->height = 1;
      }
      return;
    }
    root_tree_ = Insert(this->root_tree_, value);
  }
  void GetMin() {
    if (size_ == 0) {
      std::cout << "error\n";
      return;
    }
    std::cout << GetMin(root_tree_) << '\n';
  }
  void ExtractMax() {
    if (size_ == 0) {
      std::cout << "error\n";
      return;
    }
    if (root_tree_->right_child == nullptr) {
      if (size_ == 1) {
        std::cout << root_tree_->value << '\n';
        --size_;
        return;
      }
      --size_;
      Node* new_root = root_tree_->left_child;
      std::cout << root_tree_->value << '\n';
      delete root_tree_;
      root_tree_ = new_root;
      return;
    }
    root_tree_ = DeleteMax(root_tree_);
    --size_;
  }
  void ExtractMin() {
    if (size_ == 0) {
      std::cout << "error\n";
      return;
    }
    if (root_tree_->left_child == nullptr) {
      if (size_ == 1) {
        std::cout << root_tree_->value << '\n';
        --size_;
        return;
      }
      --size_;
      Node* new_root = root_tree_->right_child;
      std::cout << root_tree_->value << '\n';
      delete root_tree_;
      root_tree_ = new_root;
      return;
    }
    root_tree_ = DeleteMin(root_tree_);
    --size_;
  }
  void GetMax() {
    if (size_ == 0) {
      std::cout << "error\n";
      return;
    }
    std::cout << GetMax(root_tree_) << '\n';
  }
  void GetSize() {
    std::cout << size_ << '\n';
  }
  void FullClear() {
    FullClear(root_tree_);
  }

  void Clear() {
    if (size_ == 0) {
      std::cout << "ok\n";
      return;
    }
    while (size_ != 1) {
      --size_;
      root_tree_ = DeleteMaxWithoutWriting(root_tree_);
    }
    std::cout << "ok\n";
    --size_;
  }
};

bool Equal(const char* str1, const char* str2) {
  int i = 0;
  while ((str1[i] != 0) || (str2[i] != 0)) {
    if (str1[i] != str2[i]) {
      return false;
    }
    ++i;
  }
  return true;
}

int main() {
  TreeAVL tree;
  int n = 2;
  std::cin >> n;
  char command[256];
  for (int i = 0; i < n; ++i) {
    std::cin >> command;
    int64_t operand = 0;
    if (Equal(command, "insert")) {
      std::cin >> operand;
      tree.Insert(operand);
    } else if (Equal(command, "extract_min")) {
      tree.ExtractMin();
    } else if (Equal(command, "get_min")) {
      tree.GetMin();
    } else if (Equal(command, "extract_max")) {
      tree.ExtractMax();
    } else if (Equal(command, "get_max")) {
      tree.GetMax();
    } else if (Equal(command, "size")) {
      tree.GetSize();
    } else if (Equal(command, "clear")) {
      tree.Clear();
    }
  }
  tree.FullClear();
  return 0;
}

