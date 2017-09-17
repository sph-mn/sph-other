
#ifndef sc_included_lmdb_h
#include <lmdb.h>
#define sc_included_lmdb_h
#endif
#define debug_log_p 1
#ifndef sc_included_sph
#define sc_included_sph
#ifndef sc_included_inttypes_h
#include <inttypes.h>
#define sc_included_inttypes_h
#endif
#ifndef sc_included_stdio_h
#include <stdio.h>
#define sc_included_stdio_h
#endif
#define boolean b8
#define pointer_t uintptr_t
#define b0 void
#define b8 uint8_t
#define b16 uint16_t
#define b32 uint32_t
#define b64 uint64_t
#define b8_s int8_t
#define b16_s int16_t
#define b32_s int32_t
#define b64_s int64_t
#define f32_s float
#define f64_s double
#if debug_log_p
#define debug_log(format, ...)                                                 \
  fprintf(stderr, "%s:%d " format "\n", __func__, __LINE__, __VA_ARGS__)
#else
#define debug_log(format, ...) null
#endif
#define null ((b0)(0))
#define zero_p(a) (0 == a)
#endif
#ifndef sc_included_sph_dg_status
#define sc_included_sph_dg_status
#ifndef sc_included_sph_status
#define sc_included_sph_status
typedef b32_s status_i_t;
typedef struct {
  status_i_t id;
  b8 group;
} status_t;
#define status_id_success 0
#define status_group_undefined 0
#define status_init                                                            \
  status_t status = {status_id_success, status_group_undefined}
#define status_reset status_set_both(status_group_undefined, status_id_success)
#define status_success_p (status_id_success == status.id)
#define status_failure_p !status_success_p
#define status_goto goto exit
#define status_set_group(group_id) status.group = group_id
#define status_set_id(status_id) status.id = status_id
#define status_set_both(group_id, status_id)                                   \
  status_set_group(group_id);                                                  \
  status_set_id(status_id)
#define status_require                                                         \
  if (status_failure_p) {                                                      \
    status_goto;                                                               \
  }
#define status_require_x(expression)                                           \
  status = expression;                                                         \
  if (status_failure_p) {                                                      \
    status_goto;                                                               \
  }
#define status_set_id_goto(status_id)                                          \
  status_set_id(status_id);                                                    \
  status_goto
#define status_set_group_goto(group_id)                                        \
  status_set_group(group_id);                                                  \
  status_goto
#define status_set_both_goto(group_id, status_id)                              \
  status_set_both(group_id, status_id);                                        \
  status_goto
