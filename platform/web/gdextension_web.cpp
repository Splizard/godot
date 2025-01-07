// Here we export the GDExtension API to Javascript.

#include "core/extension/gdextension_interface.cpp"
#include "core/extension/gdextension_interface.h"
#include <emscripten/bind.h>
#include <stdint.h>
#include <string>

using namespace emscripten;

uint32_t stack[16];
uint32_t stack2[16];
uint32_t callframe[256];
uint32_t callframe2[256];
uint32_t callframe3[256];

GDExtensionCallError error;

void js_get_godot_version(val r_godot_version) {
	GDExtensionGodotVersion version;
	gdextension_get_godot_version(&version);
	r_godot_version.set("major", version.major);
	r_godot_version.set("minor", version.minor);
	r_godot_version.set("patch", version.patch);
	r_godot_version.set("string", version.string);
}
uint32_t js_mem_alloc(uint32_t p_size) {
	return (uintptr_t)(gdextension_mem_alloc(p_size));
}
uint32_t js_mem_realloc(uint32_t p_ptr, uint32_t p_size) {
	return (uintptr_t)(gdextension_mem_realloc((void *)p_ptr, p_size));
}
void js_mem_free(uintptr_t p_ptr) {
	gdextension_mem_free((void *)p_ptr);
}
void js_print_error(std::string p_description, std::string p_function, std::string p_file, int32_t p_line, bool p_editor_notify) {
	gdextension_print_error(p_description.c_str(), p_function.c_str(), p_file.c_str(), p_line, p_editor_notify);
}
void js_print_error_with_message(std::string p_description, std::string p_message, std::string p_function, std::string p_file, int32_t p_line, bool p_editor_notify) {
	gdextension_print_error_with_message(p_description.c_str(), p_message.c_str(), p_function.c_str(), p_file.c_str(), p_line, p_editor_notify);
}
void js_print_warning(std::string p_description, std::string p_function, std::string p_file, int32_t p_line, bool p_editor_notify) {
	gdextension_print_warning(p_description.c_str(), p_function.c_str(), p_file.c_str(), p_line, p_editor_notify);
}
void js_print_warning_with_message(std::string p_description, std::string p_message, std::string p_function, std::string p_file, int32_t p_line, bool p_editor_notify) {
	gdextension_print_warning_with_message(p_description.c_str(), p_message.c_str(), p_function.c_str(), p_file.c_str(), p_line, p_editor_notify);
}
void js_print_script_error(std::string p_description, std::string p_function, std::string p_file, int32_t p_line, bool p_editor_notify) {
	gdextension_print_script_error(p_description.c_str(), p_function.c_str(), p_file.c_str(), p_line, p_editor_notify);
}
void js_print_script_error_with_message(std::string p_description, std::string p_message, std::string p_function, std::string p_file, int32_t p_line, bool p_editor_notify) {
	gdextension_print_script_error_with_message(p_description.c_str(), p_message.c_str(), p_function.c_str(), p_file.c_str(), p_line, p_editor_notify);
}
uint32_t js_get_native_struct_size(uint32_t p_name) {
	return gdextension_get_native_struct_size(&p_name);
}

