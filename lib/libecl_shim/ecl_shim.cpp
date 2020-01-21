
#include <cstdio>
#include <cstdint>
#include <ctime>                // time_t
#include <cstdarg>              // va_list
#include <string>
#include <utility>

#include <dlfcn.h>
#include <unistd.h>
#include <Python.h>

#define ECL_DECL extern "C" [[gnu::visibility("hidden")]]
#define ECLXX_DECL [[gnu::visibility("hidden")]]

typedef int64_t offset_type;

/* These are enums */
typedef int ecl_file_enum;
typedef int ecl_type_enum;
typedef int ert_ecl_unit_enum;
typedef int node_ctype;
typedef int rng_init_mode;
typedef int rng_alg_type;

struct ecl_data_type {
  const ecl_type_enum type;
  const size_t element_size;
};

typedef struct ecl_smspec_struct ecl_smspec_type;

namespace ecl {
    struct smspec_node {
        const char *get_gen_key1() const;
    };
}

struct basic_parser_type;
struct bool_vector_type;
struct buffer_type;
struct double_vector_type;
struct ecl_file_type;
struct ecl_grid_type;
struct ecl_io_config_type;
struct ecl_kw_type;
struct ecl_sum_type;
struct fortio_type;
struct geo_surface_type;
struct hash_iter_type;
struct hash_type;
struct int_vector_type;
struct long_vector_type;
struct path_stack_type;
struct perm_vector_type;
struct rng_type;
struct size_t_vector_type;
struct string_cmp_ftype;
struct stringlist_type;
struct test_work_area_type;
struct time_t_vector_type;
struct vector_cmp_ftype;
struct vector_type;

namespace {
struct ecl_table {
    /* Manually added */
    const hash_type * (*hash_safe_cast_const)(const char * value) = nullptr;
    fortio_type * (*fortio_safe_cast)(char * value) = nullptr;
    const ecl::smspec_node & (*ecl_smspec_iget_node_w_node_index)(const ecl_smspec_type *smspec, int node_index) = nullptr;
    const char *(*smspec_node_get_gen_key1)(const ecl::smspec_node *this_);

        /* Defined in 'ert/geometry/geo_surface.hpp' */
        geo_surface_type *(*geo_surface_fload_alloc_irap)(const char *,
                                                          bool) = nullptr;
    bool (*geo_surface_fload_irap_zcoord)(const geo_surface_type *, const char *, double *) = nullptr;
    void (*geo_surface_fprintf_irap_external_zcoord)(const geo_surface_type *, const char *, const double *) = nullptr;
    void (*geo_surface_free)(geo_surface_type *) = nullptr;
    int (*geo_surface_get_size)(const geo_surface_type *) = nullptr;

    /* Defined in 'ert/util/path_stack.hpp' */
    path_stack_type * (*path_stack_alloc)() = nullptr;
    void (*path_stack_free)(path_stack_type *) = nullptr;
    void (*path_stack_pop)(path_stack_type *) = nullptr;
    void (*path_stack_push_cwd)(path_stack_type *) = nullptr;

    /* Defined in 'ert/util/rng.hpp' */
    rng_type * (*rng_alloc)(rng_alg_type, rng_init_mode) = nullptr;
    void (*rng_free)(rng_type *) = nullptr;
    void (*rng_free__)(void *) = nullptr;
    double (*rng_get_double)(rng_type *) = nullptr;
    int (*rng_get_int)(rng_type *, int) = nullptr;
    void (*rng_get_state)(const rng_type *, char *) = nullptr;
    void (*rng_load_state)(rng_type *, const char *) = nullptr;
    void (*rng_rng_init)(rng_type *, rng_type *) = nullptr;
    void (*rng_save_state)(rng_type *, const char *) = nullptr;
    void (*rng_set_state)(rng_type *, const char *) = nullptr;
    void (*rng_shuffle_int)(rng_type *, int *, int) = nullptr;
    double (*rng_std_normal)(rng_type *) = nullptr;

    /* Defined in 'ert/util/vector.hpp' */
    vector_type * (*vector_alloc_NULL_initialized)(int) = nullptr;
    vector_type * (*vector_alloc_new)() = nullptr;
    int (*vector_append_owned_ref)(vector_type *, const void *, int *) = nullptr;
    int (*vector_append_ref)(vector_type *, const void *) = nullptr;
    void (*vector_clear)(vector_type *) = nullptr;
    int (*vector_find)(const vector_type *, const void *) = nullptr;
    void (*vector_free)(vector_type *) = nullptr;
    void (*vector_free__)(void *) = nullptr;
    void * (*vector_get_last)(const vector_type *) = nullptr;
    const void * (*vector_get_last_const)(const vector_type *) = nullptr;
    int (*vector_get_size)(const vector_type *) = nullptr;
    void (*vector_grow_NULL)(vector_type *, int) = nullptr;
    void (*vector_idel)(vector_type *, int) = nullptr;
    void * (*vector_iget)(const vector_type *, int) = nullptr;
    const void * (*vector_iget_const)(const vector_type *, int) = nullptr;
    void (*vector_insert_owned_ref)(vector_type *, int, const void *, int *) = nullptr;
    void (*vector_iset_owned_ref)(vector_type *, int, const void *, int *) = nullptr;
    void (*vector_iset_ref)(vector_type *, int, const void *) = nullptr;
    void * (*vector_pop_back)(vector_type *) = nullptr;
    void * (*vector_pop_front)(vector_type *) = nullptr;
    void (*vector_push_front_ref)(vector_type *, const void *) = nullptr;
    void * (*vector_safe_iget)(const vector_type *, int) = nullptr;
    void (*vector_shrink)(vector_type *, int) = nullptr;
    void (*vector_sort)(vector_type *, vector_cmp_ftype *) = nullptr;

    /* Defined in 'ert/util/type_vector_functions.hpp' */
    int * (*bool_vector_alloc_active_index_list)(const int *, int) = nullptr;
    int * (*bool_vector_alloc_active_list)(const int *) = nullptr;
    int * (*int_vector_alloc_mask)(const int *) = nullptr;

    /* Defined in 'ert/util/test_work_area.hpp' */
    test_work_area_type * (*test_work_area_alloc__)(const char *, bool) = nullptr;
    bool (*test_work_area_copy_parent_content)(const test_work_area_type *, const char *) = nullptr;
    void (*test_work_area_free)(test_work_area_type *) = nullptr;
    const char * (*test_work_area_get_cwd)(const test_work_area_type *) = nullptr;

    /* Defined in 'ert/util/perm_vector.hpp' */
    void (*perm_vector_free)(perm_vector_type *) = nullptr;
    int (*perm_vector_iget)(const perm_vector_type *, int) = nullptr;

    /* Defined in 'ert/util/stringlist.hpp' */
    char ** (*stringlist_alloc_char_copy)(const stringlist_type *) = nullptr;
    char ** (*stringlist_alloc_char_ref)(const stringlist_type *) = nullptr;
    stringlist_type * (*stringlist_alloc_deep_copy)(const stringlist_type *) = nullptr;
    stringlist_type * (*stringlist_alloc_from_split)(const char *, const char *) = nullptr;
    char * (*stringlist_alloc_joined_string)(const stringlist_type *, const char *) = nullptr;
    char * (*stringlist_alloc_joined_substring)(const stringlist_type *, int, int, const char *) = nullptr;
    stringlist_type * (*stringlist_alloc_new)() = nullptr;
    void (*stringlist_append_copy)(stringlist_type *, const char *) = nullptr;
    void (*stringlist_append_stringlist_copy)(stringlist_type *, const stringlist_type *) = nullptr;
    const char * (*stringlist_back)(const stringlist_type *) = nullptr;
    void (*stringlist_clear)(stringlist_type *) = nullptr;
    bool (*stringlist_contains)(const stringlist_type *, const char *) = nullptr;
    void (*stringlist_deep_copy)(stringlist_type *, const stringlist_type *) = nullptr;
    bool (*stringlist_equal)(const stringlist_type *, const stringlist_type *) = nullptr;
    int (*stringlist_find_first)(const stringlist_type *, const char *) = nullptr;
    void (*stringlist_fprintf)(const stringlist_type *, const char *, FILE *) = nullptr;
    stringlist_type * (*stringlist_fread_alloc)(FILE *) = nullptr;
    void (*stringlist_free)(stringlist_type *) = nullptr;
    void (*stringlist_free__)(void *) = nullptr;
    const char * (*stringlist_front)(const stringlist_type *) = nullptr;
    void (*stringlist_fwrite)(const stringlist_type *, FILE *) = nullptr;
    const char * (*stringlist_get_last)(const stringlist_type *) = nullptr;
    int (*stringlist_get_size)(const stringlist_type *) = nullptr;
    void (*stringlist_idel)(stringlist_type *, int) = nullptr;
    bool (*stringlist_iequal)(const stringlist_type *, int, const char *) = nullptr;
    const char * (*stringlist_iget)(const stringlist_type *, int) = nullptr;
    bool (*stringlist_iget_as_bool)(const stringlist_type *, int, bool *) = nullptr;
    int (*stringlist_iget_as_int)(const stringlist_type *, int, bool *) = nullptr;
    char * (*stringlist_iget_copy)(const stringlist_type *, int) = nullptr;
    void (*stringlist_iset_copy)(stringlist_type *, int, const char *) = nullptr;
    void (*stringlist_iset_owned_ref)(stringlist_type *, int, const char *) = nullptr;
    void (*stringlist_iset_ref)(stringlist_type *, int, const char *) = nullptr;
    const char * (*stringlist_safe_iget)(const stringlist_type *, int) = nullptr;
    int (*stringlist_select_matching)(stringlist_type *, const char *) = nullptr;
    void (*stringlist_sort)(stringlist_type *, string_cmp_ftype *) = nullptr;
    bool (*stringlist_unique)(const stringlist_type *) = nullptr;

    /* Defined in 'ert/util/ecl_version.hpp' */
    int (*ecl_version_get_major_version)() = nullptr;
    const char * (*ecl_version_get_micro_version)() = nullptr;
    int (*ecl_version_get_minor_version)() = nullptr;

    /* Defined in 'ert/util/string_util.hpp' */
    int * (*string_util_alloc_active_list)(const char *) = nullptr;
    int * (*string_util_alloc_value_list)(const char *) = nullptr;
    bool (*string_util_init_value_list)(const char *, int *) = nullptr;
    bool (*string_util_update_active_list)(const char *, int *) = nullptr;
    bool (*string_util_update_active_mask)(const char *, int *) = nullptr;

    /* Defined in 'ert/util/buffer.hpp' */
    buffer_type * (*buffer_alloc)(int) = nullptr;
    buffer_type * (*buffer_alloc_private_wrapper)(void *, int) = nullptr;
    void (*buffer_clear)(buffer_type *) = nullptr;
    int (*buffer_fread)(buffer_type *, void *, int, int) = nullptr;
    buffer_type * (*buffer_fread_alloc)(const char *) = nullptr;
    int (*buffer_fread_compressed)(buffer_type *, int, void *, int) = nullptr;
    double (*buffer_fread_double)(buffer_type *) = nullptr;
    int (*buffer_fread_int)(buffer_type *) = nullptr;
    long (*buffer_fread_long)(buffer_type *) = nullptr;
    void (*buffer_fread_realloc)(buffer_type *, const char *) = nullptr;
    void (*buffer_free)(buffer_type *) = nullptr;
    void (*buffer_free_container)(buffer_type *) = nullptr;
    void (*buffer_fseek)(buffer_type *, ssize_t, int) = nullptr;
    void (*buffer_fskip)(buffer_type *, ssize_t) = nullptr;
    void (*buffer_fskip_int)(buffer_type *) = nullptr;
    void (*buffer_fskip_time_t)(buffer_type *) = nullptr;
    int (*buffer_fwrite)(buffer_type *, const void *, int, int) = nullptr;
    void (*buffer_fwrite_char)(buffer_type *, char) = nullptr;
    void (*buffer_fwrite_char_ptr)(buffer_type *, const char *) = nullptr;
    int (*buffer_fwrite_compressed)(buffer_type *, const void *, int) = nullptr;
    void (*buffer_fwrite_double)(buffer_type *, double) = nullptr;
    void (*buffer_fwrite_int)(buffer_type *, int) = nullptr;
    void (*buffer_fwrite_time_t)(buffer_type *, time_t) = nullptr;
    void * (*buffer_get_data)(const buffer_type *) = nullptr;
    int (*buffer_get_offset)(const buffer_type *) = nullptr;
    int (*buffer_get_remaining_size)(const buffer_type *) = nullptr;
    int (*buffer_get_size)(const buffer_type *) = nullptr;
    int (*buffer_get_string_size)(const buffer_type *) = nullptr;
    void (*buffer_memshift)(buffer_type *, int, ssize_t) = nullptr;
    void (*buffer_rewind)(buffer_type *) = nullptr;
    bool (*buffer_search_replace)(buffer_type *, const char *, const char *) = nullptr;
    void (*buffer_shrink_to_fit)(buffer_type *) = nullptr;
    void (*buffer_store)(const buffer_type *, const char *) = nullptr;
    void (*buffer_strcat)(buffer_type *, const char *) = nullptr;
    bool (*buffer_strchr)(buffer_type *, int) = nullptr;
    void (*buffer_stream_fread)(buffer_type *, int, FILE *) = nullptr;
    int (*buffer_stream_fwrite_n)(const buffer_type *, int, ssize_t, FILE *) = nullptr;
    bool (*buffer_strstr)(buffer_type *, const char *) = nullptr;

    /* Defined in 'ert/util/node_ctype.hpp' */
    const char * (*node_ctype_name)(node_ctype) = nullptr;

    /* Defined in 'ert/util/hash.hpp' */
    bool (*hash_add_option)(hash_type *, const char *) = nullptr;
    hash_type * (*hash_alloc)() = nullptr;
    char ** (*hash_alloc_keylist)(const hash_type *) = nullptr;
    int * (*hash_alloc_stringlist)(const hash_type *) = nullptr;
    void (*hash_clear)(hash_type *) = nullptr;
    void (*hash_del)(hash_type *, const char *) = nullptr;
    void (*hash_free)(hash_type *) = nullptr;
    void (*hash_free__)(void *) = nullptr;
    void * (*hash_get)(const hash_type *, const char *) = nullptr;
    int (*hash_get_counter)(const hash_type *, const char *) = nullptr;
    int (*hash_get_int)(const hash_type *, const char *) = nullptr;
    int (*hash_get_size)(const hash_type *) = nullptr;
    char * (*hash_get_string)(const hash_type *, const char *) = nullptr;
    bool (*hash_has_key)(const hash_type *, const char *) = nullptr;
    void (*hash_insert_double)(hash_type *, const char *, double) = nullptr;
    void (*hash_insert_hash_owned_ref)(hash_type *, const char *, const void *, int *) = nullptr;
    void (*hash_insert_int)(hash_type *, const char *, int) = nullptr;
    void (*hash_insert_ref)(hash_type *, const char *, const void *) = nullptr;
    void (*hash_insert_string)(hash_type *, const char *, const char *) = nullptr;
    hash_iter_type * (*hash_iter_alloc)(const hash_type *) = nullptr;
    void (*hash_iter_free)(hash_iter_type *) = nullptr;
    const char * (*hash_iter_get_next_key)(hash_iter_type *) = nullptr;
    void * (*hash_iter_get_next_value)(hash_iter_type *) = nullptr;
    bool (*hash_iter_is_complete)(const hash_iter_type *) = nullptr;
    void (*hash_iter_restart)(hash_iter_type *) = nullptr;
    bool (*hash_key_list_compare)(const hash_type *, const hash_type *) = nullptr;
    void * (*hash_pop)(hash_type *, const char *) = nullptr;
    void (*hash_resize)(hash_type *, int) = nullptr;
    void * (*hash_safe_get)(const hash_type *, const char *) = nullptr;

    /* Defined in 'ert/util/parser.hpp' */
    basic_parser_type * (*basic_parser_alloc)(const char *, const char *, const char *, const char *, const char *, const char *) = nullptr;
    char * (*basic_parser_fread_alloc_file_content)(const char *, const char *, const char *, const char *, const char *) = nullptr;
    void (*basic_parser_free)(basic_parser_type *) = nullptr;
    bool (*basic_parser_fseek_string)(const basic_parser_type *, int *, const char *, bool, bool) = nullptr;
    int * (*basic_parser_tokenize_buffer)(const basic_parser_type *, const char *, bool) = nullptr;
    int * (*basic_parser_tokenize_file)(const basic_parser_type *, const char *, bool) = nullptr;

    /* Defined in 'ert/ecl/ecl_grid.hpp' */
    ecl_grid_type * (*ecl_grid_alloc)(const char *) = nullptr;
    void (*ecl_grid_free)(ecl_grid_type *) = nullptr;
    int (*ecl_grid_get_active_index3)(const ecl_grid_type *, int, int, int) = nullptr;
    int (*ecl_grid_get_active_size)(const ecl_grid_type *) = nullptr;
    double (*ecl_grid_get_cdepth3)(const ecl_grid_type *, int, int, int) = nullptr;
    void (*ecl_grid_get_dims)(const ecl_grid_type *, int *, int *, int *, int *) = nullptr;
    int (*ecl_grid_get_global_index3)(const ecl_grid_type *, int, int, int) = nullptr;
    int (*ecl_grid_get_global_size)(const ecl_grid_type *) = nullptr;
    const char * (*ecl_grid_get_name)(const ecl_grid_type *) = nullptr;
    bool (*ecl_grid_ijk_valid)(const ecl_grid_type *, int, int, int) = nullptr;