#define status_id_is_p(status_id) (status_id == status.id)
#endif
enum {
  dg_status_id_undefined,
  dg_status_id_input_type,
  dg_status_id_max_id,
  dg_status_id_data_length,
  dg_status_id_not_implemented,
  dg_status_id_duplicate,
  dg_status_id_memory,
  dg_status_id_condition_unfulfilled,
  dg_status_id_missing_argument_dg_root,
  dg_status_id_path_not_accessible_dg_root,
  dg_status_id_no_more_data,
  dg_status_group_dg,
  dg_status_group_lmdb,
  dg_status_group_libc
};
b8 *dg_status_group_id_to_name(status_i_t a) {
  return (((dg_status_group_dg == a)
               ? "sph-dg"
               : ((dg_status_group_lmdb == a)
                      ? "lmdb"
                      : ((dg_status_group_libc == a) ? "libc" : ""))));
};
b8 *dg_status_description(status_t a) {
  return ((
      (dg_status_group_dg == a.group)
          ? ((b8 *)((
                (dg_status_id_input_type == a.id)
                    ? "input argument is of wrong type"
                    : ((dg_status_id_data_length == a.id)
                           ? "intern data too large"
                           : ((dg_status_id_duplicate == a.id)
                                  ? "element already exists"
                                  : ((dg_status_id_not_implemented == a.id)
                                         ? "not implemented"
                                         : ((dg_status_id_missing_argument_dg_root ==
                                             a.id)
                                                ? "missing argument 'dg-root'"
                                                : ((dg_status_id_path_not_accessible_dg_root ==
                                                    a.id)
                                                       ? "root not accessible"
                                                       : ((dg_status_id_memory ==
                                                           a.id)
                                                              ? "not enough "
                                                                "memory or "
                                                                "other memory "
                                                                "allocation "
                                                                "error"
                                                              : ((dg_status_id_max_id ==
                                                                  a.id)
                                                                     ? "maximum"
                                                                       " identi"
                                                                       "fier "
                                                                       "value "
                                                                       "has "
                                                                       "been "
                                                                       "reached"
                                                                     : ((dg_status_id_condition_unfulfilled ==
                                                                         a.id)
                                                                            ? "condition unfulfilled"
                                                                            : ((dg_status_id_no_more_data ==
                                                                                a.id)
                                                                                   ? "no more data to read"
                                                                                   : ""))))))))))))
          : ((dg_status_group_lmdb == a.group) ? ((b8 *)(mdb_strerror(a.id)))
                                               : ((b8 *)("")))));
};
b8 *dg_status_name(status_t a) {
  return ((
      (dg_status_group_dg == a.group)
          ? ((b8 *)((
                (dg_status_id_input_type == a.id)
                    ? "input-type"
                    : ((dg_status_id_data_length == a.id)
                           ? "data-length"
                           : ((dg_status_id_duplicate == a.id)
                                  ? "duplicate"
                                  : ((dg_status_id_not_implemented == a.id)
                                         ? "not-implemented"
                                         : ((dg_status_id_missing_argument_dg_root ==
                                             a.id)
                                                ? "missing-argument-dg-root"
                                                : ((dg_status_id_path_not_accessible_dg_root ==
                                                    a.id)
                                                       ? "path-not-accessible-"
                                                         "dg-root"
                                                       : ((dg_status_id_memory ==
                                                           a.id)
                                                              ? "memory"
                                                              : ((dg_status_id_max_id ==
                                                                  a.id)
                                                                     ? "max-id-"
                                                                       "reached"
                                                                     : ((dg_status_id_condition_unfulfilled ==
                                                                         a.id)
                                                                            ? "condition-unfulfilled"
                                                                            : ((dg_status_id_no_more_data ==
                                                                                a.id)
                                                                                   ? "no-more-data"
                                                                                   : "unknown"))))))))))))
          : ((dg_status_group_lmdb == a.group) ? ((b8 *)(mdb_strerror(a.id)))
                                               : ((b8 *)("unknown")))));
};
#define dg_status_set_id_goto(status_id)                                       \
  status_set_both_goto(dg_status_group_dg, status_id)
#define dg_status_require_read_x(expression)                                   \
  status = expression;                                                         \
  if (!(status_success_p || status_id_is_p(dg_status_id_no_more_data))) {      \
    status_goto;                                                               \
  }
#define dg_status_no_more_data_if_mdb_notfound                                 \
  if (dg_mdb_status_notfound_p) {                                              \
    status_set_both(dg_status_group_dg, dg_status_id_no_more_data);            \
  }
#define dg_status_success_if_mdb_notfound                                      \
  if (dg_mdb_status_notfound_p) {                                              \
    status_set_id(status_id_success);                                          \
  }
#define dg_status_success_if_no_more_data                                      \
  if (status_id_is_p(dg_status_id_no_more_data)) {                             \
    status.id = status_id_success;                                             \
  }
#define dg_mdb_status_success_p status_id_is_p(MDB_SUCCESS)
#define dg_mdb_status_failure_p !dg_mdb_status_success_p
#define dg_mdb_status_notfound_p status_id_is_p(MDB_NOTFOUND)
#define dg_mdb_status_set_id_goto(id)                                          \
  status_set_both_goto(dg_status_group_lmdb, id)
#define dg_mdb_status_require_x(expression)                                    \
  status_set_id(expression);                                                   \
  if (dg_mdb_status_failure_p) {                                               \
    status_set_group_goto(dg_status_group_lmdb);                               \
  }
#define dg_mdb_status_require                                                  \
  if (dg_mdb_status_failure_p) {                                               \
    status_set_group_goto(dg_status_group_lmdb);                               \
  }
#define dg_mdb_status_require_read                                             \
  if (!(dg_mdb_status_success_p || dg_mdb_status_notfound_p)) {                \
    status_set_group_goto(dg_status_group_lmdb);                               \
  }
#define dg_mdb_status_require_read_x(expression)                               \
  status_set_id(expression);                                                   \
  dg_mdb_status_require_read
#define dg_mdb_status_require_notfound                                         \
  if (!dg_mdb_status_notfound_p) {                                             \
    status_set_group_goto(dg_status_group_lmdb);                               \
  }
