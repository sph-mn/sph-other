
#ifndef sc_included_sph_dg
#define sc_included_sph_dg
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
#endif
#ifndef sc_included_pthread_h
#include <pthread.h>
#define sc_included_pthread_h
#endif
MDB_dbi dbi_left_to_right;
MDB_dbi dbi_right_to_left;
MDB_dbi dbi_label_to_left;
MDB_dbi dbi_id_to_data;
MDB_dbi dbi_data_extern_to_extern;
MDB_dbi dbi_data_intern_to_id;
#define dg_relation_key_equal_p(a, b)                                          \
  (dg_id_equal_p((*(a + 0)), (*(b + 0))) &&                                    \
   dg_id_equal_p((*(a + 1)), (*(b + 1))))
#ifndef sc_included_sph_dg_one
#define sc_included_sph_dg_one
#ifndef sc_included_string_h
#include <string.h>
#define sc_included_string_h
#endif
#ifndef sc_included_stdlib_h
#include <stdlib.h>
#define sc_included_stdlib_h
#endif
#ifndef sc_included_unistd_h
#include <unistd.h>
#define sc_included_unistd_h
#endif
#ifndef sc_included_sys_stat_h
#include <sys/stat.h>
#define sc_included_sys_stat_h
#endif
#ifndef sc_included_libgen_h
#include <libgen.h>
#define sc_included_libgen_h
#endif
#ifndef sc_included_errno_h
#include <errno.h>
#define sc_included_errno_h
#endif
#define file_exists_p(path) !(access(path, F_OK) == -1)
#define pointer_equal_p(a, b) (((b0 *)(a)) == ((b0 *)(b)))
#define free_and_set_zero(a)                                                   \
  free(a);                                                                     \
  a = 0
#define increment(a) a = (1 + a)
#define decrement(a) a = (a - 1)
/** set result to a new string with a trailing slash added, or the given string
  if it already has a trailing slash. returns 0 if result is the given string, 1
  if new memory could not be allocated, 2 if result is a new string */
b8 ensure_trailing_slash(b8 *a, b8 **result) {
  b32 a_len = strlen(a);
  if ((!a_len || (('/' == (*(a + (a_len - 1))))))) {
    (*result) = a;
    return (0);
  } else {
    char *new_a = malloc((2 + a_len));
    if (!new_a) {
      return (1);
    };
    memcpy(new_a, a, a_len);
    memcpy((new_a + a_len), "/", 1);
    (*(new_a + (1 + a_len))) = 0;
    (*result) = new_a;
    return (2);
  };
};
/** return a new string with the same contents as the given string. return 0 if
 * the memory allocation failed */
b8 *string_clone(b8 *a) {
  size_t a_size = (1 + strlen(a));
  b8 *result = malloc(a_size);
  if (result) {
    memcpy(result, a, a_size);
  };
  return (result);
};
/** like posix dirname, but never modifies its argument and always returns a new
 * string */
b8 *dirname_2(b8 *a) {
  b8 *path_copy = string_clone(a);
  return (dirname(path_copy));
};
/** return 1 if the path exists or has been successfully created */
boolean ensure_directory_structure(b8 *path, mode_t mkdir_mode) {
  if (file_exists_p(path)) {
    return (1);
  } else {
    b8 *path_dirname = dirname_2(path);
    boolean status = ensure_directory_structure(path_dirname, mkdir_mode);
    free(path_dirname);
    return ((status &&
             ((((EEXIST == errno)) || ((0 == mkdir(path, mkdir_mode)))))));
  };
};
/** always returns a new string */
b8 *string_append(b8 *a, b8 *b) {
  size_t a_length = strlen(a);
  size_t b_length = strlen(b);
  b8 *result = malloc((1 + a_length + b_length));
  if (result) {
    memcpy(result, a, a_length);
    memcpy((result + a_length), b, (1 + b_length));
  };
  return (result);
};
#endif
#ifndef sc_included_sph_dg_lmdb
#define sc_included_sph_dg_lmdb
#ifndef sc_included_string_h
#include <string.h>
#define sc_included_string_h
#endif
#define dg_mdb_declare_cursor(name) MDB_cursor *name = 0
#define dg_mdb_compare_get_mv_data(mdb_val) (*mdb_val).mv_data
#define dg_mdb_cursor_close_2(a, b)                                            \
  mdb_cursor_close(a);                                                         \
  mdb_cursor_close(b)
#define dg_mdb_cursor_close_3(a, b, c)                                         \
  dg_mdb_cursor_close_2(a, b);                                                 \
  mdb_cursor_close(c)
MDB_val val_null;
#define dg_mdb_cursor_get_x(cursor, val_1, val_2, cursor_operation)            \
  status_set_id(mdb_cursor_get(cursor, &val_1, &val_2, cursor_operation))
#define dg_mdb_cursor_next_dup_x(cursor, val_1, val_2)                         \
  dg_mdb_cursor_get_x(cursor, val_1, val_2, MDB_NEXT_DUP)
#define dg_mdb_cursor_next_nodup_x(cursor, val_1, val_2)                       \
  dg_mdb_cursor_get_x(cursor, val_1, val_2, MDB_NEXT_NODUP)
#define dg_mdb_cursor_del_x(cursor, flags)                                     \
  status_set_id(mdb_cursor_del(cursor, flags))
#define dg_mdb_declare_cursor_2(name_1, name_2)                                \
  dg_mdb_declare_cursor(name_1);                                               \
  dg_mdb_declare_cursor(name_2)
#define dg_mdb_declare_cursor_3(name_1, name_2, name_3)                        \
  dg_mdb_declare_cursor_2(name_1, name_2);                                     \
  dg_mdb_declare_cursor(name_3)