    /* Defined in 'ert/ecl/ecl_sum.hpp' */
    bool (*ecl_sum_case_exists)(const char *) = nullptr;
    ecl_sum_type * (*ecl_sum_fread_alloc)(const char *, const int *, const char *, bool, bool, int) = nullptr;
    ecl_sum_type * (*ecl_sum_fread_alloc_case)(const char *, const char *) = nullptr;
    void (*ecl_sum_free)(ecl_sum_type *) = nullptr;
    const char * (*ecl_sum_get_case)(const ecl_sum_type *) = nullptr;
    time_t (*ecl_sum_get_end_time)(const ecl_sum_type *) = nullptr;
    int (*ecl_sum_get_first_report_step)(const ecl_sum_type *) = nullptr;
    double (*ecl_sum_get_general_var)(const ecl_sum_type *, int, const char *) = nullptr;
    int (*ecl_sum_get_general_var_params_index)(const ecl_sum_type *, const char *) = nullptr;
    const char * (*ecl_sum_get_keyword)(const ecl_sum_type *, const char *) = nullptr;
    int (*ecl_sum_get_last_report_step)(const ecl_sum_type *) = nullptr;
    int (*ecl_sum_get_report_step_from_days)(const ecl_sum_type *, double) = nullptr;
    int (*ecl_sum_get_report_step_from_time)(const ecl_sum_type *, time_t) = nullptr;
    time_t (*ecl_sum_get_report_time)(const ecl_sum_type *, int) = nullptr;
    int * (*ecl_sum_get_smspec)(const ecl_sum_type *) = nullptr;
    time_t (*ecl_sum_get_start_time)(const ecl_sum_type *) = nullptr;
    const char * (*ecl_sum_get_wgname)(const ecl_sum_type *, const char *) = nullptr;
    bool (*ecl_sum_has_general_var)(const ecl_sum_type *, const char *) = nullptr;
    bool (*ecl_sum_has_key)(const ecl_sum_type *, const char *) = nullptr;
    bool (*ecl_sum_has_report_step)(const ecl_sum_type *, int) = nullptr;
    double (*ecl_sum_iget)(const ecl_sum_type *, int, int) = nullptr;
    int (*ecl_sum_iget_report_end)(const ecl_sum_type *, int) = nullptr;
    bool (*ecl_sum_report_step_compatible)(const ecl_sum_type *, const ecl_sum_type *) = nullptr;
    void (*ecl_sum_select_matching_general_var_list)(const ecl_sum_type *, const char *, int *) = nullptr;

    /* Defined in 'ert/ecl/ecl_file.hpp' */
    void (*ecl_file_close)(ecl_file_type *) = nullptr;
    ecl_file_type * (*ecl_file_open)(const char *, int) = nullptr;

    /* Defined in 'ert/ecl/ecl_kw.hpp' */
    ecl_kw_type * (*ecl_kw_alloc_new_shared)(const char *, int, int, void *) = nullptr;
    ecl_kw_type * (*ecl_kw_fread_alloc)(int *) = nullptr;
    void (*ecl_kw_free)(ecl_kw_type *) = nullptr;
    bool (*ecl_kw_fseek_kw)(const char *, bool, bool, int *) = nullptr;
    void (*ecl_kw_fwrite_param_fortio)(int *, const char *, int, int, void *) = nullptr;
    int (*ecl_kw_get_data_type)(const ecl_kw_type *) = nullptr;
    int (*ecl_kw_get_size)(const ecl_kw_type *) = nullptr;
    void * (*ecl_kw_get_void_ptr)(const ecl_kw_type *) = nullptr;
    bool (*ecl_kw_is_kw_file)(int *) = nullptr;

    /* Defined in 'ert/ecl/ecl_kw_grdecl.hpp' */
    void (*ecl_kw_fprintf_grdecl)(const int *, int *) = nullptr;
    int * (*ecl_kw_fscanf_alloc_grdecl_data)(int *, int, int) = nullptr;
    bool (*ecl_kw_grdecl_fseek_kw)(const char *, bool, int *) = nullptr;
    bool (*ecl_kw_grdecl_fseek_next_kw)(int *) = nullptr;

    /* Defined in 'ert/ecl/ecl_io_config.hpp' */
    ecl_io_config_type * (*ecl_io_config_alloc)(bool, bool, bool) = nullptr;
    void (*ecl_io_config_free)(ecl_io_config_type *) = nullptr;
    bool (*ecl_io_config_get_formatted)(ecl_io_config_type *) = nullptr;
    bool (*ecl_io_config_get_unified_restart)(ecl_io_config_type *) = nullptr;

    /* Defined in 'ert/ecl/ecl_smspec.hpp' */
    const char * (*ecl_smspec_get_join_string)(const ecl_smspec_type *) = nullptr;
    bool (*ecl_smspec_has_general_var)(const ecl_smspec_type *, const char *) = nullptr;
    int (*ecl_smspec_identify_var_type)(const char *) = nullptr;
    int (*ecl_smspec_num_nodes)(const ecl_smspec_type *) = nullptr;

    /* Defined in 'ert/ecl/ecl_type.hpp' */
    char * (*ecl_type_alloc_name)(const ecl_data_type) = nullptr;
    ecl_data_type (*ecl_type_create_from_type)(const ecl_type_enum) = nullptr;
    int (*ecl_type_get_sizeof_ctype)(const ecl_data_type) = nullptr;
    ecl_type_enum (*ecl_type_get_type)(const ecl_data_type) = nullptr;
    bool (*ecl_type_is_double)(const ecl_data_type) = nullptr;
    bool (*ecl_type_is_equal)(const ecl_data_type, const ecl_data_type) = nullptr;
    bool (*ecl_type_is_float)(const ecl_data_type) = nullptr;
    bool (*ecl_type_is_int)(const ecl_data_type) = nullptr;

    /* Defined in 'ert/ecl/ecl_util.hpp' */
    char * (*ecl_util_alloc_exfilename)(const char *, const char *, ecl_file_enum, bool, int) = nullptr;
    char * (*ecl_util_alloc_filename)(const char *, const char *, ecl_file_enum, bool, int) = nullptr;
    bool (*ecl_util_fmt_file)(const char *, bool *) = nullptr;
    ecl_file_enum (*ecl_util_get_file_type)(const char *, bool *, int *) = nullptr;
    int (*ecl_util_get_month_nr)(const char *) = nullptr;
    int (*ecl_util_get_num_cpu)(const char *) = nullptr;
    time_t (*ecl_util_get_start_date)(const char *) = nullptr;
    ert_ecl_unit_enum (*ecl_util_get_unit_set)(const char *) = nullptr;
    void (*ecl_util_memcpy_typed_data)(void *, const void *, int, int, int) = nullptr;
    bool (*ecl_util_valid_basename_fmt)(const char *) = nullptr;

    /* Defined in 'ert/util/double_vector.h' */
    double_vector_type * (*double_vector_alloc)(int, double) = nullptr;
    double_vector_type * (*double_vector_alloc_copy)(const double_vector_type *) = nullptr;
    int * (*double_vector_alloc_rsort_perm)(const double_vector_type *) = nullptr;
    double_vector_type * (*double_vector_alloc_shared_wrapper)(int, double, double *, int) = nullptr;
    int * (*double_vector_alloc_sort_perm)(const double_vector_type *) = nullptr;
    void (*double_vector_append)(double_vector_type *, double) = nullptr;
    int (*double_vector_element_size)(const double_vector_type *) = nullptr;
    double_vector_type * (*double_vector_fread_alloc)(FILE *) = nullptr;
    void (*double_vector_free)(double_vector_type *) = nullptr;
    void (*double_vector_fwrite)(const double_vector_type *, FILE *) = nullptr;
    double (*double_vector_get_default)(const double_vector_type *) = nullptr;
    double * (*double_vector_get_ptr)(const double_vector_type *) = nullptr;
    double (*double_vector_iget)(const double_vector_type *, int) = nullptr;
    void (*double_vector_iset)(double_vector_type *, int, double) = nullptr;
    void (*double_vector_iset_default)(double_vector_type *, int, double) = nullptr;
    void (*double_vector_memcpy)(double_vector_type *, const double_vector_type *) = nullptr;
    void (*double_vector_memcpy_from_data)(double_vector_type *, const double *, int) = nullptr;
    void (*double_vector_permute)(double_vector_type *, const int *) = nullptr;
    void (*double_vector_reset)(double_vector_type *) = nullptr;
    void (*double_vector_resize)(double_vector_type *, int, double) = nullptr;
    double (*double_vector_safe_iget)(const double_vector_type *, int) = nullptr;
    void (*double_vector_set_default)(double_vector_type *, double) = nullptr;
    int (*double_vector_size)(const double_vector_type *) = nullptr;

    /* Defined in 'ert/util/bool_vector.h' */
    bool_vector_type * (*bool_vector_alloc)(int, bool) = nullptr;
    bool_vector_type * (*bool_vector_alloc_copy)(const bool_vector_type *) = nullptr;
    int (*bool_vector_count_equal)(const bool_vector_type *, bool) = nullptr;
    void (*bool_vector_fread)(bool_vector_type *, FILE *) = nullptr;
    void (*bool_vector_free)(bool_vector_type *) = nullptr;
    void (*bool_vector_fwrite)(const bool_vector_type *, FILE *) = nullptr;
    bool * (*bool_vector_get_ptr)(const bool_vector_type *) = nullptr;
    int (*bool_vector_iget)(const bool_vector_type *, int) = nullptr;
    void (*bool_vector_iset)(bool_vector_type *, int, bool) = nullptr;
    void (*bool_vector_reset)(bool_vector_type *) = nullptr;
    void (*bool_vector_resize)(bool_vector_type *, int, bool) = nullptr;
    int (*bool_vector_safe_iget)(const bool_vector_type *, int) = nullptr;
    void (*bool_vector_set_all)(bool_vector_type *, bool) = nullptr;
    void (*bool_vector_set_default)(bool_vector_type *, bool) = nullptr;
    int (*bool_vector_size)(const bool_vector_type *) = nullptr;

    /* Defined in 'ert/util/long_vector.h' */
    long_vector_type * (*long_vector_alloc)(int, long) = nullptr;
    void (*long_vector_append)(long_vector_type *, long) = nullptr;
    void (*long_vector_free)(long_vector_type *) = nullptr;
    long (*long_vector_iget)(const long_vector_type *, int) = nullptr;
    int (*long_vector_size)(const long_vector_type *) = nullptr;

    /* Defined in 'ert/util/time_t_vector.h' */
    time_t_vector_type * (*time_t_vector_alloc)(int, time_t) = nullptr;
    time_t_vector_type * (*time_t_vector_alloc_shared_wrapper)(int, time_t, time_t *, int) = nullptr;
    int * (*time_t_vector_alloc_sort_perm)(const time_t_vector_type *) = nullptr;
    void (*time_t_vector_append)(time_t_vector_type *, time_t) = nullptr;
    int (*time_t_vector_equal)(const time_t_vector_type *, const time_t_vector_type *) = nullptr;
    time_t_vector_type * (*time_t_vector_fread_alloc)(FILE *) = nullptr;
    void (*time_t_vector_free)(time_t_vector_type *) = nullptr;
    void (*time_t_vector_fwrite)(const time_t_vector_type *, FILE *) = nullptr;
    int (*time_t_vector_iget)(const time_t_vector_type *, int) = nullptr;
    void (*time_t_vector_iset)(time_t_vector_type *, int, time_t) = nullptr;
    void (*time_t_vector_iset_block)(time_t_vector_type *, int, int, time_t) = nullptr;
    void (*time_t_vector_permute)(time_t_vector_type *, const int *) = nullptr;
    void (*time_t_vector_reset)(time_t_vector_type *) = nullptr;
    void (*time_t_vector_resize)(time_t_vector_type *, int, time_t) = nullptr;
    int (*time_t_vector_safe_iget)(const time_t_vector_type *, int) = nullptr;
    int (*time_t_vector_size)(const time_t_vector_type *) = nullptr;

    /* Defined in 'ert/util/buffer_string.h' */
    char * (*buffer_fread_alloc_string)(int *) = nullptr;
    const char * (*buffer_fread_string)(int *) = nullptr;
    void (*buffer_fwrite_string)(int *, const char *) = nullptr;

    /* Defined in 'ert/util/util.h' */
    void (*util_abort__)(const char *, const char *, int, const char *) = nullptr;
    int (*util_addmode_if_owner)(const char *, mode_t) = nullptr;
    char * (*util_alloc_abs_path)(const char *) = nullptr;
    char * (*util_alloc_atlink_target)(const char *, const char *) = nullptr;
    void * (*util_alloc_copy)(const void *, int) = nullptr;
    char * (*util_alloc_cwd)() = nullptr;
    char * (*util_alloc_date_stamp_utc)() = nullptr;
    char * (*util_alloc_dequoted_copy)(const char *) = nullptr;
    void (*util_alloc_file_components)(const char *, char **, char **, char **) = nullptr;
    char * (*util_alloc_filename)(const char *, const char *, const char *) = nullptr;
    char * (*util_alloc_joined_string)(const char **, int, const char *) = nullptr;
    char * (*util_alloc_link_target)(const char *) = nullptr;
    char * (*util_alloc_normal_path)(const char *) = nullptr;
    char * (*util_alloc_realpath)(const char *) = nullptr;
    char * (*util_alloc_realpath__)(const char *) = nullptr;
    char * (*util_alloc_rel_path)(const char *, const char *) = nullptr;
    char * (*util_alloc_sprintf)(const char *) = nullptr;
    char * (*util_alloc_sprintf_va)(const char *, va_list) = nullptr;
    char * (*util_alloc_string_copy)(const char *) = nullptr;
    char ** (*util_alloc_stringlist_copy)(const char *const *, int) = nullptr;
    char * (*util_alloc_strupr_copy)(const char *) = nullptr;
    char * (*util_alloc_substring_copy)(const char *, int, int) = nullptr;
    char * (*util_alloc_tmp_file)(const char *, const char *, bool) = nullptr;
    void (*util_binary_split_string)(const char *, const char *, bool, char **, char **) = nullptr;
    void * (*util_calloc)(int, int) = nullptr;
    int (*util_chdir)(const char *) = nullptr;
    int (*util_chdir_file)(const char *) = nullptr;
    void (*util_clamp_double)(double *, double, double) = nullptr;
    void (*util_clear_directory)(const char *, bool, bool) = nullptr;
    void (*util_copy_directory)(const char *, const char *) = nullptr;
    int (*util_copy_file)(const char *, const char *) = nullptr;
    unsigned int (*util_dev_urandom_seed)() = nullptr;
    double (*util_difftime_days)(time_t, time_t) = nullptr;
    double (*util_difftime_seconds)(time_t, time_t) = nullptr;
    double (*util_double_max)(double, double) = nullptr;
    double (*util_double_min)(double, double) = nullptr;
    void (*util_double_to_float)(float *, const double *, int) = nullptr;
    double (*util_double_vector_mean)(int, const double *) = nullptr;
    double (*util_double_vector_stddev)(int, const double *) = nullptr;
    void (*util_endian_flip_vector)(void *, int, int) = nullptr;
    int (*util_entry_exists)(const char *) = nullptr;
    int (*util_entry_readable)(const char *) = nullptr;
    void (*util_exit)(const char *) = nullptr;
    int (*util_file_exists)(const char *) = nullptr;
    time_t (*util_file_mtime)(const char *) = nullptr;
    int (*util_file_readable)(const char *) = nullptr;
    int (*util_file_size)(const char *) = nullptr;
    float (*util_float_max)(float, float) = nullptr;
    void (*util_float_to_double)(double *, const float *, int) = nullptr;
    int (*util_fmt_bit8)(const char *) = nullptr;
    int (*util_fnmatch)(const char *, const char *) = nullptr;
    FILE * (*util_fopen)(const char *, const char *) = nullptr;
    FILE * (*util_fopen__)(const char *, const char *) = nullptr;
    void (*util_fread)(void *, int, int, FILE *, const char *) = nullptr;
    char * (*util_fread_alloc_file_content)(const char *, int *) = nullptr;
    char * (*util_fread_alloc_string)(FILE *) = nullptr;
    double (*util_fread_double)(FILE *) = nullptr;
    int (*util_fread_int)(FILE *) = nullptr;
    long (*util_fread_long)(FILE *) = nullptr;
    char * (*util_fread_realloc_string)(char *, FILE *) = nullptr;
    time_t (*util_fread_time_t)(FILE *) = nullptr;
    void (*util_free_NULL_terminated_stringlist)(char **) = nullptr;
    void (*util_free_stringlist)(char **, int) = nullptr;
    char * (*util_fscanf_alloc_line)(FILE *, bool *) = nullptr;
    char * (*util_fscanf_alloc_token)(FILE *) = nullptr;
    char * (*util_fscanf_alloc_upto)(FILE *, const char *, bool) = nullptr;
    int (*util_fseek_string)(FILE *, const char *, bool, bool) = nullptr;
    void (*util_fskip_int)(FILE *) = nullptr;
    void (*util_fskip_lines)(FILE *, int) = nullptr;
    offset_type (*util_ftell)(FILE *) = nullptr;
    void (*util_fwrite)(const void *, int, int, FILE *, const char *) = nullptr;
    void (*util_fwrite_double)(double, FILE *) = nullptr;
    void (*util_fwrite_int)(int, FILE *) = nullptr;
    void (*util_fwrite_long)(long, FILE *) = nullptr;
    void (*util_fwrite_string)(const char *, FILE *) = nullptr;
    void (*util_fwrite_time_t)(time_t, FILE *) = nullptr;
    int (*util_get_current_linenr)(FILE *) = nullptr;
    void (*util_inplace_forward_days_utc)(time_t *, double) = nullptr;
    void (*util_inplace_forward_seconds_utc)(time_t *, double) = nullptr;
    void (*util_install_signals)() = nullptr;
    int (*util_int_format_count)(const char *) = nullptr;
    int (*util_int_max)(int, int) = nullptr;
    int (*util_int_min)(int, int) = nullptr;
    int (*util_is_abs_path)(const char *) = nullptr;
    int (*util_is_directory)(const char *) = nullptr;
    int (*util_is_executable)(const char *) = nullptr;
    int (*util_is_file)(const char *) = nullptr;
    int (*util_is_link)(const char *) = nullptr;
    time_t (*util_make_date_utc)(int, int, int) = nullptr;
    void (*util_make_path)(const char *) = nullptr;
    void * (*util_malloc)() = nullptr;
    FILE * (*util_mkdir_fopen)(const char *, const char *) = nullptr;
    int (*util_mkdir_p)(const char *) = nullptr;
    void (*util_move_file)(const char *, const char *) = nullptr;
    void (*util_path_split)(const char *, int *, char ***) = nullptr;
    void * (*util_realloc)(void *, int) = nullptr;
    void * (*util_realloc_copy)(void *, const void *, int) = nullptr;
    char * (*util_realloc_sprintf)(char *, const char *) = nullptr;
    char * (*util_realloc_string_copy)(char *, const char *) = nullptr;
    int (*util_same_file)(const char *, const char *) = nullptr;
    void (*util_set_date_values_utc)(time_t, int *, int *, int *) = nullptr;
    void (*util_set_datetime_values_utc)(time_t, int *, int *, int *, int *, int *, int *) = nullptr;
    int (*util_size_t_max)(int, int) = nullptr;
    pid_t (*util_spawn)(const char *, int, const char **, const char *, const char *) = nullptr;
    int (*util_spawn_blocking)(const char *, int, const char **, const char *, const char *) = nullptr;
    char * (*util_split_alloc_dirname)(const char *) = nullptr;
    char * (*util_split_alloc_filename)(const char *) = nullptr;
    void (*util_split_string)(const char *, const char *, int *, char ***) = nullptr;
    int (*util_sscanf_bool)(const char *, bool *) = nullptr;
    int (*util_sscanf_bytesize)(const char *, int *) = nullptr;
    int (*util_sscanf_date_utc)(const char *, time_t *) = nullptr;
    int (*util_sscanf_double)(const char *, double *) = nullptr;
    int (*util_sscanf_int)(const char *, int *) = nullptr;
    int (*util_sscanf_isodate)(const char *, time_t *) = nullptr;
    int (*util_sscanf_octal_int)(const char *, int *) = nullptr;
    int (*util_sscanf_percent)(const char *, double *) = nullptr;
    char * (*util_strcat_realloc)(char *, const char *) = nullptr;
    int (*util_strcmp_int)(const char *, const char *) = nullptr;
    int (*util_string_equal)(const char *, const char *) = nullptr;
    int (*util_string_has_wildcard)(const char *) = nullptr;
    int (*util_string_isspace)(const char *) = nullptr;
    int (*util_string_match)(const char *, const char *) = nullptr;
    char * (*util_string_replace_alloc)(const char *, const char *, const char *) = nullptr;
    char * (*util_string_replacen_alloc)(const char *, int, const char **, const char **) = nullptr;
    void (*util_strupr)(char *) = nullptr;
    time_t (*util_time_t_max)(time_t, time_t) = nullptr;
    void (*util_time_utc)(time_t *, struct tm *) = nullptr;
    int (*util_try_lockf)(const char *, mode_t, int *) = nullptr;
    void (*util_unlink_existing)(const char *) = nullptr;
    void (*util_update_double_max_min)(double, double *, double *) = nullptr;
    void (*util_update_float_max_min)(float, float *, float *) = nullptr;
    void (*util_update_int_max_min)(int, int *, int *) = nullptr;
    void (*util_usleep)(unsigned long) = nullptr;