#endif
#ifndef sc_included_sph_dg_config
#define sc_included_sph_dg_config
#define dg_id_t b64
#define dg_ordinal_t b32
#define dg_id_max UINT64_MAX
#define dg_size_octets_data_max 511
#define dg_size_octets_data_min 1
#define dg_size_octets_id sizeof(dg_id_t)
#define dg_size_octets_ordinal sizeof(dg_ordinal_t)
#define dg_id_equal_p(a, b) (a == b)
#define dg_id_compare(a, b) ((a < b) ? -1 : (a > b))
#define dg_pointer_to_id(a, index) (*(index + ((dg_id_t *)(a))))
#endif
#ifndef dg_id_t
#define dg_id_t b64
#endif
#ifndef dg_ordinal_t
#define dg_ordinal_t b32
#endif
#ifndef dg_id_max
#define dg_id_max UINT64_MAX
#endif
#ifndef dg_size_octets_data_max
#define dg_size_octets_data_max 511
#endif
#ifndef dg_size_octets_data_min
#define dg_size_octets_data_min 1
#endif
#ifndef dg_size_octets_id
#define dg_size_octets_id sizeof(dg_id_t)
#endif
#ifndef dg_size_octets_ordinal
#define dg_size_octets_ordinal sizeof(dg_ordinal_t)
#endif
#ifndef dg_id_equal_p
#define dg_id_equal_p(a, b) (a == b)
#endif
#ifndef dg_id_compare
#define dg_id_compare(a, b) ((a < b) ? -1 : (a > b))
#endif
#define dg_ordinal_compare dg_id_compare
#ifndef dg_pointer_to_id
#define dg_pointer_to_id(a, index) (*(index + ((dg_id_t *)(a))))
#endif
#define imht_set_key_t dg_id_t
#define dg_type_id 0
#define dg_type_intern 1
#define dg_type_extern 2
#define dg_type_intern_small 3
#define dg_type_mask 3
#define dg_id_type_step 4
#define dg_size_octets_relation_data                                           \
  (dg_size_octets_ordinal + dg_size_octets_id)
#define dg_size_octets_relation_key (2 * dg_size_octets_id)
#define dg_read_option_skip 1
#define dg_read_option_is_set_left 2
#define dg_read_option_is_set_right 4
#define dg_read_option_initialised 8
#define dg_null 0
#define dg_type_bit_id 1
#define dg_type_bit_intern 2
#define dg_type_bit_extern 4
#define dg_type_bit_intern_small 8
#ifndef sc_included_stdlib_h
#include <stdlib.h>
#define sc_included_stdlib_h
#endif
#ifndef sc_included_inttypes_h
#include <inttypes.h>
#define sc_included_inttypes_h
#endif
#ifndef imht_set_key_t
#define imht_set_key_t uint64_t
#endif
#ifndef imht_set_can_contain_zero_p
#define imht_set_can_contain_zero_p 1
#endif
#ifndef imht_set_size_factor
#define imht_set_size_factor 2
#endif
uint16_t imht_set_primes[] = {
    0,   3,   7,   13,  19,  29,  37,  43,  53,  61,  71,  79,  89,  101, 107,
    113, 131, 139, 151, 163, 173, 181, 193, 199, 223, 229, 239, 251, 263, 271,
    281, 293, 311, 317, 337, 349, 359, 373, 383, 397, 409, 421, 433, 443, 457,
    463, 479, 491, 503, 521, 541, 557, 569, 577, 593, 601, 613, 619, 641, 647,
    659, 673, 683, 701, 719, 733, 743, 757, 769, 787, 809, 821, 827, 839, 857,
    863, 881, 887, 911, 929, 941, 953, 971, 983, 997};
uint16_t *imht_set_primes_end = (imht_set_primes + 83);
typedef struct {
  size_t size;
  imht_set_key_t *content;
} imht_set_t;
size_t imht_set_calculate_hash_table_size(size_t min_size) {
  min_size = (imht_set_size_factor * min_size);
  uint16_t *primes = imht_set_primes;
  while ((primes < imht_set_primes_end)) {
    if ((min_size <= (*primes))) {
      return ((*primes));
    } else {
      primes = (1 + primes);
    };
  };
  if ((min_size <= (*primes))) {
    return ((*primes));
  };
  return ((1 | min_size));
};
uint8_t imht_set_create(size_t min_size, imht_set_t **result) {
  (*result) = malloc(sizeof(imht_set_t));
  if (!(*result)) {
    return (0);
  };
  min_size = imht_set_calculate_hash_table_size(min_size);
  (*(*result)).content = calloc(min_size, sizeof(imht_set_key_t));
  (*(*result)).size = min_size;
  return (((*(*result)).content ? 1 : 0));
};
void imht_set_destroy(imht_set_t *a) {
  if (a) {
    free((*a).content);
    free(a);
  };
};
#if imht_set_can_contain_zero_p
#define imht_set_hash(value, hash_table)                                       \
  (value ? (1 + (value % (hash_table.size - 1))) : 0)