#define dg_mdb_initialise_cursor(txn, name)                                    \
  dg_mdb_status_require_x(mdb_cursor_open(txn, dbi_##name, &name))
#define dg_mdb_initialise_cursor_2(txn, name_1, name_2)                        \
  dg_mdb_initialise_cursor(txn, name_1);                                       \
  dg_mdb_initialise_cursor(txn, name_2)
#define dg_mdb_initialise_cursor_3(txn, name_1, name_2, name_3)                \
  dg_mdb_initialise_cursor_2(txn, name_1, name_2);                             \
  dg_mdb_initialise_cursor(txn, name_3)
#define dg_mdb_introduce_cursor(txn, name)                                     \
  dg_mdb_declare_cursor(name);                                                 \
  dg_mdb_initialise_cursor(txn, name)
#define dg_mdb_introduce_cursor_2(txn, name_1, name_2)                         \
  dg_mdb_declare_cursor_2(name_1, name_2);                                     \
  dg_mdb_initialise_cursor_2(txn, name_1, name_2)
#define dg_mdb_introduce_cursor_3(txn, name_1, name_2, name_3)                 \
  dg_mdb_declare_cursor_3(name_1, name_2, name_3);                             \
  dg_mdb_initialise_cursor_3(txn, name_1, name_2, name_3)
#define dg_mdb_val_to_id_at(a, index) dg_pointer_to_id(a.mv_data, index)
#define dg_mdb_val_to_id(a) dg_pointer_to_id(a.mv_data, 0)
#define dg_mdb_introduce_val(name, size)                                       \
  MDB_val name;                                                                \
  name.mv_size = size
#define dg_mdb_introduce_val_id dg_mdb_introduce_val(val_id, dg_size_octets_id)
#define dg_mdb_introduce_val_id_2                                              \
  dg_mdb_introduce_val(val_id_2, dg_size_octets_id)
#define dg_mdb_introduce_val_id_3                                              \
  dg_mdb_introduce_val(val_id_3, dg_size_octets_id)
#define dg_mdb_introduce_val_data MDB_val val_data
#define dg_mdb_introduce_val_data_2 MDB_val val_data_2
#define dg_mdb_reset_val_null val_null.mv_size = 0
#define dg_mdb_introduce_val_relation_data                                     \
  MDB_val val_relation_data;                                                   \
  val_relation_data.mv_size = dg_size_octets_relation_data
#define dg_mdb_introduce_val_relation_key                                      \
  MDB_val val_relation_key;                                                    \
  val_relation_key.mv_size = dg_size_octets_relation_key
#define dg_mdb_val_relation_data_to_id(a) dg_relation_data_to_id(a.mv_data)
#define dg_mdb_val_relation_data_to_ordinal(a)                                 \
  dg_relation_data_to_ordinal(a.mv_data)
static int dg_mdb_compare_id(const MDB_val *a, const MDB_val *b) {
  return (dg_id_compare(dg_pointer_to_id(dg_mdb_compare_get_mv_data(a), 0),
                        dg_pointer_to_id(dg_mdb_compare_get_mv_data(b), 0)));
};
static int dg_mdb_compare_relation_key(const MDB_val *a, const MDB_val *b) {
  return (((dg_mdb_val_to_id_at((*a), 0) < dg_mdb_val_to_id_at((*b), 0))
               ? -1
               : ((dg_mdb_val_to_id_at((*a), 0) > dg_mdb_val_to_id_at((*b), 0))
                      ? 1
                      : ((dg_mdb_val_to_id_at((*a), 1) <
                          dg_mdb_val_to_id_at((*b), 1))
                             ? -1
                             : (dg_mdb_val_to_id_at((*a), 1) >
                                dg_mdb_val_to_id_at((*b), 1))))));
};
static int dg_mdb_compare_relation_data(const MDB_val *a, const MDB_val *b) {
  return (((dg_mdb_val_relation_data_to_ordinal((*a)) <
            dg_mdb_val_relation_data_to_ordinal((*b)))
               ? -1
               : ((dg_mdb_val_relation_data_to_ordinal((*a)) >
                   dg_mdb_val_relation_data_to_ordinal((*b)))
                      ? 1
                      : ((dg_mdb_val_relation_data_to_id((*a)) <
                          dg_mdb_val_relation_data_to_id((*b)))
                             ? -1
                             : (dg_mdb_val_relation_data_to_id((*a)) >
                                dg_mdb_val_relation_data_to_id((*b)))))));
};
static int dg_mdb_compare_data(const MDB_val *a, const MDB_val *b) {
  ssize_t length_difference =
      (((ssize_t)((*a).mv_size)) - ((ssize_t)((*b).mv_size)));
  return ((length_difference
               ? ((length_difference < 0) ? -1 : 1)
               : memcmp((*a).mv_data, (*b).mv_data, (*a).mv_size)));
};
#define dg_mdb_cursor_each_key(cursor, val_key, val_value, body)               \
  dg_mdb_cursor_get_x(cursor, val_key, val_value, MDB_FIRST);                  \
  while (dg_mdb_status_success_p) {                                            \
    body;                                                                      \
    dg_mdb_cursor_next_nodup_x(cursor, val_key, val_value);                    \
  };                                                                           \
  dg_mdb_status_require_notfound
#define dg_mdb_cursor_set_first_x(cursor)                                      \
  dg_mdb_status_require_x(                                                     \
      mdb_cursor_get(cursor, &val_null, &val_null, MDB_FIRST))
#define dg_mdb_val_to_relation_key(a) ((dg_id_t *)(a.mv_data))
#endif
#ifndef sc_included_sph_dg_debug
#define sc_included_sph_dg_debug
b0 dg_debug_log_ids(dg_ids_t *a) {
  while (a) {
    debug_log("%lu", dg_ids_first(a));
    a = dg_ids_rest(a);
  };
};
b0 dg_debug_log_ids_set(imht_set_t a) {
  b32 index = 0;
  while ((index < a.size)) {
    debug_log("%lu", (*(a.content + index)));
    index = (1 + index);
  };
};
b0 dg_debug_display_relation_records(dg_relation_records_t *records) {
  dg_relation_record_t record;
  printf("relation records\n");
  while (records) {
    record = dg_relation_records_first(records);
    printf("  lcor %lu %lu %lu %lu\n", record.left, record.label,
           record.ordinal, record.right);
    records = dg_relation_records_rest(records);
  };
};
status_t dg_debug_count_all_btree_entries(MDB_txn *txn, b32 *result) {
  status_init;
  dg_statistics_t stat;
  status_require_x(dg_statistics(txn, &stat));
  (*result) =
      (stat.id_to_data.ms_entries + stat.data_intern_to_id.ms_entries +
       stat.data_extern_to_extern.ms_entries + stat.left_to_right.ms_entries +
       stat.right_to_left.ms_entries + stat.label_to_left.ms_entries);
exit:
  return (status);
};
status_t dg_debug_display_btree_counts(MDB_txn *txn) {
  status_init;
  dg_statistics_t stat;
  status_require_x(dg_statistics(txn, &stat));
  printf("btree entry count\n  id->data %d data-intern->id %d\n  "
         "data-extern->extern %d left->right %d\n  right->left %d label->left "
         "%d\n",
         stat.id_to_data.ms_entries, stat.data_intern_to_id.ms_entries,
         stat.data_extern_to_extern.ms_entries, stat.left_to_right.ms_entries,
         stat.right_to_left.ms_entries, stat.label_to_left.ms_entries);
exit:
  return (status);
};
#endif
#define dg_error_log(pattern, ...)                                             \
  fprintf(stderr, "%s:%d error: " pattern "\n", __func__, __LINE__, __VA_ARGS__)
#define reduce_count count = (count - 1)
#define stop_if_count_zero                                                     \
  if ((0 == count)) {                                                          \
    goto exit;                                                                 \
  }
#define optional_count(count) ((0 == count) ? UINT32_MAX : count)
status_t dg_ids_to_set(dg_ids_t *a, imht_set_t **result) {
  status_init;
  if (!imht_set_create(dg_ids_length(a), result)) {
    dg_status_set_id_goto(dg_status_id_memory);
  };
  while (a) {
    imht_set_add((*result), dg_ids_first(a));
    a = dg_ids_rest(a);
  };
exit:
  return (status);
};
dg_init_options_t dg_init_options_set_defaults(dg_init_options_t *a) {
  (*a).read_only_p = 0;
  (*a).maximum_size_octets = 17179869183;
  (*a).maximum_reader_count = 65535;
  (*a).env_open_flags = 0;
  (*a).filesystem_has_ordered_writes_p = 1;
  (*a).file_permissions = 384;
};
b32 dg_init_env_open_flags(dg_init_options_t options) {
  return (
      (options.env_open_flags
           ? options.env_open_flags
           : (MDB_NOSUBDIR | MDB_WRITEMAP |
              (options.read_only_p ? MDB_RDONLY : 0) |
              (options.filesystem_has_ordered_writes_p ? MDB_MAPASYNC : 0))));
};
status_t dg_init_root_path_prepare(b8 *a, b16 file_permissions) {
  status_init;
  if (!a) {
    dg_status_set_id_goto(dg_status_id_missing_argument_dg_root);
  };
  if (!ensure_directory_structure(dg_root, (73 | file_permissions))) {
    dg_status_set_id_goto(dg_status_id_path_not_accessible_dg_root);
  };
exit:
  return (status);
};
#define dg_select_ensure_offset(state, offset, reader)                         \
  if (offset) {                                                                \
    (*state).options = (dg_read_option_skip | (*state).options);               \
    status = reader(state, offset, 0);                                         \
    if (!dg_mdb_status_success_p) {                                            \
      dg_mdb_status_require_notfound;                                          \
    };                                                                         \
    (*state).options = (dg_read_option_skip ^ (*state).options);               \
  }
/** expects an allocated dg-statistics-t */
status_t dg_statistics(dg_txn_t *txn, dg_statistics_t *result) {
  status_init;
#define result_set(dbi_name)                                                   \
  dg_mdb_status_require_x(mdb_stat(txn, dbi_##dbi_name, &(*result).dbi_name))
  result_set(id_to_data);
  result_set(data_intern_to_id);
  result_set(data_extern_to_extern);
  result_set(left_to_right);
  result_set(right_to_left);
  result_set(label_to_left);
#undef result_set
exit:
  return (status);
};
#ifndef sc_included_sph_dg_relation
#define sc_included_sph_dg_relation
#define dg_relation_data_ordinal_set(relation_data, value)                     \
  (*(((dg_ordinal_t *)(relation_data)) + 0)) = value
#define dg_relation_data_id_set(relation_data, value)                          \
  (*(((dg_id_t *)((1 + ((dg_ordinal_t *)(relation_data))))) + 0)) = value
#define dg_define_relation_key(name) dg_id_t name[2] = {0, 0}
#define dg_define_relation_data(name)                                          \
  b8 name[(dg_size_octets_ordinal + dg_size_octets_id)];                       \
  memset(name, 0, (dg_size_octets_ordinal + dg_size_octets_id))
#define dg_define_relation_record(name) dg_relation_record_t name = {0, 0, 0, 0}
#define dg_relation_records_add_x(target, record, target_temp)                 \
  dg_pointer_allocation_set(target, dg_relation_records_add(target, record),   \
                            target_temp)
/** search data until the given id-right has been found */
status_t dg_mdb_left_to_right_seek_right(MDB_cursor *left_to_right,
                                         dg_id_t id_right) {
  status_init;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_relation_data;
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
each_data:
  if (dg_mdb_status_success_p) {
    if ((id_right == dg_mdb_val_relation_data_to_id(val_relation_data))) {
      return (status);
    } else {
      dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,
                               val_relation_data);
      goto each_data;
    };
  } else {
    dg_mdb_status_require_notfound;
  };
exit:
  return (status);
};
status_t dg_relation_ensure(dg_txn_t *txn, dg_ids_t *left, dg_ids_t *right,
                            dg_ids_t *label,
                            dg_relation_ordinal_generator_t ordinal_generator,
                            b0 *ordinal_generator_state) {
  status_init;
  dg_ids_t *right_pointer;
  dg_ids_t *label_pointer;
  dg_id_t id_left;
  dg_id_t id_right;
  dg_id_t id_label;
  dg_ordinal_t ordinal =
      ((!ordinal_generator && ordinal_generator_state)
           ? (ordinal = (*((dg_ordinal_t *)(ordinal_generator_state))))
           : 0);
  dg_define_relation_key(relation_key);
  dg_define_relation_data(relation_data);
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_relation_data;
  dg_mdb_introduce_cursor_3(txn, left_to_right, right_to_left, label_to_left);
  while (left) {
    id_left = dg_ids_first(left);
    label_pointer = label;
    while (label_pointer) {
      id_label = dg_ids_first(label_pointer);
      right_pointer = right;
      val_id_2.mv_data = &id_label;
      while (right_pointer) {
        id_right = dg_ids_first(right_pointer);
        (*(relation_key + 0)) = id_right;
        (*(relation_key + 1)) = id_label;
        val_relation_key.mv_data = relation_key;
        val_id.mv_data = &id_left;
        dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id,
                            MDB_GET_BOTH);
        if ((MDB_NOTFOUND == status.id)) {
          dg_mdb_status_require_x(
              mdb_cursor_put(right_to_left, &val_relation_key, &val_id, 0));
          dg_mdb_status_require_x(
              mdb_cursor_put(label_to_left, &val_id_2, &val_id, 0));
          (*(relation_key + 0)) = id_left;
          (*(relation_key + 1)) = id_label;
          if (ordinal_generator) {
            ordinal = (*ordinal_generator)(ordinal_generator_state);
          };
          dg_relation_data_ordinal_set(relation_data, ordinal);
          dg_relation_data_id_set(relation_data, id_right);
          val_relation_data.mv_data = relation_data;
          dg_mdb_status_require_x(mdb_cursor_put(
              left_to_right, &val_relation_key, &val_relation_data, 0));
        } else {
          if (!dg_mdb_status_success_p) {
            status_set_group_goto(dg_status_group_lmdb);
          };
        };
        right_pointer = dg_ids_rest(right_pointer);
      };
      label_pointer = dg_ids_rest(label_pointer);
    };
    left = dg_ids_rest(left);
  };
exit:
  dg_mdb_cursor_close_3(left_to_right, right_to_left, label_to_left);
  return (status);
};
#ifndef sc_included_sph_dg_relation_delete
#define sc_included_sph_dg_relation_delete
status_t dg_relation_internal_delete_label_to_left(MDB_cursor *label_to_left,
                                                   dg_id_t id_label,
                                                   dg_id_t id_left) {
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  status_init;
  val_id.mv_data = &id_label;
  val_id_2.mv_data = &id_left;
  dg_mdb_cursor_get_x(label_to_left, val_id, val_id_2, MDB_GET_BOTH);
  if (dg_mdb_status_success_p) {
    dg_mdb_cursor_del_x(label_to_left, 0);
    dg_mdb_status_require;
  } else {
    dg_mdb_status_require_notfound;
  };
  status_set_id(status_id_success);
exit:
  return (status);
};
status_t dg_relation_internal_delete_label_to_left_conditional(
    MDB_cursor *left_to_right, MDB_cursor *label_to_left, dg_id_t id_label,
    dg_id_t id_left) {
  status_init;
  dg_mdb_introduce_val_relation_key;
  dg_define_relation_key(relation_key);
  (*(relation_key + 0)) = id_left;
  (*(relation_key + 1)) = id_label;
  val_relation_key.mv_data = relation_key;
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_null, MDB_SET);
  if (status_id_is_p(MDB_NOTFOUND)) {
    return (dg_relation_internal_delete_label_to_left(label_to_left, id_label,
                                                      id_left));
  } else {
    dg_mdb_status_require;
  };
exit:
  return (status);
};
status_t dg_relation_internal_delete_right_to_left(MDB_cursor *right_to_left,
                                                   dg_id_t id_left,
                                                   dg_id_t id_right,
                                                   dg_id_t id_label) {
  status_init;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_id;
  dg_define_relation_key(relation_key);
  (*(relation_key + 0)) = id_right;
  (*(relation_key + 1)) = id_label;
  val_relation_key.mv_data = relation_key;
  val_id.mv_data = &id_left;
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_GET_BOTH);
  if (dg_mdb_status_success_p) {
    dg_mdb_cursor_del_x(right_to_left, 0);
    dg_mdb_status_require;
  } else {
    dg_mdb_status_require_notfound;
  };
exit:
  return (status);
};
#define dg_relation_internal_delete_0010()                                     \
  dg_id_t id_label;                                                            \
  dg_id_t id_left;                                                             \
  set_key_0010:                                                                \
  id_label = dg_ids_first(label);                                              \
  val_id.mv_data = &id_label;                                                  \
  dg_mdb_cursor_get_x(label_to_left, val_id, val_id_2, MDB_SET_KEY);           \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_0010;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  each_key_0010:                                                               \
  label = dg_ids_rest(label);                                                  \
  if (label) {                                                                 \
    goto set_key_0010;                                                         \
  } else {                                                                     \
    goto exit;                                                                 \
  };                                                                           \
  each_data_0010:                                                              \
  id_left = dg_mdb_val_to_id(val_id_2);                                        \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_KEY);                                            \
  if (dg_mdb_status_success_p) {                                               \
  each_data_2_0010:                                                            \
    status = dg_relation_internal_delete_right_to_left(                        \
        right_to_left, id_left,                                                \
        dg_mdb_val_relation_data_to_id(val_relation_data), id_label);          \
    dg_mdb_status_require_read;                                                \
    dg_mdb_cursor_del_x(left_to_right, 0);                                     \
    dg_mdb_status_require;                                                     \
    dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,                  \
                             val_relation_data);                               \
    if (dg_mdb_status_success_p) {                                             \
      goto each_data_2_0010;                                                   \
    } else {                                                                   \
      dg_mdb_status_require_notfound;                                          \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  dg_mdb_cursor_del_x(label_to_left, 0);                                       \
  dg_mdb_status_require;                                                       \
  dg_mdb_cursor_next_dup_x(label_to_left, val_id, val_id_2);                   \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_0010;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  goto each_key_0010
#define dg_relation_internal_delete_0110()                                     \
  dg_id_t id_right;                                                            \
  dg_id_t id_left;                                                             \
  dg_id_t id_label;                                                            \
  dg_ids_t *right_pointer = right;                                             \
  set_key_0110:                                                                \
  id_right = dg_ids_first(right_pointer);                                      \
  id_label = dg_ids_first(label);                                              \
  (*(relation_key + 0)) = id_right;                                            \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_SET_KEY);   \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_0110;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  each_key_0110:                                                               \
  right_pointer = dg_ids_rest(right_pointer);                                  \
  if (right_pointer) {                                                         \
    goto set_key_0110;                                                         \
  } else {                                                                     \
    label = dg_ids_rest(label);                                                \
    if (label) {                                                               \
      right_pointer = right;                                                   \
      goto set_key_0110;                                                       \
    } else {                                                                   \
      goto exit;                                                               \
    };                                                                         \
  };                                                                           \
  each_data_0110:                                                              \
  id_left = dg_mdb_val_to_id(val_id);                                          \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_KEY);                                            \
  if (dg_mdb_status_success_p) {                                               \
    status = dg_mdb_left_to_right_seek_right(left_to_right, id_right);         \
    if (dg_mdb_status_success_p) {                                             \
      dg_mdb_cursor_del_x(left_to_right, 0);                                   \
      dg_mdb_status_require;                                                   \
    } else {                                                                   \
      dg_mdb_status_require_notfound;                                          \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  status = dg_relation_internal_delete_label_to_left(label_to_left, id_label,  \
                                                     id_left);                 \
  dg_mdb_status_require_read;                                                  \
  dg_mdb_cursor_del_x(right_to_left, 0);                                       \
  dg_mdb_status_require;                                                       \
  dg_mdb_cursor_next_dup_x(right_to_left, val_relation_key, val_id);           \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_0110;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  goto each_key_0110
#define dg_relation_internal_delete_1010()                                     \
  dg_id_t id_label;                                                            \
  dg_id_t id_left;                                                             \
  dg_ids_t *label_pointer;                                                     \
  while (left) {                                                               \
    id_left = dg_ids_first(left);                                              \
    label_pointer = label;                                                     \
    while (label_pointer) {                                                    \
      id_label = dg_ids_first(label_pointer);                                  \
      (*(relation_key + 0)) = id_left;                                         \
      (*(relation_key + 1)) = id_label;                                        \
      val_relation_key.mv_data = relation_key;                                 \
      dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,  \
                          MDB_SET_KEY);                                        \
      if (dg_mdb_status_success_p) {                                           \
        do {                                                                   \
          dg_relation_internal_delete_right_to_left(                           \
              right_to_left, id_left,                                          \
              dg_mdb_val_relation_data_to_id(val_relation_data), id_label);    \
          dg_relation_internal_delete_label_to_left(label_to_left, id_label,   \
                                                    id_left);                  \
          dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,            \
                                   val_relation_data);                         \
        } while (dg_mdb_status_success_p);                                     \
        dg_mdb_status_require_notfound;                                        \
        (*(relation_key + 0)) = id_left;                                       \
        (*(relation_key + 1)) = id_label;                                      \
        val_relation_key.mv_data = relation_key;                               \
        dg_mdb_cursor_get_x(left_to_right, val_relation_key,                   \
                            val_relation_data, MDB_SET_KEY);                   \
        dg_mdb_status_require;                                                 \
        dg_mdb_cursor_del_x(left_to_right, MDB_NODUPDATA);                     \
        dg_mdb_status_require;                                                 \
      } else {                                                                 \
        dg_mdb_status_require_notfound;                                        \
      };                                                                       \
      label_pointer = dg_ids_rest(label_pointer);                              \
    };                                                                         \
    left = dg_ids_rest(left);                                                  \
  }