    /* Defined in 'ert/util/int_vector.h' */
    int_vector_type * (*int_vector_alloc)(int, int) = nullptr;
    int_vector_type * (*int_vector_alloc_copy)(const int_vector_type *) = nullptr;
    int * (*int_vector_alloc_data_copy)(const int_vector_type *) = nullptr;
    void (*int_vector_append)(int_vector_type *, int) = nullptr;
    int (*int_vector_contains)(const int_vector_type *, int) = nullptr;
    int (*int_vector_contains_sorted)(const int_vector_type *, int) = nullptr;
    int (*int_vector_equal)(const int_vector_type *, const int_vector_type *) = nullptr;
    void (*int_vector_fread)(int_vector_type *, FILE *) = nullptr;
    void (*int_vector_free)(int_vector_type *) = nullptr;
    void (*int_vector_fwrite)(const int_vector_type *, FILE *) = nullptr;
    const int * (*int_vector_get_const_ptr)(const int_vector_type *) = nullptr;
    int (*int_vector_get_first)(const int_vector_type *) = nullptr;
    int (*int_vector_get_last)(const int_vector_type *) = nullptr;
    int * (*int_vector_get_ptr)(const int_vector_type *) = nullptr;
    int (*int_vector_iget)(const int_vector_type *, int) = nullptr;
    int (*int_vector_index_sorted)(const int_vector_type *, int) = nullptr;
    void (*int_vector_init_range)(int_vector_type *, int, int, int) = nullptr;
    void (*int_vector_iset)(int_vector_type *, int, int) = nullptr;
    void (*int_vector_iset_block)(int_vector_type *, int, int, int) = nullptr;
    void (*int_vector_iset_default)(int_vector_type *, int, int) = nullptr;
    void (*int_vector_memcpy)(int_vector_type *, const int_vector_type *) = nullptr;
    void (*int_vector_reset)(int_vector_type *) = nullptr;
    void (*int_vector_resize)(int_vector_type *, int, int) = nullptr;
    int (*int_vector_safe_iget)(const int_vector_type *, int) = nullptr;
    void (*int_vector_select_unique)(int_vector_type *) = nullptr;
    void (*int_vector_set_default)(int_vector_type *, int) = nullptr;
    void (*int_vector_shrink)(int_vector_type *) = nullptr;
    int (*int_vector_size)(const int_vector_type *) = nullptr;
    void (*int_vector_sort)(int_vector_type *) = nullptr;

    /* Defined in 'ert/util/size_t_vector.h' */
    void (*size_t_vector_append)(size_t_vector_type *, size_t) = nullptr;
    void (*size_t_vector_free)(size_t_vector_type *) = nullptr;
    int (*size_t_vector_iget)(const size_t_vector_type *, int) = nullptr;
    void (*size_t_vector_iset_default)(size_t_vector_type *, int, size_t) = nullptr;
    int (*size_t_vector_safe_iget)(const size_t_vector_type *, int) = nullptr;

    /* Defined in 'ert/ecl/smspec_node.h' */
    char * (*smspec_alloc_block_ijk_key)(const char *, const char *, int, int, int) = nullptr;

    /* Defined in 'ert/ecl/fortio.h' */
    fortio_type * (*fortio_alloc_FILE_wrapper)(const char *, int, int, int, FILE *) = nullptr;
    void (*fortio_fclose)(fortio_type *) = nullptr;
    void (*fortio_free_FILE_wrapper)(fortio_type *) = nullptr;
    fortio_type * (*fortio_open_reader)(const char *, int, int) = nullptr;
    fortio_type * (*fortio_open_writer)(const char *, int, int) = nullptr;
} t;
}// namespace

namespace {
std::string libecl_path;
void *dlhandle = nullptr;

template <class R, class... Args>
void link(const char *key, R (*fn)(Args...)) noexcept {
    auto sym = dlsym(dlhandle, key);
    if (!sym) {
        PyErr_Format(PyExc_ImportError, "Symbol %s not found in %s", key, libecl_path.c_str());
    }
    fn = reinterpret_cast<R(*)(Args...)>(sym);
}
} // namespace

