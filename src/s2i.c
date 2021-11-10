/*
  (C) Nguyen Ba Ngoc 2021
*/

#include "s2i.h"

#include <string.h>

int s2i_compare_data(const char *q, bn_node_t n) {
  const char *s1 = (char*)q;
  const char *s2 = s2i_node_key(n);
  return strcmp(s1, s2);
}

bn_node_t s2i_create_node(const char *key, long value) {
  s2i_node_t n = calloc(1, sizeof(struct s2i_node));
  n->key = strdup(key);
  n->value = value;
  return to_bn(n);
}

bn_node_t s2i_insert(bn_tree_t t, const char *key, long value) {
  bn_node_t same = NULL_PTR, parent = NULL_PTR;
  bn_node_t *loc;
  bns_insert_setup(loc, t->root, key, s2i_compare_data, same, parent);
  if (same) {
    return same;
  }
  bn_node_t n = s2i_create_node(key, value);
  rb_insert(t, n, loc, parent);
  return n;
}

bn_node_t s2i_set(bn_tree_t t, const char *key, long value) {
  bn_node_t same = NULL_PTR, parent = NULL_PTR;
  bn_node_t *loc;
  bns_insert_setup(loc, t->root, key, s2i_compare_data, same, parent);
  if (same) {
    s2i_node_value(same) = value;
    return same;
  }
  bn_node_t n = s2i_create_node(key, value);
  rb_insert(t, n, loc, parent);
  return n;
}

s2i_node_t s2i_search(bn_tree_t t, const char *key) {
  bns_search_inline(result, t, key, s2i_compare_data, return to_s2i(result));
}

long *s2i_vref(bn_tree_t t, const char *key) {
  s2i_node_t n = s2i_search(t, key);
  if (n) {
    return &(n->value);
  }
  return NULL_PTR;
}

long s2i_value(bn_tree_t t, const char *key) {
  long *value = s2i_vref(t, key);
  if (value) {
    return *value;
  }
  return k_s2i_invalid;
}

int s2i_delete(bn_tree_t t, const char *key) {
  s2i_node_t n = s2i_search(t, key);
  if (n) {
    rb_delete(t, to_bn(n));
    s2i_free_node(to_bn(n));
    return 1;
  }
  return 0;
}

void s2i_free_node(bn_node_t n) {
  s2i_node_t p = to_s2i(n);
  free(p->key);
  free(n);
}

void s2i_free(bn_tree_t tp) {
  bn_traverse_lnr(cur, tp) {
    free(to_s2i(cur)->key);
  }
  bn_free_tree(tp);
}

void s2i_postorder_print(bn_tree_t tree) {
  bn_node_t cur;
  bn_traverse_lrn(cur, tree) {
    printf("%s: %ld\n", to_s2i(cur)->key, to_s2i(cur)->value);
  }
}

void s2i_print_node(bn_node_t n) {
  s2i_node_t tmp = to_s2i(n);
  printf("(%s, %ld) - %s\n", tmp->key, tmp->value, rb_color_str(n));
}