#define dg_relation_internal_delete_0100()                                     \
  dg_id_t id_left;                                                             \
  dg_id_t id_right;                                                            \
  dg_id_t id_label;                                                            \
  set_range_0100:                                                              \
  id_right = dg_ids_first(right);                                              \
  (*(relation_key + 0)) = id_right;                                            \
  (*(relation_key + 1)) = 0;                                                   \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_SET_RANGE); \
  if (dg_mdb_status_success_p) {                                               \
    if ((id_right == dg_mdb_val_to_id_at(val_relation_key, 0))) {              \
      if (dg_mdb_status_success_p) {                                           \
      };                                                                       \
      id_label = dg_mdb_val_to_id_at(val_relation_key, 1);                     \
      goto each_data_0100;                                                     \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  right = dg_ids_rest(right);                                                  \
  if (right) {                                                                 \
    goto set_range_0100;                                                       \
  } else {                                                                     \
    goto exit;                                                                 \
  };                                                                           \
  each_data_0100:                                                              \
  id_left = dg_mdb_val_to_id(val_id);                                          \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_KEY);                                            \
  if (dg_mdb_status_success_p) {                                               \
    status = dg_mdb_left_to_right_seek_right(left_to_right, id_right);         \
    if (dg_mdb_status_success_p) {                                             \
      dg_mdb_cursor_del_x(left_to_right, 0);                                   \
      dg_mdb_status_require;                                                   \
    } else {                                                                   \
      dg_mdb_status_require_notfound;                                          \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  status_require_x(dg_relation_internal_delete_label_to_left_conditional(      \
      left_to_right, label_to_left, id_label, id_left));                       \
  dg_mdb_cursor_del_x(right_to_left, 0);                                       \
  dg_mdb_status_require;                                                       \
  dg_mdb_cursor_next_dup_x(right_to_left, val_relation_key, val_id);           \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_0100;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  goto set_range_0100
#define dg_relation_internal_delete_1000()                                     \
  dg_id_t id_left;                                                             \
  dg_id_t id_label;                                                            \
  dg_id_t id_right;                                                            \
  set_range_1000:                                                              \
  id_left = dg_ids_first(left);                                                \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = 0;                                                   \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_RANGE);                                          \
  each_key_1000:                                                               \
  if (dg_mdb_status_success_p) {                                               \
    if ((id_left == dg_mdb_val_to_id_at(val_relation_key, 0))) {               \
      id_label = dg_mdb_val_to_id_at(val_relation_key, 1);                     \
      goto each_data_1000;                                                     \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  left = dg_ids_rest(left);                                                    \
  if (left) {                                                                  \
    goto set_range_1000;                                                       \
  } else {                                                                     \
    goto exit;                                                                 \
  };                                                                           \
  each_data_1000:                                                              \
  id_right = dg_mdb_val_relation_data_to_id(val_relation_data);                \
  dg_relation_internal_delete_right_to_left(right_to_left, id_left, id_right,  \
                                            id_label);                         \
  dg_relation_internal_delete_label_to_left(label_to_left, id_label, id_left); \
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,                    \
                           val_relation_data);                                 \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_1000;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_KEY);                                            \
  dg_mdb_status_require;                                                       \
  dg_mdb_cursor_del_x(left_to_right, MDB_NODUPDATA);                           \
  dg_mdb_status_require;                                                       \
  dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,                  \
                             val_relation_data);                               \
  goto each_key_1000
#define dg_relation_internal_delete_1100()                                     \
  dg_id_t id_left;                                                             \
  dg_id_t id_right;                                                            \
  dg_id_t id_label;                                                            \
  imht_set_t *right_set;                                                       \
  status_require_x(dg_ids_to_set(right, &right_set));                          \
  (*(relation_key + 1)) = 0;                                                   \
  set_range_1100:                                                              \
  id_left = dg_ids_first(left);                                                \
  (*(relation_key + 0)) = id_left;                                             \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_RANGE);                                          \
  each_key_1100:                                                               \
  if (dg_mdb_status_success_p) {                                               \
    if ((id_left == dg_mdb_val_to_id_at(val_relation_key, 0))) {               \
      id_label = dg_mdb_val_to_id_at(val_relation_key, 1);                     \
      goto each_data_1100;                                                     \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  left = dg_ids_rest(left);                                                    \
  if (left) {                                                                  \
    (*(relation_key + 1)) = 0;                                                 \
    goto set_range_1100;                                                       \
  } else {                                                                     \
    goto exit;                                                                 \
  };                                                                           \
  each_data_1100:                                                              \
  id_right = dg_mdb_val_relation_data_to_id(val_relation_data);                \
  if (imht_set_contains_p(right_set, id_right)) {                              \
    dg_relation_internal_delete_right_to_left(right_to_left, id_left,          \
                                              id_right, id_label);             \
    dg_mdb_cursor_del_x(left_to_right, 0);                                     \
    dg_mdb_status_require;                                                     \
  };                                                                           \
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,                    \
                           val_relation_data);                                 \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_1100;                                                       \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  status_require_x(dg_relation_internal_delete_label_to_left_conditional(      \
      left_to_right, label_to_left, id_label, id_left));                       \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_KEY);                                            \
  if (dg_mdb_status_success_p) {                                               \
    dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,                \
                               val_relation_data);                             \
    goto each_key_1100;                                                        \
  } else {                                                                     \
    if (status_id_is_p(MDB_NOTFOUND)) {                                        \
      goto set_range_1100;                                                     \
    } else {                                                                   \
      status_set_group_goto(dg_status_group_lmdb);                             \
    };                                                                         \
  }
#define dg_relation_internal_delete_1110()                                     \
  dg_id_t id_left;                                                             \
  dg_id_t id_label;                                                            \
  imht_set_t *right_set;                                                       \
  dg_id_t id_right;                                                            \
  dg_ids_t *label_first = label;                                               \
  status_require_x(dg_ids_to_set(right, &right_set));                          \
  while (left) {                                                               \
    id_left = dg_ids_first(left);                                              \
    while (label) {                                                            \
      id_label = dg_ids_first(label);                                          \
      (*(relation_key + 0)) = id_left;                                         \
      (*(relation_key + 1)) = id_label;                                        \
      val_relation_key.mv_data = relation_key;                                 \
      dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,  \
                          MDB_SET_KEY);                                        \
      while (dg_mdb_status_success_p) {                                        \
        if (imht_set_contains_p(right_set, dg_mdb_val_relation_data_to_id(     \
                                               val_relation_data))) {          \
          id_right = dg_mdb_val_relation_data_to_id(val_relation_data);        \
          dg_relation_internal_delete_right_to_left(right_to_left, id_left,    \
                                                    id_right, id_label);       \
          dg_mdb_cursor_del_x(left_to_right, 0);                               \
          dg_mdb_status_require;                                               \
        };                                                                     \
        dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,              \
                                 val_relation_data);                           \
      };                                                                       \
      dg_relation_internal_delete_label_to_left_conditional(                   \
          left_to_right, label_to_left, id_label, id_left);                    \
      label = dg_ids_rest(label);                                              \
    };                                                                         \
    label = label_first;                                                       \
    left = dg_ids_rest(left);                                                  \
  }
#define dg_relation_internal_delete_get_ordinal_data(ordinal)                  \
  dg_ordinal_t ordinal_min = (*ordinal).min;                                   \
  dg_ordinal_t ordinal_max = (*ordinal).max
#define dg_relation_internal_delete_1001_1101()                                \
  dg_id_t id_left;                                                             \
  dg_id_t id_right;                                                            \
  dg_id_t id_label;                                                            \
  imht_set_t *right_set;                                                       \
  dg_relation_internal_delete_get_ordinal_data(ordinal);                       \
  (*(relation_data + 0)) = ordinal_min;                                        \
  (*(relation_key + 1)) = 0;                                                   \
  if (right) {                                                                 \
    status_require_x(dg_ids_to_set(right, &right_set));                        \
  };                                                                           \
  set_range_1001_1101:                                                         \
  id_left = dg_ids_first(left);                                                \
  (*(relation_key + 0)) = id_left;                                             \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_RANGE);                                          \
  each_key_1001_1101:                                                          \
  if (dg_mdb_status_success_p) {                                               \
    if ((id_left == dg_mdb_val_to_id_at(val_relation_key, 0))) {               \
      val_relation_data.mv_data = relation_data;                               \
      dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,  \
                          MDB_GET_BOTH_RANGE);                                 \
      if (dg_mdb_status_success_p) {                                           \
        id_label = dg_mdb_val_to_id_at(val_relation_key, 1);                   \
        goto each_data_1001_1101;                                              \
      } else {                                                                 \
        dg_mdb_status_require_notfound;                                        \
      };                                                                       \
      dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,              \
                                 val_relation_data);                           \
      goto each_key_1001_1101;                                                 \
    };                                                                         \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  left = dg_ids_rest(left);                                                    \
  if (left) {                                                                  \
    (*(relation_key + 1)) = 0;                                                 \
    goto set_range_1001_1101;                                                  \
  } else {                                                                     \
    goto exit;                                                                 \
  };                                                                           \
  each_data_1001_1101:                                                         \
  if ((!ordinal_max || ((dg_mdb_val_relation_data_to_ordinal(                  \
                             val_relation_data) <= ordinal_max)))) {           \
    id_right = dg_mdb_val_relation_data_to_id(val_relation_data);              \
    if ((!right || imht_set_contains_p(right_set, id_right))) {                \
      status = dg_relation_internal_delete_right_to_left(                      \
          right_to_left, id_left, id_right, id_label);                         \
      dg_mdb_status_require_read;                                              \
      dg_mdb_cursor_del_x(left_to_right, 0);                                   \
      dg_mdb_status_require;                                                   \
    };                                                                         \
  } else {                                                                     \
    goto next_label_1001_1101;                                                 \
  };                                                                           \
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,                    \
                           val_relation_data);                                 \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_1001_1101;                                                  \
  } else {                                                                     \
    dg_mdb_status_require_notfound;                                            \
  };                                                                           \
  status_require_x(dg_relation_internal_delete_label_to_left_conditional(      \
      left_to_right, label_to_left, id_label, id_left));                       \
  next_label_1001_1101:                                                        \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_SET_KEY);                                            \
  if (dg_mdb_status_success_p) {                                               \
    dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,                \
                               val_relation_data);                             \
    goto each_key_1001_1101;                                                   \
  } else {                                                                     \
    if (status_id_is_p(MDB_NOTFOUND)) {                                        \
      goto set_range_1001_1101;                                                \
    } else {                                                                   \
      status_set_group_goto(dg_status_group_lmdb);                             \
    };                                                                         \
  }
#define dg_relation_internal_delete_1011_1111()                                \
  dg_id_t id_left;                                                             \
  dg_id_t id_label;                                                            \
  imht_set_t *right_set;                                                       \
  dg_id_t id_right;                                                            \
  dg_ids_t *left_pointer = left;                                               \
  if (right) {                                                                 \
    status_require_x(dg_ids_to_set(right, &right_set));                        \
  };                                                                           \
  dg_relation_internal_delete_get_ordinal_data(ordinal);                       \
  (*(relation_data + 0)) = ordinal_min;                                        \
  id_label = dg_ids_first(label);                                              \
  set_key_1011_1111:                                                           \
  id_left = dg_ids_first(left_pointer);                                        \
  (*(relation_key + 0)) = id_left;                                             \
  (*(relation_key + 1)) = id_label;                                            \
  val_relation_key.mv_data = relation_key;                                     \
  val_relation_data.mv_data = relation_data;                                   \
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,      \
                      MDB_GET_BOTH_RANGE);                                     \
  if (dg_mdb_status_success_p) {                                               \
    goto each_data_1011_1111;                                                  \
  } else {                                                                     \
  each_key_1011_1111:                                                          \
    left_pointer = dg_ids_rest(left_pointer);                                  \
    if (left_pointer) {                                                        \
      goto set_key_1011_1111;                                                  \
    } else {                                                                   \
      label = dg_ids_rest(label);                                              \
      if (label) {                                                             \
        left_pointer = left;                                                   \
        id_label = dg_ids_first(label);                                        \
        goto set_key_1011_1111;                                                \
      } else {                                                                 \
        goto exit;                                                             \
      };                                                                       \
    };                                                                         \
  };                                                                           \
  each_data_1011_1111:                                                         \
  if ((!ordinal_max || ((dg_mdb_val_relation_data_to_ordinal(                  \
                             val_relation_data) <= ordinal_max)))) {           \
    if ((!right ||                                                             \
         imht_set_contains_p(                                                  \
             right_set, dg_mdb_val_relation_data_to_id(val_relation_data)))) { \
      id_right = dg_mdb_val_relation_data_to_id(val_relation_data);            \
      status = dg_relation_internal_delete_right_to_left(                      \
          right_to_left, id_left, id_right, id_label);                         \
      dg_mdb_status_require_read;                                              \
      dg_mdb_cursor_del_x(left_to_right, 0);                                   \
      dg_mdb_status_require;                                                   \
    };                                                                         \
    dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,                  \
                             val_relation_data);                               \
    if (dg_mdb_status_success_p) {                                             \
      goto each_data_1011_1111;                                                \
    } else {                                                                   \
      dg_mdb_status_require_notfound;                                          \
    };                                                                         \
  };                                                                           \
  status_require_x(dg_relation_internal_delete_label_to_left_conditional(      \
      left_to_right, label_to_left, id_label, id_left));                       \
  goto each_key_1011_1111
/** dg-relation-internal-delete does not open/close cursors.
  1111 / left-right-label-ordinal.
  tip: the code is nice to debug if variable state is displayed near the
    beginning of goto labels, before cursor operations.
    example display on stdout: (debug-log "each-key-1100 %lu %lu" id-left
  id-right) */