extern "C" void res_link_with_ecl(const char * lib_path) noexcept {
    if (dlhandle)
        return;
    dlhandle = dlopen(lib_path, RTLD_NOW | RTLD_GLOBAL);
    if (!dlhandle) {
        PyErr_Format(PyExc_ImportError, "Failed to load libecl at %s: %s", lib_path, dlerror());
        return;
    }
    libecl_path = lib_path;

    /* Manually added */
    link("hash_safe_cast_const", t.hash_safe_cast_const);
    link("fortio_safe_cast", t.fortio_safe_cast);
    link("_Z33ecl_smspec_iget_node_w_node_indexPK17ecl_smspec_structi", t.ecl_smspec_iget_node_w_node_index);
    link("_ZNK3ecl11smspec_node12get_gen_key1Ev", t.smspec_node_get_gen_key1);

    /* Defined in 'ert/geometry/geo_surface.hpp' */
    link("geo_surface_fload_alloc_irap", t.geo_surface_fload_alloc_irap);
    link("geo_surface_fload_irap_zcoord", t.geo_surface_fload_irap_zcoord);
    link("geo_surface_fprintf_irap_external_zcoord", t.geo_surface_fprintf_irap_external_zcoord);
    link("geo_surface_free", t.geo_surface_free);
    link("geo_surface_get_size", t.geo_surface_get_size);

    /* Defined in 'ert/util/path_stack.hpp' */
    link("path_stack_alloc", t.path_stack_alloc);
    link("path_stack_free", t.path_stack_free);
    link("path_stack_pop", t.path_stack_pop);
    link("path_stack_push_cwd", t.path_stack_push_cwd);

    /* Defined in 'ert/util/rng.hpp' */
    link("rng_alloc", t.rng_alloc);
    link("rng_free", t.rng_free);
    link("rng_free__", t.rng_free__);
    link("rng_get_double", t.rng_get_double);
    link("rng_get_int", t.rng_get_int);
    link("rng_get_state", t.rng_get_state);
    link("rng_load_state", t.rng_load_state);
    link("rng_rng_init", t.rng_rng_init);
    link("rng_save_state", t.rng_save_state);
    link("rng_set_state", t.rng_set_state);
    link("rng_shuffle_int", t.rng_shuffle_int);
    link("rng_std_normal", t.rng_std_normal);

    /* Defined in 'ert/util/vector.hpp' */
    link("vector_alloc_NULL_initialized", t.vector_alloc_NULL_initialized);
    link("vector_alloc_new", t.vector_alloc_new);
    link("vector_append_owned_ref", t.vector_append_owned_ref);
    link("vector_append_ref", t.vector_append_ref);
    link("vector_clear", t.vector_clear);
    link("vector_find", t.vector_find);
    link("vector_free", t.vector_free);
    link("vector_free__", t.vector_free__);
    link("vector_get_last", t.vector_get_last);
    link("vector_get_last_const", t.vector_get_last_const);
    link("vector_get_size", t.vector_get_size);
    link("vector_grow_NULL", t.vector_grow_NULL);
    link("vector_idel", t.vector_idel);
    link("vector_iget", t.vector_iget);
    link("vector_iget_const", t.vector_iget_const);
    link("vector_insert_owned_ref", t.vector_insert_owned_ref);
    link("vector_iset_owned_ref", t.vector_iset_owned_ref);
    link("vector_iset_ref", t.vector_iset_ref);
    link("vector_pop_back", t.vector_pop_back);
    link("vector_pop_front", t.vector_pop_front);
    link("vector_push_front_ref", t.vector_push_front_ref);
    link("vector_safe_iget", t.vector_safe_iget);
    link("vector_shrink", t.vector_shrink);
    link("vector_sort", t.vector_sort);

    /* Defined in 'ert/util/type_vector_functions.hpp' */
    link("bool_vector_alloc_active_index_list", t.bool_vector_alloc_active_index_list);
    link("bool_vector_alloc_active_list", t.bool_vector_alloc_active_list);
    link("int_vector_alloc_mask", t.int_vector_alloc_mask);

    /* Defined in 'ert/util/test_work_area.hpp' */
    link("test_work_area_alloc__", t.test_work_area_alloc__);
    link("test_work_area_copy_parent_content", t.test_work_area_copy_parent_content);
    link("test_work_area_free", t.test_work_area_free);
    link("test_work_area_get_cwd", t.test_work_area_get_cwd);

    /* Defined in 'ert/util/perm_vector.hpp' */
    link("perm_vector_free", t.perm_vector_free);
    link("perm_vector_iget", t.perm_vector_iget);

    /* Defined in 'ert/util/stringlist.hpp' */
    link("stringlist_alloc_char_copy", t.stringlist_alloc_char_copy);
    link("stringlist_alloc_char_ref", t.stringlist_alloc_char_ref);
    link("stringlist_alloc_deep_copy", t.stringlist_alloc_deep_copy);
    link("stringlist_alloc_from_split", t.stringlist_alloc_from_split);
    link("stringlist_alloc_joined_string", t.stringlist_alloc_joined_string);
    link("stringlist_alloc_joined_substring", t.stringlist_alloc_joined_substring);
    link("stringlist_alloc_new", t.stringlist_alloc_new);
    link("stringlist_append_copy", t.stringlist_append_copy);
    link("stringlist_append_stringlist_copy", t.stringlist_append_stringlist_copy);
    link("stringlist_back", t.stringlist_back);
    link("stringlist_clear", t.stringlist_clear);
    link("stringlist_contains", t.stringlist_contains);
    link("stringlist_deep_copy", t.stringlist_deep_copy);
    link("stringlist_equal", t.stringlist_equal);
    link("stringlist_find_first", t.stringlist_find_first);
    link("stringlist_fprintf", t.stringlist_fprintf);
    link("stringlist_fread_alloc", t.stringlist_fread_alloc);
    link("stringlist_free", t.stringlist_free);
    link("stringlist_free__", t.stringlist_free__);
    link("stringlist_front", t.stringlist_front);
    link("stringlist_fwrite", t.stringlist_fwrite);
    link("stringlist_get_last", t.stringlist_get_last);
    link("stringlist_get_size", t.stringlist_get_size);
    link("stringlist_idel", t.stringlist_idel);
    link("stringlist_iequal", t.stringlist_iequal);
    link("stringlist_iget", t.stringlist_iget);
    link("stringlist_iget_as_bool", t.stringlist_iget_as_bool);
    link("stringlist_iget_as_int", t.stringlist_iget_as_int);
    link("stringlist_iget_copy", t.stringlist_iget_copy);
    link("stringlist_iset_copy", t.stringlist_iset_copy);
    link("stringlist_iset_owned_ref", t.stringlist_iset_owned_ref);
    link("stringlist_iset_ref", t.stringlist_iset_ref);
    link("stringlist_safe_iget", t.stringlist_safe_iget);
    link("stringlist_select_matching", t.stringlist_select_matching);
    link("stringlist_sort", t.stringlist_sort);
    link("stringlist_unique", t.stringlist_unique);

    /* Defined in 'ert/util/ecl_version.hpp' */
    link("ecl_version_get_major_version", t.ecl_version_get_major_version);
    link("ecl_version_get_micro_version", t.ecl_version_get_micro_version);
    link("ecl_version_get_minor_version", t.ecl_version_get_minor_version);

    /* Defined in 'ert/util/string_util.hpp' */
    link("string_util_alloc_active_list", t.string_util_alloc_active_list);
    link("string_util_alloc_value_list", t.string_util_alloc_value_list);
    link("string_util_init_value_list", t.string_util_init_value_list);
    link("string_util_update_active_list", t.string_util_update_active_list);
    link("string_util_update_active_mask", t.string_util_update_active_mask);

    /* Defined in 'ert/util/buffer.hpp' */
    link("buffer_alloc", t.buffer_alloc);
    link("buffer_alloc_private_wrapper", t.buffer_alloc_private_wrapper);
    link("buffer_clear", t.buffer_clear);
    link("buffer_fread", t.buffer_fread);
    link("buffer_fread_alloc", t.buffer_fread_alloc);
    link("buffer_fread_compressed", t.buffer_fread_compressed);
    link("buffer_fread_double", t.buffer_fread_double);
    link("buffer_fread_int", t.buffer_fread_int);
    link("buffer_fread_long", t.buffer_fread_long);
    link("buffer_fread_realloc", t.buffer_fread_realloc);
    link("buffer_free", t.buffer_free);
    link("buffer_free_container", t.buffer_free_container);
    link("buffer_fseek", t.buffer_fseek);
    link("buffer_fskip", t.buffer_fskip);
    link("buffer_fskip_int", t.buffer_fskip_int);
    link("buffer_fskip_time_t", t.buffer_fskip_time_t);
    link("buffer_fwrite", t.buffer_fwrite);
    link("buffer_fwrite_char", t.buffer_fwrite_char);
    link("buffer_fwrite_char_ptr", t.buffer_fwrite_char_ptr);
    link("buffer_fwrite_compressed", t.buffer_fwrite_compressed);
    link("buffer_fwrite_double", t.buffer_fwrite_double);
    link("buffer_fwrite_int", t.buffer_fwrite_int);
    link("buffer_fwrite_time_t", t.buffer_fwrite_time_t);
    link("buffer_get_data", t.buffer_get_data);
    link("buffer_get_offset", t.buffer_get_offset);
    link("buffer_get_remaining_size", t.buffer_get_remaining_size);
    link("buffer_get_size", t.buffer_get_size);
    link("buffer_get_string_size", t.buffer_get_string_size);
    link("buffer_memshift", t.buffer_memshift);
    link("buffer_rewind", t.buffer_rewind);
    link("buffer_search_replace", t.buffer_search_replace);
    link("buffer_shrink_to_fit", t.buffer_shrink_to_fit);
    link("buffer_store", t.buffer_store);
    link("buffer_strcat", t.buffer_strcat);
    link("buffer_strchr", t.buffer_strchr);
    link("buffer_stream_fread", t.buffer_stream_fread);
    link("buffer_stream_fwrite_n", t.buffer_stream_fwrite_n);
    link("buffer_strstr", t.buffer_strstr);

    /* Defined in 'ert/util/node_ctype.hpp' */
    link("node_ctype_name", t.node_ctype_name);

    /* Defined in 'ert/util/hash.hpp' */
    link("hash_add_option", t.hash_add_option);
    link("hash_alloc", t.hash_alloc);
    link("hash_alloc_keylist", t.hash_alloc_keylist);
    link("hash_alloc_stringlist", t.hash_alloc_stringlist);
    link("hash_clear", t.hash_clear);
    link("hash_del", t.hash_del);
    link("hash_free", t.hash_free);
    link("hash_free__", t.hash_free__);
    link("hash_get", t.hash_get);
    link("hash_get_counter", t.hash_get_counter);
    link("hash_get_int", t.hash_get_int);
    link("hash_get_size", t.hash_get_size);
    link("hash_get_string", t.hash_get_string);
    link("hash_has_key", t.hash_has_key);
    link("hash_insert_double", t.hash_insert_double);
    link("hash_insert_hash_owned_ref", t.hash_insert_hash_owned_ref);
    link("hash_insert_int", t.hash_insert_int);
    link("hash_insert_ref", t.hash_insert_ref);
    link("hash_insert_string", t.hash_insert_string);
    link("hash_iter_alloc", t.hash_iter_alloc);
    link("hash_iter_free", t.hash_iter_free);
    link("hash_iter_get_next_key", t.hash_iter_get_next_key);
    link("hash_iter_get_next_value", t.hash_iter_get_next_value);
    link("hash_iter_is_complete", t.hash_iter_is_complete);
    link("hash_iter_restart", t.hash_iter_restart);
    link("hash_key_list_compare", t.hash_key_list_compare);
    link("hash_pop", t.hash_pop);
    link("hash_resize", t.hash_resize);
    link("hash_safe_get", t.hash_safe_get);

    /* Defined in 'ert/util/parser.hpp' */
    link("basic_parser_alloc", t.basic_parser_alloc);
    link("basic_parser_fread_alloc_file_content", t.basic_parser_fread_alloc_file_content);
    link("basic_parser_free", t.basic_parser_free);
    link("basic_parser_fseek_string", t.basic_parser_fseek_string);
    link("basic_parser_tokenize_buffer", t.basic_parser_tokenize_buffer);
    link("basic_parser_tokenize_file", t.basic_parser_tokenize_file);

    /* Defined in 'ert/ecl/ecl_grid.hpp' */
    link("ecl_grid_alloc", t.ecl_grid_alloc);
    link("ecl_grid_free", t.ecl_grid_free);
    link("ecl_grid_get_active_index3", t.ecl_grid_get_active_index3);
    link("ecl_grid_get_active_size", t.ecl_grid_get_active_size);
    link("ecl_grid_get_cdepth3", t.ecl_grid_get_cdepth3);
    link("ecl_grid_get_dims", t.ecl_grid_get_dims);
    link("ecl_grid_get_global_index3", t.ecl_grid_get_global_index3);
    link("ecl_grid_get_global_size", t.ecl_grid_get_global_size);
    link("ecl_grid_get_name", t.ecl_grid_get_name);
    link("ecl_grid_ijk_valid", t.ecl_grid_ijk_valid);

    /* Defined in 'ert/ecl/ecl_sum.hpp' */
    link("ecl_sum_case_exists", t.ecl_sum_case_exists);
    link("ecl_sum_fread_alloc", t.ecl_sum_fread_alloc);
    link("ecl_sum_fread_alloc_case", t.ecl_sum_fread_alloc_case);
    link("ecl_sum_free", t.ecl_sum_free);
    link("ecl_sum_get_case", t.ecl_sum_get_case);
    link("ecl_sum_get_end_time", t.ecl_sum_get_end_time);
    link("ecl_sum_get_first_report_step", t.ecl_sum_get_first_report_step);
    link("ecl_sum_get_general_var", t.ecl_sum_get_general_var);
    link("ecl_sum_get_general_var_params_index", t.ecl_sum_get_general_var_params_index);
    link("ecl_sum_get_keyword", t.ecl_sum_get_keyword);
    link("ecl_sum_get_last_report_step", t.ecl_sum_get_last_report_step);
    link("ecl_sum_get_report_step_from_days", t.ecl_sum_get_report_step_from_days);
    link("ecl_sum_get_report_step_from_time", t.ecl_sum_get_report_step_from_time);
    link("ecl_sum_get_report_time", t.ecl_sum_get_report_time);
    link("ecl_sum_get_smspec", t.ecl_sum_get_smspec);
    link("ecl_sum_get_start_time", t.ecl_sum_get_start_time);
    link("ecl_sum_get_wgname", t.ecl_sum_get_wgname);
    link("ecl_sum_has_general_var", t.ecl_sum_has_general_var);
    link("ecl_sum_has_key", t.ecl_sum_has_key);
    link("ecl_sum_has_report_step", t.ecl_sum_has_report_step);
    link("ecl_sum_iget", t.ecl_sum_iget);
    link("ecl_sum_iget_report_end", t.ecl_sum_iget_report_end);
    link("ecl_sum_report_step_compatible", t.ecl_sum_report_step_compatible);
    link("ecl_sum_select_matching_general_var_list", t.ecl_sum_select_matching_general_var_list);

    /* Defined in 'ert/ecl/ecl_file.hpp' */
    link("ecl_file_close", t.ecl_file_close);
    link("ecl_file_open", t.ecl_file_open);

    /* Defined in 'ert/ecl/ecl_kw.hpp' */
    link("ecl_kw_alloc_new_shared", t.ecl_kw_alloc_new_shared);
    link("ecl_kw_fread_alloc", t.ecl_kw_fread_alloc);
    link("ecl_kw_free", t.ecl_kw_free);
    link("ecl_kw_fseek_kw", t.ecl_kw_fseek_kw);
    link("ecl_kw_fwrite_param_fortio", t.ecl_kw_fwrite_param_fortio);
    link("ecl_kw_get_data_type", t.ecl_kw_get_data_type);
    link("ecl_kw_get_size", t.ecl_kw_get_size);
    link("ecl_kw_get_void_ptr", t.ecl_kw_get_void_ptr);
    link("ecl_kw_is_kw_file", t.ecl_kw_is_kw_file);

    /* Defined in 'ert/ecl/ecl_kw_grdecl.hpp' */
    link("ecl_kw_fprintf_grdecl", t.ecl_kw_fprintf_grdecl);
    link("ecl_kw_fscanf_alloc_grdecl_data", t.ecl_kw_fscanf_alloc_grdecl_data);
    link("ecl_kw_grdecl_fseek_kw", t.ecl_kw_grdecl_fseek_kw);
    link("ecl_kw_grdecl_fseek_next_kw", t.ecl_kw_grdecl_fseek_next_kw);

    /* Defined in 'ert/ecl/ecl_io_config.hpp' */
    link("ecl_io_config_alloc", t.ecl_io_config_alloc);
    link("ecl_io_config_free", t.ecl_io_config_free);
    link("ecl_io_config_get_formatted", t.ecl_io_config_get_formatted);
    link("ecl_io_config_get_unified_restart", t.ecl_io_config_get_unified_restart);

    /* Defined in 'ert/ecl/ecl_smspec.hpp' */
    link("ecl_smspec_get_join_string", t.ecl_smspec_get_join_string);
    link("ecl_smspec_has_general_var", t.ecl_smspec_has_general_var);
    link("ecl_smspec_identify_var_type", t.ecl_smspec_identify_var_type);
    link("ecl_smspec_num_nodes", t.ecl_smspec_num_nodes);

    /* Defined in 'ert/ecl/ecl_type.hpp' */
    link("ecl_type_alloc_name", t.ecl_type_alloc_name);
    link("ecl_type_create_from_type", t.ecl_type_create_from_type);
    link("ecl_type_get_sizeof_ctype", t.ecl_type_get_sizeof_ctype);
    link("ecl_type_get_type", t.ecl_type_get_type);
    link("ecl_type_is_double", t.ecl_type_is_double);
    link("ecl_type_is_equal", t.ecl_type_is_equal);
    link("ecl_type_is_float", t.ecl_type_is_float);
    link("ecl_type_is_int", t.ecl_type_is_int);

    /* Defined in 'ert/ecl/ecl_util.hpp' */
    link("ecl_util_alloc_exfilename", t.ecl_util_alloc_exfilename);
    link("ecl_util_alloc_filename", t.ecl_util_alloc_filename);
    link("ecl_util_fmt_file", t.ecl_util_fmt_file);
    link("ecl_util_get_file_type", t.ecl_util_get_file_type);
    link("ecl_util_get_month_nr", t.ecl_util_get_month_nr);
    link("ecl_util_get_num_cpu", t.ecl_util_get_num_cpu);
    link("ecl_util_get_start_date", t.ecl_util_get_start_date);
    link("ecl_util_get_unit_set", t.ecl_util_get_unit_set);
    link("ecl_util_memcpy_typed_data", t.ecl_util_memcpy_typed_data);
    link("ecl_util_valid_basename_fmt", t.ecl_util_valid_basename_fmt);

    /* Defined in 'ert/util/double_vector.h' */
    link("double_vector_alloc", t.double_vector_alloc);
    link("double_vector_alloc_copy", t.double_vector_alloc_copy);
    link("double_vector_alloc_rsort_perm", t.double_vector_alloc_rsort_perm);
    link("double_vector_alloc_shared_wrapper", t.double_vector_alloc_shared_wrapper);
    link("double_vector_alloc_sort_perm", t.double_vector_alloc_sort_perm);
    link("double_vector_append", t.double_vector_append);
    link("double_vector_element_size", t.double_vector_element_size);
    link("double_vector_fread_alloc", t.double_vector_fread_alloc);
    link("double_vector_free", t.double_vector_free);
    link("double_vector_fwrite", t.double_vector_fwrite);
    link("double_vector_get_default", t.double_vector_get_default);
    link("double_vector_get_ptr", t.double_vector_get_ptr);
    link("double_vector_iget", t.double_vector_iget);
    link("double_vector_iset", t.double_vector_iset);
    link("double_vector_iset_default", t.double_vector_iset_default);
    link("double_vector_memcpy", t.double_vector_memcpy);
    link("double_vector_memcpy_from_data", t.double_vector_memcpy_from_data);
    link("double_vector_permute", t.double_vector_permute);
    link("double_vector_reset", t.double_vector_reset);
    link("double_vector_resize", t.double_vector_resize);
    link("double_vector_safe_iget", t.double_vector_safe_iget);
    link("double_vector_set_default", t.double_vector_set_default);
    link("double_vector_size", t.double_vector_size);

    /* Defined in 'ert/util/bool_vector.h' */
    link("bool_vector_alloc", t.bool_vector_alloc);
    link("bool_vector_alloc_copy", t.bool_vector_alloc_copy);
    link("bool_vector_count_equal", t.bool_vector_count_equal);
    link("bool_vector_fread", t.bool_vector_fread);
    link("bool_vector_free", t.bool_vector_free);
    link("bool_vector_fwrite", t.bool_vector_fwrite);
    link("bool_vector_get_ptr", t.bool_vector_get_ptr);
    link("bool_vector_iget", t.bool_vector_iget);
    link("bool_vector_iset", t.bool_vector_iset);
    link("bool_vector_reset", t.bool_vector_reset);
    link("bool_vector_resize", t.bool_vector_resize);
    link("bool_vector_safe_iget", t.bool_vector_safe_iget);
    link("bool_vector_set_all", t.bool_vector_set_all);
    link("bool_vector_set_default", t.bool_vector_set_default);
    link("bool_vector_size", t.bool_vector_size);

    /* Defined in 'ert/util/long_vector.h' */
    link("long_vector_alloc", t.long_vector_alloc);
    link("long_vector_append", t.long_vector_append);
    link("long_vector_free", t.long_vector_free);
    link("long_vector_iget", t.long_vector_iget);
    link("long_vector_size", t.long_vector_size);

    /* Defined in 'ert/util/time_t_vector.h' */
    link("time_t_vector_alloc", t.time_t_vector_alloc);
    link("time_t_vector_alloc_shared_wrapper", t.time_t_vector_alloc_shared_wrapper);
    link("time_t_vector_alloc_sort_perm", t.time_t_vector_alloc_sort_perm);
    link("time_t_vector_append", t.time_t_vector_append);
    link("time_t_vector_equal", t.time_t_vector_equal);
    link("time_t_vector_fread_alloc", t.time_t_vector_fread_alloc);
    link("time_t_vector_free", t.time_t_vector_free);
    link("time_t_vector_fwrite", t.time_t_vector_fwrite);
    link("time_t_vector_iget", t.time_t_vector_iget);
    link("time_t_vector_iset", t.time_t_vector_iset);
    link("time_t_vector_iset_block", t.time_t_vector_iset_block);
    link("time_t_vector_permute", t.time_t_vector_permute);
    link("time_t_vector_reset", t.time_t_vector_reset);
    link("time_t_vector_resize", t.time_t_vector_resize);
    link("time_t_vector_safe_iget", t.time_t_vector_safe_iget);
    link("time_t_vector_size", t.time_t_vector_size);

    /* Defined in 'ert/util/buffer_string.h' */
    link("buffer_fread_alloc_string", t.buffer_fread_alloc_string);
    link("buffer_fread_string", t.buffer_fread_string);
    link("buffer_fwrite_string", t.buffer_fwrite_string);

    /* Defined in 'ert/util/util.h' */
    link("util_abort__", t.util_abort__);
    link("util_addmode_if_owner", t.util_addmode_if_owner);
    link("util_alloc_abs_path", t.util_alloc_abs_path);
    link("util_alloc_atlink_target", t.util_alloc_atlink_target);
    link("util_alloc_copy", t.util_alloc_copy);
    link("util_alloc_cwd", t.util_alloc_cwd);
    link("util_alloc_date_stamp_utc", t.util_alloc_date_stamp_utc);
    link("util_alloc_dequoted_copy", t.util_alloc_dequoted_copy);
    link("util_alloc_file_components", t.util_alloc_file_components);
    link("util_alloc_filename", t.util_alloc_filename);
    link("util_alloc_joined_string", t.util_alloc_joined_string);
    link("util_alloc_link_target", t.util_alloc_link_target);
    link("util_alloc_normal_path", t.util_alloc_normal_path);
    link("util_alloc_realpath", t.util_alloc_realpath);
    link("util_alloc_realpath__", t.util_alloc_realpath__);
    link("util_alloc_rel_path", t.util_alloc_rel_path);
    link("util_alloc_sprintf", t.util_alloc_sprintf);
    link("util_alloc_sprintf_va", t.util_alloc_sprintf_va);
    link("util_alloc_string_copy", t.util_alloc_string_copy);
    link("util_alloc_stringlist_copy", t.util_alloc_stringlist_copy);
    link("util_alloc_strupr_copy", t.util_alloc_strupr_copy);
    link("util_alloc_substring_copy", t.util_alloc_substring_copy);
    link("util_alloc_tmp_file", t.util_alloc_tmp_file);
    link("util_binary_split_string", t.util_binary_split_string);
    link("util_calloc", t.util_calloc);
    link("util_chdir", t.util_chdir);
    link("util_chdir_file", t.util_chdir_file);
    link("util_clamp_double", t.util_clamp_double);
    link("util_clear_directory", t.util_clear_directory);
    link("util_copy_directory", t.util_copy_directory);
    link("util_copy_file", t.util_copy_file);
    link("util_dev_urandom_seed", t.util_dev_urandom_seed);
    link("util_difftime_days", t.util_difftime_days);
    link("util_difftime_seconds", t.util_difftime_seconds);
    link("util_double_max", t.util_double_max);
    link("util_double_min", t.util_double_min);
    link("util_double_to_float", t.util_double_to_float);
    link("util_double_vector_mean", t.util_double_vector_mean);
    link("util_double_vector_stddev", t.util_double_vector_stddev);
    link("util_endian_flip_vector", t.util_endian_flip_vector);
    link("util_entry_exists", t.util_entry_exists);
    link("util_entry_readable", t.util_entry_readable);
    link("util_exit", t.util_exit);
    link("util_file_exists", t.util_file_exists);
    link("util_file_mtime", t.util_file_mtime);
    link("util_file_readable", t.util_file_readable);
    link("util_file_size", t.util_file_size);
    link("util_float_max", t.util_float_max);
    link("util_float_to_double", t.util_float_to_double);
    link("util_fmt_bit8", t.util_fmt_bit8);
    link("util_fnmatch", t.util_fnmatch);
    link("util_fopen", t.util_fopen);
    link("util_fopen__", t.util_fopen__);
    link("util_fread", t.util_fread);
    link("util_fread_alloc_file_content", t.util_fread_alloc_file_content);
    link("util_fread_alloc_string", t.util_fread_alloc_string);
    link("util_fread_double", t.util_fread_double);
    link("util_fread_int", t.util_fread_int);
    link("util_fread_long", t.util_fread_long);
    link("util_fread_realloc_string", t.util_fread_realloc_string);
    link("util_fread_time_t", t.util_fread_time_t);
    link("util_free_NULL_terminated_stringlist", t.util_free_NULL_terminated_stringlist);
    link("util_free_stringlist", t.util_free_stringlist);
    link("util_fscanf_alloc_line", t.util_fscanf_alloc_line);
    link("util_fscanf_alloc_token", t.util_fscanf_alloc_token);
    link("util_fscanf_alloc_upto", t.util_fscanf_alloc_upto);
    link("util_fseek_string", t.util_fseek_string);
    link("util_fskip_int", t.util_fskip_int);
    link("util_fskip_lines", t.util_fskip_lines);
    link("util_ftell", t.util_ftell);
    link("util_fwrite", t.util_fwrite);
    link("util_fwrite_double", t.util_fwrite_double);
    link("util_fwrite_int", t.util_fwrite_int);
    link("util_fwrite_long", t.util_fwrite_long);
    link("util_fwrite_string", t.util_fwrite_string);
    link("util_fwrite_time_t", t.util_fwrite_time_t);
    link("util_get_current_linenr", t.util_get_current_linenr);
    link("util_inplace_forward_days_utc", t.util_inplace_forward_days_utc);
    link("util_inplace_forward_seconds_utc", t.util_inplace_forward_seconds_utc);
    link("util_install_signals", t.util_install_signals);
    link("util_int_format_count", t.util_int_format_count);
    link("util_int_max", t.util_int_max);
    link("util_int_min", t.util_int_min);
    link("util_is_abs_path", t.util_is_abs_path);
    link("util_is_directory", t.util_is_directory);
    link("util_is_executable", t.util_is_executable);
    link("util_is_file", t.util_is_file);
    link("util_is_link", t.util_is_link);
    link("util_make_date_utc", t.util_make_date_utc);
    link("util_make_path", t.util_make_path);
    link("util_malloc", t.util_malloc);
    link("util_mkdir_fopen", t.util_mkdir_fopen);
    link("util_mkdir_p", t.util_mkdir_p);
    link("util_move_file", t.util_move_file);
    link("util_path_split", t.util_path_split);
    link("util_realloc", t.util_realloc);
    link("util_realloc_copy", t.util_realloc_copy);
    link("util_realloc_sprintf", t.util_realloc_sprintf);
    link("util_realloc_string_copy", t.util_realloc_string_copy);
    link("util_same_file", t.util_same_file);
    link("util_set_date_values_utc", t.util_set_date_values_utc);
    link("util_set_datetime_values_utc", t.util_set_datetime_values_utc);
    link("util_size_t_max", t.util_size_t_max);
    link("util_spawn", t.util_spawn);
    link("util_spawn_blocking", t.util_spawn_blocking);
    link("util_split_alloc_dirname", t.util_split_alloc_dirname);
    link("util_split_alloc_filename", t.util_split_alloc_filename);
    link("util_split_string", t.util_split_string);
    link("util_sscanf_bool", t.util_sscanf_bool);
    link("util_sscanf_bytesize", t.util_sscanf_bytesize);
    link("util_sscanf_date_utc", t.util_sscanf_date_utc);
    link("util_sscanf_double", t.util_sscanf_double);
    link("util_sscanf_int", t.util_sscanf_int);
    link("util_sscanf_isodate", t.util_sscanf_isodate);
    link("util_sscanf_octal_int", t.util_sscanf_octal_int);
    link("util_sscanf_percent", t.util_sscanf_percent);
    link("util_strcat_realloc", t.util_strcat_realloc);
    link("util_strcmp_int", t.util_strcmp_int);
    link("util_string_equal", t.util_string_equal);
    link("util_string_has_wildcard", t.util_string_has_wildcard);
    link("util_string_isspace", t.util_string_isspace);
    link("util_string_match", t.util_string_match);
    link("util_string_replace_alloc", t.util_string_replace_alloc);
    link("util_string_replacen_alloc", t.util_string_replacen_alloc);
    link("util_strupr", t.util_strupr);
    link("util_time_t_max", t.util_time_t_max);
    link("util_time_utc", t.util_time_utc);
    link("util_try_lockf", t.util_try_lockf);
    link("util_unlink_existing", t.util_unlink_existing);
    link("util_update_double_max_min", t.util_update_double_max_min);
    link("util_update_float_max_min", t.util_update_float_max_min);
    link("util_update_int_max_min", t.util_update_int_max_min);
    link("util_usleep", t.util_usleep);

    /* Defined in 'ert/util/int_vector.h' */
    link("int_vector_alloc", t.int_vector_alloc);
    link("int_vector_alloc_copy", t.int_vector_alloc_copy);
    link("int_vector_alloc_data_copy", t.int_vector_alloc_data_copy);
    link("int_vector_append", t.int_vector_append);
    link("int_vector_contains", t.int_vector_contains);
    link("int_vector_contains_sorted", t.int_vector_contains_sorted);
    link("int_vector_equal", t.int_vector_equal);
    link("int_vector_fread", t.int_vector_fread);
    link("int_vector_free", t.int_vector_free);
    link("int_vector_fwrite", t.int_vector_fwrite);
    link("int_vector_get_const_ptr", t.int_vector_get_const_ptr);
    link("int_vector_get_first", t.int_vector_get_first);
    link("int_vector_get_last", t.int_vector_get_last);
    link("int_vector_get_ptr", t.int_vector_get_ptr);
    link("int_vector_iget", t.int_vector_iget);
    link("int_vector_index_sorted", t.int_vector_index_sorted);
    link("int_vector_init_range", t.int_vector_init_range);
    link("int_vector_iset", t.int_vector_iset);
    link("int_vector_iset_block", t.int_vector_iset_block);
    link("int_vector_iset_default", t.int_vector_iset_default);
    link("int_vector_memcpy", t.int_vector_memcpy);
    link("int_vector_reset", t.int_vector_reset);
    link("int_vector_resize", t.int_vector_resize);
    link("int_vector_safe_iget", t.int_vector_safe_iget);
    link("int_vector_select_unique", t.int_vector_select_unique);
    link("int_vector_set_default", t.int_vector_set_default);
    link("int_vector_shrink", t.int_vector_shrink);
    link("int_vector_size", t.int_vector_size);
    link("int_vector_sort", t.int_vector_sort);

    /* Defined in 'ert/util/size_t_vector.h' */
    link("size_t_vector_append", t.size_t_vector_append);
    link("size_t_vector_free", t.size_t_vector_free);
    link("size_t_vector_iget", t.size_t_vector_iget);
    link("size_t_vector_iset_default", t.size_t_vector_iset_default);
    link("size_t_vector_safe_iget", t.size_t_vector_safe_iget);

    /* Defined in 'ert/ecl/smspec_node.h' */
    link("smspec_alloc_block_ijk_key", t.smspec_alloc_block_ijk_key);

    /* Defined in 'ert/ecl/fortio.h' */
    link("fortio_alloc_FILE_wrapper", t.fortio_alloc_FILE_wrapper);
    link("fortio_fclose", t.fortio_fclose);
    link("fortio_free_FILE_wrapper", t.fortio_free_FILE_wrapper);
    link("fortio_open_reader", t.fortio_open_reader);
    link("fortio_open_writer", t.fortio_open_writer);
}