#else
#define imht_set_hash(value, hash_table) (value % hash_table.size)
#endif
/** returns the address of the element in the set, 0 if it was not found.
  caveat: if imht-set-can-contain-zero? is defined, which is the default,
  dereferencing a returned address for the found value 0 will return 1 instead
*/
imht_set_key_t *imht_set_find(imht_set_t *a, imht_set_key_t value) {
  imht_set_key_t *h = ((*a).content + imht_set_hash(value, (*a)));
  if ((*h)) {
#if imht_set_can_contain_zero_p
    if (((((*h) == value)) || ((0 == value)))) {
      return (h);
    };
#else
    if (((*h) == value)) {
      return (h);
    };
#endif
    imht_set_key_t *content_end = ((*a).content + ((*a).size - 1));
    imht_set_key_t *h2 = (1 + h);
    while ((h2 < content_end)) {
      if (!(*h2)) {
        return (0);
      } else {
        if ((value == (*h2))) {
          return (h2);
        };
      };
      h2 = (1 + h2);
    };
    if (!(*h2)) {
      return (0);
    } else {
      if ((value == (*h2))) {
        return (h2);
      };
    };
    h2 = (*a).content;
    while ((h2 < h)) {
      if (!(*h2)) {
        return (0);
      } else {
        if ((value == (*h2))) {
          return (h2);
        };
      };
      h2 = (1 + h2);
    };
  };
  return (0);
};
#define imht_set_contains_p(a, value) ((0 == imht_set_find(a, value)) ? 0 : 1)
/** returns 1 if the element was removed, 0 if it was not found */
uint8_t imht_set_remove(imht_set_t *a, imht_set_key_t value) {
  imht_set_key_t *value_address = imht_set_find(a, value);
  if (value_address) {
    (*value_address) = 0;
    return (1);
  } else {
    return (0);
  };
};
/** returns the address of the added or already included element, 0 if there is
 * no space left in the set */
imht_set_key_t *imht_set_add(imht_set_t *a, imht_set_key_t value) {
  imht_set_key_t *h = ((*a).content + imht_set_hash(value, (*a)));
  if ((*h)) {
#if imht_set_can_contain_zero_p
    if ((((value == (*h))) || ((0 == value)))) {
      return (h);
    };
#else
    if ((value == (*h))) {
      return (h);
    };
#endif
    imht_set_key_t *content_end = ((*a).content + ((*a).size - 1));
    imht_set_key_t *h2 = (1 + h);
    while ((((h2 <= content_end)) && (*h2))) {
      h2 = (1 + h2);
    };
    if ((h2 > content_end)) {
      h2 = (*a).content;
      while (((h2 < h) && (*h2))) {
        h2 = (1 + h2);
      };
      if ((h2 == h)) {
        return (0);
      } else {
#if imht_set_can_contain_zero_p
        (*h2) = ((0 == value) ? 1 : value);
#else
        (*h2) = value;
#endif
      };
    } else {
#if imht_set_can_contain_zero_p
      (*h2) = ((0 == value) ? 1 : value);
#else
      (*h2) = value;
#endif
    };
  } else {
#if imht_set_can_contain_zero_p
    (*h) = ((0 == value) ? 1 : value);
#else
    (*h) = value;
#endif
    return (h);
  };
};
typedef struct {
  b0 *data;
  size_t size;
} dg_data_t;
typedef struct {
  dg_id_t id;
  size_t size;
  b0 *data;
} dg_data_record_t;
typedef struct {
  dg_id_t left;
  dg_id_t right;
  dg_id_t label;
  dg_ordinal_t ordinal;
} dg_relation_record_t;
#define mi_list_name_prefix dg_ids
#define mi_list_element_t dg_id_t
#ifndef sc_included_stdlib_h
#include <stdlib.h>
#define sc_included_stdlib_h
#endif
#ifndef sc_included_inttypes_h
#include <inttypes.h>
#define sc_included_inttypes_h
#endif
#ifndef mi_list_name_prefix
#define mi_list_name_prefix mi_list_64
#endif
#ifndef mi_list_element_t
#define mi_list_element_t uint64_t
#endif
#ifndef mi_list_name_concat
#define mi_list_name_concat(a, b) a##_##b
#define mi_list_name_concatenator(a, b) mi_list_name_concat(a, b)
#define mi_list_name(name) mi_list_name_concatenator(mi_list_name_prefix, name)
#endif
#define mi_list_struct_name mi_list_name(struct)
#define mi_list_t mi_list_name(t)
typedef struct mi_list_struct_name {
  struct mi_list_struct_name *link;
  mi_list_element_t data;
} mi_list_t;
#ifndef mi_list_first
#define mi_list_first(a) (*a).data
#define mi_list_first_address(a) &(*a).data
#define mi_list_rest(a) (*a).link
#endif
mi_list_t *mi_list_name(drop)(mi_list_t *a) {
  mi_list_t *a_next = mi_list_rest(a);
  free(a);
  return (a_next);
};
void mi_list_name(destroy)(mi_list_t *a) {
  mi_list_t *a_next = 0;
  while (a) {
    a_next = (*a).link;
    free(a);
    a = a_next;
  };
};
mi_list_t *mi_list_name(add)(mi_list_t *a, mi_list_element_t value) {
  mi_list_t *element = calloc(1, sizeof(mi_list_t));
  if (!element) {
    return (0);
  };
  (*element).data = value;
  (*element).link = a;
  return (element);
};
size_t mi_list_name(length)(mi_list_t *a) {
  size_t result = 0;
  while (a) {
    result = (1 + result);
    a = mi_list_rest(a);
  };
  return (result);
};
#undef mi_list_name_prefix
#undef mi_list_element_t
#undef mi_list_struct_name
#undef mi_list_t