void js_variant_new_copy(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t src[6] = {v1, v2, v3, v4, v5, v6};
	gdextension_variant_new_copy(&stack, &src);
}
void js_variant_new_nil() {
	gdextension_variant_new_nil(&stack);
}
void js_variant_destroy(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	gdextension_variant_destroy(&self);
}
int js_variant_call(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t method, int64_t argc) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	error.error = (GDExtensionCallErrorType)0;
	gdextension_variant_call(&self, &method, (const GDExtensionConstVariantPtr *)&callframe, argc, &stack, &error);
	return error.error;
}
int js_variant_call_static(uint32_t vtype, uint32_t method, int64_t argc) {
	error.error = (GDExtensionCallErrorType)0;
	gdextension_variant_call_static((GDExtensionVariantType)vtype, &method, (const GDExtensionConstVariantPtr *)&callframe, argc, &stack, &error);
	return error.error;
}
bool js_variant_evaluate(uint32_t op,
	uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6,
	uint32_t b1, uint32_t b2, uint32_t b3, uint32_t b4, uint32_t b5, uint32_t b6
) {
	uint32_t a[6] = {a1, a2, a3, a4, a5, a6};
	uint32_t b[6] = {b1, b2, b3, b4, b5, b6};
	bool valid;
	gdextension_variant_evaluate((GDExtensionVariantOperator)op, &a, &b, &stack, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_set(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6,
	uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4, uint32_t val5, uint32_t val6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	uint32_t val[6] = {val1, val2, val3, val4, val5, val6};
	bool valid;
	gdextension_variant_set(&self, &key, &val, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_set_named(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t name,
	uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4, uint32_t val5, uint32_t val6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t val[6] = {val1, val2, val3, val4, val5, val6};
	bool valid;
	gdextension_variant_set_named(&self, &name, &val, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_set_keyed(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6,
	uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4, uint32_t val5, uint32_t val6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	uint32_t val[6] = {val1, val2, val3, val4, val5, val6};
	bool valid;
	gdextension_variant_set_keyed(&self, &key, &val, (GDExtensionBool *)&valid);
	return valid;
}
int js_variant_set_indexed(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	int64_t index,
	uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4, uint32_t val5, uint32_t val6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t val[6] = {val1, val2, val3, val4, val5, val6};
	bool valid;
	bool oob;
	gdextension_variant_set_indexed(&self, index, &val, (GDExtensionBool *)&valid, (GDExtensionBool *)&oob);
	return valid + oob;
}
bool js_variant_get(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	bool valid;
	gdextension_variant_get(&self, &key, &stack, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_get_named(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t name
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	gdextension_variant_get_named(&self, &name, &stack, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_get_keyed(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	bool valid;
	gdextension_variant_get_keyed(&self, &key, &stack, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_get_indexed(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	int64_t index
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	bool oob;
	gdextension_variant_get_indexed(&self, index, &stack, (GDExtensionBool *)&valid, (GDExtensionBool *)&oob);
	return valid + oob;
}
bool js_variant_iter_init(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	gdextension_variant_iter_init(&self, &stack, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_iter_next(
	uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6
) {
	uint32_t iter[6] = {i1, i2, i3, i4, i5, i6};
	bool valid;
	gdextension_variant_iter_next(&iter, &stack, (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_iter_get(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	gdextension_variant_iter_get(&self, &stack, &stack2, (GDExtensionBool *)&valid);
	return valid;
}
int64_t js_variant_hash(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_hash(&self);
}
int64_t js_variant_recursive_hash(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	int64_t p_recursion_count
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_recursive_hash(&self, p_recursion_count);
}
bool js_variant_hash_compare(
	uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6,
	uint32_t b1, uint32_t b2, uint32_t b3, uint32_t b4, uint32_t b5, uint32_t b6
) {
	uint32_t a[6] = {a1, a2, a3, a4, a5, a6};
	uint32_t b[6] = {b1, b2, b3, b4, b5, b6};
	return gdextension_variant_hash_compare(&a, &b);
}
bool js_variant_booleanize(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_booleanize(&self);
}
void js_variant_duplicate(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	bool p_deep
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	gdextension_variant_duplicate(&self,  &stack, p_deep);
}
uint32_t js_variant_stringify(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t result;
	gdextension_variant_stringify(&self, &result);
	return result;
}
uint32_t js_variant_get_type(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_get_type(&self);
}
bool js_variant_has_method(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t p_method) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_has_method(&self, &p_method);
}
bool js_variant_has_member(uint32_t p_type, uint32_t p_member) {
	return gdextension_variant_has_member((GDExtensionVariantType)p_type, &p_member);
}
bool js_variant_has_key(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t p_key) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_has_key(&self, &p_key, (GDExtensionBool*)&stack);
}
uint64_t js_variant_get_object_instance_id(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	return gdextension_variant_get_object_instance_id(&self);
}
uint32_t js_variant_get_type_name(uint32_t p_type) {
	uint32_t result;
	gdextension_variant_get_type_name((GDExtensionVariantType)p_type, &result);
	return result;
}
bool js_variant_can_convert(uint32_t p_type, uint32_t p_to_type) {
	return gdextension_variant_can_convert((GDExtensionVariantType)p_type, (GDExtensionVariantType)p_to_type);
}
bool js_variant_can_convert_strict(uint32_t p_type, uint32_t p_to_type) {
	return gdextension_variant_can_convert_strict((GDExtensionVariantType)p_type, (GDExtensionVariantType)p_to_type);
}
uint32_t js_get_variant_from_type_constructor(uint32_t p_type) {
	return (uint32_t)gdextension_get_variant_from_type_constructor((GDExtensionVariantType)p_type);
}
void js_call_variant_from_type_constructor(uint32_t fn) {
	((GDExtensionVariantFromTypeConstructorFunc)fn)(&stack, &callframe);
}
uint32_t js_get_variant_to_type_constructor(uint32_t p_type) {
	return (uint32_t)gdextension_get_variant_to_type_constructor((GDExtensionVariantType)p_type);
}
void js_call_variant_to_type_constructor(uint32_t fn) {
	((GDExtensionTypeFromVariantConstructorFunc)fn)(&stack, &callframe);
}
uint32_t js_variant_get_ptr_internal_getter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_internal_getter((GDExtensionVariantType)p_type);
}
uint32_t js_call_variant_get_ptr_internal_getter(uint32_t fn) {
	return (uintptr_t)((GDExtensionVariantGetInternalPtrFunc)fn)(&callframe);
}
uint32_t js_variant_get_ptr_operator_evaluator(uint32_t p_op, uint32_t a_type, uint32_t b_type) {
	return (uint32_t)gdextension_variant_get_ptr_operator_evaluator((GDExtensionVariantOperator)p_op, (GDExtensionVariantType)a_type, (GDExtensionVariantType)b_type);
}
void js_call_variant_get_ptr_operator_evaluator(uint32_t fn) {
	((GDExtensionPtrOperatorEvaluator)fn)(&callframe, &callframe2, &stack);
}
uint32_t js_variant_get_ptr_builtin_method(uint32_t p_type, uint32_t p_method, int64_t hash) {
	return (uint32_t)gdextension_variant_get_ptr_builtin_method((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_method, hash);
}
void js_call_variant_get_ptr_builtin_method(uint32_t fn, int argc) {
	((GDExtensionPtrBuiltInMethod)fn)(&callframe, (const GDExtensionConstTypePtr *
	)&callframe2, &stack, argc);
}
uint32_t js_variant_get_ptr_constructor(uint32_t p_type, int32_t p_constructor) {
	return (uint32_t)gdextension_variant_get_ptr_constructor((GDExtensionVariantType)p_type, p_constructor);
}
void js_call_variant_get_ptr_constructor(uint32_t fn) {
	((GDExtensionPtrConstructor)fn)(&stack, (const GDExtensionConstTypePtr *)&callframe);
}
uint32_t js_variant_get_ptr_destructor(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_destructor((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_destructor(uint32_t fn) {
	((GDExtensionPtrDestructor)fn)(&callframe);
}
int js_variant_construct(uint32_t p_type, int32_t p_argument_count) {
	error.error = (GDExtensionCallErrorType)0;
	gdextension_variant_construct((GDExtensionVariantType)p_type, &stack, (const GDExtensionConstVariantPtr *)&callframe, p_argument_count, &error);
	return error.error;
}
uint32_t js_variant_get_ptr_setter(uint32_t p_type, uint32_t p_member) {
	return (uint32_t)gdextension_variant_get_ptr_setter((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_member);
}
void js_call_variant_get_ptr_setter(uint32_t fn) {
	((GDExtensionPtrSetter)fn)(&callframe, &callframe2);
}
uint32_t js_variant_get_ptr_getter(uint32_t p_type, uint32_t p_member) {
	return (uint32_t)gdextension_variant_get_ptr_getter((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_member);
}
void js_call_variant_get_ptr_getter(uint32_t fn) {
	((GDExtensionPtrGetter)fn)(&callframe, &stack);
}
uint32_t js_variant_get_ptr_indexed_setter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_indexed_setter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_indexed_setter(uint32_t fn, int64_t index) {
	((GDExtensionPtrIndexedSetter)fn)(&callframe, index, &callframe2);
}
uint32_t js_variant_get_ptr_indexed_getter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_indexed_getter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_indexed_getter(uint32_t fn, int64_t index) {
	((GDExtensionPtrIndexedGetter)fn)(&callframe, index, &stack);
}
uint32_t js_variant_get_ptr_keyed_setter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_keyed_setter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_keyed_setter(uint32_t fn) {
	((GDExtensionPtrKeyedSetter)fn)(&callframe, &callframe2, &callframe3);
}
uint32_t js_variant_get_ptr_keyed_getter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_keyed_getter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_keyed_getter(uint32_t fn) {
	((GDExtensionPtrKeyedGetter)fn)(&callframe, &callframe2, &stack);
}
uint32_t js_variant_get_ptr_keyed_checker(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_keyed_checker((GDExtensionVariantType)p_type);
}
uint32_t js_call_variant_get_ptr_keyed_checker(uint32_t fn) {
	return ((GDExtensionPtrKeyedChecker)fn)(&callframe, &stack);
}
void js_variant_get_constant_value(uint32_t p_type, uint32_t p_constant) {
	gdextension_variant_get_constant_value((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_constant, &stack);
}
uint32_t js_variant_get_ptr_utility_function(uint32_t p_function, int64_t hash) {
	return (uint32_t)gdextension_variant_get_ptr_utility_function((GDExtensionStringNamePtr)p_function, hash);
}
void js_call_variant_get_ptr_utility_function(uint32_t fn, int argc) {
	((GDExtensionPtrUtilityFunction)fn)(&stack, (const GDExtensionConstTypePtr *)&callframe, argc);
}
uint32_t js_string_new(std::string contents) {
	uint32_t result;
	gdextension_string_new_with_latin1_chars(&result, contents.c_str());
	return result;
}
std::string js_string_get(uint32_t s, uint32_t length) {
	std::string result;
	result.resize(length);
	gdextension_string_to_latin1_chars(&s, &result[0], length);
	return result;
}
uint32_t js_string_name_new(std::string contents) {
	uint32_t result;
	gdextension_string_name_new_with_latin1_chars(&result, contents.c_str(), false);
	return result;
}
uint8_t js_packed_byte_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *gdextension_packed_byte_array_operator_index(&a, p_index);
}
float js_packed_float32_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *gdextension_packed_float32_array_operator_index(&a, p_index);
}
double js_packed_float64_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *gdextension_packed_float64_array_operator_index(&a, p_index);
}
int32_t js_packed_int32_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *gdextension_packed_int32_array_operator_index(&a, p_index);
}
int64_t js_packed_int64_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *gdextension_packed_int64_array_operator_index(&a, p_index);
}
uint32_t js_packed_string_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *(uint32_t*)gdextension_packed_string_array_operator_index(&a, p_index);
}
uint32_t js_packed_vector2_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return (uintptr_t)gdextension_packed_vector2_array_operator_index(&a, p_index);
}
uint32_t js_packed_vector3_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return (uintptr_t)gdextension_packed_vector3_array_operator_index(&a, p_index);
}
uint32_t js_packed_vector4_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return (uintptr_t)gdextension_packed_vector4_array_operator_index(&a, p_index);
}
uint32_t js_packed_color_array_operator_index(uint32_t a1, uint32_t a2, int64_t p_index) {
	uint32_t a[2] = {a1, a2};
	return (uintptr_t)gdextension_packed_color_array_operator_index(&a, p_index);
}
void js_packed_byte_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, uint8_t p_value) {
	uint32_t a[2] = {a1, a2};
	*gdextension_packed_byte_array_operator_index(&a, p_index) = p_value;
}
void js_packed_float32_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, float p_value) {
	uint32_t a[2] = {a1, a2};
	*gdextension_packed_float32_array_operator_index(&a, p_index) = p_value;
}
void js_packed_float64_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, double p_value) {
	uint32_t a[2] = {a1, a2};
	*gdextension_packed_float64_array_operator_index(&a, p_index) = p_value;
}
void js_packed_int32_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, int32_t p_value) {
	uint32_t a[2] = {a1, a2};
	*gdextension_packed_int32_array_operator_index(&a, p_index) = p_value;
}
void js_packed_int64_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, int64_t p_value) {
	uint32_t a[2] = {a1, a2};
	*gdextension_packed_int64_array_operator_index(&a, p_index) = p_value;
}
void js_packed_vector2_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, float x, float y) {
	uint32_t a[2] = {a1, a2};
	float* vec = (float*)gdextension_packed_vector2_array_operator_index(&a, p_index);
	vec[0] = x;
	vec[1] = y;
}
void js_packed_vector3_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, float x, float y, float z) {
	uint32_t a[2] = {a1, a2};
	float* vec = (float*)gdextension_packed_vector3_array_operator_index(&a, p_index);
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}
void js_packed_vector4_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, float x, float y, float z, float w) {
	uint32_t a[2] = {a1, a2};
	float* vec = (float*)gdextension_packed_vector4_array_operator_index(&a, p_index);
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	vec[3] = w;
}
void js_packed_color_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, float r, float g, float b, float a) {
	uint32_t self[2] = {a1, a2};
	float* vec = (float*)gdextension_packed_color_array_operator_index(&self, p_index);
	vec[0] = r;
	vec[1] = g;
	vec[2] = b;
	vec[3] = a;
}
void js_packed_string_array_operator_index_set(uint32_t a1, uint32_t a2, int64_t p_index, uint32_t p_value) {
	uint32_t a[2] = {a1, a2};
	*(uint32_t*)gdextension_packed_string_array_operator_index(&a, p_index) = p_value;
}
uint32_t js_array_operator_index(uint32_t a, int64_t p_index) {
	return (uintptr_t)gdextension_array_operator_index(&a, p_index);
}
void js_array_operator_index_set(uint32_t a, int64_t p_index, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t value[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t* current = (uint32_t*) gdextension_array_operator_index(&a, p_index);
	for (int i = 0; i < 6; i++) {
		current[i] = value[i];
	}
}
void js_array_ref(uint32_t a, int64_t from) {
	gdextension_array_ref(&a, &from);
}
void js_array_set_typed(uint32_t a, uint32_t p_type, uint32_t class_name, uint32_t script) {
	gdextension_array_set_typed(&a, (GDExtensionVariantType)p_type, &class_name, &script);
}
uintptr_t js_dictionary_operator_index(uint32_t d, uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6) {
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	return (uintptr_t)gdextension_dictionary_operator_index(&d, &key);
}
void js_dictionary_operator_index_set(uint32_t d, uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	uint32_t value[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t* current = (uint32_t*) gdextension_dictionary_operator_index(&d, &key);
	for (int i = 0; i < 6; i++) {
		current[i] = value[i];
	}
}
void js_dictionary_set_typed(uint32_t d, uint32_t key_type, uint32_t key_class, uint32_t key_script, uint32_t p_type, uint32_t class_name, uint32_t script) {
	gdextension_dictionary_set_typed(&d, (GDExtensionVariantType)key_type, &key_class, &key_script, (GDExtensionVariantType)p_type, &class_name, &script);
}
void js_object_bind_method_call(uint32_t method, uint32_t object, int64_t argc) {
	gdextension_object_method_bind_call(&method, &object, (const GDExtensionConstVariantPtr *)&callframe, argc, &stack, &error);
}
void js_object_method_bind_ptrcall(uint32_t method, uint32_t object) {
	gdextension_object_method_bind_ptrcall(&method, &object, (const GDExtensionConstVariantPtr *)&callframe, &stack);
}
void js_object_destroy(uint32_t object) {
	gdextension_object_destroy(&object);
}
uint32_t js_global_get_singleton(uint32_t name) {
	return *(uint32_t*)gdextension_global_get_singleton(&name);
}

void *js_object_instance_binding_create_callback(void *token, void *p_instance) { return 0; } // TODO
void js_object_instance_binding_free_callback(void *token, void *p_instance, void *p_binding) {} // TODO
GDExtensionBool js_object_instance_binding_reference_callback(void *token, void *p_binding, GDExtensionBool reference) {return 0;} // TODO

void js_object_set_instance_binding(uint32_t object, uint32_t token, uint32_t binding, val callbacks) {
	GDExtensionInstanceBindingCallbacks cb = {};
	if (callbacks.hasOwnProperty("create_callback")) cb.create_callback = &js_object_instance_binding_create_callback;
	if (callbacks.hasOwnProperty("free_callback")) cb.free_callback = &js_object_instance_binding_free_callback;
	if (callbacks.hasOwnProperty("reference_callback")) cb.reference_callback = &js_object_instance_binding_reference_callback;
	gdextension_object_set_instance_binding(&object, (void*)token, (void*)binding, &cb);
}

EMSCRIPTEN_BINDINGS(my_module) {
	function("gdextension_get_godot_version", &js_get_godot_version);
	function("gdextension_mem_alloc", &js_mem_alloc);
	function("gdextension_mem_realloc", &js_mem_realloc);
	function("gdextension_mem_free", &js_mem_free);
	function("gdextension_print_error", &js_print_error);
	function("gdextension_print_error_with_message", &js_print_error_with_message);
	function("gdextension_print_warning", &js_print_warning);
	function("gdextension_print_warning_with_message", &js_print_warning_with_message);
	function("gdextension_print_script_error", &js_print_script_error);
	function("gdextension_print_script_error_with_message", &js_print_script_error_with_message);
	function("gdextension_get_native_struct_size", &js_get_native_struct_size);
	function("gdextension_variant_new_copy", &js_variant_new_copy);
	function("gdextension_variant_new_nil", &js_variant_new_nil);
	function("gdextension_variant_destroy", &js_variant_destroy);
	function("gdextension_variant_call", &js_variant_call);
	function("gdextension_variant_call_static", &js_variant_call_static);
	function("gdextension_variant_evaluate", &js_variant_evaluate);
	function("gdextension_variant_set", &js_variant_set);
	function("gdextension_variant_set_named", &js_variant_set_named);
	function("gdextension_variant_set_keyed", &js_variant_set_keyed);
	function("gdextension_variant_set_indexed", &js_variant_set_indexed);
	function("gdextension_variant_get", &js_variant_get);
	function("gdextension_variant_get_named", &js_variant_get_named);
	function("gdextension_variant_get_keyed", &js_variant_get_keyed);
	function("gdextension_variant_get_indexed", &js_variant_get_indexed);
	function("gdextension_variant_iter_init", &js_variant_iter_init);
	function("gdextension_variant_iter_next", &js_variant_iter_next);
	function("gdextension_variant_iter_get", &js_variant_iter_get);
	function("gdextension_variant_hash", &js_variant_hash);
	function("gdextension_variant_recursive_hash", &js_variant_recursive_hash);
	function("gdextension_variant_hash_compare", &js_variant_hash_compare);
	function("gdextension_variant_booleanize", &js_variant_booleanize);
	function("gdextension_variant_duplicate", &js_variant_duplicate);
	function("gdextension_variant_stringify", &js_variant_stringify);
	function("gdextension_variant_get_type", &js_variant_get_type);
	function("gdextension_variant_has_method", &js_variant_has_method);
	function("gdextension_variant_has_member", &js_variant_has_member);
	function("gdextension_variant_has_key", &js_variant_has_key);
	function("gdextension_variant_get_object_instance_id", &js_variant_get_object_instance_id);
	function("gdextension_variant_get_type_name", &js_variant_get_type_name);
	function("gdextension_variant_can_convert", &js_variant_can_convert);
	function("gdextension_get_variant_from_type_constructor", &js_get_variant_from_type_constructor);
	function("gdextension_call_variant_from_type_constructor", &js_call_variant_from_type_constructor);
	function("gdextension_get_variant_to_type_constructor", &js_get_variant_to_type_constructor);
	function("gdextension_call_variant_to_type_constructor", &js_call_variant_to_type_constructor);
	function("gdextension_variant_get_ptr_internal_getter", &js_variant_get_ptr_internal_getter);
	function("gdextension_call_variant_get_ptr_internal_getter", &js_call_variant_get_ptr_internal_getter);
	function("gdextension_variant_get_ptr_operator_evaluator", &js_variant_get_ptr_operator_evaluator);
	function("gdextension_call_variant_get_ptr_operator_evaluator", &js_call_variant_get_ptr_operator_evaluator);
	function("gdextension_variant_get_ptr_builtin_method", &js_variant_get_ptr_builtin_method);
	function("gdextension_call_variant_get_ptr_builtin_method", &js_call_variant_get_ptr_builtin_method);
	function("gdextension_variant_get_ptr_constructor", &js_variant_get_ptr_constructor);
	function("gdextension_call_variant_get_ptr_constructor", &js_call_variant_get_ptr_constructor);
	function("gdextension_variant_get_ptr_destructor", &js_variant_get_ptr_destructor);
	function("gdextension_call_variant_get_ptr_destructor", &js_call_variant_get_ptr_destructor);
	function("gdextension_variant_construct", &js_variant_construct);
	function("gdextension_variant_get_ptr_setter", &js_variant_get_ptr_setter);
	function("gdextension_call_variant_get_ptr_setter", &js_call_variant_get_ptr_setter);
	function("gdextension_variant_get_ptr_getter", &js_variant_get_ptr_getter);
	function("gdextension_call_variant_get_ptr_getter", &js_call_variant_get_ptr_getter);
	function("gdextension_variant_get_ptr_indexed_setter", &js_variant_get_ptr_indexed_setter);
	function("gdextension_call_variant_get_ptr_indexed_setter", &js_call_variant_get_ptr_indexed_setter);
	function("gdextension_variant_get_ptr_indexed_getter", &js_variant_get_ptr_indexed_getter);
	function("gdextension_call_variant_get_ptr_indexed_getter", &js_call_variant_get_ptr_indexed_getter);
	function("gdextension_variant_get_ptr_keyed_setter", &js_variant_get_ptr_keyed_setter);
	function("gdextension_call_variant_get_ptr_keyed_setter", &js_call_variant_get_ptr_keyed_setter);
	function("gdextension_variant_get_ptr_keyed_getter", &js_variant_get_ptr_keyed_getter);
	function("gdextension_call_variant_get_ptr_keyed_getter", &js_call_variant_get_ptr_keyed_getter);
	function("gdextension_variant_get_ptr_keyed_checker", &js_variant_get_ptr_keyed_checker);
	function("gdextension_call_variant_get_ptr_keyed_checker", &js_call_variant_get_ptr_keyed_checker);
	function("gdextension_variant_get_constant_value", &js_variant_get_constant_value);
	function("gdextension_variant_get_ptr_utility_function", &js_variant_get_ptr_utility_function);
	function("gdextension_call_variant_get_ptr_utility_function", &js_call_variant_get_ptr_utility_function);
	function("gdextension_string_new", &js_string_new);
	function("gdextension_string_get", &js_string_get);
	function("gdextension_string_name_new", &js_string_name_new);
	function("gdextension_packed_byte_array_operator_index", &js_packed_byte_array_operator_index);
	function("gdextension_packed_float32_array_operator_index", &js_packed_float32_array_operator_index);
	function("gdextension_packed_float64_array_operator_index", &js_packed_float64_array_operator_index);
	function("gdextension_packed_int32_array_operator_index", &js_packed_int32_array_operator_index);
	function("gdextension_packed_int64_array_operator_index", &js_packed_int64_array_operator_index);
	function("gdextension_packed_string_array_operator_index", &js_packed_string_array_operator_index);
	function("gdextension_packed_vector2_array_operator_index", &js_packed_vector2_array_operator_index);
	function("gdextension_packed_vector3_array_operator_index", &js_packed_vector3_array_operator_index);
	function("gdextension_packed_vector4_array_operator_index", &js_packed_vector4_array_operator_index);
	function("gdextension_packed_color_array_operator_index", &js_packed_color_array_operator_index);
	function("gdextension_packed_byte_array_operator_index_set", &js_packed_byte_array_operator_index_set);
	function("gdextension_packed_float32_array_operator_index_set", &js_packed_float32_array_operator_index_set);
	function("gdextension_packed_float64_array_operator_index_set", &js_packed_float64_array_operator_index_set);
	function("gdextension_packed_int32_array_operator_index_set", &js_packed_int32_array_operator_index_set);
	function("gdextension_packed_int64_array_operator_index_set", &js_packed_int64_array_operator_index_set);
	function("gdextension_packed_vector2_array_operator_index_set", &js_packed_vector2_array_operator_index_set);
	function("gdextension_packed_vector3_array_operator_index_set", &js_packed_vector3_array_operator_index_set);
	function("gdextension_packed_vector4_array_operator_index_set", &js_packed_vector4_array_operator_index_set);
	function("gdextension_packed_color_array_operator_index_set", &js_packed_color_array_operator_index_set);
	function("gdextension_packed_string_array_operator_index_set", &js_packed_string_array_operator_index_set);
	function("gdextension_array_operator_index", &js_array_operator_index);
	function("gdextension_array_operator_index_set", &js_array_operator_index_set);
	function("gdextension_array_ref", &js_array_ref);
	function("gdextension_array_set_typed", &js_array_set_typed);
	function("gdextension_dictionary_operator_index", &js_dictionary_operator_index);
	function("gdextension_dictionary_operator_index_set", &js_dictionary_operator_index_set);
	function("gdextension_dictionary_set_typed", &js_dictionary_set_typed);
	function("gdextension_object_bind_method_call", &js_object_bind_method_call);
	function("gdextension_object_method_bind_ptrcall", &js_object_method_bind_ptrcall);
	function("gdextension_object_destroy", &js_object_destroy);
	function("gdextension_global_get_singleton", &js_global_get_singleton);
	function("gdextension_object_set_instance_binding", &js_object_set_instance_binding);
}