// ----------------------------------------
// Manually added
// ----------------------------------------

ECL_DECL const hash_type * hash_safe_cast_const(const char * value) {
    return t.hash_safe_cast_const(value);
}

ECL_DECL const fortio_type *fortio_safe_cast(char *value) {
  return t.fortio_safe_cast(value);
}

ECLXX_DECL const ecl::smspec_node &ecl_smspec_iget_node_w_node_index(const ecl_smspec_type *smspec, int node_index) {
    return t.ecl_smspec_iget_node_w_node_index(smspec, node_index);
}

const char *ecl::smspec_node::get_gen_key1() const {
    return t.smspec_node_get_gen_key1(this);
}

// ----------------------------------------
// ert/geometry/geo_surface.hpp
// ----------------------------------------

ECL_DECL geo_surface_type * geo_surface_fload_alloc_irap(const char * x0, bool x1) {
    return t.geo_surface_fload_alloc_irap(x0, x1);
}

ECL_DECL bool geo_surface_fload_irap_zcoord(const geo_surface_type * x0, const char * x1, double * x2) {
    return t.geo_surface_fload_irap_zcoord(x0, x1, x2);
}

ECL_DECL void geo_surface_fprintf_irap_external_zcoord(const geo_surface_type * x0, const char * x1, const double * x2) {
    t.geo_surface_fprintf_irap_external_zcoord(x0, x1, x2);
}

ECL_DECL void geo_surface_free(geo_surface_type * x0) {
    t.geo_surface_free(x0);
}

ECL_DECL int geo_surface_get_size(const geo_surface_type * x0) {
    return t.geo_surface_get_size(x0);
}

// ----------------------------------------
// ert/util/path_stack.hpp
// ----------------------------------------

ECL_DECL path_stack_type * path_stack_alloc() {
    return t.path_stack_alloc();
}

ECL_DECL void path_stack_free(path_stack_type * x0) {
    t.path_stack_free(x0);
}

ECL_DECL void path_stack_pop(path_stack_type * x0) {
    t.path_stack_pop(x0);
}

ECL_DECL void path_stack_push_cwd(path_stack_type * x0) {
    t.path_stack_push_cwd(x0);
}

// ----------------------------------------
// ert/util/rng.hpp
// ----------------------------------------

ECL_DECL rng_type * rng_alloc(rng_alg_type x0, rng_init_mode x1) {
    return t.rng_alloc(x0, x1);
}

ECL_DECL void rng_free(rng_type * x0) {
    t.rng_free(x0);
}

ECL_DECL void rng_free__(void * x0) {
    t.rng_free__(x0);
}

ECL_DECL double rng_get_double(rng_type * x0) {
    return t.rng_get_double(x0);
}

ECL_DECL int rng_get_int(rng_type * x0, int x1) {
    return t.rng_get_int(x0, x1);
}

ECL_DECL void rng_get_state(const rng_type * x0, char * x1) {
    t.rng_get_state(x0, x1);
}

ECL_DECL void rng_load_state(rng_type * x0, const char * x1) {
    t.rng_load_state(x0, x1);
}

ECL_DECL void rng_rng_init(rng_type * x0, rng_type * x1) {
    t.rng_rng_init(x0, x1);
}

ECL_DECL void rng_save_state(rng_type * x0, const char * x1) {
    t.rng_save_state(x0, x1);
}

ECL_DECL void rng_set_state(rng_type * x0, const char * x1) {
    t.rng_set_state(x0, x1);
}

ECL_DECL void rng_shuffle_int(rng_type * x0, int * x1, int x2) {
    t.rng_shuffle_int(x0, x1, x2);
}

ECL_DECL double rng_std_normal(rng_type * x0) {
    return t.rng_std_normal(x0);
}

// ----------------------------------------
// ert/util/vector.hpp
// ----------------------------------------

ECL_DECL vector_type * vector_alloc_NULL_initialized(int x0) {
    return t.vector_alloc_NULL_initialized(x0);
}

ECL_DECL vector_type * vector_alloc_new() {
    return t.vector_alloc_new();
}

ECL_DECL int vector_append_owned_ref(vector_type * x0, const void * x1, int * x2) {
    return t.vector_append_owned_ref(x0, x1, x2);
}

ECL_DECL int vector_append_ref(vector_type * x0, const void * x1) {
    return t.vector_append_ref(x0, x1);
}

ECL_DECL void vector_clear(vector_type * x0) {
    t.vector_clear(x0);
}

ECL_DECL int vector_find(const vector_type * x0, const void * x1) {
    return t.vector_find(x0, x1);
}

ECL_DECL void vector_free(vector_type * x0) {
    t.vector_free(x0);
}

ECL_DECL void vector_free__(void * x0) {
    t.vector_free__(x0);
}

ECL_DECL void * vector_get_last(const vector_type * x0) {
    return t.vector_get_last(x0);
}

ECL_DECL const void * vector_get_last_const(const vector_type * x0) {
    return t.vector_get_last_const(x0);
}

ECL_DECL int vector_get_size(const vector_type * x0) {
    return t.vector_get_size(x0);
}

ECL_DECL void vector_grow_NULL(vector_type * x0, int x1) {
    t.vector_grow_NULL(x0, x1);
}

ECL_DECL void vector_idel(vector_type * x0, int x1) {
    t.vector_idel(x0, x1);
}

ECL_DECL void * vector_iget(const vector_type * x0, int x1) {
    return t.vector_iget(x0, x1);
}

ECL_DECL const void * vector_iget_const(const vector_type * x0, int x1) {
    return t.vector_iget_const(x0, x1);
}

ECL_DECL void vector_insert_owned_ref(vector_type * x0, int x1, const void * x2, int * x3) {
    t.vector_insert_owned_ref(x0, x1, x2, x3);
}

ECL_DECL void vector_iset_owned_ref(vector_type * x0, int x1, const void * x2, int * x3) {
    t.vector_iset_owned_ref(x0, x1, x2, x3);
}

ECL_DECL void vector_iset_ref(vector_type * x0, int x1, const void * x2) {
    t.vector_iset_ref(x0, x1, x2);
}

ECL_DECL void * vector_pop_back(vector_type * x0) {
    return t.vector_pop_back(x0);
}

ECL_DECL void * vector_pop_front(vector_type * x0) {
    return t.vector_pop_front(x0);
}

ECL_DECL void vector_push_front_ref(vector_type * x0, const void * x1) {
    t.vector_push_front_ref(x0, x1);
}

ECL_DECL void * vector_safe_iget(const vector_type * x0, int x1) {
    return t.vector_safe_iget(x0, x1);
}

ECL_DECL void vector_shrink(vector_type * x0, int x1) {
    t.vector_shrink(x0, x1);
}

ECL_DECL void vector_sort(vector_type * x0, vector_cmp_ftype * x1) {
    t.vector_sort(x0, x1);
}

// ----------------------------------------
// ert/util/type_vector_functions.hpp
// ----------------------------------------

ECL_DECL int * bool_vector_alloc_active_index_list(const int * x0, int x1) {
    return t.bool_vector_alloc_active_index_list(x0, x1);
}

ECL_DECL int * bool_vector_alloc_active_list(const int * x0) {
    return t.bool_vector_alloc_active_list(x0);
}

ECL_DECL int * int_vector_alloc_mask(const int * x0) {
    return t.int_vector_alloc_mask(x0);
}

// ----------------------------------------
// ert/util/test_work_area.hpp
// ----------------------------------------

ECL_DECL test_work_area_type * test_work_area_alloc__(const char * x0, bool x1) {
    return t.test_work_area_alloc__(x0, x1);
}

ECL_DECL bool test_work_area_copy_parent_content(const test_work_area_type * x0, const char * x1) {
    return t.test_work_area_copy_parent_content(x0, x1);
}

ECL_DECL void test_work_area_free(test_work_area_type * x0) {
    t.test_work_area_free(x0);
}

ECL_DECL const char * test_work_area_get_cwd(const test_work_area_type * x0) {
    return t.test_work_area_get_cwd(x0);
}

// ----------------------------------------
// ert/util/perm_vector.hpp
// ----------------------------------------

ECL_DECL void perm_vector_free(perm_vector_type * x0) {
    t.perm_vector_free(x0);
}

ECL_DECL int perm_vector_iget(const perm_vector_type * x0, int x1) {
    return t.perm_vector_iget(x0, x1);
}

// ----------------------------------------
// ert/util/stringlist.hpp
// ----------------------------------------

ECL_DECL char ** stringlist_alloc_char_copy(const stringlist_type * x0) {
    return t.stringlist_alloc_char_copy(x0);
}

ECL_DECL char ** stringlist_alloc_char_ref(const stringlist_type * x0) {
    return t.stringlist_alloc_char_ref(x0);
}

ECL_DECL stringlist_type * stringlist_alloc_deep_copy(const stringlist_type * x0) {
    return t.stringlist_alloc_deep_copy(x0);
}

ECL_DECL stringlist_type * stringlist_alloc_from_split(const char * x0, const char * x1) {
    return t.stringlist_alloc_from_split(x0, x1);
}

ECL_DECL char * stringlist_alloc_joined_string(const stringlist_type * x0, const char * x1) {
    return t.stringlist_alloc_joined_string(x0, x1);
}

ECL_DECL char * stringlist_alloc_joined_substring(const stringlist_type * x0, int x1, int x2, const char * x3) {
    return t.stringlist_alloc_joined_substring(x0, x1, x2, x3);
}

ECL_DECL stringlist_type * stringlist_alloc_new() {
    return t.stringlist_alloc_new();
}

ECL_DECL void stringlist_append_copy(stringlist_type * x0, const char * x1) {
    t.stringlist_append_copy(x0, x1);
}

ECL_DECL void stringlist_append_stringlist_copy(stringlist_type * x0, const stringlist_type * x1) {
    t.stringlist_append_stringlist_copy(x0, x1);
}

ECL_DECL const char * stringlist_back(const stringlist_type * x0) {
    return t.stringlist_back(x0);
}

ECL_DECL void stringlist_clear(stringlist_type * x0) {
    t.stringlist_clear(x0);
}

ECL_DECL bool stringlist_contains(const stringlist_type * x0, const char * x1) {
    return t.stringlist_contains(x0, x1);
}

ECL_DECL void stringlist_deep_copy(stringlist_type * x0, const stringlist_type * x1) {
    t.stringlist_deep_copy(x0, x1);
}

ECL_DECL bool stringlist_equal(const stringlist_type * x0, const stringlist_type * x1) {
    return t.stringlist_equal(x0, x1);
}

ECL_DECL int stringlist_find_first(const stringlist_type * x0, const char * x1) {
    return t.stringlist_find_first(x0, x1);
}

ECL_DECL void stringlist_fprintf(const stringlist_type * x0, const char * x1, FILE * x2) {
    t.stringlist_fprintf(x0, x1, x2);
}

ECL_DECL stringlist_type * stringlist_fread_alloc(FILE * x0) {
    return t.stringlist_fread_alloc(x0);
}

ECL_DECL void stringlist_free(stringlist_type * x0) {
    t.stringlist_free(x0);
}

ECL_DECL void stringlist_free__(void * x0) {
    t.stringlist_free__(x0);
}

ECL_DECL const char * stringlist_front(const stringlist_type * x0) {
    return t.stringlist_front(x0);
}

ECL_DECL void stringlist_fwrite(const stringlist_type * x0, FILE * x1) {
    t.stringlist_fwrite(x0, x1);
}

ECL_DECL const char * stringlist_get_last(const stringlist_type * x0) {
    return t.stringlist_get_last(x0);
}

ECL_DECL int stringlist_get_size(const stringlist_type * x0) {
    return t.stringlist_get_size(x0);
}

ECL_DECL void stringlist_idel(stringlist_type * x0, int x1) {
    t.stringlist_idel(x0, x1);
}

ECL_DECL bool stringlist_iequal(const stringlist_type * x0, int x1, const char * x2) {
    return t.stringlist_iequal(x0, x1, x2);
}

ECL_DECL const char * stringlist_iget(const stringlist_type * x0, int x1) {
    return t.stringlist_iget(x0, x1);
}

ECL_DECL bool stringlist_iget_as_bool(const stringlist_type * x0, int x1, bool * x2) {
    return t.stringlist_iget_as_bool(x0, x1, x2);
}

ECL_DECL int stringlist_iget_as_int(const stringlist_type * x0, int x1, bool * x2) {
    return t.stringlist_iget_as_int(x0, x1, x2);
}

ECL_DECL char * stringlist_iget_copy(const stringlist_type * x0, int x1) {
    return t.stringlist_iget_copy(x0, x1);
}

ECL_DECL void stringlist_iset_copy(stringlist_type * x0, int x1, const char * x2) {
    t.stringlist_iset_copy(x0, x1, x2);
}

ECL_DECL void stringlist_iset_owned_ref(stringlist_type * x0, int x1, const char * x2) {
    t.stringlist_iset_owned_ref(x0, x1, x2);
}

ECL_DECL void stringlist_iset_ref(stringlist_type * x0, int x1, const char * x2) {
    t.stringlist_iset_ref(x0, x1, x2);
}

ECL_DECL const char * stringlist_safe_iget(const stringlist_type * x0, int x1) {
    return t.stringlist_safe_iget(x0, x1);
}

ECL_DECL int stringlist_select_matching(stringlist_type * x0, const char * x1) {
    return t.stringlist_select_matching(x0, x1);
}

ECL_DECL void stringlist_sort(stringlist_type * x0, string_cmp_ftype * x1) {
    t.stringlist_sort(x0, x1);
}

ECL_DECL bool stringlist_unique(const stringlist_type * x0) {
    return t.stringlist_unique(x0);
}

// ----------------------------------------
// ert/util/ecl_version.hpp
// ----------------------------------------

ECL_DECL int ecl_version_get_major_version() {
    return t.ecl_version_get_major_version();
}

ECL_DECL const char * ecl_version_get_micro_version() {
    return t.ecl_version_get_micro_version();
}

ECL_DECL int ecl_version_get_minor_version() {
    return t.ecl_version_get_minor_version();
}

// ----------------------------------------
// ert/util/string_util.hpp
// ----------------------------------------

ECL_DECL int * string_util_alloc_active_list(const char * x0) {
    return t.string_util_alloc_active_list(x0);
}

ECL_DECL int * string_util_alloc_value_list(const char * x0) {
    return t.string_util_alloc_value_list(x0);
}

ECL_DECL bool string_util_init_value_list(const char * x0, int * x1) {
    return t.string_util_init_value_list(x0, x1);
}

ECL_DECL bool string_util_update_active_list(const char * x0, int * x1) {
    return t.string_util_update_active_list(x0, x1);
}

ECL_DECL bool string_util_update_active_mask(const char * x0, int * x1) {
    return t.string_util_update_active_mask(x0, x1);
}

// ----------------------------------------
// ert/util/buffer.hpp
// ----------------------------------------

ECL_DECL buffer_type * buffer_alloc(int x0) {
    return t.buffer_alloc(x0);
}

ECL_DECL buffer_type * buffer_alloc_private_wrapper(void * x0, int x1) {
    return t.buffer_alloc_private_wrapper(x0, x1);
}

ECL_DECL void buffer_clear(buffer_type * x0) {
    t.buffer_clear(x0);
}

ECL_DECL int buffer_fread(buffer_type * x0, void * x1, int x2, int x3) {
    return t.buffer_fread(x0, x1, x2, x3);
}