#define mi_list_name_prefix dg_data_list
#define mi_list_element_t dg_data_t
#ifndef sc_included_stdlib_h
#include <stdlib.h>
#define sc_included_stdlib_h
#endif
#ifndef sc_included_inttypes_h
#include <inttypes.h>
#define sc_included_inttypes_h
#endif
#ifndef mi_list_name_prefix
#define mi_list_name_prefix mi_list_64
#endif
#ifndef mi_list_element_t
#define mi_list_element_t uint64_t
#endif
#ifndef mi_list_name_concat
#define mi_list_name_concat(a, b) a##_##b
#define mi_list_name_concatenator(a, b) mi_list_name_concat(a, b)
#define mi_list_name(name) mi_list_name_concatenator(mi_list_name_prefix, name)
#endif
#define mi_list_struct_name mi_list_name(struct)
#define mi_list_t mi_list_name(t)
typedef struct mi_list_struct_name {
  struct mi_list_struct_name *link;
  mi_list_element_t data;
} mi_list_t;
#ifndef mi_list_first
#define mi_list_first(a) (*a).data
#define mi_list_first_address(a) &(*a).data
#define mi_list_rest(a) (*a).link
#endif
mi_list_t *mi_list_name(drop)(mi_list_t *a) {
  mi_list_t *a_next = mi_list_rest(a);
  free(a);
  return (a_next);
};
void mi_list_name(destroy)(mi_list_t *a) {
  mi_list_t *a_next = 0;
  while (a) {
    a_next = (*a).link;
    free(a);
    a = a_next;
  };
};
mi_list_t *mi_list_name(add)(mi_list_t *a, mi_list_element_t value) {
  mi_list_t *element = calloc(1, sizeof(mi_list_t));
  if (!element) {
    return (0);
  };
  (*element).data = value;
  (*element).link = a;
  return (element);
};
size_t mi_list_name(length)(mi_list_t *a) {
  size_t result = 0;
  while (a) {
    result = (1 + result);
    a = mi_list_rest(a);
  };
  return (result);
};
#undef mi_list_name_prefix
#undef mi_list_element_t
#undef mi_list_struct_name
#undef mi_list_t

