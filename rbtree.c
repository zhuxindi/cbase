/**
 * File: rbtree.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <rbtree.h>

void rbtree_init(struct rbtree *tree, struct rbtree_node *sentinel, insert_func i)
{
	rbt_node_black(sentinel);
	tree->root = sentinel;
	tree->sentinel = sentinel;
	tree->insert = i;
}

static void rbtree_left_rotate(struct rbtree_node **root,
			       struct rbtree_node *sentinel,
			       struct rbtree_node *node)
{
	struct rbtree_node *temp = node->right;

	node->right = temp->left;

	if (temp->left != sentinel)
		temp->left->parent = node;

	temp->parent = node->parent;

	if (node == *root)
		*root = temp;
	else if (node == node->parent->left)
		node->parent->left = temp;
	else
		node->parent->right = temp;

	temp->left = node;
	node->parent = temp;
}

static void rbtree_right_rotate(struct rbtree_node **root,
				struct rbtree_node *sentinel,
				struct rbtree_node *node)
{
	struct rbtree_node *temp = node->left;

	node->left = temp->right;

	if (temp->right != sentinel)
		temp->right->parent = node;

	temp->parent = node->parent;

	if (node == *root)
		*root = temp;
	else if (node == node->parent->right)
		node->parent->right = temp;
	else
		node->parent->left = temp;

	temp->right = node;
	node->parent = temp;
}

void rbtree_insert(struct rbtree *tree, struct rbtree_node *node)
{
	struct rbtree_node **root = &tree->root;
	struct rbtree_node *sentinel = tree->sentinel;

	/* a binary tree insert */
	if (*root == sentinel) {
		node->parent = 0;
		node->left = sentinel;
		node->right = sentinel;
		rbt_node_black(node);
		*root = node;
		return;
	}

	tree->insert(*root, node, sentinel);

	/* re-balance tree */
	while (node != *root && rbt_node_is_red(node->parent)) {
		if (node->parent == node->parent->parent->left) {
			struct rbtree_node *temp = node->parent->parent->right;

			if (rbt_node_is_red(temp)) {
				rbt_node_black(node->parent);
				rbt_node_black(temp);
				rbt_node_red(node->parent->parent);
				node = node->parent->parent;
			} else {
				if (node == node->parent->right) {
					node = node->parent;
					rbtree_left_rotate(root, sentinel, node);
				}

				rbt_node_black(node->parent);
				rbt_node_red(node->parent->parent);
				rbtree_right_rotate(root, sentinel, node->parent->parent);
			}

		} else {
			struct rbtree_node *temp = node->parent->parent->left;

			if (rbt_node_is_red(temp)) {
				rbt_node_black(node->parent);
				rbt_node_black(temp);
				rbt_node_red(node->parent->parent);
				node = node->parent->parent;
			} else {
				if (node == node->parent->left) {
					node = node->parent;
					rbtree_right_rotate(root, sentinel, node);
				}

				rbt_node_black(node->parent);
				rbt_node_red(node->parent->parent);
				rbtree_left_rotate(root, sentinel, node->parent->parent);
			}
		}
	}

	rbt_node_black(*root);
}

void rbtree_delete(struct rbtree *tree, struct rbtree_node *node)
{
	struct rbtree_node **root = &tree->root;
	struct rbtree_node *sentinel = tree->sentinel;
	struct rbtree_node *subst, *temp;
	int red;

	/* a binary tree delete */
	if (node->left == sentinel) {
		temp = node->right;
		subst = node;
	} else if (node->right == sentinel) {
		temp = node->left;
		subst = node;
	} else {
		subst = __rbtree_min(node->right, sentinel);
		if (subst->left != sentinel)
			temp = subst->left;
		else
			temp = subst->right;
	}

	if (subst == *root) {
		*root = temp;
		rbt_node_black(temp);
		return;
	}

	red = rbt_node_is_red(subst);

	if (subst == subst->parent->left)
		subst->parent->left = temp;
	else
		subst->parent->right = temp;

	if (subst == node)
		temp->parent = subst->parent;
	else {
		if (subst->parent == node)
			temp->parent = subst;
		else
			temp->parent = subst->parent;

		subst->left = node->left;
		subst->right = node->right;
		subst->parent = node->parent;
		rbt_node_copy_color(subst, node);

		if (node == *root)
			*root = subst;
		else {
			if (node == node->parent->left)
				node->parent->left = subst;
			else
				node->parent->right = subst;
		}

		if (subst->left != sentinel)
			subst->left->parent = subst;

		if (subst->right != sentinel)
			subst->right->parent = subst;
	}

	if (red)
		return;

	/* a delete fixup */
	while (temp != *root && rbt_node_is_black(temp)) {

		if (temp == temp->parent->left) {
			struct rbtree_node *w = temp->parent->right;

			if (rbt_node_is_red(w)) {
				rbt_node_black(w);
				rbt_node_red(temp->parent);
				rbtree_left_rotate(root, sentinel, temp->parent);
				w = temp->parent->right;
			}

			if (rbt_node_is_black(w->left) && rbt_node_is_black(w->right)) {
				rbt_node_red(w);
				temp = temp->parent;
			} else {
				if (rbt_node_is_black(w->right)) {
					rbt_node_black(w->left);
					rbt_node_red(w);
					rbtree_right_rotate(root, sentinel, w);
					w = temp->parent->right;
				}

				rbt_node_copy_color(w, temp->parent);
				rbt_node_black(temp->parent);
				rbt_node_black(w->right);
				rbtree_left_rotate(root, sentinel, temp->parent);
				temp = *root;
			}

		} else {
			struct rbtree_node *w = temp->parent->left;

			if (rbt_node_is_red(w)) {
				rbt_node_black(w);
				rbt_node_red(temp->parent);
				rbtree_right_rotate(root, sentinel, temp->parent);
				w = temp->parent->left;
			}

			if (rbt_node_is_black(w->left) && rbt_node_is_black(w->right)) {
				rbt_node_red(w);
				temp = temp->parent;

			} else {
				if (rbt_node_is_black(w->left)) {
					rbt_node_black(w->right);
					rbt_node_red(w);
					rbtree_left_rotate(root, sentinel, w);
					w = temp->parent->left;
				}

				rbt_node_copy_color(w, temp->parent);
				rbt_node_black(temp->parent);
				rbt_node_black(w->left);
				rbtree_right_rotate(root, sentinel, temp->parent);
				temp = *root;
			}
		}
	}

	rbt_node_black(temp);
}

void rbtree_insert_value(struct rbtree_node *root, struct rbtree_node *node,
			 struct rbtree_node *sentinel)
{
	struct rbtree_node **p;

	while (1) {
		p = (node->key < root->key) ? &root->left : &root->right;
		if (*p == sentinel)
			break;

		root = *p;
	}

	*p = node;
	node->parent = root;
	node->left = sentinel;
	node->right = sentinel;
	rbt_node_red(node);
}