ECL_DECL buffer_type * buffer_fread_alloc(const char * x0) {
    return t.buffer_fread_alloc(x0);
}

ECL_DECL int buffer_fread_compressed(buffer_type * x0, int x1, void * x2, int x3) {
    return t.buffer_fread_compressed(x0, x1, x2, x3);
}

ECL_DECL double buffer_fread_double(buffer_type * x0) {
    return t.buffer_fread_double(x0);
}

ECL_DECL int buffer_fread_int(buffer_type * x0) {
    return t.buffer_fread_int(x0);
}

ECL_DECL long buffer_fread_long(buffer_type * x0) {
    return t.buffer_fread_long(x0);
}

ECL_DECL void buffer_fread_realloc(buffer_type * x0, const char * x1) {
    t.buffer_fread_realloc(x0, x1);
}

ECL_DECL void buffer_free(buffer_type * x0) {
    t.buffer_free(x0);
}

ECL_DECL void buffer_free_container(buffer_type * x0) {
    t.buffer_free_container(x0);
}

ECL_DECL void buffer_fseek(buffer_type * x0, ssize_t x1, int x2) {
    t.buffer_fseek(x0, x1, x2);
}

ECL_DECL void buffer_fskip(buffer_type * x0, ssize_t x1) {
    t.buffer_fskip(x0, x1);
}

ECL_DECL void buffer_fskip_int(buffer_type * x0) {
    t.buffer_fskip_int(x0);
}

ECL_DECL void buffer_fskip_time_t(buffer_type * x0) {
    t.buffer_fskip_time_t(x0);
}

ECL_DECL int buffer_fwrite(buffer_type * x0, const void * x1, int x2, int x3) {
    return t.buffer_fwrite(x0, x1, x2, x3);
}

ECL_DECL void buffer_fwrite_char(buffer_type * x0, char x1) {
    t.buffer_fwrite_char(x0, x1);
}

ECL_DECL void buffer_fwrite_char_ptr(buffer_type * x0, const char * x1) {
    t.buffer_fwrite_char_ptr(x0, x1);
}

ECL_DECL int buffer_fwrite_compressed(buffer_type * x0, const void * x1, int x2) {
    return t.buffer_fwrite_compressed(x0, x1, x2);
}

ECL_DECL void buffer_fwrite_double(buffer_type * x0, double x1) {
    t.buffer_fwrite_double(x0, x1);
}

ECL_DECL void buffer_fwrite_int(buffer_type * x0, int x1) {
    t.buffer_fwrite_int(x0, x1);
}

ECL_DECL void buffer_fwrite_time_t(buffer_type * x0, time_t x1) {
    t.buffer_fwrite_time_t(x0, x1);
}

ECL_DECL void * buffer_get_data(const buffer_type * x0) {
    return t.buffer_get_data(x0);
}

ECL_DECL int buffer_get_offset(const buffer_type * x0) {
    return t.buffer_get_offset(x0);
}

ECL_DECL int buffer_get_remaining_size(const buffer_type * x0) {
    return t.buffer_get_remaining_size(x0);
}

ECL_DECL int buffer_get_size(const buffer_type * x0) {
    return t.buffer_get_size(x0);
}

ECL_DECL int buffer_get_string_size(const buffer_type * x0) {
    return t.buffer_get_string_size(x0);
}

ECL_DECL void buffer_memshift(buffer_type * x0, int x1, ssize_t x2) {
    t.buffer_memshift(x0, x1, x2);
}

ECL_DECL void buffer_rewind(buffer_type * x0) {
    t.buffer_rewind(x0);
}

ECL_DECL bool buffer_search_replace(buffer_type * x0, const char * x1, const char * x2) {
    return t.buffer_search_replace(x0, x1, x2);
}

ECL_DECL void buffer_shrink_to_fit(buffer_type * x0) {
    t.buffer_shrink_to_fit(x0);
}

ECL_DECL void buffer_store(const buffer_type * x0, const char * x1) {
    t.buffer_store(x0, x1);
}

ECL_DECL void buffer_strcat(buffer_type * x0, const char * x1) {
    t.buffer_strcat(x0, x1);
}

ECL_DECL bool buffer_strchr(buffer_type * x0, int x1) {
    return t.buffer_strchr(x0, x1);
}

ECL_DECL void buffer_stream_fread(buffer_type * x0, int x1, FILE * x2) {
    t.buffer_stream_fread(x0, x1, x2);
}

ECL_DECL int buffer_stream_fwrite_n(const buffer_type * x0, int x1, ssize_t x2, FILE * x3) {
    return t.buffer_stream_fwrite_n(x0, x1, x2, x3);
}

ECL_DECL bool buffer_strstr(buffer_type * x0, const char * x1) {
    return t.buffer_strstr(x0, x1);
}

// ----------------------------------------
// ert/util/node_ctype.hpp
// ----------------------------------------

ECL_DECL const char * node_ctype_name(node_ctype x0) {
    return t.node_ctype_name(x0);
}

// ----------------------------------------
// ert/util/hash.hpp
// ----------------------------------------

ECL_DECL bool hash_add_option(hash_type * x0, const char * x1) {
    return t.hash_add_option(x0, x1);
}

ECL_DECL hash_type * hash_alloc() {
    return t.hash_alloc();
}

ECL_DECL char ** hash_alloc_keylist(const hash_type * x0) {
    return t.hash_alloc_keylist(x0);
}

ECL_DECL int * hash_alloc_stringlist(const hash_type * x0) {
    return t.hash_alloc_stringlist(x0);
}

ECL_DECL void hash_clear(hash_type * x0) {
    t.hash_clear(x0);
}

ECL_DECL void hash_del(hash_type * x0, const char * x1) {
    t.hash_del(x0, x1);
}

ECL_DECL void hash_free(hash_type * x0) {
    t.hash_free(x0);
}

ECL_DECL void hash_free__(void * x0) {
    t.hash_free__(x0);
}

ECL_DECL void * hash_get(const hash_type * x0, const char * x1) {
    return t.hash_get(x0, x1);
}

ECL_DECL int hash_get_counter(const hash_type * x0, const char * x1) {
    return t.hash_get_counter(x0, x1);
}

ECL_DECL int hash_get_int(const hash_type * x0, const char * x1) {
    return t.hash_get_int(x0, x1);
}

ECL_DECL int hash_get_size(const hash_type * x0) {
    return t.hash_get_size(x0);
}

ECL_DECL char * hash_get_string(const hash_type * x0, const char * x1) {
    return t.hash_get_string(x0, x1);
}

ECL_DECL bool hash_has_key(const hash_type * x0, const char * x1) {
    return t.hash_has_key(x0, x1);
}

ECL_DECL void hash_insert_double(hash_type * x0, const char * x1, double x2) {
    t.hash_insert_double(x0, x1, x2);
}

ECL_DECL void hash_insert_hash_owned_ref(hash_type * x0, const char * x1, const void * x2, int * x3) {
    t.hash_insert_hash_owned_ref(x0, x1, x2, x3);
}

ECL_DECL void hash_insert_int(hash_type * x0, const char * x1, int x2) {
    t.hash_insert_int(x0, x1, x2);
}

ECL_DECL void hash_insert_ref(hash_type * x0, const char * x1, const void * x2) {
    t.hash_insert_ref(x0, x1, x2);
}

ECL_DECL void hash_insert_string(hash_type * x0, const char * x1, const char * x2) {
    t.hash_insert_string(x0, x1, x2);
}

ECL_DECL hash_iter_type * hash_iter_alloc(const hash_type * x0) {
    return t.hash_iter_alloc(x0);
}

ECL_DECL void hash_iter_free(hash_iter_type * x0) {
    t.hash_iter_free(x0);
}

ECL_DECL const char * hash_iter_get_next_key(hash_iter_type * x0) {
    return t.hash_iter_get_next_key(x0);
}

ECL_DECL void * hash_iter_get_next_value(hash_iter_type * x0) {
    return t.hash_iter_get_next_value(x0);
}

ECL_DECL bool hash_iter_is_complete(const hash_iter_type * x0) {
    return t.hash_iter_is_complete(x0);
}

ECL_DECL void hash_iter_restart(hash_iter_type * x0) {
    t.hash_iter_restart(x0);
}

ECL_DECL bool hash_key_list_compare(const hash_type * x0, const hash_type * x1) {
    return t.hash_key_list_compare(x0, x1);
}

ECL_DECL void * hash_pop(hash_type * x0, const char * x1) {
    return t.hash_pop(x0, x1);
}

ECL_DECL void hash_resize(hash_type * x0, int x1) {
    t.hash_resize(x0, x1);
}

ECL_DECL void * hash_safe_get(const hash_type * x0, const char * x1) {
    return t.hash_safe_get(x0, x1);
}

// ----------------------------------------
// ert/util/parser.hpp
// ----------------------------------------

ECL_DECL basic_parser_type * basic_parser_alloc(const char * x0, const char * x1, const char * x2, const char * x3, const char * x4, const char * x5) {
    return t.basic_parser_alloc(x0, x1, x2, x3, x4, x5);
}

ECL_DECL char * basic_parser_fread_alloc_file_content(const char * x0, const char * x1, const char * x2, const char * x3, const char * x4) {
    return t.basic_parser_fread_alloc_file_content(x0, x1, x2, x3, x4);
}

ECL_DECL void basic_parser_free(basic_parser_type * x0) {
    t.basic_parser_free(x0);
}

ECL_DECL bool basic_parser_fseek_string(const basic_parser_type * x0, int * x1, const char * x2, bool x3, bool x4) {
    return t.basic_parser_fseek_string(x0, x1, x2, x3, x4);
}

ECL_DECL int * basic_parser_tokenize_buffer(const basic_parser_type * x0, const char * x1, bool x2) {
    return t.basic_parser_tokenize_buffer(x0, x1, x2);
}

ECL_DECL int * basic_parser_tokenize_file(const basic_parser_type * x0, const char * x1, bool x2) {
    return t.basic_parser_tokenize_file(x0, x1, x2);
}

// ----------------------------------------
// ert/ecl/ecl_grid.hpp
// ----------------------------------------

ECL_DECL ecl_grid_type * ecl_grid_alloc(const char * x0) {
    return t.ecl_grid_alloc(x0);
}

ECL_DECL void ecl_grid_free(ecl_grid_type * x0) {
    t.ecl_grid_free(x0);
}

ECL_DECL int ecl_grid_get_active_index3(const ecl_grid_type * x0, int x1, int x2, int x3) {
    return t.ecl_grid_get_active_index3(x0, x1, x2, x3);
}

ECL_DECL int ecl_grid_get_active_size(const ecl_grid_type * x0) {
    return t.ecl_grid_get_active_size(x0);
}

ECL_DECL double ecl_grid_get_cdepth3(const ecl_grid_type * x0, int x1, int x2, int x3) {
    return t.ecl_grid_get_cdepth3(x0, x1, x2, x3);
}

ECL_DECL void ecl_grid_get_dims(const ecl_grid_type * x0, int * x1, int * x2, int * x3, int * x4) {
    t.ecl_grid_get_dims(x0, x1, x2, x3, x4);
}

ECL_DECL int ecl_grid_get_global_index3(const ecl_grid_type * x0, int x1, int x2, int x3) {
    return t.ecl_grid_get_global_index3(x0, x1, x2, x3);
}

ECL_DECL int ecl_grid_get_global_size(const ecl_grid_type * x0) {
    return t.ecl_grid_get_global_size(x0);
}

ECL_DECL const char * ecl_grid_get_name(const ecl_grid_type * x0) {
    return t.ecl_grid_get_name(x0);
}

ECL_DECL bool ecl_grid_ijk_valid(const ecl_grid_type * x0, int x1, int x2, int x3) {
    return t.ecl_grid_ijk_valid(x0, x1, x2, x3);
}

// ----------------------------------------
// ert/ecl/ecl_sum.hpp
// ----------------------------------------

ECL_DECL bool ecl_sum_case_exists(const char * x0) {
    return t.ecl_sum_case_exists(x0);
}

ECL_DECL ecl_sum_type * ecl_sum_fread_alloc(const char * x0, const int * x1, const char * x2, bool x3, bool x4, int x5) {
    return t.ecl_sum_fread_alloc(x0, x1, x2, x3, x4, x5);
}

ECL_DECL ecl_sum_type * ecl_sum_fread_alloc_case(const char * x0, const char * x1) {
    return t.ecl_sum_fread_alloc_case(x0, x1);
}

ECL_DECL void ecl_sum_free(ecl_sum_type * x0) {
    t.ecl_sum_free(x0);
}

ECL_DECL const char * ecl_sum_get_case(const ecl_sum_type * x0) {
    return t.ecl_sum_get_case(x0);
}

ECL_DECL time_t ecl_sum_get_end_time(const ecl_sum_type * x0) {
    return t.ecl_sum_get_end_time(x0);
}

ECL_DECL int ecl_sum_get_first_report_step(const ecl_sum_type * x0) {
    return t.ecl_sum_get_first_report_step(x0);
}

ECL_DECL double ecl_sum_get_general_var(const ecl_sum_type * x0, int x1, const char * x2) {
    return t.ecl_sum_get_general_var(x0, x1, x2);
}

ECL_DECL int ecl_sum_get_general_var_params_index(const ecl_sum_type * x0, const char * x1) {
    return t.ecl_sum_get_general_var_params_index(x0, x1);
}

ECL_DECL const char * ecl_sum_get_keyword(const ecl_sum_type * x0, const char * x1) {
    return t.ecl_sum_get_keyword(x0, x1);
}

ECL_DECL int ecl_sum_get_last_report_step(const ecl_sum_type * x0) {
    return t.ecl_sum_get_last_report_step(x0);
}

ECL_DECL int ecl_sum_get_report_step_from_days(const ecl_sum_type * x0, double x1) {
    return t.ecl_sum_get_report_step_from_days(x0, x1);
}

ECL_DECL int ecl_sum_get_report_step_from_time(const ecl_sum_type * x0, time_t x1) {
    return t.ecl_sum_get_report_step_from_time(x0, x1);
}

ECL_DECL time_t ecl_sum_get_report_time(const ecl_sum_type * x0, int x1) {
    return t.ecl_sum_get_report_time(x0, x1);
}

ECL_DECL int * ecl_sum_get_smspec(const ecl_sum_type * x0) {
    return t.ecl_sum_get_smspec(x0);
}

ECL_DECL time_t ecl_sum_get_start_time(const ecl_sum_type * x0) {
    return t.ecl_sum_get_start_time(x0);
}

ECL_DECL const char * ecl_sum_get_wgname(const ecl_sum_type * x0, const char * x1) {
    return t.ecl_sum_get_wgname(x0, x1);
}

ECL_DECL bool ecl_sum_has_general_var(const ecl_sum_type * x0, const char * x1) {
    return t.ecl_sum_has_general_var(x0, x1);
}

ECL_DECL bool ecl_sum_has_key(const ecl_sum_type * x0, const char * x1) {
    return t.ecl_sum_has_key(x0, x1);
}

ECL_DECL bool ecl_sum_has_report_step(const ecl_sum_type * x0, int x1) {
    return t.ecl_sum_has_report_step(x0, x1);
}

ECL_DECL double ecl_sum_iget(const ecl_sum_type * x0, int x1, int x2) {
    return t.ecl_sum_iget(x0, x1, x2);
}

ECL_DECL int ecl_sum_iget_report_end(const ecl_sum_type * x0, int x1) {
    return t.ecl_sum_iget_report_end(x0, x1);
}

ECL_DECL bool ecl_sum_report_step_compatible(const ecl_sum_type * x0, const ecl_sum_type * x1) {
    return t.ecl_sum_report_step_compatible(x0, x1);
}

ECL_DECL void ecl_sum_select_matching_general_var_list(const ecl_sum_type * x0, const char * x1, int * x2) {
    t.ecl_sum_select_matching_general_var_list(x0, x1, x2);
}

// ----------------------------------------
// ert/ecl/ecl_file.hpp
// ----------------------------------------

ECL_DECL void ecl_file_close(ecl_file_type * x0) {
    t.ecl_file_close(x0);
}

ECL_DECL ecl_file_type * ecl_file_open(const char * x0, int x1) {
    return t.ecl_file_open(x0, x1);
}

// ----------------------------------------
// ert/ecl/ecl_kw.hpp
// ----------------------------------------

ECL_DECL ecl_kw_type * ecl_kw_alloc_new_shared(const char * x0, int x1, int x2, void * x3) {
    return t.ecl_kw_alloc_new_shared(x0, x1, x2, x3);
}

ECL_DECL ecl_kw_type * ecl_kw_fread_alloc(int * x0) {
    return t.ecl_kw_fread_alloc(x0);
}

ECL_DECL void ecl_kw_free(ecl_kw_type * x0) {
    t.ecl_kw_free(x0);
}

ECL_DECL bool ecl_kw_fseek_kw(const char * x0, bool x1, bool x2, int * x3) {
    return t.ecl_kw_fseek_kw(x0, x1, x2, x3);
}

ECL_DECL void ecl_kw_fwrite_param_fortio(int * x0, const char * x1, int x2, int x3, void * x4) {
    t.ecl_kw_fwrite_param_fortio(x0, x1, x2, x3, x4);
}

ECL_DECL int ecl_kw_get_data_type(const ecl_kw_type * x0) {
    return t.ecl_kw_get_data_type(x0);
}

ECL_DECL int ecl_kw_get_size(const ecl_kw_type * x0) {
    return t.ecl_kw_get_size(x0);
}

ECL_DECL void * ecl_kw_get_void_ptr(const ecl_kw_type * x0) {
    return t.ecl_kw_get_void_ptr(x0);
}

ECL_DECL bool ecl_kw_is_kw_file(int * x0) {
    return t.ecl_kw_is_kw_file(x0);
}

// ----------------------------------------
// ert/ecl/ecl_kw_grdecl.hpp
// ----------------------------------------

ECL_DECL void ecl_kw_fprintf_grdecl(const int * x0, int * x1) {
    t.ecl_kw_fprintf_grdecl(x0, x1);
}

ECL_DECL int * ecl_kw_fscanf_alloc_grdecl_data(int * x0, int x1, int x2) {
    return t.ecl_kw_fscanf_alloc_grdecl_data(x0, x1, x2);
}

ECL_DECL bool ecl_kw_grdecl_fseek_kw(const char * x0, bool x1, int * x2) {
    return t.ecl_kw_grdecl_fseek_kw(x0, x1, x2);
}