#define mi_list_name_prefix dg_data_records
#define mi_list_element_t dg_data_record_t
#ifndef sc_included_stdlib_h
#include <stdlib.h>
#define sc_included_stdlib_h
#endif
#ifndef sc_included_inttypes_h
#include <inttypes.h>
#define sc_included_inttypes_h
#endif
#ifndef mi_list_name_prefix
#define mi_list_name_prefix mi_list_64
#endif
#ifndef mi_list_element_t
#define mi_list_element_t uint64_t
#endif
#ifndef mi_list_name_concat
#define mi_list_name_concat(a, b) a##_##b
#define mi_list_name_concatenator(a, b) mi_list_name_concat(a, b)
#define mi_list_name(name) mi_list_name_concatenator(mi_list_name_prefix, name)
#endif
#define mi_list_struct_name mi_list_name(struct)
#define mi_list_t mi_list_name(t)
typedef struct mi_list_struct_name {
  struct mi_list_struct_name *link;
  mi_list_element_t data;
} mi_list_t;
#ifndef mi_list_first
#define mi_list_first(a) (*a).data
#define mi_list_first_address(a) &(*a).data
#define mi_list_rest(a) (*a).link
#endif
mi_list_t *mi_list_name(drop)(mi_list_t *a) {
  mi_list_t *a_next = mi_list_rest(a);
  free(a);
  return (a_next);
};
void mi_list_name(destroy)(mi_list_t *a) {
  mi_list_t *a_next = 0;
  while (a) {
    a_next = (*a).link;
    free(a);
    a = a_next;
  };
};
mi_list_t *mi_list_name(add)(mi_list_t *a, mi_list_element_t value) {
  mi_list_t *element = calloc(1, sizeof(mi_list_t));
  if (!element) {
    return (0);
  };
  (*element).data = value;
  (*element).link = a;
  return (element);
};
size_t mi_list_name(length)(mi_list_t *a) {
  size_t result = 0;
  while (a) {
    result = (1 + result);
    a = mi_list_rest(a);
  };
  return (result);
};
#undef mi_list_name_prefix
#undef mi_list_element_t
#undef mi_list_struct_name
#undef mi_list_t

#define mi_list_name_prefix dg_relation_records
#define mi_list_element_t dg_relation_record_t
#ifndef sc_included_stdlib_h
#include <stdlib.h>
#define sc_included_stdlib_h
#endif
#ifndef sc_included_inttypes_h
#include <inttypes.h>
#define sc_included_inttypes_h
#endif
#ifndef mi_list_name_prefix
#define mi_list_name_prefix mi_list_64
#endif
#ifndef mi_list_element_t
#define mi_list_element_t uint64_t
#endif
#ifndef mi_list_name_concat
#define mi_list_name_concat(a, b) a##_##b
#define mi_list_name_concatenator(a, b) mi_list_name_concat(a, b)
#define mi_list_name(name) mi_list_name_concatenator(mi_list_name_prefix, name)
#endif
#define mi_list_struct_name mi_list_name(struct)
#define mi_list_t mi_list_name(t)
typedef struct mi_list_struct_name {
  struct mi_list_struct_name *link;
  mi_list_element_t data;
} mi_list_t;
#ifndef mi_list_first
#define mi_list_first(a) (*a).data
#define mi_list_first_address(a) &(*a).data
#define mi_list_rest(a) (*a).link
#endif
mi_list_t *mi_list_name(drop)(mi_list_t *a) {
  mi_list_t *a_next = mi_list_rest(a);
  free(a);
  return (a_next);
};
void mi_list_name(destroy)(mi_list_t *a) {
  mi_list_t *a_next = 0;
  while (a) {
    a_next = (*a).link;
    free(a);
    a = a_next;
  };
};
mi_list_t *mi_list_name(add)(mi_list_t *a, mi_list_element_t value) {
  mi_list_t *element = calloc(1, sizeof(mi_list_t));
  if (!element) {
    return (0);
  };
  (*element).data = value;
  (*element).link = a;
  return (element);
};
size_t mi_list_name(length)(mi_list_t *a) {
  size_t result = 0;
  while (a) {
    result = (1 + result);
    a = mi_list_rest(a);
  };
  return (result);
};
#undef mi_list_name_prefix
#undef mi_list_element_t
#undef mi_list_struct_name
#undef mi_list_t

#define dg_ids_first mi_list_first
#define dg_ids_first_address mi_list_first_address
#define dg_ids_rest mi_list_rest
#define dg_data_list_first mi_list_first
#define dg_data_list_first_address mi_list_first_address
#define dg_data_list_rest mi_list_rest
#define dg_data_records_first mi_list_first
#define dg_data_records_first_address mi_list_first_address
#define dg_data_records_rest mi_list_rest
#define dg_relation_records_first mi_list_first
#define dg_relation_records_first_address mi_list_first_address
#define dg_relation_records_rest mi_list_rest
typedef MDB_txn dg_txn_t;
#define dg_txn_introduce dg_txn_t *dg_txn = 0
#define dg_txn_begin                                                           \
  dg_mdb_status_require_x(mdb_txn_begin(dg_mdb_env, 0, MDB_RDONLY, &dg_txn))