status_t dg_relation_internal_delete(dg_txn_t *txn, dg_ids_t *left,
                                     dg_ids_t *right, dg_ids_t *label,
                                     dg_ordinal_match_data_t *ordinal,
                                     MDB_cursor *left_to_right,
                                     MDB_cursor *right_to_left,
                                     MDB_cursor *label_to_left) {
  status_init;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_relation_data;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  dg_define_relation_key(relation_key);
  dg_define_relation_data(relation_data);
  if (left) {
    if (ordinal) {
      if (label) {
        dg_relation_internal_delete_1011_1111();
      } else {
        dg_relation_internal_delete_1001_1101();
      };
    } else {
      if (label) {
        if (right) {
          dg_relation_internal_delete_1110();
        } else {
          dg_relation_internal_delete_1010();
        };
      } else {
        if (right) {
          dg_relation_internal_delete_1100();
        } else {
          dg_relation_internal_delete_1000();
        };
      };
    };
  } else {
    if (right) {
      if (label) {
        dg_relation_internal_delete_0110();
      } else {
        dg_relation_internal_delete_0100();
      };
    } else {
      if (label) {
        dg_relation_internal_delete_0010();
      } else {
        dg_status_set_id_goto(dg_status_id_not_implemented);
      };
    };
  };
exit:
  dg_status_success_if_mdb_notfound;
  return (status);
};
status_t dg_relation_delete(dg_txn_t *txn, dg_ids_t *left, dg_ids_t *right,
                            dg_ids_t *label, dg_ordinal_match_data_t *ordinal) {
  status_init;
  dg_mdb_introduce_cursor_3(txn, left_to_right, right_to_left, label_to_left);
  status =
      dg_relation_internal_delete(txn, left, right, label, ordinal,
                                  left_to_right, right_to_left, label_to_left);
exit:
  dg_mdb_cursor_close_3(left_to_right, right_to_left, label_to_left);
  return (status);
};
#endif
#ifndef sc_included_sph_dg_relation_read
#define sc_included_sph_dg_relation_read
#define no_more_data_exit                                                      \
  status_set_both_goto(dg_status_group_dg, dg_status_id_no_more_data)
#define dg_relation_select_cursor_initialise(name, state, state_field_name)    \
  dg_mdb_initialise_cursor(txn, name);                                         \
  status_set_id(mdb_cursor_get(name, &val_null, &val_null, MDB_FIRST));        \
  if (!dg_mdb_status_success_p) {                                              \
    dg_mdb_status_require_notfound;                                            \
    status_set_both_goto(dg_status_group_dg, dg_status_id_no_more_data);       \
  };                                                                           \
  (*state).state_field_name = name
#define dg_relation_select_initialise_set(name, state)                         \
  imht_set_t *name##_set;                                                      \
  status_require_x(dg_ids_to_set(name, &name##_set));                          \
  (*state).name = name##_set;                                                  \
  (*state).options = (dg_read_option_is_set_##name | (*state).options)
#define dg_relation_reader_header(state)                                       \
  status_init;                                                                 \
  dg_mdb_introduce_val_relation_key;                                           \
  dg_define_relation_key(relation_key);                                        \
  dg_define_relation_record(record);                                           \
  dg_relation_records_t *result_temp;                                          \
  boolean skip_p = (dg_read_option_skip & (*state).options)
#define dg_relation_reader_header_0000(state)                                  \
  status_init;                                                                 \
  dg_mdb_introduce_val_relation_key;                                           \
  dg_define_relation_record(record);                                           \
  dg_relation_records_t *result_temp;                                          \
  boolean skip_p = (dg_read_option_skip & (*state).options)
#define dg_relation_reader_get_ordinal_data(state)                             \
  dg_ordinal_t ordinal_min = (*(*state).ordinal).min;                          \
  dg_ordinal_t ordinal_max = (*(*state).ordinal).max
status_t dg_relation_read_1000(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_relation_data;
  MDB_cursor *left_to_right = (*state).cursor;
  dg_ids_t *left = (*state).left;
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
  dg_mdb_status_require;
  (*(relation_key + 0)) = dg_ids_first(left);
  if (dg_id_equal_p(dg_mdb_val_to_id_at(val_relation_key, 0),
                    (*(relation_key + 0)))) {
    goto each_data;
  } else {
  set_range:
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                        MDB_SET_RANGE);
  each_key:
    if (dg_mdb_status_success_p) {
      if (dg_id_equal_p(dg_mdb_val_to_id_at(val_relation_key, 0),
                        (*(relation_key + 0)))) {
        goto each_data;
      };
    } else {
      dg_mdb_status_require_notfound;
    };
    left = dg_ids_rest(left);
    if (left) {
      (*(relation_key + 0)) = dg_ids_first(left);
      goto set_range;
    } else {
      no_more_data_exit;
    };
  };
each_data:
  stop_if_count_zero;
  if (!skip_p) {
    record.left = dg_mdb_val_to_id_at(val_relation_key, 0);
    record.right = dg_mdb_val_relation_data_to_id(val_relation_data);
    record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
    record.ordinal = dg_mdb_val_relation_data_to_ordinal(val_relation_data);
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key, val_relation_data);
  if (dg_mdb_status_success_p) {
    goto each_data;
  } else {
    dg_mdb_status_require_notfound;
  };
  dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,
                             val_relation_data);
  goto each_key;
exit:
  (*state).status = status;
  (*state).left = left;
  return (status);
};
status_t dg_relation_read_1010(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_relation_data;
  MDB_cursor *left_to_right = (*state).cursor;
  dg_ids_t *left = (*state).left;
  dg_ids_t *left_first = (*state).left_first;
  dg_ids_t *label = (*state).label;
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
  dg_mdb_status_require;
  (*(relation_key + 0)) = dg_ids_first(left);
  (*(relation_key + 1)) = dg_ids_first(label);
  if (dg_relation_key_equal_p(relation_key,
                              dg_mdb_val_to_relation_key(val_relation_key))) {
    goto each_data;
  } else {
  set_key:
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                        MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      goto each_data;
    } else {
      dg_mdb_status_require_notfound;
    };
  next_key:
    left = dg_ids_rest(left);
    if (left) {
      (*(relation_key + 0)) = dg_ids_first(left);
      goto set_key;
    } else {
      label = dg_ids_rest(label);
      if (label) {
        left = left_first;
        (*(relation_key + 0)) = dg_ids_first(left);
        (*(relation_key + 1)) = dg_ids_first(label);
        goto set_key;
      } else {
        no_more_data_exit;
      };
    };
  };
each_data:
  stop_if_count_zero;
  if (!skip_p) {
    record.left = dg_mdb_val_to_id_at(val_relation_key, 0);
    record.right = dg_mdb_val_relation_data_to_id(val_relation_data);
    record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
    record.ordinal = dg_mdb_val_relation_data_to_ordinal(val_relation_data);
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key, val_relation_data);
  if (dg_mdb_status_success_p) {
    goto each_data;
  } else {
    goto next_key;
  };
exit:
  (*state).status = status;
  (*state).left = left;
  (*state).label = label;
  return (status);
};
status_t dg_relation_read_1100(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_id;
  MDB_cursor *right_to_left = (*state).cursor;
  dg_ids_t *left = (*state).left;
  dg_ids_t *left_first = (*state).left_first;
  dg_ids_t *right = (*state).right;
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_GET_CURRENT);
  dg_mdb_status_require;
  (*(relation_key + 0)) = dg_ids_first(right);
  if (dg_id_equal_p(dg_mdb_val_to_id_at(val_relation_key, 0),
                    (*(relation_key + 0)))) {
    goto each_left;
  } else {
  set_range:
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_SET_RANGE);
  each_right:
    if (dg_mdb_status_success_p) {
      if (dg_id_equal_p(dg_mdb_val_to_id_at(val_relation_key, 0),
                        (*(relation_key + 0)))) {
        goto each_left;
      };
    } else {
      dg_mdb_status_require_notfound;
    };
    right = dg_ids_rest(right);
    if (right) {
      (*(relation_key + 0)) = dg_ids_first(right);
    } else {
      no_more_data_exit;
    };
    goto set_range;
  };
each_left:
  stop_if_count_zero;
  val_id.mv_data = dg_ids_first_address(left);
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_GET_BOTH);
  if (dg_mdb_status_success_p) {
    if (!skip_p) {
      record.left = dg_mdb_val_to_id(val_id);
      record.right = dg_mdb_val_to_id_at(val_relation_key, 0);
      record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
      dg_relation_records_add_x((*result), record, result_temp);
      reduce_count;
    };
  } else {
    dg_mdb_status_require_notfound;
  };
  left = dg_ids_rest(left);
  if (left) {
    goto each_left;
  } else {
    left = left_first;
  };
  dg_mdb_cursor_next_nodup_x(right_to_left, val_relation_key, val_id);
  goto each_right;
exit:
  (*state).status = status;
  (*state).left = left;
  (*state).right = right;
  return (status);
};
status_t dg_relation_read_1110(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_id;
  MDB_cursor *right_to_left = (*state).cursor;
  dg_ids_t *left = (*state).left;
  dg_ids_t *left_first = (*state).left_first;
  dg_ids_t *right = (*state).right;
  dg_ids_t *right_first = (*state).right_first;
  dg_ids_t *label = (*state).label;
  dg_id_t id_left;
  (*(relation_key + 1)) = dg_ids_first(label);
  id_left = dg_ids_first(left);
  (*(relation_key + 0)) = dg_ids_first(right);
set_cursor:
  val_relation_key.mv_data = relation_key;
  val_id.mv_data = &id_left;
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_GET_BOTH);
  if (dg_mdb_status_success_p) {
    goto match;
  } else {
    dg_mdb_status_require_notfound;
  };
next_query:
  right = dg_ids_rest(right);
  if (right) {
    stop_if_count_zero;
    (*(relation_key + 0)) = dg_ids_first(right);
    goto set_cursor;
  } else {
    right = right_first;
    (*(relation_key + 0)) = dg_ids_first(right);
    left = dg_ids_rest(left);
    if (left) {
      stop_if_count_zero;
      id_left = dg_ids_first(left);
      goto set_cursor;
    } else {
      left = left_first;
      id_left = dg_ids_first(left);
      label = dg_ids_rest(label);
      if (label) {
        stop_if_count_zero;
        (*(relation_key + 1)) = dg_ids_first(label);
        goto set_cursor;
      } else {
        no_more_data_exit;
      };
    };
  };
match:
  if (!skip_p) {
    record.left = dg_mdb_val_to_id(val_id);
    record.right = dg_mdb_val_to_id_at(val_relation_key, 0);
    record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  goto next_query;
exit:
  (*state).status = status;
  (*state).left = left;
  (*state).right = right;
  (*state).label = label;
  return (status);
};
status_t dg_relation_read_1001_1101(dg_relation_read_state_t *state, b32 count,
                                    dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_relation_data;
  MDB_cursor *left_to_right = (*state).cursor;
  dg_ids_t *left = (*state).left;
  imht_set_t *right = (*state).right;
  dg_relation_reader_get_ordinal_data(state);
  dg_define_relation_data(relation_data);
  dg_relation_data_set_ordinal(relation_data, ordinal_min);
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
  dg_mdb_status_require;
  if (left) {
    (*(relation_key + 0)) = dg_ids_first(left);
  } else {
    no_more_data_exit;
  };
  if ((dg_id_equal_p(dg_mdb_val_to_id_at(val_relation_key, 0),
                     (*(relation_key + 0))) &&
       ((!ordinal_min || ((dg_mdb_val_relation_data_to_ordinal(
                               val_relation_data) >= ordinal_min)))) &&
       ((!ordinal_max || ((dg_mdb_val_relation_data_to_ordinal(
                               val_relation_data) <= ordinal_max)))))) {
    goto each_data;
  } else {
  each_left:
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                        MDB_SET_RANGE);
  each_key:
    if (dg_mdb_status_success_p) {
      if (dg_id_equal_p(dg_mdb_val_to_id_at(val_relation_key, 0),
                        (*(relation_key + 0)))) {
        val_relation_data.mv_data = relation_data;
        dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                            MDB_GET_BOTH_RANGE);
        if (dg_mdb_status_success_p) {
          goto each_data;
        } else {
          dg_mdb_status_require_notfound;
        };
        dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,
                                   val_relation_data);
        goto each_key;
      };
    } else {
      dg_mdb_status_require_notfound;
    };
    left = dg_ids_rest(left);
    if (left) {
      (*(relation_key + 0)) = dg_ids_first(left);
    } else {
      no_more_data_exit;
    };
    goto each_left;
  };
each_data:
  stop_if_count_zero;
  if ((!ordinal_max || ((dg_mdb_val_relation_data_to_ordinal(
                             val_relation_data) <= ordinal_max)))) {
    if ((!right || imht_set_contains_p(right, dg_mdb_val_relation_data_to_id(
                                                  val_relation_data)))) {
      if (!skip_p) {
        record.left = dg_mdb_val_to_id_at(val_relation_key, 0);
        record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
        record.ordinal = dg_mdb_val_relation_data_to_ordinal(val_relation_data);
        record.right = dg_mdb_val_relation_data_to_id(val_relation_data);
        dg_relation_records_add_x((*result), record, result_temp);
      };
      reduce_count;
    };
    dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,
                             val_relation_data);
    if (dg_mdb_status_success_p) {
      goto each_data;
    } else {
      dg_mdb_status_require_notfound;
    };
  };
  dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,
                             val_relation_data);
  goto each_key;
exit:
  (*state).status = status;
  (*state).left = left;
  return (status);
};
status_t dg_relation_read_1011_1111(dg_relation_read_state_t *state, b32 count,
                                    dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_relation_data;
  dg_define_relation_data(relation_data);
  MDB_cursor *left_to_right = (*state).cursor;
  dg_ids_t *left = (*state).left;
  dg_ids_t *left_first = (*state).left_first;
  dg_ids_t *label = (*state).label;
  imht_set_t *right = (*state).right;
  dg_relation_reader_get_ordinal_data(state);
  dg_relation_data_set_ordinal(relation_data, ordinal_min);
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
  dg_mdb_status_require;
  (*(relation_key + 0)) = dg_ids_first(left);
  (*(relation_key + 1)) = dg_ids_first(label);
  if (dg_relation_key_equal_p(relation_key,
                              dg_mdb_val_to_relation_key(val_relation_key))) {
    goto each_data;
  } else {
  set_key:
    val_relation_key.mv_data = relation_key;
    val_relation_data.mv_data = relation_data;
    dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                        MDB_GET_BOTH_RANGE);
    if (dg_mdb_status_success_p) {
      goto each_data;
    } else {
      dg_mdb_status_require_notfound;
    each_key:
      left = dg_ids_rest(left);
      if (left) {
        (*(relation_key + 0)) = dg_ids_first(left);
      } else {
        label = dg_ids_rest(label);
        if (label) {
          (*(relation_key + 1)) = dg_ids_first(label);
          left = left_first;
          (*(relation_key + 0)) = dg_ids_first(left);
        } else {
          no_more_data_exit;
        };
      };
      goto set_key;
    };
  };