ECL_DECL bool ecl_kw_grdecl_fseek_next_kw(int * x0) {
    return t.ecl_kw_grdecl_fseek_next_kw(x0);
}

// ----------------------------------------
// ert/ecl/ecl_io_config.hpp
// ----------------------------------------

ECL_DECL ecl_io_config_type * ecl_io_config_alloc(bool x0, bool x1, bool x2) {
    return t.ecl_io_config_alloc(x0, x1, x2);
}

ECL_DECL void ecl_io_config_free(ecl_io_config_type * x0) {
    t.ecl_io_config_free(x0);
}

ECL_DECL bool ecl_io_config_get_formatted(ecl_io_config_type * x0) {
    return t.ecl_io_config_get_formatted(x0);
}

ECL_DECL bool ecl_io_config_get_unified_restart(ecl_io_config_type * x0) {
    return t.ecl_io_config_get_unified_restart(x0);
}

// ----------------------------------------
// ert/ecl/ecl_smspec.hpp
// ----------------------------------------

ECL_DECL const char * ecl_smspec_get_join_string(const ecl_smspec_type * x0) {
    return t.ecl_smspec_get_join_string(x0);
}

ECL_DECL bool ecl_smspec_has_general_var(const ecl_smspec_type * x0, const char * x1) {
    return t.ecl_smspec_has_general_var(x0, x1);
}

ECL_DECL int ecl_smspec_identify_var_type(const char * x0) {
    return t.ecl_smspec_identify_var_type(x0);
}

ECL_DECL int ecl_smspec_num_nodes(const ecl_smspec_type * x0) {
    return t.ecl_smspec_num_nodes(x0);
}

// ----------------------------------------
// ert/ecl/ecl_type.hpp
// ----------------------------------------

ECL_DECL char * ecl_type_alloc_name(const ecl_data_type x0) {
    return t.ecl_type_alloc_name(x0);
}

ECL_DECL ecl_data_type ecl_type_create_from_type(const ecl_type_enum x0) {
    return t.ecl_type_create_from_type(x0);
}

ECL_DECL int ecl_type_get_sizeof_ctype(const ecl_data_type x0) {
    return t.ecl_type_get_sizeof_ctype(x0);
}

ECL_DECL ecl_type_enum ecl_type_get_type(const ecl_data_type x0) {
    return t.ecl_type_get_type(x0);
}

ECL_DECL bool ecl_type_is_double(const ecl_data_type x0) {
    return t.ecl_type_is_double(x0);
}

ECL_DECL bool ecl_type_is_equal(const ecl_data_type x0, const ecl_data_type x1) {
    return t.ecl_type_is_equal(x0, x1);
}

ECL_DECL bool ecl_type_is_float(const ecl_data_type x0) {
    return t.ecl_type_is_float(x0);
}

ECL_DECL bool ecl_type_is_int(const ecl_data_type x0) {
    return t.ecl_type_is_int(x0);
}

// ----------------------------------------
// ert/ecl/ecl_util.hpp
// ----------------------------------------

ECL_DECL char * ecl_util_alloc_exfilename(const char * x0, const char * x1, ecl_file_enum x2, bool x3, int x4) {
    return t.ecl_util_alloc_exfilename(x0, x1, x2, x3, x4);
}

ECL_DECL char * ecl_util_alloc_filename(const char * x0, const char * x1, ecl_file_enum x2, bool x3, int x4) {
    return t.ecl_util_alloc_filename(x0, x1, x2, x3, x4);
}

ECL_DECL bool ecl_util_fmt_file(const char * x0, bool * x1) {
    return t.ecl_util_fmt_file(x0, x1);
}

ECL_DECL ecl_file_enum ecl_util_get_file_type(const char * x0, bool * x1, int * x2) {
    return t.ecl_util_get_file_type(x0, x1, x2);
}

ECL_DECL int ecl_util_get_month_nr(const char * x0) {
    return t.ecl_util_get_month_nr(x0);
}

ECL_DECL int ecl_util_get_num_cpu(const char * x0) {
    return t.ecl_util_get_num_cpu(x0);
}

ECL_DECL time_t ecl_util_get_start_date(const char * x0) {
    return t.ecl_util_get_start_date(x0);
}

ECL_DECL ert_ecl_unit_enum ecl_util_get_unit_set(const char * x0) {
    return t.ecl_util_get_unit_set(x0);
}

ECL_DECL void ecl_util_memcpy_typed_data(void * x0, const void * x1, int x2, int x3, int x4) {
    t.ecl_util_memcpy_typed_data(x0, x1, x2, x3, x4);
}

ECL_DECL bool ecl_util_valid_basename_fmt(const char * x0) {
    return t.ecl_util_valid_basename_fmt(x0);
}

// ----------------------------------------
// ert/util/double_vector.h
// ----------------------------------------

ECL_DECL double_vector_type * double_vector_alloc(int x0, double x1) {
    return t.double_vector_alloc(x0, x1);
}

ECL_DECL double_vector_type * double_vector_alloc_copy(const double_vector_type * x0) {
    return t.double_vector_alloc_copy(x0);
}

ECL_DECL int * double_vector_alloc_rsort_perm(const double_vector_type * x0) {
    return t.double_vector_alloc_rsort_perm(x0);
}

ECL_DECL double_vector_type * double_vector_alloc_shared_wrapper(int x0, double x1, double * x2, int x3) {
    return t.double_vector_alloc_shared_wrapper(x0, x1, x2, x3);
}

ECL_DECL int * double_vector_alloc_sort_perm(const double_vector_type * x0) {
    return t.double_vector_alloc_sort_perm(x0);
}

ECL_DECL void double_vector_append(double_vector_type * x0, double x1) {
    t.double_vector_append(x0, x1);
}

ECL_DECL int double_vector_element_size(const double_vector_type * x0) {
    return t.double_vector_element_size(x0);
}

ECL_DECL double_vector_type * double_vector_fread_alloc(FILE * x0) {
    return t.double_vector_fread_alloc(x0);
}

ECL_DECL void double_vector_free(double_vector_type * x0) {
    t.double_vector_free(x0);
}

ECL_DECL void double_vector_fwrite(const double_vector_type * x0, FILE * x1) {
    t.double_vector_fwrite(x0, x1);
}

ECL_DECL double double_vector_get_default(const double_vector_type * x0) {
    return t.double_vector_get_default(x0);
}

ECL_DECL double * double_vector_get_ptr(const double_vector_type * x0) {
    return t.double_vector_get_ptr(x0);
}

ECL_DECL double double_vector_iget(const double_vector_type * x0, int x1) {
    return t.double_vector_iget(x0, x1);
}

ECL_DECL void double_vector_iset(double_vector_type * x0, int x1, double x2) {
    t.double_vector_iset(x0, x1, x2);
}

ECL_DECL void double_vector_iset_default(double_vector_type * x0, int x1, double x2) {
    t.double_vector_iset_default(x0, x1, x2);
}

ECL_DECL void double_vector_memcpy(double_vector_type * x0, const double_vector_type * x1) {
    t.double_vector_memcpy(x0, x1);
}

ECL_DECL void double_vector_memcpy_from_data(double_vector_type * x0, const double * x1, int x2) {
    t.double_vector_memcpy_from_data(x0, x1, x2);
}

ECL_DECL void double_vector_permute(double_vector_type * x0, const int * x1) {
    t.double_vector_permute(x0, x1);
}

ECL_DECL void double_vector_reset(double_vector_type * x0) {
    t.double_vector_reset(x0);
}

ECL_DECL void double_vector_resize(double_vector_type * x0, int x1, double x2) {
    t.double_vector_resize(x0, x1, x2);
}

ECL_DECL double double_vector_safe_iget(const double_vector_type * x0, int x1) {
    return t.double_vector_safe_iget(x0, x1);
}

ECL_DECL void double_vector_set_default(double_vector_type * x0, double x1) {
    t.double_vector_set_default(x0, x1);
}

ECL_DECL int double_vector_size(const double_vector_type * x0) {
    return t.double_vector_size(x0);
}

// ----------------------------------------
// ert/util/bool_vector.h
// ----------------------------------------

ECL_DECL bool_vector_type * bool_vector_alloc(int x0, bool x1) {
    return t.bool_vector_alloc(x0, x1);
}

ECL_DECL bool_vector_type * bool_vector_alloc_copy(const bool_vector_type * x0) {
    return t.bool_vector_alloc_copy(x0);
}

ECL_DECL int bool_vector_count_equal(const bool_vector_type * x0, bool x1) {
    return t.bool_vector_count_equal(x0, x1);
}

ECL_DECL void bool_vector_fread(bool_vector_type * x0, FILE * x1) {
    t.bool_vector_fread(x0, x1);
}

ECL_DECL void bool_vector_free(bool_vector_type * x0) {
    t.bool_vector_free(x0);
}

ECL_DECL void bool_vector_fwrite(const bool_vector_type * x0, FILE * x1) {
    t.bool_vector_fwrite(x0, x1);
}

ECL_DECL bool * bool_vector_get_ptr(const bool_vector_type * x0) {
    return t.bool_vector_get_ptr(x0);
}

ECL_DECL int bool_vector_iget(const bool_vector_type * x0, int x1) {
    return t.bool_vector_iget(x0, x1);
}

ECL_DECL void bool_vector_iset(bool_vector_type * x0, int x1, bool x2) {
    t.bool_vector_iset(x0, x1, x2);
}

ECL_DECL void bool_vector_reset(bool_vector_type * x0) {
    t.bool_vector_reset(x0);
}

ECL_DECL void bool_vector_resize(bool_vector_type * x0, int x1, bool x2) {
    t.bool_vector_resize(x0, x1, x2);
}

ECL_DECL int bool_vector_safe_iget(const bool_vector_type * x0, int x1) {
    return t.bool_vector_safe_iget(x0, x1);
}

ECL_DECL void bool_vector_set_all(bool_vector_type * x0, bool x1) {
    t.bool_vector_set_all(x0, x1);
}

ECL_DECL void bool_vector_set_default(bool_vector_type * x0, bool x1) {
    t.bool_vector_set_default(x0, x1);
}

ECL_DECL int bool_vector_size(const bool_vector_type * x0) {
    return t.bool_vector_size(x0);
}

// ----------------------------------------
// ert/util/long_vector.h
// ----------------------------------------

ECL_DECL long_vector_type * long_vector_alloc(int x0, long x1) {
    return t.long_vector_alloc(x0, x1);
}

ECL_DECL void long_vector_append(long_vector_type * x0, long x1) {
    t.long_vector_append(x0, x1);
}

ECL_DECL void long_vector_free(long_vector_type * x0) {
    t.long_vector_free(x0);
}

ECL_DECL long long_vector_iget(const long_vector_type * x0, int x1) {
    return t.long_vector_iget(x0, x1);
}

ECL_DECL int long_vector_size(const long_vector_type * x0) {
    return t.long_vector_size(x0);
}

// ----------------------------------------
// ert/util/time_t_vector.h
// ----------------------------------------

ECL_DECL time_t_vector_type * time_t_vector_alloc(int x0, time_t x1) {
    return t.time_t_vector_alloc(x0, x1);
}

ECL_DECL time_t_vector_type * time_t_vector_alloc_shared_wrapper(int x0, time_t x1, time_t * x2, int x3) {
    return t.time_t_vector_alloc_shared_wrapper(x0, x1, x2, x3);
}

ECL_DECL int * time_t_vector_alloc_sort_perm(const time_t_vector_type * x0) {
    return t.time_t_vector_alloc_sort_perm(x0);
}

ECL_DECL void time_t_vector_append(time_t_vector_type * x0, time_t x1) {
    t.time_t_vector_append(x0, x1);
}

ECL_DECL int time_t_vector_equal(const time_t_vector_type * x0, const time_t_vector_type * x1) {
    return t.time_t_vector_equal(x0, x1);
}

ECL_DECL time_t_vector_type * time_t_vector_fread_alloc(FILE * x0) {
    return t.time_t_vector_fread_alloc(x0);
}

ECL_DECL void time_t_vector_free(time_t_vector_type * x0) {
    t.time_t_vector_free(x0);
}

ECL_DECL void time_t_vector_fwrite(const time_t_vector_type * x0, FILE * x1) {
    t.time_t_vector_fwrite(x0, x1);
}

ECL_DECL int time_t_vector_iget(const time_t_vector_type * x0, int x1) {
    return t.time_t_vector_iget(x0, x1);
}

ECL_DECL void time_t_vector_iset(time_t_vector_type * x0, int x1, time_t x2) {
    t.time_t_vector_iset(x0, x1, x2);
}

ECL_DECL void time_t_vector_iset_block(time_t_vector_type * x0, int x1, int x2, time_t x3) {
    t.time_t_vector_iset_block(x0, x1, x2, x3);
}

ECL_DECL void time_t_vector_permute(time_t_vector_type * x0, const int * x1) {
    t.time_t_vector_permute(x0, x1);
}

ECL_DECL void time_t_vector_reset(time_t_vector_type * x0) {
    t.time_t_vector_reset(x0);
}

ECL_DECL void time_t_vector_resize(time_t_vector_type * x0, int x1, time_t x2) {
    t.time_t_vector_resize(x0, x1, x2);
}

ECL_DECL int time_t_vector_safe_iget(const time_t_vector_type * x0, int x1) {
    return t.time_t_vector_safe_iget(x0, x1);
}

ECL_DECL int time_t_vector_size(const time_t_vector_type * x0) {
    return t.time_t_vector_size(x0);
}

// ----------------------------------------
// ert/util/buffer_string.h
// ----------------------------------------

ECL_DECL char * buffer_fread_alloc_string(int * x0) {
    return t.buffer_fread_alloc_string(x0);
}

ECL_DECL const char * buffer_fread_string(int * x0) {
    return t.buffer_fread_string(x0);
}

ECL_DECL void buffer_fwrite_string(int * x0, const char * x1) {
    t.buffer_fwrite_string(x0, x1);
}

// ----------------------------------------
// ert/util/util.h
// ----------------------------------------

ECL_DECL void util_abort__(const char * x0, const char * x1, int x2, const char * x3) {
    t.util_abort__(x0, x1, x2, x3);
}

ECL_DECL int util_addmode_if_owner(const char * x0, mode_t x1) {
    return t.util_addmode_if_owner(x0, x1);
}

ECL_DECL char * util_alloc_abs_path(const char * x0) {
    return t.util_alloc_abs_path(x0);
}

ECL_DECL char * util_alloc_atlink_target(const char * x0, const char * x1) {
    return t.util_alloc_atlink_target(x0, x1);
}

ECL_DECL void * util_alloc_copy(const void * x0, int x1) {
    return t.util_alloc_copy(x0, x1);
}

ECL_DECL char * util_alloc_cwd() {
    return t.util_alloc_cwd();
}

ECL_DECL char * util_alloc_date_stamp_utc() {
    return t.util_alloc_date_stamp_utc();
}

ECL_DECL char * util_alloc_dequoted_copy(const char * x0) {
    return t.util_alloc_dequoted_copy(x0);
}

ECL_DECL void util_alloc_file_components(const char * x0, char ** x1, char ** x2, char ** x3) {
    t.util_alloc_file_components(x0, x1, x2, x3);
}

ECL_DECL char * util_alloc_filename(const char * x0, const char * x1, const char * x2) {
    return t.util_alloc_filename(x0, x1, x2);
}

ECL_DECL char * util_alloc_joined_string(const char ** x0, int x1, const char * x2) {
    return t.util_alloc_joined_string(x0, x1, x2);
}

ECL_DECL char * util_alloc_link_target(const char * x0) {
    return t.util_alloc_link_target(x0);
}

ECL_DECL char * util_alloc_normal_path(const char * x0) {
    return t.util_alloc_normal_path(x0);
}

ECL_DECL char * util_alloc_realpath(const char * x0) {
    return t.util_alloc_realpath(x0);
}

ECL_DECL char * util_alloc_realpath__(const char * x0) {
    return t.util_alloc_realpath__(x0);
}

ECL_DECL char * util_alloc_rel_path(const char * x0, const char * x1) {
    return t.util_alloc_rel_path(x0, x1);
}

ECL_DECL char * util_alloc_sprintf(const char * x0) {
    return t.util_alloc_sprintf(x0);
}

ECL_DECL char * util_alloc_sprintf_va(const char * x0, va_list x1) {
    return t.util_alloc_sprintf_va(x0, x1);
}

ECL_DECL char * util_alloc_string_copy(const char * x0) {
    return t.util_alloc_string_copy(x0);
}

ECL_DECL char ** util_alloc_stringlist_copy(const char *const * x0, int x1) {
    return t.util_alloc_stringlist_copy(x0, x1);
}

ECL_DECL char * util_alloc_strupr_copy(const char * x0) {
    return t.util_alloc_strupr_copy(x0);
}

ECL_DECL char * util_alloc_substring_copy(const char * x0, int x1, int x2) {
    return t.util_alloc_substring_copy(x0, x1, x2);
}

ECL_DECL char * util_alloc_tmp_file(const char * x0, const char * x1, bool x2) {
    return t.util_alloc_tmp_file(x0, x1, x2);
}

ECL_DECL void util_binary_split_string(const char * x0, const char * x1, bool x2, char ** x3, char ** x4) {
    t.util_binary_split_string(x0, x1, x2, x3, x4);
}

ECL_DECL void * util_calloc(int x0, int x1) {
    return t.util_calloc(x0, x1);
}

ECL_DECL int util_chdir(const char * x0) {
    return t.util_chdir(x0);
}

ECL_DECL int util_chdir_file(const char * x0) {
    return t.util_chdir_file(x0);
}

ECL_DECL void util_clamp_double(double * x0, double x1, double x2) {
    t.util_clamp_double(x0, x1, x2);
}

ECL_DECL void util_clear_directory(const char * x0, bool x1, bool x2) {
    t.util_clear_directory(x0, x1, x2);
}

ECL_DECL void util_copy_directory(const char * x0, const char * x1) {
    t.util_copy_directory(x0, x1);
}

ECL_DECL int util_copy_file(const char * x0, const char * x1) {
    return t.util_copy_file(x0, x1);
}

ECL_DECL unsigned int util_dev_urandom_seed() {
    return t.util_dev_urandom_seed();
}

ECL_DECL double util_difftime_days(time_t x0, time_t x1) {
    return t.util_difftime_days(x0, x1);
}

ECL_DECL double util_difftime_seconds(time_t x0, time_t x1) {
    return t.util_difftime_seconds(x0, x1);
}

ECL_DECL double util_double_max(double x0, double x1) {
    return t.util_double_max(x0, x1);
}

