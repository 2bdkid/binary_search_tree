#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <iostream>

class Node {
public:
  /// create node with key
  Node() = default;
  Node(int key);
  /// pointers to parent and children
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
  /// data
  int key = 0;
};

Node::Node(int key) : key(key) {}

class BinarySearchTree {
public:
  BinarySearchTree() = default;
  /// create tree from existing nodes
  BinarySearchTree(Node nodes[], unsigned n);
  /// insert key into tree
  void insert(Node* node);
  /// remove node with key
  void remove(Node* node);
  /// find minimum of entire tree
  Node* minimum() const;
  /// find smallest node in subtree
  Node* minimum(Node* node) const;
  /// find maximum key of entire tree
  Node* maximum() const;
  /// find maximum key in subtree
  Node* maximum(Node* node) const;
  /// find successor of node
  Node* successor(Node* node) const;
  /// find predecessor of node
  Node* predecessor(Node* node) const;
  /// replace subtree at a with subtree at b
  void transplant(Node* a, Node* b);
  /// search for key in entire tree
  Node* find(int key) const;
  /// search for key in subtree
  Node* find(int key, Node* subtree) const;  
  /// find kth smallest element
  Node* kth(unsigned k) const;
  /// write data to stream
  friend std::ostream& operator<<(std::ostream& out, const BinarySearchTree& tree);

private:
  /// build balanced tree from sorted data (or nodes)
  void balanced_bst_insert(BinarySearchTree& tree, Node nodes[], unsigned n);
  /// pointer to the root
  Node* root = nullptr;
};

void BinarySearchTree::balanced_bst_insert(BinarySearchTree& tree, Node nodes[], unsigned n) {
  if (n == 0) return;
  tree.insert(&nodes[n/2]);
  balanced_bst_insert(tree, nodes, n/2);
  balanced_bst_insert(tree, nodes + n/2 + 1, n - 1 - n/2);
}

BinarySearchTree::BinarySearchTree(Node nodes[], unsigned n) {
  balanced_bst_insert(*this, nodes, n);
}

void BinarySearchTree::insert(Node* node) {
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

void BinarySearchTree::remove(Node* node) {
  if (!node->left && !node->right) {
    // special case where only node is root
    if (node->parent) {
    // remove leaf node      
      if (node->parent->key < node->key) {
        node->parent->right = nullptr;
      } else {
        node->parent->left = nullptr;
      }
    } else {
      root = nullptr;
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

Node* BinarySearchTree::minimum() const {
  return minimum(root);
}

Node* BinarySearchTree::minimum(Node* node) const {
  Node* parent = nullptr;
  Node* child = node;

  while (child) {
    parent = child;
    child = child->left;
  }

  return parent;
}

Node* BinarySearchTree::maximum() const {
  return maximum(root);
}

Node* BinarySearchTree::maximum(Node* node) const {
  Node* parent = nullptr;
  Node* child = node;

  while (child) {
    parent = child;
    child = child->right;
  }

  return parent;
}

Node* BinarySearchTree::successor(Node* node) const {
  if (node->right) {
    // find minimum of right subtree
    return minimum(node->right);
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

Node* BinarySearchTree::predecessor(Node* node) const {
  if (node->left) {
    return maximum(node->left);
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

void BinarySearchTree::transplant(Node* a, Node* b) {
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

Node* BinarySearchTree::find(int key) const {
  return find(key, root);
}

Node* BinarySearchTree::find(int key, Node* subtree) const {
  if (!subtree) return nullptr;
  if (subtree->key < key)
    return find(key, subtree->right);
  else if (subtree->key > key)
    return find(key, subtree->left);
  else
    return subtree;
}

Node* BinarySearchTree::kth(unsigned k) const {
  Node* kth = minimum(root);
  for (unsigned i = 0; i < k-1; ++i) {
    if (kth)
      kth = successor(kth);
    else
      return kth;
  }

  // returns nullptr if kth element does not exist
  return kth;
}

/// in-order traversal/print
std::ostream& operator<<(std::ostream& out, const Node& node) {
  if (node.left)
    out << *node.left;
  out << node.key << ' ';
  if (node.right)
    out << *node.right;
  return out;
}

/// print the entire tree
std::ostream& operator<<(std::ostream& out, const BinarySearchTree& tree) {
  if (!tree.root) return out;
  return out << *tree.root;
}

#endif