#define dg_txn_abort                                                           \
  mdb_txn_abort(dg_txn);                                                       \
  dg_txn = 0
#define dg_txn_commit                                                          \
  dg_mdb_status_require_x(mdb_txn_commit(dg_txn));                             \
  dg_txn = 0
#define dg_txn_write_begin                                                     \
  dg_mdb_status_require_x(mdb_txn_begin(dg_mdb_env, 0, 0, &dg_txn))
#define dg_intern_small_id_to_data(id) (id >> 2)
#define dg_intern_small_data_to_id(data) (dg_type_intern_small & (data << 2))
typedef struct {
  MDB_stat id_to_data;
  MDB_stat data_intern_to_id;
  MDB_stat data_extern_to_extern;
  MDB_stat left_to_right;
  MDB_stat right_to_left;
  MDB_stat label_to_left;
} dg_statistics_t;
typedef struct {
  b8 read_only_p;
  size_t maximum_size_octets;
  b32 maximum_reader_count;
  b8 filesystem_has_ordered_writes_p;
  b32 env_open_flags;
  b16 file_permissions;
} dg_init_options_t;
typedef struct {
  boolean errors_p;
  dg_relation_records_t *missing_right_left;
  dg_relation_records_t *missing_label_left;
  dg_relation_records_t *excess_right_left;
  dg_relation_records_t *excess_label_left;
} dg_index_errors_relation_t;
typedef struct {
  boolean errors_p;
  dg_ids_t *different_data_id;
  dg_ids_t *excess_data_id;
  dg_ids_t *different_id_data;
  dg_ids_t *missing_id_data;
} dg_index_errors_intern_t;
typedef struct {
  boolean errors_p;
  dg_ids_t *different_data_extern;
  dg_ids_t *excess_data_extern;
  dg_ids_t *different_id_data;
  dg_ids_t *missing_id_data;
} dg_index_errors_extern_t;
dg_index_errors_relation_t dg_index_errors_relation_null = {0, 0, 0, 0, 0};
dg_index_errors_intern_t dg_index_errors_intern_null = {0, 0, 0, 0, 0};
dg_index_errors_extern_t dg_index_errors_extern_null = {0, 0, 0, 0, 0};
typedef struct {
  dg_ordinal_t min;
  dg_ordinal_t max;
} dg_ordinal_match_data_t;
typedef struct {
  status_t status;
  MDB_cursor *restrict cursor;
  b8 types;
  b8 options;
} dg_node_read_state_t;
typedef struct {
  status_t status;
  MDB_cursor *restrict cursor;
  b8 options;
} dg_intern_read_state_t;
typedef struct {
  status_t status;
  MDB_cursor *restrict cursor;
  MDB_cursor *restrict cursor_2;
  b0 *left;
  b0 *right;
  b0 *label;
  dg_ids_t *left_first;
  dg_ids_t *right_first;
  dg_ordinal_match_data_t *ordinal;
  b8 options;
  b0 *reader;
} dg_relation_read_state_t;
typedef status_t (*dg_relation_reader_t)(dg_relation_read_state_t *, b32,
                                         dg_relation_records_t **);
typedef dg_ordinal_t (*dg_relation_ordinal_generator_t)(b0 *);
#define dg_type_p(dg_type_name, id) (dg_type_name == (id & dg_type_mask))
status_t dg_node_read(dg_node_read_state_t *state, b32 count,
                      dg_data_records_t **result);
status_t dg_node_select(dg_txn_t *txn, b8 types, b32 offset,
                        dg_node_read_state_t *state);
b0 dg_node_selection_destroy(dg_node_read_state_t *state);
b0 dg_relation_selection_destroy(dg_relation_read_state_t *state);
status_t dg_intern_data_to_id(dg_txn_t *txn, dg_data_list_t *data,
                              boolean every_p, dg_ids_t **result);
status_t dg_intern_ensure(dg_txn_t *txn, dg_data_list_t *data,
                          dg_ids_t **result);
status_t dg_intern_update(dg_txn_t *txn, dg_id_t id, dg_data_t data);
status_t dg_extern_update(dg_txn_t *txn, dg_id_t id, dg_data_t data);
status_t dg_intern_id_to_data(dg_txn_t *txn, dg_ids_t *ids, boolean every_p,
                              dg_data_list_t **result);
b0 dg_exit();
status_t dg_extern_create(dg_txn_t *txn, b32 count, dg_data_t *data,
                          dg_ids_t **result);
status_t dg_extern_id_to_data(dg_txn_t *txn, dg_ids_t *ids, boolean every_p,
                              dg_data_list_t **result);
