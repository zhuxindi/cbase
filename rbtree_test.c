/**
 * File: rbtree_test.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <rbtree.h>
#include <log.h>
#include <stdio.h>
#include <stdlib.h>

void print_node(struct rbtree_node *node, struct rbtree_node *sentinel)
{
	log_info("%p left=%p right=%p key=%lu", node,
		 node->left != sentinel ? node->left : NULL,
		 node->right != sentinel ? node->right : NULL, node->key);
	if (node->left != sentinel)
		print_node(node->left, sentinel);
	if (node->right != sentinel)
		print_node(node->right, sentinel);
}

void print_tree(const struct rbtree *tree)
{
	if (tree->root != tree->sentinel)
		print_node(tree->root, tree->sentinel);
}

struct rbtree_node *search_tree(const struct rbtree *tree, unsigned long key)
{
	struct rbtree_node *root = tree->root;
	struct rbtree_node *sentinel = tree->sentinel;

	while (1) {
		struct rbtree_node *p;

		if (key < root->key)
			p = root->left;
		else if (key < root->key)
			p = root->right;
		else
			return p;

		if (p == sentinel)
			return p;

		root = p;
	}
}

int main()
{
	struct rbtree tree;
	struct rbtree_node sentinel;
	struct rbtree_node nodes[10];
	struct rbtree_node *p;
	int i;
	unsigned long key;

	update_sys_time();
	set_log_level(LOG_DEBUG);

	rbtree_init(&tree, &sentinel, rbtree_insert_value);
	for (i = 0; i < 10; i++) {
		nodes[i].key = rand() % 10;
		log_info("add %p key=%lu", &nodes[i], nodes[i].key);
		rbtree_insert(&tree, &nodes[i]);
	}
	print_tree(&tree);

	p = rbtree_min(&tree);
	log_info("min=%p key=%lu", p, p->key);

	key = 1;
	p = search_tree(&tree, key);
	if (p != &sentinel)
		log_info("found %p key=%lu", p, key);
	else
		log_info("not found key=%lu", key);

	log_info("del %p key=%lu", &nodes[2], nodes[2].key);
	rbtree_delete(&tree, &nodes[2]);
	log_info("del %p key=%lu", &nodes[5], nodes[5].key);
	rbtree_delete(&tree, &nodes[5]);
	log_info("del %p key=%lu", &nodes[9], nodes[9].key);
	rbtree_delete(&tree, &nodes[9]);
	print_tree(&tree);

	p = rbtree_min(&tree);
	log_info("min=%p key=%lu", p, p->key);

	p = search_tree(&tree, key);
	if (p != &sentinel)
		log_info("found %p key=%lu", p, key);
	else
		log_info("not found key=%lu", key);

	return 0;
}