ECL_DECL double util_double_min(double x0, double x1) {
    return t.util_double_min(x0, x1);
}

ECL_DECL void util_double_to_float(float * x0, const double * x1, int x2) {
    t.util_double_to_float(x0, x1, x2);
}

ECL_DECL double util_double_vector_mean(int x0, const double * x1) {
    return t.util_double_vector_mean(x0, x1);
}

ECL_DECL double util_double_vector_stddev(int x0, const double * x1) {
    return t.util_double_vector_stddev(x0, x1);
}

ECL_DECL void util_endian_flip_vector(void * x0, int x1, int x2) {
    t.util_endian_flip_vector(x0, x1, x2);
}

ECL_DECL int util_entry_exists(const char * x0) {
    return t.util_entry_exists(x0);
}

ECL_DECL int util_entry_readable(const char * x0) {
    return t.util_entry_readable(x0);
}

ECL_DECL void util_exit(const char * x0) {
    t.util_exit(x0);
}

ECL_DECL int util_file_exists(const char * x0) {
    return t.util_file_exists(x0);
}

ECL_DECL time_t util_file_mtime(const char * x0) {
    return t.util_file_mtime(x0);
}

ECL_DECL int util_file_readable(const char * x0) {
    return t.util_file_readable(x0);
}

ECL_DECL int util_file_size(const char * x0) {
    return t.util_file_size(x0);
}

ECL_DECL float util_float_max(float x0, float x1) {
    return t.util_float_max(x0, x1);
}

ECL_DECL void util_float_to_double(double * x0, const float * x1, int x2) {
    t.util_float_to_double(x0, x1, x2);
}

ECL_DECL int util_fmt_bit8(const char * x0) {
    return t.util_fmt_bit8(x0);
}

ECL_DECL int util_fnmatch(const char * x0, const char * x1) {
    return t.util_fnmatch(x0, x1);
}

ECL_DECL FILE * util_fopen(const char * x0, const char * x1) {
    return t.util_fopen(x0, x1);
}

ECL_DECL FILE * util_fopen__(const char * x0, const char * x1) {
    return t.util_fopen__(x0, x1);
}

ECL_DECL void util_fread(void * x0, int x1, int x2, FILE * x3, const char * x4) {
    t.util_fread(x0, x1, x2, x3, x4);
}

ECL_DECL char * util_fread_alloc_file_content(const char * x0, int * x1) {
    return t.util_fread_alloc_file_content(x0, x1);
}

ECL_DECL char * util_fread_alloc_string(FILE * x0) {
    return t.util_fread_alloc_string(x0);
}

ECL_DECL double util_fread_double(FILE * x0) {
    return t.util_fread_double(x0);
}

ECL_DECL int util_fread_int(FILE * x0) {
    return t.util_fread_int(x0);
}

ECL_DECL long util_fread_long(FILE * x0) {
    return t.util_fread_long(x0);
}

ECL_DECL char * util_fread_realloc_string(char * x0, FILE * x1) {
    return t.util_fread_realloc_string(x0, x1);
}

ECL_DECL time_t util_fread_time_t(FILE * x0) {
    return t.util_fread_time_t(x0);
}

ECL_DECL void util_free_NULL_terminated_stringlist(char ** x0) {
    t.util_free_NULL_terminated_stringlist(x0);
}

ECL_DECL void util_free_stringlist(char ** x0, int x1) {
    t.util_free_stringlist(x0, x1);
}

ECL_DECL char * util_fscanf_alloc_line(FILE * x0, bool * x1) {
    return t.util_fscanf_alloc_line(x0, x1);
}

ECL_DECL char * util_fscanf_alloc_token(FILE * x0) {
    return t.util_fscanf_alloc_token(x0);
}

ECL_DECL char * util_fscanf_alloc_upto(FILE * x0, const char * x1, bool x2) {
    return t.util_fscanf_alloc_upto(x0, x1, x2);
}

ECL_DECL int util_fseek_string(FILE * x0, const char * x1, bool x2, bool x3) {
    return t.util_fseek_string(x0, x1, x2, x3);
}

ECL_DECL void util_fskip_int(FILE * x0) {
    t.util_fskip_int(x0);
}

ECL_DECL void util_fskip_lines(FILE * x0, int x1) {
    t.util_fskip_lines(x0, x1);
}

ECL_DECL offset_type util_ftell(FILE * x0) {
    return t.util_ftell(x0);
}

ECL_DECL void util_fwrite(const void * x0, int x1, int x2, FILE * x3, const char * x4) {
    t.util_fwrite(x0, x1, x2, x3, x4);
}

ECL_DECL void util_fwrite_double(double x0, FILE * x1) {
    t.util_fwrite_double(x0, x1);
}

ECL_DECL void util_fwrite_int(int x0, FILE * x1) {
    t.util_fwrite_int(x0, x1);
}

ECL_DECL void util_fwrite_long(long x0, FILE * x1) {
    t.util_fwrite_long(x0, x1);
}

ECL_DECL void util_fwrite_string(const char * x0, FILE * x1) {
    t.util_fwrite_string(x0, x1);
}

ECL_DECL void util_fwrite_time_t(time_t x0, FILE * x1) {
    t.util_fwrite_time_t(x0, x1);
}

ECL_DECL int util_get_current_linenr(FILE * x0) {
    return t.util_get_current_linenr(x0);
}

ECL_DECL void util_inplace_forward_days_utc(time_t * x0, double x1) {
    t.util_inplace_forward_days_utc(x0, x1);
}

ECL_DECL void util_inplace_forward_seconds_utc(time_t * x0, double x1) {
    t.util_inplace_forward_seconds_utc(x0, x1);
}

ECL_DECL void util_install_signals() {
    t.util_install_signals();
}

ECL_DECL int util_int_format_count(const char * x0) {
    return t.util_int_format_count(x0);
}

ECL_DECL int util_int_max(int x0, int x1) {
    return t.util_int_max(x0, x1);
}

ECL_DECL int util_int_min(int x0, int x1) {
    return t.util_int_min(x0, x1);
}

ECL_DECL int util_is_abs_path(const char * x0) {
    return t.util_is_abs_path(x0);
}

ECL_DECL int util_is_directory(const char * x0) {
    return t.util_is_directory(x0);
}

ECL_DECL int util_is_executable(const char * x0) {
    return t.util_is_executable(x0);
}

ECL_DECL int util_is_file(const char * x0) {
    return t.util_is_file(x0);
}

ECL_DECL int util_is_link(const char * x0) {
    return t.util_is_link(x0);
}

ECL_DECL time_t util_make_date_utc(int x0, int x1, int x2) {
    return t.util_make_date_utc(x0, x1, x2);
}

ECL_DECL void util_make_path(const char * x0) {
    t.util_make_path(x0);
}

ECL_DECL void * util_malloc() {
    return t.util_malloc();
}

ECL_DECL FILE * util_mkdir_fopen(const char * x0, const char * x1) {
    return t.util_mkdir_fopen(x0, x1);
}

ECL_DECL int util_mkdir_p(const char * x0) {
    return t.util_mkdir_p(x0);
}

ECL_DECL void util_move_file(const char * x0, const char * x1) {
    t.util_move_file(x0, x1);
}

ECL_DECL void util_path_split(const char * x0, int * x1, char *** x2) {
    t.util_path_split(x0, x1, x2);
}

ECL_DECL void * util_realloc(void * x0, int x1) {
    return t.util_realloc(x0, x1);
}

ECL_DECL void * util_realloc_copy(void * x0, const void * x1, int x2) {
    return t.util_realloc_copy(x0, x1, x2);
}

ECL_DECL char * util_realloc_sprintf(char * x0, const char * x1) {
    return t.util_realloc_sprintf(x0, x1);
}

ECL_DECL char * util_realloc_string_copy(char * x0, const char * x1) {
    return t.util_realloc_string_copy(x0, x1);
}

ECL_DECL int util_same_file(const char * x0, const char * x1) {
    return t.util_same_file(x0, x1);
}

ECL_DECL void util_set_date_values_utc(time_t x0, int * x1, int * x2, int * x3) {
    t.util_set_date_values_utc(x0, x1, x2, x3);
}

ECL_DECL void util_set_datetime_values_utc(time_t x0, int * x1, int * x2, int * x3, int * x4, int * x5, int * x6) {
    t.util_set_datetime_values_utc(x0, x1, x2, x3, x4, x5, x6);
}

ECL_DECL int util_size_t_max(int x0, int x1) {
    return t.util_size_t_max(x0, x1);
}

ECL_DECL pid_t util_spawn(const char * x0, int x1, const char ** x2, const char * x3, const char * x4) {
    return t.util_spawn(x0, x1, x2, x3, x4);
}

ECL_DECL int util_spawn_blocking(const char * x0, int x1, const char ** x2, const char * x3, const char * x4) {
    return t.util_spawn_blocking(x0, x1, x2, x3, x4);
}

ECL_DECL char * util_split_alloc_dirname(const char * x0) {
    return t.util_split_alloc_dirname(x0);
}

ECL_DECL char * util_split_alloc_filename(const char * x0) {
    return t.util_split_alloc_filename(x0);
}

ECL_DECL void util_split_string(const char * x0, const char * x1, int * x2, char *** x3) {
    t.util_split_string(x0, x1, x2, x3);
}

ECL_DECL int util_sscanf_bool(const char * x0, bool * x1) {
    return t.util_sscanf_bool(x0, x1);
}

ECL_DECL int util_sscanf_bytesize(const char * x0, int * x1) {
    return t.util_sscanf_bytesize(x0, x1);
}

ECL_DECL int util_sscanf_date_utc(const char * x0, time_t * x1) {
    return t.util_sscanf_date_utc(x0, x1);
}

ECL_DECL int util_sscanf_double(const char * x0, double * x1) {
    return t.util_sscanf_double(x0, x1);
}

ECL_DECL int util_sscanf_int(const char * x0, int * x1) {
    return t.util_sscanf_int(x0, x1);
}

ECL_DECL int util_sscanf_isodate(const char * x0, time_t * x1) {
    return t.util_sscanf_isodate(x0, x1);
}

ECL_DECL int util_sscanf_octal_int(const char * x0, int * x1) {
    return t.util_sscanf_octal_int(x0, x1);
}

ECL_DECL int util_sscanf_percent(const char * x0, double * x1) {
    return t.util_sscanf_percent(x0, x1);
}

ECL_DECL char * util_strcat_realloc(char * x0, const char * x1) {
    return t.util_strcat_realloc(x0, x1);
}

ECL_DECL int util_strcmp_int(const char * x0, const char * x1) {
    return t.util_strcmp_int(x0, x1);
}

ECL_DECL int util_string_equal(const char * x0, const char * x1) {
    return t.util_string_equal(x0, x1);
}

ECL_DECL int util_string_has_wildcard(const char * x0) {
    return t.util_string_has_wildcard(x0);
}

ECL_DECL int util_string_isspace(const char * x0) {
    return t.util_string_isspace(x0);
}

ECL_DECL int util_string_match(const char * x0, const char * x1) {
    return t.util_string_match(x0, x1);
}

ECL_DECL char * util_string_replace_alloc(const char * x0, const char * x1, const char * x2) {
    return t.util_string_replace_alloc(x0, x1, x2);
}

ECL_DECL char * util_string_replacen_alloc(const char * x0, int x1, const char ** x2, const char ** x3) {
    return t.util_string_replacen_alloc(x0, x1, x2, x3);
}

ECL_DECL void util_strupr(char * x0) {
    t.util_strupr(x0);
}

ECL_DECL time_t util_time_t_max(time_t x0, time_t x1) {
    return t.util_time_t_max(x0, x1);
}

ECL_DECL void util_time_utc(time_t * x0, struct tm * x1) {
    t.util_time_utc(x0, x1);
}

ECL_DECL int util_try_lockf(const char * x0, mode_t x1, int * x2) {
    return t.util_try_lockf(x0, x1, x2);
}

ECL_DECL void util_unlink_existing(const char * x0) {
    t.util_unlink_existing(x0);
}

ECL_DECL void util_update_double_max_min(double x0, double * x1, double * x2) {
    t.util_update_double_max_min(x0, x1, x2);
}

ECL_DECL void util_update_float_max_min(float x0, float * x1, float * x2) {
    t.util_update_float_max_min(x0, x1, x2);
}

ECL_DECL void util_update_int_max_min(int x0, int * x1, int * x2) {
    t.util_update_int_max_min(x0, x1, x2);
}

ECL_DECL void util_usleep(unsigned long x0) {
    t.util_usleep(x0);
}

// ----------------------------------------
// ert/util/int_vector.h
// ----------------------------------------

ECL_DECL int_vector_type * int_vector_alloc(int x0, int x1) {
    return t.int_vector_alloc(x0, x1);
}

ECL_DECL int_vector_type * int_vector_alloc_copy(const int_vector_type * x0) {
    return t.int_vector_alloc_copy(x0);
}

ECL_DECL int * int_vector_alloc_data_copy(const int_vector_type * x0) {
    return t.int_vector_alloc_data_copy(x0);
}

ECL_DECL void int_vector_append(int_vector_type * x0, int x1) {
    t.int_vector_append(x0, x1);
}

ECL_DECL int int_vector_contains(const int_vector_type * x0, int x1) {
    return t.int_vector_contains(x0, x1);
}

ECL_DECL int int_vector_contains_sorted(const int_vector_type * x0, int x1) {
    return t.int_vector_contains_sorted(x0, x1);
}

ECL_DECL int int_vector_equal(const int_vector_type * x0, const int_vector_type * x1) {
    return t.int_vector_equal(x0, x1);
}

ECL_DECL void int_vector_fread(int_vector_type * x0, FILE * x1) {
    t.int_vector_fread(x0, x1);
}

ECL_DECL void int_vector_free(int_vector_type * x0) {
    t.int_vector_free(x0);
}

ECL_DECL void int_vector_fwrite(const int_vector_type * x0, FILE * x1) {
    t.int_vector_fwrite(x0, x1);
}

ECL_DECL const int * int_vector_get_const_ptr(const int_vector_type * x0) {
    return t.int_vector_get_const_ptr(x0);
}

ECL_DECL int int_vector_get_first(const int_vector_type * x0) {
    return t.int_vector_get_first(x0);
}

ECL_DECL int int_vector_get_last(const int_vector_type * x0) {
    return t.int_vector_get_last(x0);
}

ECL_DECL int * int_vector_get_ptr(const int_vector_type * x0) {
    return t.int_vector_get_ptr(x0);
}

ECL_DECL int int_vector_iget(const int_vector_type * x0, int x1) {
    return t.int_vector_iget(x0, x1);
}

ECL_DECL int int_vector_index_sorted(const int_vector_type * x0, int x1) {
    return t.int_vector_index_sorted(x0, x1);
}

ECL_DECL void int_vector_init_range(int_vector_type * x0, int x1, int x2, int x3) {
    t.int_vector_init_range(x0, x1, x2, x3);
}

ECL_DECL void int_vector_iset(int_vector_type * x0, int x1, int x2) {
    t.int_vector_iset(x0, x1, x2);
}

ECL_DECL void int_vector_iset_block(int_vector_type * x0, int x1, int x2, int x3) {
    t.int_vector_iset_block(x0, x1, x2, x3);
}

ECL_DECL void int_vector_iset_default(int_vector_type * x0, int x1, int x2) {
    t.int_vector_iset_default(x0, x1, x2);
}

ECL_DECL void int_vector_memcpy(int_vector_type * x0, const int_vector_type * x1) {
    t.int_vector_memcpy(x0, x1);
}

ECL_DECL void int_vector_reset(int_vector_type * x0) {
    t.int_vector_reset(x0);
}

ECL_DECL void int_vector_resize(int_vector_type * x0, int x1, int x2) {
    t.int_vector_resize(x0, x1, x2);
}

ECL_DECL int int_vector_safe_iget(const int_vector_type * x0, int x1) {
    return t.int_vector_safe_iget(x0, x1);
}

ECL_DECL void int_vector_select_unique(int_vector_type * x0) {
    t.int_vector_select_unique(x0);
}

ECL_DECL void int_vector_set_default(int_vector_type * x0, int x1) {
    t.int_vector_set_default(x0, x1);
}

ECL_DECL void int_vector_shrink(int_vector_type * x0) {
    t.int_vector_shrink(x0);
}

ECL_DECL int int_vector_size(const int_vector_type * x0) {
    return t.int_vector_size(x0);
}

ECL_DECL void int_vector_sort(int_vector_type * x0) {
    t.int_vector_sort(x0);
}

// ----------------------------------------
// ert/util/size_t_vector.h
// ----------------------------------------

ECL_DECL void size_t_vector_append(size_t_vector_type * x0, size_t x1) {
    t.size_t_vector_append(x0, x1);
}

ECL_DECL void size_t_vector_free(size_t_vector_type * x0) {
    t.size_t_vector_free(x0);
}

ECL_DECL int size_t_vector_iget(const size_t_vector_type * x0, int x1) {
    return t.size_t_vector_iget(x0, x1);
}

ECL_DECL void size_t_vector_iset_default(size_t_vector_type * x0, int x1, size_t x2) {
    t.size_t_vector_iset_default(x0, x1, x2);
}

ECL_DECL int size_t_vector_safe_iget(const size_t_vector_type * x0, int x1) {
    return t.size_t_vector_safe_iget(x0, x1);
}

// ----------------------------------------
// ert/ecl/smspec_node.h
// ----------------------------------------

ECL_DECL char * smspec_alloc_block_ijk_key(const char * x0, const char * x1, int x2, int x3, int x4) {
    return t.smspec_alloc_block_ijk_key(x0, x1, x2, x3, x4);
}

// ----------------------------------------
// ert/ecl/fortio.h
// ----------------------------------------

ECL_DECL fortio_type * fortio_alloc_FILE_wrapper(const char * x0, int x1, int x2, int x3, FILE * x4) {
    return t.fortio_alloc_FILE_wrapper(x0, x1, x2, x3, x4);
}

ECL_DECL void fortio_fclose(fortio_type * x0) {
    t.fortio_fclose(x0);
}

ECL_DECL void fortio_free_FILE_wrapper(fortio_type * x0) {
    t.fortio_free_FILE_wrapper(x0);
}

ECL_DECL fortio_type * fortio_open_reader(const char * x0, int x1, int x2) {
    return t.fortio_open_reader(x0, x1, x2);
}

ECL_DECL fortio_type * fortio_open_writer(const char * x0, int x1, int x2) {
    return t.fortio_open_writer(x0, x1, x2);
}