status_t dg_extern_data_to_id(dg_txn_t *txn, dg_data_t data, dg_ids_t **result);
status_t dg_id_create(dg_txn_t *txn, b32 count, dg_ids_t **result);
status_t dg_exists_p(dg_txn_t *txn, dg_ids_t *ids, boolean *result);
boolean dg_extern_p(dg_id_t id);
boolean dg_id_p(dg_id_t id);
boolean dg_intern_small_p(dg_id_t id);
status_t dg_identify(dg_txn_t *txn, dg_ids_t *ids, dg_ids_t **result);
boolean dg_intern_p(dg_id_t id);
boolean dg_relation_p(dg_id_t id);
status_t dg_index_errors_extern(dg_txn_t *txn,
                                dg_index_errors_extern_t *result);
status_t dg_index_errors_intern(dg_txn_t *txn,
                                dg_index_errors_intern_t *result);
status_t dg_index_errors_relation(dg_txn_t *txn,
                                  dg_index_errors_relation_t *result);
status_t dg_index_recreate_extern();
status_t dg_index_recreate_intern();
status_t dg_index_recreate_relation();
status_t dg_init(b8 *dg_root_argument, dg_init_options_t *options);
dg_init_options_t dg_init_options_set_defaults(dg_init_options_t *a);
status_t dg_relation_ensure(dg_txn_t *txn, dg_ids_t *left, dg_ids_t *right,
                            dg_ids_t *label,
                            dg_relation_ordinal_generator_t ordinal_generator,
                            b0 *ordinal_generator_state);
status_t dg_statistics(dg_txn_t *txn, dg_statistics_t *result);
status_t dg_delete(dg_txn_t *txn, dg_ids_t *ids);
status_t dg_relation_delete(dg_txn_t *txn, dg_ids_t *left, dg_ids_t *right,
                            dg_ids_t *label, dg_ordinal_match_data_t *ordinal);
b8 *dg_status_description(status_t a);
b8 *dg_status_name(status_t a);
b8 *dg_status_group_id_to_name(status_i_t a);
status_t dg_relation_select(dg_txn_t *txn, dg_ids_t *left, dg_ids_t *right,
                            dg_ids_t *label, dg_ordinal_match_data_t *ordinal,
                            b32 offset, dg_relation_read_state_t *result);
status_t dg_relation_read(dg_relation_read_state_t *state, b32 count,
                          dg_relation_records_t **result);
b8 *dg_root;
boolean dg_initialised;
MDB_env *dg_mdb_env;
#define dg_pointer_allocation_set(result, expression, result_temp)             \
  result_temp = expression;                                                    \
  if (result_temp) {                                                           \
    result = result_temp;                                                      \
  } else {                                                                     \
    dg_status_set_id_goto(dg_status_id_memory);                                \
  }
#define dg_ids_add_x(target, source, ids_temp)                                 \
  dg_pointer_allocation_set(target, dg_ids_add(target, source), ids_temp)
#define dg_define_ids(name) dg_ids_t *name = 0
#define dg_define_ids_2(name_1, name_2)                                        \
  dg_define_ids(name_1);                                                       \
  dg_define_ids(name_2)
#define dg_define_ids_3(name_1, name_2, name_3)                                \
  dg_define_ids_2(name_1, name_2);                                             \
  dg_define_ids(name_3)
#define dg_relation_data_to_id(a)                                              \
  dg_pointer_to_id((1 + ((dg_ordinal_t *)(a))), 0)
#define dg_relation_data_to_ordinal(a) (*((dg_ordinal_t *)(a)))
#define dg_relation_data_set_id(a, value) dg_relation_data_to_id(a) = value
#define dg_relation_data_set_ordinal(a, value)                                 \
  dg_relation_data_to_ordinal(a) = value
#define dg_relation_data_set_both(a, ordinal, id)                              \
  dg_relation_data_set_ordinal(ordinal);                                       \
  dg_relation_data_set_id(id)
b0 dg_debug_log_ids(dg_ids_t *a);
b0 dg_debug_log_ids_set(imht_set_t a);
b0 dg_debug_display_relation_records(dg_relation_records_t *records);
status_t dg_debug_count_all_btree_entries(MDB_txn *txn, b32 *result);
status_t dg_debug_display_btree_counts(MDB_txn *txn);
status_t dg_debug_display_content_left_to_right(dg_txn_t *txn);
status_t dg_debug_display_content_right_to_left(dg_txn_t *txn);