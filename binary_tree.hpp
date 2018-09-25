#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

class Node {
public:
  /// create node with key
  Node(int key);
  /// pointers to parent and children
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
  /// data
  int key;
};

Node::Node(int key) : key(key) {}

class BinaryTree {
public:
  /// insert key into tree
  void insert(Node* node);
  /// remove node with key
  void remove(Node* node);
  /// find successor of node
  Node* successor(Node* node) const;
  /// find predecessor of node
  Node* predecessor(Node* node) const;
  /// replace subtree at a with subtree at b
  void transplant(Node* a, Node* b);
  /// pointer to the root
  Node* root = nullptr;
};

void BinaryTree::insert(Node* node) {
  Node* parent = nullptr;
  Node* child = root;

  while (child) {
    parent = child;
    if (node->key < child->key)
      child = child->left;
    else
      child = child->right;
  }

  node->parent = parent;
  if (!parent) {
    root = node;
  } else if (node->key < parent->key) {
    parent->left = node;
  } else {
    parent->right = node;
  }
}

void BinaryTree::remove(Node* node) {
  if (!node->left && !node->right) {
    // remove leaf node
    if (node->parent) {
      if (node->parent->key < node->key) {
        node->parent->right = nullptr;
      } else {
        node->parent->left = nullptr;
      }
    }
    node->parent = nullptr;
  } else if (!node->left && node->right) {
    // no left child
    transplant(node, node->right);
  } else if (node->left && !node->right) {
    // no right child
    transplant(node, node->left);
  } else {
    // check if min = successor(node)
    Node* min = successor(node);
    if (min->parent != node) {
      transplant(min, min->right);
      min->right = node->right;
      min->right->parent = min;
    }
    transplant(node, min);
    min->left = node->left;
    min->left->parent = min;
  }
}

Node* BinaryTree::successor(Node* node) const {
  if (node->right) {
    // find minimum of right subtree
    Node* parent = nullptr;
    Node* child = node->right;
    while (child) {
      parent = child;
      child = child->left;
    }
    return parent;
  } else {
    // find parent of furthest node through right branches
    Node* iter = node;
    while (iter->parent) {
      if (iter->parent->key < iter->key) {
        iter = iter->parent;
      } else {
        break;
      }
    }

    // will return nullptr if no successor
    return iter->parent;
  }
}

Node* BinaryTree::predecessor(Node* node) const {
  if (node->left) {
    // find maximum of left subtree
    Node* parent = nullptr;
    Node* child = node->left;

    while (child) {
      parent = child;
      child = child->right;
    }
    return parent;
  } else {
    // find parent of furthest node through left branches
    Node* iter = node;
    while (iter->parent) {
      if (iter->key < iter->parent->key) {
        iter = iter->parent;
      } else {
        break;
      }
    }

    // will return nullptr if no predecessor exists
    return iter->parent;
  }
}

void BinaryTree::transplant(Node* a, Node* b) {
  if (b->parent->key < b->key) {
    b->parent->right = nullptr;
  } else {
    b->parent->left = nullptr;
  }

  b->parent = a->parent;

  // special case when a is root
  if (a->parent) {
    if (a->parent->key < a->key) {
      a->parent->right = b;
    } else {
      a->parent->left = b;
    }
  } else {
    root = b;
  }
}

#endif