each_data:
  stop_if_count_zero;
  if ((!ordinal_max || ((dg_mdb_val_relation_data_to_ordinal(
                             val_relation_data) <= ordinal_max)))) {
    if ((!right || imht_set_contains_p(right, dg_mdb_val_relation_data_to_id(
                                                  val_relation_data)))) {
      if (!skip_p) {
        record.left = dg_mdb_val_to_id_at(val_relation_key, 0);
        record.right = dg_mdb_val_relation_data_to_id(val_relation_data);
        record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
        record.ordinal = dg_mdb_val_relation_data_to_ordinal(val_relation_data);
        dg_relation_records_add_x((*result), record, result_temp);
      };
      reduce_count;
    };
    dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,
                             val_relation_data);
    if (dg_mdb_status_success_p) {
      goto each_data;
    } else {
      goto each_key;
    };
  } else {
    goto each_key;
  };
exit:
  (*state).status = status;
  (*state).left = left;
  (*state).label = label;
  return (status);
};
status_t dg_relation_read_0010(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  dg_mdb_introduce_val_relation_data;
  MDB_cursor *label_to_left = (*state).cursor;
  MDB_cursor *left_to_right = (*state).cursor_2;
  dg_ids_t *label = (*state).label;
  dg_id_t id_left;
  dg_id_t id_label;
  dg_mdb_cursor_get_x(label_to_left, val_id, val_id_2, MDB_GET_CURRENT);
  dg_mdb_status_require;
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
  dg_mdb_status_require;
  if (label) {
    id_label = dg_ids_first(label);
  } else {
    no_more_data_exit;
  };
  if (dg_id_equal_p(id_label, dg_mdb_val_to_id(val_id))) {
    (*(relation_key + 1)) = id_label;
    goto each_label_data;
  } else {
  set_label_key:
    val_id.mv_data = &id_label;
    dg_mdb_cursor_get_x(label_to_left, val_id, val_id_2, MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      (*(relation_key + 1)) = id_label;
      goto each_label_data;
    } else {
      dg_mdb_status_require_notfound;
      label = dg_ids_rest(label);
      if (label) {
        id_label = dg_ids_first(label);
      } else {
        no_more_data_exit;
      };
      goto set_label_key;
    };
  };
each_label_data:
  id_left = dg_mdb_val_to_id(val_id_2);
  if (dg_id_equal_p(id_left, dg_mdb_val_to_id_at(val_relation_key, 0))) {
    goto each_left_data;
  } else {
    (*(relation_key + 0)) = id_left;
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                        MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      goto each_left_data;
    } else {
      goto exit;
    };
  };
each_left_data:
  stop_if_count_zero;
  if (!skip_p) {
    record.left = id_left;
    record.right = dg_mdb_val_relation_data_to_id(val_relation_data);
    record.label = id_label;
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key, val_relation_data);
  if (dg_mdb_status_success_p) {
    goto each_left_data;
  } else {
    dg_mdb_status_require_notfound;
  };
  dg_mdb_cursor_next_dup_x(label_to_left, val_id, val_id_2);
  if (dg_mdb_status_success_p) {
    goto each_label_data;
  } else {
    label = dg_ids_rest(label);
    if (label) {
      id_label = dg_ids_first(label);
    } else {
      no_more_data_exit;
    };
    goto set_label_key;
  };
exit:
  (*state).status = status;
  (*state).label = label;
  return (status);
};
status_t dg_relation_read_0110(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_id;
  MDB_cursor *right_to_left = (*state).cursor;
  dg_ids_t *label = (*state).label;
  dg_ids_t *right = (*state).right;
  dg_ids_t *right_first = (*state).right_first;
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_GET_CURRENT);
  dg_mdb_status_require;
  (*(relation_key + 1)) = dg_ids_first(label);
  (*(relation_key + 0)) = dg_ids_first(right);
  if (dg_relation_key_equal_p(relation_key,
                              dg_mdb_val_to_relation_key(val_relation_key))) {
    goto each_data;
  } else {
  set_key:
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      goto each_data;
    } else {
    each_key:
      dg_mdb_status_require_notfound;
      right = dg_ids_rest(right);
      if (right) {
        (*(relation_key + 0)) = dg_ids_first(right);
      } else {
        label = dg_ids_rest(label);
        if (label) {
          (*(relation_key + 1)) = dg_ids_first(label);
          right = right_first;
          (*(relation_key + 0)) = dg_ids_first(right);
        } else {
          no_more_data_exit;
        };
      };
      goto set_key;
    };
  };
each_data:
  stop_if_count_zero;
  if (!skip_p) {
    record.left = dg_mdb_val_to_id(val_id);
    record.right = (*(relation_key + 0));
    record.label = (*(relation_key + 1));
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  dg_mdb_cursor_next_dup_x(right_to_left, val_relation_key, val_id);
  if (dg_mdb_status_success_p) {
    goto each_data;
  } else {
    goto each_key;
  };
exit:
  (*state).status = status;
  (*state).right = right;
  (*state).label = label;
  return (status);
};
status_t dg_relation_read_0100(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header(state);
  dg_mdb_introduce_val_id;
  MDB_cursor *right_to_left = (*state).cursor;
  dg_ids_t *right = (*state).right;
  dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_GET_CURRENT);
  dg_mdb_status_require;
  (*(relation_key + 0)) = dg_ids_first(right);
  if (dg_id_equal_p((*(relation_key + 0)),
                    dg_mdb_val_to_id_at(val_relation_key, 0))) {
    goto each_key;
  } else {
  set_range:
    val_relation_key.mv_data = relation_key;
    dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_SET_RANGE);
    if (dg_mdb_status_success_p) {
      if (dg_id_equal_p((*(relation_key + 0)),
                        dg_mdb_val_to_id_at(val_relation_key, 0))) {
        goto each_key;
      };
    } else {
      dg_mdb_status_require_notfound;
    };
    right = dg_ids_rest(right);
    if (right) {
      (*(relation_key + 0)) = dg_ids_first(right);
    } else {
      no_more_data_exit;
    };
    goto set_range;
  };
each_key:
each_data:
  stop_if_count_zero;
  if (!skip_p) {
    record.left = dg_mdb_val_to_id(val_id);
    record.right = dg_mdb_val_to_id_at(val_relation_key, 0);
    record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  dg_mdb_cursor_next_dup_x(right_to_left, val_relation_key, val_id);
  if (dg_mdb_status_success_p) {
    goto each_data;
  } else {
    dg_mdb_status_require_notfound;
  };
  dg_mdb_cursor_next_nodup_x(right_to_left, val_relation_key, val_id);
  if (dg_mdb_status_success_p) {
    if (dg_id_equal_p((*(relation_key + 0)),
                      dg_mdb_val_to_id_at(val_relation_key, 0))) {
      goto each_key;
    };
  } else {
    dg_mdb_status_require_notfound;
  };
  right = dg_ids_rest(right);
  if (right) {
    (*(relation_key + 0)) = dg_ids_first(right);
  } else {
    no_more_data_exit;
  };
  goto set_range;
exit:
  (*state).status = status;
  (*state).right = right;
  return (status);
};
status_t dg_relation_read_0000(dg_relation_read_state_t *state, b32 count,
                               dg_relation_records_t **result) {
  dg_relation_reader_header_0000(state);
  dg_mdb_introduce_val_relation_data;
  MDB_cursor *left_to_right = (*state).cursor;
  dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                      MDB_GET_CURRENT);
  dg_mdb_status_require;
each_key:
each_data:
  stop_if_count_zero;
  if (!skip_p) {
    record.left = dg_mdb_val_to_id_at(val_relation_key, 0);
    record.right = dg_mdb_val_relation_data_to_id(val_relation_data);
    record.label = dg_mdb_val_to_id_at(val_relation_key, 1);
    record.ordinal = dg_mdb_val_relation_data_to_ordinal(val_relation_data);
    dg_relation_records_add_x((*result), record, result_temp);
  };
  reduce_count;
  dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key, val_relation_data);
  if (dg_mdb_status_success_p) {
    goto each_data;
  } else {
    dg_mdb_status_require_notfound;
  };
  dg_mdb_cursor_next_nodup_x(left_to_right, val_relation_key,
                             val_relation_data);
  if (dg_mdb_status_success_p) {
    goto each_key;
  } else {
    dg_mdb_status_require_notfound;
  };
exit:
  (*state).status = status;
  return (status);
};
/** prepare the state and select the reader.
  readers are specialised for filter combinations.
  the 1/0 pattern at the end of reader names corresponds to the filter
  combination the reader is supposed to handle. 1 stands for filter given, 0
  stands for not given. the order is left-right-label-ordinal. readers always
  leave cursors at a valid entry, usually the next entry unless the results have
  been exhausted */
status_t dg_relation_select(dg_txn_t *txn, dg_ids_t *left, dg_ids_t *right,
                            dg_ids_t *label, dg_ordinal_match_data_t *ordinal,
                            b32 offset, dg_relation_read_state_t *result) {
  status_init;
  dg_mdb_declare_cursor_3(left_to_right, right_to_left, label_to_left);
  (*result).status = status;
  (*result).left = left;
  (*result).left_first = left;
  (*result).right = right;
  (*result).right_first = right;
  (*result).label = label;
  (*result).ordinal = ordinal;
  (*result).cursor = 0;
  (*result).cursor_2 = 0;
  (*result).options = 0;
  if (left) {
    if (ordinal) {
      if (right) {
        dg_relation_select_initialise_set(right, result);
      };
      dg_relation_select_cursor_initialise(left_to_right, result, cursor);
      if (label) {
        (*result).reader = dg_relation_read_1011_1111;
      } else {
        (*result).reader = dg_relation_read_1001_1101;
      };
    } else {
      if (right) {
        dg_relation_select_cursor_initialise(right_to_left, result, cursor);
        if (label) {
          (*result).reader = dg_relation_read_1110;
        } else {
          (*result).reader = dg_relation_read_1100;
        };
      } else {
        dg_relation_select_cursor_initialise(left_to_right, result, cursor);
        if (label) {
          (*result).reader = dg_relation_read_1010;
        } else {
          (*result).reader = dg_relation_read_1000;
        };
      };
    };
  } else {
    if (right) {
      dg_relation_select_cursor_initialise(right_to_left, result, cursor);
      (*result).reader =
          (label ? dg_relation_read_0110 : dg_relation_read_0100);
    } else {
      if (label) {
        dg_relation_select_cursor_initialise(label_to_left, result, cursor);
        dg_relation_select_cursor_initialise(left_to_right, result, cursor_2);
        (*result).reader = dg_relation_read_0010;
      } else {
        dg_relation_select_cursor_initialise(left_to_right, result, cursor);
        (*result).reader = dg_relation_read_0000;
      };
    };
  };
  dg_relation_reader_t reader = (*result).reader;
  dg_select_ensure_offset(result, offset, reader);
exit:
  (*result).status = status;
  return (status);
};
status_t dg_relation_read(dg_relation_read_state_t *state, b32 count,
                          dg_relation_records_t **result) {
  status_init;
  count = optional_count(count);
  status_require_x((*state).status);
  status = ((dg_relation_reader_t)((*state).reader))(state, count, result);
exit:
  dg_status_no_more_data_if_mdb_notfound;
  return (status);
};
b0 dg_relation_selection_destroy(dg_relation_read_state_t *state) {
  dg_mdb_cursor_close_2((*state).cursor, (*state).cursor_2);
  if ((dg_read_option_is_set_right & (*state).options)) {
    imht_set_destroy(((imht_set_t *)((*state).right)));
    (*state).right = 0;
  };
};
#endif
#ifndef sc_included_sph_dg_debug_relation
#define sc_included_sph_dg_debug_relation
status_t dg_debug_display_content_left_to_right(dg_txn_t *txn) {
  status_init;
  dg_mdb_introduce_cursor(txn, left_to_right);
  dg_id_t id_left;
  dg_id_t id_right;
  dg_id_t id_label;
  dg_ordinal_t ordinal;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_relation_data;
  printf("left->right\n");
  dg_mdb_cursor_each_key(left_to_right, val_relation_key, val_relation_data, {
    id_left = dg_mdb_val_to_id_at(val_relation_key, 0);
    id_label = dg_mdb_val_to_id_at(val_relation_key, 1);
    do {
      id_right = dg_mdb_val_relation_data_to_id(val_relation_data);
      ordinal = dg_mdb_val_relation_data_to_ordinal(val_relation_data);
      printf("  (%lu %lu) (%lu %lu)\n", id_left, id_label, ordinal, id_right);
      dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,
                               val_relation_data);
    } while (dg_mdb_status_success_p);
  });
exit:
  mdb_cursor_close(left_to_right);
  dg_status_success_if_mdb_notfound;
  return (status);
};
status_t dg_debug_display_content_right_to_left(dg_txn_t *txn) {
  status_init;
  dg_mdb_introduce_cursor(txn, right_to_left);
  dg_id_t id_left;
  dg_id_t id_right;
  dg_id_t id_label;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_id;
  printf("right->left\n");
  dg_mdb_cursor_each_key(right_to_left, val_relation_key, val_id, {
    id_right = dg_mdb_val_to_id_at(val_relation_key, 0);
    id_label = dg_mdb_val_to_id_at(val_relation_key, 1);
    do {
      id_left = dg_mdb_val_to_id(val_id);
      printf("  (%lu %lu) %lu\n", id_right, id_label, id_left);
      dg_mdb_cursor_next_dup_x(right_to_left, val_relation_key, val_id);
    } while (dg_mdb_status_success_p);
  });
exit:
  mdb_cursor_close(right_to_left);
  dg_status_success_if_mdb_notfound;
  return (status);
};
#endif
#endif
#ifndef sc_included_sph_dg_node
#define sc_included_sph_dg_node
pthread_mutex_t id_next_mutex;
b64 id_next_id;
b64 id_next_intern;
b64 id_next_extern;
#ifndef id_next_search_limit
#define id_next_search_limit 1000000
#endif
#define dg_id_remove_type(id) (~dg_type_mask & id)
/** initialise the next-identifier counter.
  if the database is empty, start at 1, because 0 is reserved for the special
  null id. otherwise starts from the last existing database entry and searches
  backwards to find the highest id for each type. if it can not find a value for
  a type after "id-next-search-limit" number of lookups, use (- last-id
  id-next-search-limit). id counters must be set to a multiple of
  dg-id-type-step - with the type mask being zero */
