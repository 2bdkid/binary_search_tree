#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <iostream>

struct bst {
  explicit bst(int key) : key(key) {}
  
  int key;
  bst* parent = nullptr;
  bst* left = nullptr;
  bst* right = nullptr;

  void insert(bst* node);
  void remove(bst* node);
  void transplant(bst* node);
  bst* successor();
  bst* predecessor();
  bst* minimum();
  bst* maximum();
  bst* find(int key);
  bst* kth(unsigned k);
};

void bst::insert(bst* node) {
  bst* parent = nullptr;
  bst* child = this;

  while (child) {
    parent = child;
    if (child->key < node->key)
      child = child->right;
    else
      child = child->left;
  }

  node->parent = parent;
  if (parent->key < node->key)
    parent->right = node;
  else 
    parent->left = node;
}

void bst::remove(bst* node) {
  // no children then disconnect from parent
  if (!node->left && !node->right) {
    if (node->parent) {
      if (node->parent->key < node->key) {
	node->parent->right = nullptr;
      } else {
	node->parent->left = nullptr;
      }
    }
    // only right child
  } else if (!node->left && node->right) {
    node->transplant(node->right);
    // only left child
  } else if (node->left && !node->right) {
    node->transplant(node->left);
    // both children
  } else {
    bst* min = node->successor();
    if (min->parent != node) {
      min->transplant(min->right);
      min->right = node->right;
      min->right->parent = min;
    }
    node->transplant(min);
    min->left = node->left;
    min->left->parent = min;
  }
}

void bst::transplant(bst* node) {
  if (node->parent->key < node->key) {
    node->parent->right = nullptr;
  } else {
    node->parent->left = nullptr;
  }

  node->parent = parent;

  if (node->parent->key < node->key) {
    node->parent->right = node;
  } else {
    node->parent->left = node;
  }
}

bst* bst::successor() {
  if (right) {
    return right->minimum();
  } else {
    bst* iter = this;
    while (iter->parent) {
      if (iter->parent->key < iter->key) {
	iter = iter->parent;
      } else {
	break;
      }
    }

    return iter->parent;
  }
}

bst* bst::predecessor() {
  if (left) {
    return left->maximum();
  } else {
    bst* iter = this;
    while (iter->parent) {
      if (iter->key < iter->parent->key) {
	iter = iter->parent;
      } else {
	break;
      }
    }

    return iter->parent;
  }
}

bst* bst::minimum() {
  bst* parent = nullptr;
  bst* child = this;

  while (child) {
    parent = child;
    child = child->left;
  }

  return parent;
}

bst* bst::maximum() {
  bst* parent = nullptr;
  bst* child = this;

  while (child) {
    parent = child;
    child = child->right;
  }

  return parent;
}

bst* bst::find(int k) {
  if (k < key) return left->find(k);
  if (k > key) return right->find(k);
  return this;
}

bst* bst::kth(unsigned k) {
  bst* kth = minimum();
  for (unsigned i = 0; i < k-1; ++i) {
    if (kth)
      kth = kth->successor();
    else
      return kth;
  }

  return kth;
}

std::ostream& operator<<(std::ostream& out, const bst& tree) {
  if (tree.left)
    out << *tree.left;
  out << tree.key << ' ';
  if (tree.right)
    out << *tree.right;
  return out;
}


#endif
