/**
 * File: rbtree.h
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#ifndef _RBTREE_H
#define _RBTREE_H

#define rbt_node_red(n)			((n)->red = 1)
#define rbt_node_black(n)		((n)->red = 0)
#define rbt_node_is_red(n)		((n)->red)
#define rbt_node_is_black(n)		(!rbt_node_is_red(n))
#define rbt_node_copy_color(n1, n2)	(n1->red = n2->red)

struct rbtree_node {
	unsigned long key;		/* for node comparing */
	struct rbtree_node *left;	/* left child node */
	struct rbtree_node *right;	/* right child node */
	struct rbtree_node *parent;	/* parent node */
	int red;			/* color */
};

typedef void (*insert_func)(struct rbtree_node *root, struct rbtree_node *node,
			    struct rbtree_node *sentinel);

struct rbtree {
	struct rbtree_node *root;	/* root node */
	struct rbtree_node *sentinel;	/* sentinel node */
	insert_func insert;		/* insert function */
};

/* init an empty tree */
void rbtree_init(struct rbtree *tree, struct rbtree_node *sentinel, insert_func i);

/* insert a node into the tree */
void rbtree_insert(struct rbtree *tree, struct rbtree_node *node);

/* delete a node from the tree */
void rbtree_delete(struct rbtree *tree, struct rbtree_node *node);

static inline struct rbtree_node *__rbtree_min(struct rbtree_node *node,
					       struct rbtree_node *sentinel)
{
	while (node->left != sentinel)
		node = node->left;

	return node;
}

/* get the min node of the tree */
static inline struct rbtree_node *rbtree_min(struct rbtree *tree)
{
	if (tree->root == tree->sentinel)
		return tree->sentinel;
	return __rbtree_min(tree->root, tree->sentinel);
}

void rbtree_insert_value(struct rbtree_node *root, struct rbtree_node *node,
			 struct rbtree_node *sentinel);

#endif /* _RBTREE_H */