status_t id_next_initialise(dg_txn_t *txn) {
  status_init;
  dg_mdb_introduce_val_id;
  b8 found = 0;
  b32 max = id_next_search_limit;
  b8 type;
  dg_id_t id;
  dg_mdb_introduce_cursor(txn, id_to_data);
  dg_mdb_cursor_get_x(id_to_data, val_id, val_null, MDB_LAST);
  if (dg_mdb_status_success_p) {
  loop:
    id = dg_mdb_val_to_id(val_id);
    type = (dg_type_mask & id);
    if ((((dg_type_id == type)) && !(1 & found))) {
      id_next_id = (dg_id_type_step + id);
      found = (1 | found);
    } else {
      if ((((dg_type_intern == type)) && !(2 & found))) {
        id_next_intern = (dg_id_type_step + id);
        found = (2 | found);
      } else {
        if ((((dg_type_extern == type)) && !(4 & found))) {
          id_next_extern = (dg_id_type_step + id);
          found = (4 | found);
        };
      };
    };
    if (((max > 0) && (!(7 == found)))) {
      max = (max - 1);
      dg_mdb_cursor_get_x(id_to_data, val_id, val_null, MDB_PREV_NODUP);
      if (dg_mdb_status_success_p) {
        goto loop;
      } else {
        dg_status_success_if_mdb_notfound;
      };
    };
    if (!(7 == found)) {
      if (!(1 & found)) {
        if ((id >= dg_id_type_step)) {
          id_next_id = (dg_type_id | dg_id_remove_type(id));
        } else {
          id_next_id = dg_id_type_step;
        };
      };
      if (!(2 & found)) {
        id_next_intern = (dg_type_intern | dg_id_remove_type(id));
      };
      if (!(4 & found)) {
        id_next_extern = (dg_type_extern | dg_id_remove_type(id));
      };
    };
  } else {
    if ((MDB_NOTFOUND == status.id)) {
      id_next_id = dg_id_type_step;
      id_next_intern = (dg_type_intern | dg_id_type_step);
      id_next_extern = (dg_type_extern | dg_id_type_step);
      status.id = status_id_success;
    };
  };
exit:
  mdb_cursor_close(id_to_data);
  if ((7 == found)) {
    status.id = status_id_success;
  };
  return (status);
};
/** return one new, unique and typed identifier */
#define dg_id_next(type_name, r)                                               \
  status_init;                                                                 \
  pthread_mutex_lock(&id_next_mutex);                                          \
  if ((id_next_##type_name < (dg_id_max - dg_id_type_step))) {                 \
    (*r) = id_next_##type_name;                                                \
    id_next_##type_name = (id_next_##type_name + dg_id_type_step);             \
    pthread_mutex_unlock(&id_next_mutex);                                      \
  } else {                                                                     \
    status_set_both_goto(dg_status_group_dg, dg_status_id_max_id);             \
  };                                                                           \
  exit:                                                                        \
  return (status)
;
status_t dg_id_next_id(dg_id_t *r) { dg_id_next(id, r); };
status_t dg_id_next_intern(dg_id_t *r) { dg_id_next(intern, r); };
status_t dg_id_next_extern(dg_id_t *r) { dg_id_next(extern, r); };
boolean dg_id_p(dg_id_t id) { return (dg_type_p(dg_type_id, id)); };
/** create "count" number of id-type nodes and add their ids to "result" */
status_t dg_id_create(dg_txn_t *txn, b32 count, dg_ids_t **result) {
  status_init;
  dg_mdb_reset_val_null;
  dg_mdb_declare_cursor(id_to_data);
  dg_mdb_initialise_cursor(txn, id_to_data);
  dg_mdb_introduce_val_id;
  dg_id_t id;
  dg_ids_t *ids_temp;
  val_id.mv_data = &id;
  while (count) {
    status_require_x(dg_id_next_id(&id));
    dg_mdb_status_require_x(mdb_cursor_put(id_to_data, &val_id, &val_null, 0));
    dg_ids_add_x((*result), id, ids_temp);
    decrement(count);
  };
exit:
  mdb_cursor_close(id_to_data);
  return (status);
};
status_t dg_exists_p(dg_txn_t *txn, dg_ids_t *ids, boolean *result) {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_cursor(txn, id_to_data);
  while (ids) {
    val_id.mv_data = dg_ids_first_address(ids);
    status.id = mdb_cursor_get(id_to_data, &val_id, &val_null, MDB_SET);
    if ((MDB_NOTFOUND == status.id)) {
      (*result) = 0;
      status.id = status_id_success;
      goto exit;
    } else {
      status_require;
    };
    ids = dg_ids_rest(ids);
  };
  (*result) = 1;
exit:
  mdb_cursor_close(id_to_data);
  return (status);
};
/** filter existing ids from the list of given ids and add the result to
 * "result" */
status_t dg_identify(dg_txn_t *txn, dg_ids_t *ids, dg_ids_t **result) {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_declare_cursor(id_to_data);
  dg_mdb_initialise_cursor(txn, id_to_data);
  while (ids) {
    val_id.mv_data = dg_ids_first_address(ids);
    status.id = mdb_cursor_get(id_to_data, &val_id, &val_null, MDB_SET);
    if (dg_mdb_status_success_p) {
      (*result) = dg_ids_add((*result), dg_ids_first(ids));
    } else {
      dg_mdb_status_require_notfound;
    };
    ids = dg_ids_rest(ids);
  };
exit:
  mdb_cursor_close(id_to_data);
  if ((MDB_NOTFOUND == status.id)) {
    status.id = status_id_success;
  };
  return (status);
};
boolean dg_extern_p(dg_id_t id) { return (dg_type_p(dg_type_extern, id)); };
/** create "count" number of extern-type nodes and add their ids to "result".
  "data" is optional and can be null. if data is given, all created nodes share
  the same data */
status_t dg_extern_create(dg_txn_t *txn, b32 count, dg_data_t *data,
                          dg_ids_t **result) {
  status_init;
  dg_mdb_reset_val_null;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_id_t id;
  dg_ids_t *result_temp;
  val_id.mv_data = &id;
  dg_mdb_declare_cursor_2(id_to_data, data_extern_to_extern);
  dg_mdb_initialise_cursor(txn, id_to_data);
  if (data) {
    val_data.mv_data = (*data).data;
    val_data.mv_size = (*data).size;
    dg_mdb_initialise_cursor(txn, data_extern_to_extern);
    while (count) {
      status_require_x(dg_id_next_extern(&id));
      dg_mdb_status_require_x(
          mdb_cursor_put(id_to_data, &val_id, &val_data, 0));
      dg_mdb_status_require_x(
          mdb_cursor_put(data_extern_to_extern, &val_data, &val_id, 0));
      dg_ids_add_x((*result), id, result_temp);
      decrement(count);
    };
  } else {
    while (count) {
      status_require_x(dg_id_next_extern(&id));
      dg_mdb_status_require_x(
          mdb_cursor_put(id_to_data, &val_id, &val_null, 0));
      dg_ids_add_x((*result), id, result_temp);
      decrement(count);
    };
  };
exit:
  mdb_cursor_close(id_to_data);
  mdb_cursor_close(data_extern_to_extern);
  return (status);
};
status_t dg_extern_data_to_id(dg_txn_t *txn, dg_data_t data,
                              dg_ids_t **result) {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_declare_cursor(data_extern_to_extern);
  dg_mdb_initialise_cursor(txn, data_extern_to_extern);
  val_data.mv_size = data.size;
  val_data.mv_data = data.data;
  status.id =
      mdb_cursor_get(data_extern_to_extern, &val_data, &val_id, MDB_SET_KEY);
  while (dg_mdb_status_success_p) {
    (*result) = dg_ids_add((*result), dg_mdb_val_to_id(val_id));
    status.id =
        mdb_cursor_get(data_extern_to_extern, &val_data, &val_id, MDB_NEXT_DUP);
  };
  dg_status_success_if_mdb_notfound;
exit:
  mdb_cursor_close(data_extern_to_extern);
  return (status);
};
status_t dg_extern_id_to_data(dg_txn_t *txn, dg_ids_t *ids, boolean every_p,
                              dg_data_list_t **result) {
  status_init;
  dg_mdb_introduce_val_data;
  dg_mdb_introduce_val_id;
  dg_mdb_declare_cursor(id_to_data);
  dg_mdb_initialise_cursor(txn, id_to_data);
  dg_data_t result_element;
  while (ids) {
    val_id.mv_data = dg_ids_first_address(ids);
    status.id = mdb_cursor_get(id_to_data, &val_id, &val_data, MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      result_element.size = val_data.mv_size;
      result_element.data = val_data.mv_data;
      (*result) = dg_data_list_add((*result), result_element);
    } else {
      if ((MDB_NOTFOUND == status.id)) {
        if (every_p) {
          dg_status_set_id_goto(dg_status_id_condition_unfulfilled);
        };
      } else {
        status.group = dg_status_group_lmdb;
        goto exit;
      };
    };
    ids = dg_ids_rest(ids);
  };
  dg_status_success_if_mdb_notfound;
exit:
  mdb_cursor_close(id_to_data);
  return (status);
};
/** set the data of one extern-type node identified by "id" */
status_t dg_extern_update(dg_txn_t *txn, dg_id_t id, dg_data_t data) {
  status_init;
  dg_mdb_reset_val_null;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_declare_cursor_2(id_to_data, data_extern_to_extern);
  dg_mdb_initialise_cursor_2(txn, id_to_data, data_extern_to_extern);
  val_id.mv_data = &id;
  status.id = mdb_cursor_get(id_to_data, &val_id, &val_data, MDB_SET_KEY);
  if (dg_mdb_status_success_p) {
    status.id =
        mdb_cursor_get(data_extern_to_extern, &val_data, &val_id, MDB_GET_BOTH);
    if (dg_mdb_status_success_p) {
      mdb_cursor_del(data_extern_to_extern, 0);
    } else {
      dg_mdb_status_require_notfound;
    };
    val_data.mv_data = data.data;
    dg_mdb_status_require_x(
        mdb_cursor_put(id_to_data, &val_null, &val_data, MDB_CURRENT));
    dg_mdb_status_require_x(
        mdb_cursor_put(data_extern_to_extern, &val_data, &val_id, 0));
  } else {
    dg_mdb_status_require_notfound;
  };
exit:
  mdb_cursor_close(id_to_data);
  mdb_cursor_close(data_extern_to_extern);
  return (status);
};
boolean dg_intern_p(dg_id_t id) { return (dg_type_p(dg_type_intern, id)); };
status_t dg_intern_ensure(dg_txn_t *txn, dg_data_list_t *data,
                          dg_ids_t **result) {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_introduce_cursor_2(txn, id_to_data, data_intern_to_id);
  dg_id_t id;
  val_id.mv_data = &id;
  while (data) {
    if (((dg_data_list_first(data).size > dg_size_octets_data_max) ||
         (dg_data_list_first(data).size < dg_size_octets_data_min))) {
      dg_status_set_id_goto(dg_status_id_data_length);
    };
    val_data.mv_size = dg_data_list_first(data).size;
    val_data.mv_data = dg_data_list_first(data).data;
    dg_mdb_cursor_get_x(data_intern_to_id, val_data, val_id, MDB_SET_KEY);
    if (status_id_is_p(MDB_NOTFOUND)) {
      status_require_x(dg_id_next_intern(&id));
      dg_mdb_status_require_x(
          mdb_cursor_put(id_to_data, &val_id, &val_data, 0));
      dg_mdb_status_require_x(
          mdb_cursor_put(data_intern_to_id, &val_data, &val_id, 0));
      (*result) = dg_ids_add((*result), id);
    } else {
      if (dg_mdb_status_success_p) {
        (*result) = dg_ids_add((*result), dg_mdb_val_to_id(val_id));
        val_id.mv_data = &id;
      } else {
        status_set_group_goto(dg_status_group_lmdb);
      };
    };
    data = dg_data_list_rest(data);
  };
exit:
  dg_mdb_cursor_close_2(id_to_data, data_intern_to_id);
  return (status);
};
status_t dg_intern_data_to_id(dg_txn_t *txn, dg_data_list_t *data,
                              boolean every_p, dg_ids_t **result) {
  status_init;
  dg_mdb_introduce_val_data;
  dg_mdb_introduce_val_id;
  dg_ids_t *result_temp = 0;
  dg_mdb_declare_cursor(data_intern_to_id);
  dg_mdb_initialise_cursor(txn, data_intern_to_id);
  while (data) {
    val_data.mv_size = dg_data_list_first(data).size;
    val_data.mv_data = dg_data_list_first(data).data;
    status.id =
        mdb_cursor_get(data_intern_to_id, &val_data, &val_id, MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      result_temp = dg_ids_add(result_temp, dg_mdb_val_to_id(val_id));
    } else {
      if ((MDB_NOTFOUND == status.id)) {
        if (every_p) {
          dg_status_set_id_goto(dg_status_id_condition_unfulfilled);
        };
      } else {
        status.group = dg_status_group_lmdb;
        goto exit;
      };
    };
    data = dg_data_list_rest(data);
  };
  dg_status_success_if_mdb_notfound;
  (*result) = result_temp;
exit:
  mdb_cursor_close(data_intern_to_id);
  if (status_failure_p) {
    dg_ids_destroy(result_temp);
  };
  return (status);
};
status_t dg_intern_id_to_data(dg_txn_t *txn, dg_ids_t *ids, boolean every_p,
                              dg_data_list_t **result) {
  status_init;
  dg_mdb_introduce_val_data;
  dg_mdb_introduce_val_id;
  dg_data_t result_element;
  dg_data_list_t *result_temp = 0;
  dg_mdb_declare_cursor(id_to_data);
  dg_mdb_initialise_cursor(txn, id_to_data);
  while (ids) {
    val_id.mv_data = dg_ids_first_address(ids);
    status.id = mdb_cursor_get(id_to_data, &val_id, &val_data, MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      result_element.size = val_data.mv_size;
      result_element.data = val_data.mv_data;
      result_temp = dg_data_list_add(result_temp, result_element);
    } else {
      if ((MDB_NOTFOUND == status.id)) {
        if (every_p) {
          dg_status_set_id_goto(dg_status_id_condition_unfulfilled);
        };
      } else {
        status.group = dg_status_group_lmdb;
        goto exit;
      };
    };
    ids = dg_ids_rest(ids);
  };
  dg_status_success_if_mdb_notfound;
  (*result) = result_temp;
exit:
  mdb_cursor_close(id_to_data);
  if (status_failure_p) {
    dg_data_list_destroy(result_temp);
  };
  return (status);
};
status_t dg_intern_update(dg_txn_t *txn, dg_id_t id, dg_data_t data) {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  val_data.mv_data = data.data;
  val_data.mv_size = data.size;
  dg_mdb_declare_cursor_2(id_to_data, data_intern_to_id);
  dg_mdb_initialise_cursor_2(txn, id_to_data, data_intern_to_id);
  dg_mdb_cursor_get_x(data_intern_to_id, val_data, val_null, MDB_SET);
  if (dg_mdb_status_success_p) {
    dg_status_set_id_goto(dg_status_id_duplicate);
  } else {
    dg_mdb_status_require_notfound;
  };
  val_id.mv_data = &id;
  status.id = mdb_cursor_get(id_to_data, &val_id, &val_data, MDB_SET_KEY);
  if (dg_mdb_status_success_p) {
    status.id =
        mdb_cursor_get(data_intern_to_id, &val_data, &val_null, MDB_SET);
    if (dg_mdb_status_success_p) {
      mdb_cursor_del(data_intern_to_id, 0);
    } else {
      dg_mdb_status_require_notfound;
    };
    val_data.mv_data = data.data;
    val_data.mv_size = data.size;
    dg_mdb_status_require_x(
        mdb_cursor_put(id_to_data, &val_id, &val_data, MDB_CURRENT));
    dg_mdb_status_require_x(
        mdb_cursor_put(data_intern_to_id, &val_data, &val_id, 0));
  } else {
    dg_mdb_status_require_notfound;
  };
exit:
  return (status);
};
boolean dg_intern_small_p(dg_id_t id) {
  return (dg_type_p(dg_type_intern_small, id));
};
status_t dg_relation_internal_delete(dg_txn_t *txn, dg_ids_t *left,
                                     dg_ids_t *right, dg_ids_t *label,
                                     dg_ordinal_match_data_t *ordinal,
                                     MDB_cursor *left_to_right,
                                     MDB_cursor *right_to_left,
                                     MDB_cursor *label_to_left);
status_t dg_delete_one(dg_txn_t *txn, dg_id_t id, MDB_cursor *id_to_data,
                       MDB_cursor *data_intern_to_id,
                       MDB_cursor *data_extern_to_extern,
                       MDB_cursor *left_to_right, MDB_cursor *right_to_left,
                       MDB_cursor *label_to_left) {
  status_init;
  dg_mdb_introduce_val_relation_data;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  dg_mdb_introduce_val_data;
  val_id.mv_data = &id;
  dg_mdb_cursor_get_x(id_to_data, val_id, val_data, MDB_SET_KEY);
  if (dg_mdb_status_success_p) {
    if (dg_intern_p(id)) {
      status.id =
          mdb_cursor_get(data_intern_to_id, &val_data, &val_null, MDB_SET);
      if (dg_mdb_status_success_p) {
        dg_mdb_cursor_del_x(data_intern_to_id, 0);
      } else {
        dg_mdb_status_require_notfound;
      };
    } else {
      if ((dg_extern_p(id) && val_data.mv_size)) {
        status.id = mdb_cursor_get(data_extern_to_extern, &val_data, &val_null,
                                   MDB_SET);
        if (dg_mdb_status_success_p) {
          dg_mdb_cursor_del_x(data_extern_to_extern, 0);
        } else {
          dg_mdb_status_require_notfound;
        };
      };
    };
  } else {
    dg_mdb_status_require_notfound;
  };
  dg_mdb_cursor_del_x(id_to_data, 0);
exit:
  return (status);
};
status_t dg_delete(dg_txn_t *txn, dg_ids_t *ids) {
  status_init;
  if (!ids) {
    return (status);
  };
  dg_mdb_declare_cursor_3(id_to_data, data_intern_to_id, data_extern_to_extern);
  dg_mdb_declare_cursor_3(left_to_right, right_to_left, label_to_left);
  dg_mdb_initialise_cursor_3(txn, left_to_right, right_to_left, label_to_left);
  status_require_x(dg_relation_internal_delete(txn, 0, 0, ids, 0, left_to_right,
                                               right_to_left, label_to_left));
  status_require_x(dg_relation_internal_delete(txn, ids, 0, 0, 0, left_to_right,
                                               right_to_left, label_to_left));
  status_require_x(dg_relation_internal_delete(txn, 0, ids, 0, 0, left_to_right,
                                               right_to_left, label_to_left));
  dg_mdb_initialise_cursor_3(txn, id_to_data, data_intern_to_id,
                             data_extern_to_extern);
  while ((ids && status_success_p)) {
    status = dg_delete_one(txn, dg_ids_first(ids), id_to_data,
                           data_intern_to_id, data_extern_to_extern,
                           left_to_right, right_to_left, label_to_left);
    ids = dg_ids_rest(ids);
  };
exit:
  dg_mdb_cursor_close_3(id_to_data, data_intern_to_id, data_extern_to_extern);
  dg_mdb_cursor_close_3(left_to_right, right_to_left, label_to_left);
  return (status);
};
#define dg_node_types_match_p(types, id)                                       \
  (!types ||                                                                   \
   ((!(dg_type_bit_id & types) || dg_type_p(dg_type_id, id)) &&                \
    (!(dg_type_bit_intern & types) || dg_type_p(dg_type_intern, id)) &&        \
    (!(dg_type_bit_extern & types) || dg_type_p(dg_type_extern, id)) &&        \
    (!(dg_type_bit_intern_small & types) ||                                    \
     dg_type_p(dg_type_intern_small, id))))
status_t dg_node_read(dg_node_read_state_t *state, b32 count,
                      dg_data_records_t **result) {
  status_init;
  count = optional_count(count);
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
#define id_to_data (*state).cursor
#define skip (dg_read_option_skip & (*state).options)
#define types (*state).types
  status_require_x((*state).status);
  dg_data_records_t *data_records;
  dg_data_record_t data_record;
  if (skip) {
    while (count) {
      dg_mdb_status_require_x(
          mdb_cursor_get(id_to_data, &val_null, &val_null, MDB_NEXT_NODUP));
      if (dg_node_types_match_p(types, dg_mdb_val_to_id(val_id))) {
        count = (count - 1);
      };
    };
  } else {
    dg_mdb_status_require_x(
        mdb_cursor_get(id_to_data, &val_id, &val_data, MDB_GET_CURRENT));
    while (count) {
      if (dg_node_types_match_p(types, dg_mdb_val_to_id(val_id))) {
        data_record.id = dg_mdb_val_to_id(val_id);
        data_record.data = val_data.mv_data;
        data_record.size = val_data.mv_size;
        data_records = dg_data_records_add((*result), data_record);
        if (!data_records) {
          dg_status_set_id_goto(dg_status_id_memory);
        };
        (*result) = data_records;
        count = (count - 1);
      };
      dg_mdb_status_require_x(
          mdb_cursor_get(id_to_data, &val_id, &val_data, MDB_NEXT_NODUP));
    };
  };
exit:
  dg_status_no_more_data_if_mdb_notfound;
  (*state).status = status;
  return (status);
#undef id_to_data
#undef skip
#undef types
};
/** select nodes optionally filtered by type.
  type-bits is the bit-or of dg-type-bit-* values */
status_t dg_node_select(dg_txn_t *txn, b8 type_bits, b32 offset,
                        dg_node_read_state_t *result) {
  status_init;
  dg_mdb_introduce_cursor(txn, id_to_data);
  dg_mdb_status_require_x(
      mdb_cursor_get(id_to_data, &val_null, &val_null, MDB_FIRST));
  (*result).cursor = id_to_data;
  (*result).types = type_bits;
  (*result).status = status;
  (*result).options = 0;
  dg_select_ensure_offset(result, offset, dg_node_read);
exit:
  if (!dg_mdb_status_success_p) {
    mdb_cursor_close(id_to_data);
    if (status_id_is_p(MDB_NOTFOUND)) {
      status_set_id(dg_status_id_no_more_data);
      (*result).cursor = 0;
    };
  };
  (*result).status = status;
  return (status);
};
b0 dg_node_selection_destroy(dg_node_read_state_t *state) {
  if ((*state).cursor) {
    mdb_cursor_close((*state).cursor);
  };
};
#endif
#ifndef sc_included_sph_dg_index
#define sc_included_sph_dg_index
#define dg_index_errors_relation_log(message, left, right, label)              \
  dg_error_log("(groups index relation) (description \"%s\") (left %lu) "      \
               "(right %lu) (label %lu)",                                      \
               message, left, right, label)
#define dg_index_errors_data_log(message, type, id)                            \
  dg_error_log("(groups index %s) (description %s) (id %lu)", type, message, id)
status_t dg_index_recreate_relation() {
  status_init;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_relation_data;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  dg_define_relation_data(relation_data);
  dg_define_relation_key(relation_key);
  dg_mdb_declare_cursor_3(left_to_right, right_to_left, label_to_left);
  dg_txn_introduce;
  dg_txn_write_begin;
  dg_mdb_status_require_x(mdb_drop(dg_txn, dbi_right_to_left, 0));
  dg_mdb_status_require_x(mdb_drop(dg_txn, dbi_label_to_left, 0));
  dg_txn_commit;
  dg_txn_write_begin;
  dg_mdb_initialise_cursor_3(dg_txn, left_to_right, right_to_left,
                             label_to_left);
  dg_id_t id_left;
  dg_id_t id_right;
  dg_id_t id_label;
  dg_mdb_cursor_each_key(left_to_right, val_relation_key, val_relation_data, {
    id_left = dg_mdb_val_to_id_at(val_relation_key, 0);
    id_label = dg_mdb_val_to_id_at(val_relation_key, 1);
    do {
      id_right = dg_mdb_val_relation_data_to_id(val_relation_data);
      (*(relation_key + 0)) = id_right;
      (*(relation_key + 1)) = id_label;
      val_relation_key.mv_data = relation_key;
      val_id.mv_data = &id_left;
      dg_mdb_status_require_x(
          mdb_cursor_put(right_to_left, &val_relation_key, &val_id, 0));
      val_id_2.mv_data = &id_label;
      dg_mdb_status_require_x(
          mdb_cursor_put(label_to_left, &val_id_2, &val_id, 0));
      dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,
                               val_relation_data);
    } while (dg_mdb_status_success_p);
  });
  dg_txn_commit;
exit:
  if (dg_txn) {
    dg_txn_abort;
  };
  return (status);
};
status_t dg_index_recreate_intern() {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_declare_cursor_2(id_to_data, data_intern_to_id);
  dg_txn_introduce;
  dg_txn_write_begin;
  mdb_drop(dg_txn, dbi_data_intern_to_id, 0);
  dg_txn_commit;
  dg_txn_write_begin;
  dg_mdb_initialise_cursor_2(dg_txn, id_to_data, data_intern_to_id);
  dg_mdb_cursor_each_key(id_to_data, val_id, val_data, {
    if ((val_data.mv_size && dg_intern_p(dg_mdb_val_to_id(val_id)))) {
      dg_mdb_status_require_x(
          mdb_cursor_put(data_intern_to_id, &val_data, &val_id, 0));
    };
  });
  dg_txn_commit;
exit:
  if (dg_txn) {
    dg_txn_abort;
  };
  return (status);
};
status_t dg_index_recreate_extern() {
  status_init;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_declare_cursor(id_to_data);
  dg_mdb_declare_cursor(data_intern_to_id);
  dg_txn_introduce;
  dg_txn_write_begin;
  mdb_drop(dg_txn, dbi_data_intern_to_id, 0);
  dg_txn_commit;
  dg_txn_write_begin;
  dg_mdb_initialise_cursor(dg_txn, id_to_data);
  dg_mdb_initialise_cursor(dg_txn, data_intern_to_id);
  dg_mdb_cursor_each_key(id_to_data, val_id, val_data, {
    if ((val_data.mv_size && dg_intern_p(dg_mdb_val_to_id(val_id)))) {
      dg_mdb_status_require_x(
          mdb_cursor_put(data_intern_to_id, &val_data, &val_id, 0));
    };
  });
  dg_txn_commit;
exit:
  if (dg_txn) {
    dg_txn_abort;
  };
  return (status);
};
status_t dg_index_errors_relation(dg_txn_t *dg_txn,
                                  dg_index_errors_relation_t *result) {
  status_init;
  (*result) = dg_index_errors_relation_null;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_id_2;
  dg_mdb_introduce_val_relation_key;
  dg_mdb_introduce_val_relation_data;
  dg_id_t id_right;
  dg_id_t id_left;
  dg_id_t id_label;
  dg_relation_records_t *records_temp;
  dg_relation_record_t record;
  dg_define_relation_key(relation_key);
  dg_define_relation_data(relation_data);
  dg_mdb_introduce_cursor_3(dg_txn, left_to_right, right_to_left,
                            label_to_left);
  dg_mdb_cursor_each_key(left_to_right, val_relation_key, val_relation_data, {
    id_left = dg_mdb_val_to_id_at(val_relation_key, 0);
    id_label = dg_mdb_val_to_id_at(val_relation_key, 1);
    do {
      id_right = dg_mdb_val_relation_data_to_id(val_relation_data);
      (*(relation_key + 0)) = id_right;
      (*(relation_key + 1)) = id_label;
      val_relation_key.mv_data = relation_key;
      val_id.mv_data = &id_left;
      dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id, MDB_SET_KEY);
      dg_mdb_cursor_get_x(right_to_left, val_relation_key, val_id,
                          MDB_GET_BOTH);
      if (dg_mdb_status_failure_p) {
        if ((MDB_NOTFOUND == status.id)) {
          dg_index_errors_relation_log(
              "entry from left->right not in right->left", id_left, id_right,
              id_label);
          (*result).errors_p = 1;
          record.left = id_left;
          record.right = id_right;
          record.label = id_label;
          dg_relation_records_add_x((*result).missing_right_left, record,
                                    records_temp);
        } else {
          status_goto;
        };
      };
      val_id_2.mv_data = &id_label;
      dg_mdb_cursor_get_x(label_to_left, val_id_2, val_id, MDB_GET_BOTH);
      if (!dg_mdb_status_success_p) {
        if ((MDB_NOTFOUND == status.id)) {
          dg_index_errors_relation_log(
              "entry from left->right not in label->left", id_left, id_right,
              id_label);
          (*result).errors_p = 1;
          record.left = id_left;
          record.right = id_right;
          record.label = id_label;
          dg_relation_records_add_x((*result).missing_label_left, record,
                                    records_temp);
        } else {
          status_goto;
        };
      };
      dg_mdb_cursor_next_dup_x(left_to_right, val_relation_key,
                               val_relation_data);
    } while (dg_mdb_status_success_p);
  });
  dg_mdb_cursor_each_key(right_to_left, val_relation_key, val_id, {
    id_right = dg_mdb_val_to_id_at(val_relation_key, 0);
    id_label = dg_mdb_val_to_id_at(val_relation_key, 1);
    do {
      id_left = dg_mdb_val_to_id(val_id);
      (*(relation_key + 0)) = id_left;
      (*(relation_key + 1)) = id_label;
      val_relation_key.mv_data = relation_key;
      dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                          MDB_SET_KEY);
      if (dg_mdb_status_success_p) {
        status = dg_mdb_left_to_right_seek_right(left_to_right, id_right);
      };
      if (!dg_mdb_status_success_p) {
        if ((MDB_NOTFOUND == status.id)) {
          dg_index_errors_relation_log(
              "entry from right->left not in left->right", id_left, id_right,
              id_label);
          (*result).errors_p = 1;
          record.left = id_left;
          record.right = id_right;
          record.label = id_label;
          dg_relation_records_add_x((*result).excess_right_left, record,
                                    records_temp);
        } else {
          status_goto;
        };
      };
      dg_mdb_cursor_next_dup_x(right_to_left, val_relation_key, val_id);
    } while (dg_mdb_status_success_p);
  });
  dg_mdb_cursor_each_key(label_to_left, val_id, val_id_2, {
    id_label = dg_mdb_val_to_id(val_id);
    do {
      id_left = dg_mdb_val_to_id(val_id_2);
      (*(relation_key + 0)) = id_left;
      (*(relation_key + 1)) = id_label;
      val_relation_key.mv_data = relation_key;
      dg_mdb_cursor_get_x(left_to_right, val_relation_key, val_relation_data,
                          MDB_SET);
      if (!dg_mdb_status_success_p) {
        if ((MDB_NOTFOUND == status.id)) {
          dg_index_errors_relation_log(
              "entry from label->left not in left->right", id_left, id_right,
              id_label);
          (*result).errors_p = 1;
          record.left = id_left;
          record.right = 0;
          record.label = id_label;
          dg_relation_records_add_x((*result).excess_label_left, record,
                                    records_temp);
        } else {
          status_goto;
        };
      };
      dg_mdb_cursor_next_dup_x(label_to_left, val_id, val_id_2);
    } while (dg_mdb_status_success_p);
  });
  dg_status_success_if_mdb_notfound;
exit:
  dg_mdb_cursor_close_3(left_to_right, right_to_left, label_to_left);
  return (status);
};
status_t dg_index_errors_intern(dg_txn_t *txn,
                                dg_index_errors_intern_t *result) {
  status_init;
  (*result) = dg_index_errors_intern_null;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_introduce_val_data_2;
  dg_mdb_introduce_cursor_2(txn, data_intern_to_id, id_to_data);
  dg_ids_t *ids_temp;
  dg_mdb_cursor_each_key(data_intern_to_id, val_data, val_id, {
    dg_mdb_cursor_get_x(id_to_data, val_id, val_data_2, MDB_SET_KEY);
    if (dg_mdb_status_success_p) {
      if (dg_mdb_compare_data(&val_data, &val_data_2)) {
        dg_index_errors_data_log(
            "intern", "data from data-intern->id differs in id->data",
            dg_mdb_val_to_id(val_id));
        (*result).errors_p = 1;
        dg_ids_add_x((*result).different_data_id, dg_mdb_val_to_id(val_id),
                     ids_temp);
      };
    } else {
      if ((MDB_NOTFOUND == status.id)) {
        dg_index_errors_data_log("intern",
                                 "data from data-intern->id not in id->data",
                                 dg_mdb_val_to_id(val_id));
        (*result).errors_p = 1;
        dg_ids_add_x((*result).excess_data_id, dg_mdb_val_to_id(val_id),
                     ids_temp);
      } else {
        status_goto;
      };
    };
  });
  dg_mdb_introduce_val_id_2;
  dg_mdb_cursor_each_key(id_to_data, val_id, val_data, {
    if (dg_intern_p(dg_mdb_val_to_id(val_id))) {
      dg_mdb_cursor_get_x(data_intern_to_id, val_data, val_id_2, MDB_SET_KEY);
      if (dg_mdb_status_success_p) {
        if (!dg_id_equal_p(dg_mdb_val_to_id(val_id),
                           dg_mdb_val_to_id(val_id_2))) {
          dg_index_errors_data_log(
              "intern", "data from id->data differs in data-intern->id",
              dg_mdb_val_to_id(val_id));
          (*result).errors_p = 1;
          dg_ids_add_x((*result).different_id_data, dg_mdb_val_to_id(val_id),
                       ids_temp);
        };
      } else {
        if ((MDB_NOTFOUND == status.id)) {
          dg_index_errors_data_log("intern",
                                   "data from id->data not in data-intern->id",
                                   dg_mdb_val_to_id(val_id_2));
          (*result).errors_p = 1;
          dg_ids_add_x((*result).missing_id_data, dg_mdb_val_to_id(val_id_2),
                       ids_temp);
        } else {
          status_goto;
        };
      };
    };
  });
  dg_status_success_if_mdb_notfound;
exit:
  dg_mdb_cursor_close_2(id_to_data, data_intern_to_id);
  return (status);
};
status_t dg_index_errors_extern(dg_txn_t *txn,
                                dg_index_errors_extern_t *result) {
  status_init;
  (*result) = dg_index_errors_extern_null;
  dg_mdb_introduce_val_id;
  dg_mdb_introduce_val_data;
  dg_mdb_introduce_val_data_2;
  dg_ids_t *ids_temp;
  dg_mdb_declare_cursor_2(id_to_data, data_extern_to_extern);
  dg_mdb_initialise_cursor_2(txn, id_to_data, data_extern_to_extern);
  dg_mdb_cursor_each_key(data_extern_to_extern, val_data, val_id, {
    if (val_data.mv_size) {
      dg_mdb_cursor_get_x(id_to_data, val_id, val_data_2, MDB_SET_KEY);
      if (dg_mdb_status_success_p) {
        if (dg_mdb_compare_data(&val_data, &val_data_2)) {
          dg_index_errors_data_log(
              "extern", "data from data-extern->extern differs in id->data",
              dg_mdb_val_to_id(val_id));
          (*result).errors_p = 1;
          dg_ids_add_x((*result).different_data_extern,
                       dg_mdb_val_to_id(val_id), ids_temp);
        };
      } else {
        if ((MDB_NOTFOUND == status.id)) {
          dg_index_errors_data_log(
              "extern", "data from data-extern->extern not in id->data",
              dg_mdb_val_to_id(val_id));
          (*result).errors_p = 1;
          dg_ids_add_x((*result).excess_data_extern, dg_mdb_val_to_id(val_id),
                       ids_temp);
        } else {
          status_goto;
        };
      };
    };
  });
  dg_mdb_cursor_each_key(id_to_data, val_id, val_data, {
    if ((dg_extern_p(dg_mdb_val_to_id(val_id)) && val_data.mv_size)) {
      dg_mdb_cursor_get_x(data_extern_to_extern, val_data, val_id,
                          MDB_GET_BOTH);
      if ((MDB_NOTFOUND == status.id)) {
        dg_index_errors_data_log(
            "extern", "data from id->data not in data-extern->extern",
            dg_mdb_val_to_id(val_id));
        (*result).errors_p = 1;
        dg_ids_add_x((*result).missing_id_data, dg_mdb_val_to_id(val_id),
                     ids_temp);
      } else {
        status_goto;
      };
    };
  });
  dg_status_success_if_mdb_notfound;
exit:
  dg_mdb_cursor_close_2(id_to_data, data_extern_to_extern);
  return (status);
};
#endif
status_t dg_init(b8 *dg_root_argument, dg_init_options_t *options_pointer) {
  status_init;
  dg_root = string_clone(dg_root_argument);
  if (!dg_root) {
    dg_status_set_id_goto(dg_status_id_memory);
  };
  dg_init_options_t options;
  if (options_pointer) {
    options = (*options_pointer);
  } else {
    dg_init_options_set_defaults(&options);
  };
  if (dg_initialised) {
    return (status);
  };
  dg_txn_introduce;
  b8 *path_database = 0;
  status_require_x(
      dg_init_root_path_prepare(dg_root, options.file_permissions));
  path_database = string_append(dg_root, "/data");
  if (!path_database) {
    dg_status_set_id_goto(dg_status_id_memory);
  };
  dg_mdb_status_require_x(mdb_env_create(&dg_mdb_env));
  dg_mdb_status_require_x(mdb_env_set_maxdbs(dg_mdb_env, 16));
  dg_mdb_status_require_x(
      mdb_env_set_mapsize(dg_mdb_env, options.maximum_size_octets));
  dg_mdb_status_require_x(
      mdb_env_set_maxreaders(dg_mdb_env, options.maximum_reader_count));
  dg_mdb_status_require_x(mdb_env_open(dg_mdb_env, path_database,
                                       dg_init_env_open_flags(options),
                                       options.file_permissions));
  b32 db_options = MDB_CREATE;
  dg_txn_write_begin;
  dg_mdb_status_require_x(
      mdb_dbi_open(dg_txn, "id->data", db_options, &dbi_id_to_data));
  dg_mdb_status_require_x(mdb_dbi_open(dg_txn, "data-intern->id", db_options,
                                       &dbi_data_intern_to_id));
  dg_mdb_status_require_x(mdb_set_compare(
      dg_txn, dbi_id_to_data, ((MDB_cmp_func *)(dg_mdb_compare_id))));
  dg_mdb_status_require_x(mdb_set_compare(
      dg_txn, dbi_data_intern_to_id, ((MDB_cmp_func *)(dg_mdb_compare_data))));
  db_options = (MDB_CREATE | MDB_DUPSORT | MDB_DUPFIXED);
  dg_mdb_status_require_x(
      mdb_dbi_open(dg_txn, "left->right", db_options, &dbi_left_to_right));
  dg_mdb_status_require_x(
      mdb_dbi_open(dg_txn, "right->left", db_options, &dbi_right_to_left));
  dg_mdb_status_require_x(
      mdb_dbi_open(dg_txn, "label->left", db_options, &dbi_label_to_left));
  dg_mdb_status_require_x(mdb_dbi_open(dg_txn, "data-extern->extern",
                                       (MDB_DUPFIXED ^ db_options),
                                       &dbi_data_extern_to_extern));
  dg_mdb_status_require_x(
      mdb_set_compare(dg_txn, dbi_left_to_right,
                      ((MDB_cmp_func *)(dg_mdb_compare_relation_key))));
  dg_mdb_status_require_x(
      mdb_set_compare(dg_txn, dbi_right_to_left,
                      ((MDB_cmp_func *)(dg_mdb_compare_relation_key))));
  dg_mdb_status_require_x(mdb_set_compare(
      dg_txn, dbi_label_to_left, ((MDB_cmp_func *)(dg_mdb_compare_id))));
  dg_mdb_status_require_x(
      mdb_set_compare(dg_txn, dbi_data_extern_to_extern,
                      ((MDB_cmp_func *)(dg_mdb_compare_data))));
  dg_mdb_status_require_x(
      mdb_set_dupsort(dg_txn, dbi_left_to_right,
                      ((MDB_cmp_func *)(dg_mdb_compare_relation_data))));
  dg_mdb_status_require_x(mdb_set_dupsort(
      dg_txn, dbi_right_to_left, ((MDB_cmp_func *)(dg_mdb_compare_id))));
  dg_mdb_status_require_x(mdb_set_dupsort(
      dg_txn, dbi_label_to_left, ((MDB_cmp_func *)(dg_mdb_compare_id))));
  dg_mdb_status_require_x(
      mdb_set_dupsort(dg_txn, dbi_data_extern_to_extern,
                      ((MDB_cmp_func *)(dg_mdb_compare_id))));
  status_require_x(id_next_initialise(dg_txn));
  dg_txn_commit;
  dg_mdb_reset_val_null;
  dg_initialised = 1;
exit:
  if (status_failure_p) {
    mdb_env_close(dg_mdb_env);
    if (dg_txn) {
      dg_txn_abort;
    };
  };
  free(path_database);
  return (status);
};
b0 dg_exit() {
  mdb_dbi_close(dg_mdb_env, dbi_id_to_data);
  mdb_dbi_close(dg_mdb_env, dbi_left_to_right);
  mdb_dbi_close(dg_mdb_env, dbi_right_to_left);
  mdb_dbi_close(dg_mdb_env, dbi_data_intern_to_id);
  mdb_dbi_close(dg_mdb_env, dbi_data_extern_to_extern);
  mdb_env_close(dg_mdb_env);
  pthread_mutex_destroy(&id_next_mutex);
  free(dg_root);
  dg_initialised = 0;
};