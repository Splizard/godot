// Here we export the GDExtension API to Javascript.

#include "core/extension/gdextension_interface.cpp"
#include "core/extension/gdextension_interface.h"
#include <emscripten/bind.h>
#include <stdint.h>
#include <string>

using namespace emscripten;

uint32_t result_buffer[3][16][16];
uint32_t params_buffer[3][16][16];
void *params_points[16] = {
	&params_buffer[0][0][0], &params_buffer[0][1][0], &params_buffer[0][2][0], &params_buffer[0][3][0],
	&params_buffer[0][4][0], &params_buffer[0][5][0], &params_buffer[0][6][0], &params_buffer[0][7][0],
	&params_buffer[0][8][0], &params_buffer[0][9][0], &params_buffer[0][10][0], &params_buffer[0][11][0],
	&params_buffer[0][12][0], &params_buffer[0][13][0], &params_buffer[0][14][0], &params_buffer[0][15][0]
};

struct JavascriptUserData {
	emscripten::val value;
};

uint32_t js_read_result_buffer(uint32_t buffer, uint32_t index, uint32_t word) { return result_buffer[buffer][index][word]; }
void js_write_params_buffer(uint32_t buffer, uint32_t index, uint32_t word, uint32_t v1) {
	params_buffer[buffer][index][word] = v1;
}
void js_write_params_buffer16(uint32_t buffer, uint32_t index,
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8,
	uint32_t v9, uint32_t v10, uint32_t v11, uint32_t v12, uint32_t v13, uint32_t v14, uint32_t v15, uint32_t v16
) {
	params_buffer[buffer][index][0] = v1;
	params_buffer[buffer][index][1] = v2;
	params_buffer[buffer][index][2] = v3;
	params_buffer[buffer][index][3] = v4;
	params_buffer[buffer][index][4] = v5;
	params_buffer[buffer][index][5] = v6;
	params_buffer[buffer][index][6] = v7;
	params_buffer[buffer][index][7] = v8;
	params_buffer[buffer][index][8] = v9;
	params_buffer[buffer][index][9] = v10;
	params_buffer[buffer][index][10] = v11;
	params_buffer[buffer][index][11] = v12;
	params_buffer[buffer][index][12] = v13;
	params_buffer[buffer][index][13] = v14;
	params_buffer[buffer][index][14] = v15;
	params_buffer[buffer][index][15] = v16;
}

GDExtensionCallError error;
void js_get_godot_version(val arg0_js) {
	GDExtensionGodotVersion arg0;
	gdextension_get_godot_version(&arg0);
	arg0_js.set("major", arg0.major);
	arg0_js.set("minor", arg0.minor);
	arg0_js.set("patch", arg0.patch);
	arg0_js.set("string", arg0.string);
}
uintptr_t js_mem_alloc(size_t arg0) {
	return (uintptr_t)(gdextension_mem_alloc((size_t)arg0));
}
uintptr_t js_mem_realloc(uintptr_t arg0, size_t arg1) {
	return (uintptr_t)(gdextension_mem_realloc((void *)arg0, (size_t)arg1));
}
void js_mem_free(uintptr_t arg0) {
	gdextension_mem_free((void *)arg0);
}
void js_print_error(std::string arg0, std::string arg1, std::string arg2, int32_t arg3, uint8_t arg4) {
	gdextension_print_error(arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3, arg4);
}
void js_print_error_with_message(std::string arg0, std::string arg1, std::string arg2, std::string arg3, int32_t arg4, uint8_t arg5) {
	gdextension_print_error_with_message(arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str(), arg4, arg5);
}
void js_print_warning(std::string arg0, std::string arg1, std::string arg2, int32_t arg3, uint8_t arg4) {
	gdextension_print_warning(arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3, arg4);
}
void js_print_warning_with_message(std::string arg0, std::string arg1, std::string arg2, std::string arg3, int32_t arg4, uint8_t arg5) {
	gdextension_print_warning_with_message(arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str(), arg4, arg5);
}
void js_print_script_error(std::string arg0, std::string arg1, std::string arg2, int32_t arg3, uint8_t arg4) {
	gdextension_print_script_error(arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3, arg4);
}
void js_print_script_error_with_message(std::string arg0, std::string arg1, std::string arg2, std::string arg3, int32_t arg4, uint8_t arg5) {
	gdextension_print_script_error_with_message(arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str(), arg4, arg5);
}
uint32_t js_get_native_struct_size(uint32_t arg0) {
	return (uint32_t)(gdextension_get_native_struct_size((GDExtensionConstStringNamePtr)&arg0));
}
void js_variant_new_copy(uint32_t arg1_0, uint32_t arg1_1, uint32_t arg1_2, uint32_t arg1_3, uint32_t arg1_4, uint32_t arg1_5) {
	void *arg0 = &result_buffer[0][0][0];
	uint32_t arg1[6] = { arg1_0,  arg1_1,  arg1_2,  arg1_3,  arg1_4,  arg1_5};
	gdextension_variant_new_copy((GDExtensionUninitializedVariantPtr)arg0, (GDExtensionConstVariantPtr)arg1);
}
void js_variant_new_nil() {
	gdextension_variant_new_nil(&result_buffer[0][0][0]);
}
void js_variant_destroy(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	gdextension_variant_destroy(&self);
}
int js_variant_call(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t method, uint32_t argc) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	error.error = (GDExtensionCallErrorType)0;
	gdextension_variant_call(&self, &method, (const GDExtensionConstVariantPtr *)&params_buffer[0][0][0], argc, &result_buffer[0][0][0], &error);
	return error.error;
}
int js_variant_call_static(uint32_t vtype, uint32_t method, uint32_t argc) {
	error.error = (GDExtensionCallErrorType)0;
	gdextension_variant_call_static((GDExtensionVariantType)vtype, &method, (const GDExtensionConstVariantPtr *)&params_buffer[0][0][0], argc, &result_buffer[0], &error);
	return error.error;
}
bool js_variant_evaluate(uint32_t op,
	uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6,
	uint32_t b1, uint32_t b2, uint32_t b3, uint32_t b4, uint32_t b5, uint32_t b6
) {
	uint32_t a[6] = {a1, a2, a3, a4, a5, a6};
	uint32_t b[6] = {b1, b2, b3, b4, b5, b6};
	bool valid;
	gdextension_variant_evaluate((GDExtensionVariantOperator)op, &a, &b, &result_buffer[0], (GDExtensionBool *)&valid);
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
	uint32_t index,
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
	gdextension_variant_get(&self, &key, &result_buffer[0], (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_get_named(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t name
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	gdextension_variant_get_named(&self, &name, &result_buffer[0], (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_get_keyed(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	bool valid;
	gdextension_variant_get_keyed(&self, &key, &result_buffer[0], (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_get_indexed(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t index
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	bool oob;
	gdextension_variant_get_indexed(&self, index, &result_buffer[0], (GDExtensionBool *)&valid, (GDExtensionBool *)&oob);
	return valid + oob;
}
bool js_variant_iter_init(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	gdextension_variant_iter_init(&self, &result_buffer[0], (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_iter_next(
	uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6
) {
	uint32_t iter[6] = {i1, i2, i3, i4, i5, i6};
	bool valid;
	gdextension_variant_iter_next(&iter, &result_buffer[0], (GDExtensionBool *)&valid);
	return valid;
}
bool js_variant_iter_get(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	bool valid;
	gdextension_variant_iter_get(&self, &result_buffer[0], &result_buffer[1], (GDExtensionBool *)&valid);
	return valid;
}
double js_variant_hash(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	GDExtensionInt r = gdextension_variant_hash(&self);
	return *(double*)&r;
}
double js_variant_recursive_hash(
	uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6,
	uint32_t p_recursion_count
) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	GDExtensionInt r = gdextension_variant_recursive_hash(&self, p_recursion_count);
	return *(double*)&r;
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
	gdextension_variant_duplicate(&self,  &result_buffer[0], p_deep);
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
	return gdextension_variant_has_key(&self, &p_key, (GDExtensionBool*)&result_buffer[0]);
}
double js_variant_get_object_instance_id(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t self[6] = {v1, v2, v3, v4, v5, v6};
	GDExtensionInt r = gdextension_variant_get_object_instance_id(&self);
	return *(double*)&r;
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
	((GDExtensionVariantFromTypeConstructorFunc)fn)(&result_buffer[0][0][0], &params_buffer[0][0][0]);
}
uint32_t js_get_variant_to_type_constructor(uint32_t p_type) {
	return (uint32_t)gdextension_get_variant_to_type_constructor((GDExtensionVariantType)p_type);
}
void js_call_variant_to_type_constructor(uint32_t fn) {
	((GDExtensionTypeFromVariantConstructorFunc)fn)(&result_buffer[0][0][0], &params_buffer[0][0][0]);
}
uint32_t js_variant_get_ptr_internal_getter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_internal_getter((GDExtensionVariantType)p_type);
}
uint32_t js_call_variant_get_ptr_internal_getter(uint32_t fn) {
	return (uintptr_t)((GDExtensionVariantGetInternalPtrFunc)fn)(&params_buffer[0][0][0]);
}
uint32_t js_variant_get_ptr_operator_evaluator(uint32_t p_op, uint32_t a_type, uint32_t b_type) {
	return (uint32_t)gdextension_variant_get_ptr_operator_evaluator((GDExtensionVariantOperator)p_op, (GDExtensionVariantType)a_type, (GDExtensionVariantType)b_type);
}
void js_call_variant_get_ptr_operator_evaluator(uint32_t fn) {
	((GDExtensionPtrOperatorEvaluator)fn)(&params_buffer[0][0][0], &params_buffer[1][0][0], &result_buffer[0][0][0]);
}
uint32_t js_variant_get_ptr_builtin_method(uint32_t p_type, uint32_t p_method, double hash) {
	return (uint32_t)gdextension_variant_get_ptr_builtin_method((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)&p_method, *(int64_t*)&hash);
}
void js_call_variant_get_ptr_builtin_method(uint32_t fn, int argc) {
	((GDExtensionPtrBuiltInMethod)fn)(&params_buffer[1][0][0],
		(const GDExtensionConstTypePtr *)&params_points, &result_buffer[0][0][0], argc);
	result_buffer[1][0][0] = params_buffer[1][0][0]; // packed arrays are mutable.
	result_buffer[1][0][1] = params_buffer[1][0][1];
}
uint32_t js_variant_get_ptr_constructor(uint32_t p_type, int32_t p_constructor) {
	return (uint32_t)gdextension_variant_get_ptr_constructor((GDExtensionVariantType)p_type, p_constructor);
}
void js_call_variant_get_ptr_constructor(uint32_t fn) {
	((GDExtensionPtrConstructor)fn)(&result_buffer[0][0][0], (const GDExtensionConstTypePtr *)&params_points);
}
uint32_t js_variant_get_ptr_destructor(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_destructor((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_destructor(uint32_t fn) {
	((GDExtensionPtrDestructor)fn)(&params_buffer[0][0][0]);
}
int js_variant_construct(uint32_t p_type, int32_t p_argument_count) {
	error.error = (GDExtensionCallErrorType)0;
	gdextension_variant_construct((GDExtensionVariantType)p_type, &result_buffer[0], (const GDExtensionConstVariantPtr *)&params_points, p_argument_count, &error);
	return error.error;
}
uint32_t js_variant_get_ptr_setter(uint32_t p_type, uint32_t p_member) {
	return (uint32_t)gdextension_variant_get_ptr_setter((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_member);
}
void js_call_variant_get_ptr_setter(uint32_t fn) {
	((GDExtensionPtrSetter)fn)(&params_buffer[0][0][0], &params_buffer[1][0][0]);
}
uint32_t js_variant_get_ptr_getter(uint32_t p_type, uint32_t p_member) {
	return (uint32_t)gdextension_variant_get_ptr_getter((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_member);
}
void js_call_variant_get_ptr_getter(uint32_t fn) {
	((GDExtensionPtrGetter)fn)(&params_buffer[0][0][0], &result_buffer[0][0][0]);
}
uint32_t js_variant_get_ptr_indexed_setter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_indexed_setter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_indexed_setter(uint32_t fn, uint32_t index) {
	((GDExtensionPtrIndexedSetter)fn)(&params_buffer[0][0][0], index, &params_buffer[1][0][0]);
}
uint32_t js_variant_get_ptr_indexed_getter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_indexed_getter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_indexed_getter(uint32_t fn, uint32_t index) {
	((GDExtensionPtrIndexedGetter)fn)(&params_buffer[0][0][0], index, &result_buffer[0][0][0]);
}
uint32_t js_variant_get_ptr_keyed_setter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_keyed_setter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_keyed_setter(uint32_t fn) {
	((GDExtensionPtrKeyedSetter)fn)(&params_buffer[0][0][0], &params_buffer[1][0][0], &params_buffer[2][0][0]);
}
uint32_t js_variant_get_ptr_keyed_getter(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_keyed_getter((GDExtensionVariantType)p_type);
}
void js_call_variant_get_ptr_keyed_getter(uint32_t fn) {
	((GDExtensionPtrKeyedGetter)fn)(&params_buffer[0][0][0], &params_buffer[1][0][0], &result_buffer[0][0][0]);
}
uint32_t js_variant_get_ptr_keyed_checker(uint32_t p_type) {
	return (uint32_t)gdextension_variant_get_ptr_keyed_checker((GDExtensionVariantType)p_type);
}
uint32_t js_call_variant_get_ptr_keyed_checker(uint32_t fn) {
	return ((GDExtensionPtrKeyedChecker)fn)(&params_buffer[0][0][0], &result_buffer[0][0][0]);
}
void js_variant_get_constant_value(uint32_t p_type, uint32_t p_constant) {
	gdextension_variant_get_constant_value((GDExtensionVariantType)p_type, (GDExtensionStringNamePtr)p_constant, &result_buffer[0]);
}
uint32_t js_variant_get_ptr_utility_function(uint32_t p_function, double hash) {
	return (uint32_t)gdextension_variant_get_ptr_utility_function((GDExtensionStringNamePtr)&p_function, *(int64_t*)&hash);
}
void js_call_variant_get_ptr_utility_function(uint32_t fn, int argc) {
	((GDExtensionPtrUtilityFunction)fn)(&result_buffer[0], (const GDExtensionConstTypePtr *)&params_points, argc);
}
uint32_t js_string_new(std::string contents) {
	uint32_t result;
	gdextension_string_new_with_latin1_chars(&result, contents.c_str());
	return result;
}
std::string js_string_get(uint32_t s, uint32_t length) {
	std::string result;
	result.resize(length);
	gdextension_string_to_latin1_chars(&s, result.data(), length);
	return result;
}
uint32_t js_string_name_new(std::string contents) {
	uint32_t result;
	gdextension_string_name_new_with_latin1_chars(&result, contents.c_str(), false);
	return result;
}
uint32_t js_string_operator_plus_eq_string(uint32_t a, uint32_t b) {
	gdextension_string_operator_plus_eq_string(&a, &b);
	return a;
}
void js_packed_byte_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	uint8_t *out = (uint8_t *)&result_buffer[0][0][0];
	*out = *gdextension_packed_byte_array_operator_index(&a, p_index);
}
void js_packed_float32_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float *out = (float*)&result_buffer[0][0][0];
	*out = *gdextension_packed_float32_array_operator_index(&a, p_index);
}
void js_packed_float64_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	double *out = (double*)&result_buffer[0][0][0];
	*out = *gdextension_packed_float64_array_operator_index(&a, p_index);
}
void js_packed_int32_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	int32_t *out = (int32_t*)&result_buffer[0][0][0];
	*out = *gdextension_packed_int32_array_operator_index(&a, p_index);
}
void js_packed_int64_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	int64_t *out = (int64_t*)&result_buffer[0][0][0];
	*out = *gdextension_packed_int64_array_operator_index(&a, p_index);
}
void js_packed_vector2_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float *out = (float*)&result_buffer[0][0][0];
	float *vec = (float*)gdextension_packed_vector2_array_operator_index(&a, p_index);
	out[0] = vec[0];
	out[1] = vec[1];
}
void js_packed_vector3_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float *out = (float*)&result_buffer[0][0][0];
	float *vec = (float*)gdextension_packed_vector3_array_operator_index(&a, p_index);
	out[0] = vec[0];
	out[1] = vec[1];
	out[2] = vec[2];
}
void js_packed_vector4_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float *out = (float*)&result_buffer[0][0][0];
	float *vec = (float*)gdextension_packed_vector4_array_operator_index(&a, p_index);
	out[0] = vec[0];
	out[1] = vec[1];
	out[2] = vec[2];
	out[3] = vec[3];
}
void js_packed_color_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float *out = (float*)&result_buffer[0][0][0];
	float *vec = (float*)gdextension_packed_color_array_operator_index(&a, p_index);
	out[0] = vec[0];
	out[1] = vec[1];
	out[2] = vec[2];
	out[3] = vec[3];
}
void js_packed_byte_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	uint8_t p_value = params_buffer[0][0][0];
	*gdextension_packed_byte_array_operator_index(&a, p_index) = p_value;
}
void js_packed_float32_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float p_value = *(float*)&params_buffer[0][0][0];
	*gdextension_packed_float32_array_operator_index(&a, p_index) = p_value;
}
void js_packed_float64_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	double p_value = *(double*)&params_buffer[0][0][0];
	*gdextension_packed_float64_array_operator_index(&a, p_index) = p_value;
}
void js_packed_int32_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	int32_t p_value = *(int32_t*)&params_buffer[0][0][0];
	*gdextension_packed_int32_array_operator_index(&a, p_index) = p_value;
}
void js_packed_int64_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	int64_t p_value = *(int64_t*)&params_buffer[0][0][0];
	*gdextension_packed_int64_array_operator_index(&a, p_index) = p_value;
}
void js_packed_vector2_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float x = *(float*)&params_buffer[0][0][0];
	float y = *(float*)&params_buffer[0][0][1];
	float* vec = (float*)gdextension_packed_vector2_array_operator_index(&a, p_index);
	vec[0] = x;
	vec[1] = y;
}
void js_packed_vector3_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float x = *(float*)&params_buffer[0][0][0];
	float y = *(float*)&params_buffer[0][0][1];
	float z = *(float*)&params_buffer[0][0][2];
	float* vec = (float*)gdextension_packed_vector3_array_operator_index(&a, p_index);
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}
void js_packed_vector4_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	float x = *(float*)&params_buffer[0][0][0];
	float y = *(float*)&params_buffer[0][0][1];
	float z = *(float*)&params_buffer[0][0][2];
	float w = *(float*)&params_buffer[0][0][3];
	float* vec = (float*)gdextension_packed_vector4_array_operator_index(&a, p_index);
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	vec[3] = w;
}
void js_packed_color_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t self[2] = {a1, a2};
	float r = *(float*)&params_buffer[0][0][0];
	float g = *(float*)&params_buffer[0][0][1];
	float b = *(float*)&params_buffer[0][0][2];
	float a = *(float*)&params_buffer[0][0][3];
	float* vec = (float*)gdextension_packed_color_array_operator_index(&self, p_index);
	vec[0] = r;
	vec[1] = g;
	vec[2] = b;
	vec[3] = a;
}
uint32_t js_packed_string_array_operator_index(uint32_t a1, uint32_t a2, uint32_t p_index) {
	uint32_t a[2] = {a1, a2};
	return *(uint32_t *)(GDExtensionStringPtr)gdextension_packed_string_array_operator_index(&a, p_index);
}
void js_packed_string_array_operator_index_set(uint32_t a1, uint32_t a2, uint32_t p_index, uint32_t p_value) {
	uint32_t a[2] = {a1, a2};
	*(uint32_t*)gdextension_packed_string_array_operator_index(&a, p_index) = p_value;
}
void js_array_operator_index(uint32_t a, uint32_t p_index) {
	uint32_t *val = (uint32_t*)gdextension_array_operator_index(&a, p_index);
	for (int i = 0; i < 6; i++) {
		result_buffer[0][0][i] = val[i];
	}
}
void js_array_operator_index_set(uint32_t a, uint32_t p_index, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	uint32_t value[6] = {v1, v2, v3, v4, v5, v6};
	uint32_t* current = (uint32_t*) gdextension_array_operator_index(&a, p_index);
	for (int i = 0; i < 6; i++) {
		current[i] = value[i];
	}
}
void js_array_ref(uint32_t a, uint32_t from) {
	gdextension_array_ref(&a, &from);
}
void js_array_set_typed(uint32_t a, uint32_t p_type, uint32_t class_name, uint32_t script) {
	gdextension_array_set_typed(&a, (GDExtensionVariantType)p_type, &class_name, &script);
}
void js_dictionary_operator_index(uint32_t d, uint32_t key1, uint32_t key2, uint32_t key3, uint32_t key4, uint32_t key5, uint32_t key6) {
	uint32_t key[6] = {key1, key2, key3, key4, key5, key6};
	uint32_t *val = (uint32_t*)gdextension_dictionary_operator_index(&d, &key);
	for (int i = 0; i < 6; i++) {
		result_buffer[0][0][i] = val[i];
	}
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
void js_object_bind_method_call(uint32_t method, uint32_t object, uint32_t argc) {
	gdextension_object_method_bind_call((void *)method, (void *)object, (const GDExtensionConstVariantPtr *)&params_points, argc, &result_buffer[0][0][0], &error);
}
void js_object_method_bind_ptrcall(uint32_t method, uint32_t object) {
	gdextension_object_method_bind_ptrcall((void *)method, (void *)object, (const GDExtensionConstTypePtr *)&params_points, &result_buffer[0][0][0]);
}
void js_object_destroy(uint32_t object) {
	gdextension_object_destroy((void *)object);
}
uint32_t js_global_get_singleton(uint32_t name) {
	return (uint32_t)gdextension_global_get_singleton(&name);
}

void *js_object_instance_binding_create_callback(void *token, void *p_instance) { return 0; } // TODO
void js_object_instance_binding_free_callback(void *token, void *p_instance, void *p_binding) {} // TODO
GDExtensionBool js_object_instance_binding_reference_callback(void *token, void *p_binding, GDExtensionBool reference) {return 0;} // TODO

void js_object_set_instance_binding(uint32_t object, uint32_t token, uint32_t binding, val callbacks) {
	GDExtensionInstanceBindingCallbacks cb = {};
	if (callbacks.hasOwnProperty("create_callback")) cb.create_callback = &js_object_instance_binding_create_callback;
	if (callbacks.hasOwnProperty("free_callback")) cb.free_callback = &js_object_instance_binding_free_callback;
	if (callbacks.hasOwnProperty("reference_callback")) cb.reference_callback = &js_object_instance_binding_reference_callback;
	gdextension_object_set_instance_binding((void *)object, (void*)token, (void*)binding, &cb);
}
void js_object_free_instance_binding(uint32_t object, uint32_t token) {
	gdextension_object_free_instance_binding((void *)object, (void*)token);
}
void js_object_set_instance(uint32_t object, uint32_t extension_class_name, val instance) {
	JavascriptUserData *userdata = new JavascriptUserData();
	userdata->value = val(instance);
	gdextension_object_set_instance((void *)object, &extension_class_name, userdata);
}
uint32_t js_object_get_class_name(uint32_t object, uint32_t token) {
	uint32_t result;
	gdextension_object_get_class_name((void *)object, (void*)token, &result);
	return result;
}
uint32_t js_object_cast_to(uint32_t object, uint32_t class_tag) {
	uint32_t result = (uint32_t)gdextension_object_cast_to((void *)object, (void *)class_tag);
	return result;
}
uint32_t js_object_get_instance_from_id(double id) {
	return (uint32_t)gdextension_object_get_instance_from_id(*(uint64_t*)&id);
}
double js_object_get_instance_id(uint32_t object) {
	uint64_t result = gdextension_object_get_instance_id((void *)object);
	return *(double*)&result;
}
bool js_object_has_script_method(uint32_t object, uint32_t method) {
	return gdextension_object_has_script_method((void *)object, &method);
}
int js_object_call_script_method(uint32_t object, uint32_t method, int argc) {
	error.error = (GDExtensionCallErrorType)0;
	gdextension_object_call_script_method((void *)object, &method, (const GDExtensionConstVariantPtr *)&params_buffer[0][0][0], argc, &result_buffer[0], &error);
	return error.error;
}
uint32_t js_ref_get_object(uint32_t ref) {
	return (uint32_t)gdextension_ref_get_object((void *)ref);
}
void js_ref_set_object(uint32_t ref, uint32_t object) {
	gdextension_ref_set_object((void *)ref, (void *)object);
}

struct ScriptInstanceWrapper {
	emscripten::val info;
	uint32_t instance;
};

GDExtensionBool js_script_instance_set(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionConstVariantPtr p_value) {
	ScriptInstanceWrapper *wrapper = (ScriptInstanceWrapper*)p_instance;
	uint32_t *name = (uint32_t*)p_name;
	uint32_t *v = (uint32_t *)p_value;
	return wrapper->info.call<bool>("set", wrapper->instance, name, v[0], v[1], v[2], v[3], v[4], v[5]);
}
GDExtensionBool js_script_instance_get(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr p_r_value) {
	ScriptInstanceWrapper *wrapper = (ScriptInstanceWrapper*)p_instance;
	uint32_t *name = (uint32_t*)p_name;
	uint32_t *v = (uint32_t*)p_r_value;
	bool valid = wrapper->info.call<bool>("get", wrapper->instance, name);
	for (int i = 0; i < 6; i++) {
		result_buffer[0][0][i] = v[i];
	}
	return valid;
}
void js_script_instance_free(GDExtensionScriptInstanceDataPtr p_instance) {
	ScriptInstanceWrapper *wrapper = (ScriptInstanceWrapper*)p_instance;
	delete wrapper;
}

uint32_t js_script_instance_create3(val info_js, uint32_t instance) {
	GDExtensionScriptInstanceInfo3 info = {};
	info.set_func = js_script_instance_set;
	info.get_func = js_script_instance_get;
	/*info.get_property_list_func = js_script_instance_get_property_list;
	info.free_property_list_func = js_script_instance_free_property_list;
	info.get_class_category_func = js_script_instance_get_class_category;
	info.property_can_revert_func = js_script_instance_property_can_revert;
	info.property_get_revert_func = js_script_instance_property_get_revert;
	info.get_owner_func = js_script_instance_get_owner;
	info.get_property_state_func = js_script_instance_get_property_state;
	info.get_method_list_func = js_script_instance_get_method_list;
	info.free_method_list_func = js_script_instance_free_method_list;
	info.get_property_type_func = js_script_instance_get_property_type;
	info.validate_property_func = js_script_instance_validate_property;
	info.has_method_func = js_script_instance_has_method;
	info.get_method_argument_count_func = js_script_instance_get_method_argument_count;
	info.call_func = js_script_instance_call;
	info.notification_func = js_script_instance_notification;
	info.to_string_func = js_script_instance_to_string;
	info.refcount_incremented_func = js_script_instance_refcount_incremented;
	info.refcount_decremented_func = js_script_instance_refcount_decremented;
	info.get_script_func = js_script_instance_get_script;
	info.is_placeholder_func = js_script_instance_is_placeholder;
	info.set_fallback_func = js_script_instance_set_fallback;
	info.get_fallback_func = js_script_instance_get_fallback;
	info.get_language_func = js_script_instance_get_language;*/
	info.free_func = js_script_instance_free;
	ScriptInstanceWrapper *wrapper = new ScriptInstanceWrapper{info_js, instance};
	return (uint32_t)gdextension_script_instance_create3(&info, (GDExtensionScriptInstanceDataPtr)wrapper);
}
uint32_t js_placeholder_script_instance_create(uint32_t a, uint32_t b, uint32_t c) {
	return (uint32_t)gdextension_placeholder_script_instance_create((GDExtensionObjectPtr)a, (GDExtensionObjectPtr)b, (GDExtensionObjectPtr)c);
}
void js_placeholder_script_instance_update(uint32_t instance, uint32_t arr, uint32_t d) {
	gdextension_placeholder_script_instance_update((GDExtensionScriptInstancePtr)instance, (GDExtensionConstTypePtr)&arr, (GDExtensionConstTypePtr)&d);
}
uint32_t js_object_get_script_instance(uint32_t a, uint32_t b) {
	return (uint32_t)gdextension_object_get_script_instance((void *)a, (void *)b);
}

void js_callable_call(void *callable_userdata, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error) {
	JavascriptUserData *userdata = (JavascriptUserData*)callable_userdata;
	for (int i = 0; i < p_argument_count; i++) {
		uint32_t *v = (uint32_t*)p_args[i];
		for (int j = 0; j < 6; j++) {
			result_buffer[0][i][j] = v[j];
		}
	}
	r_error->error = (GDExtensionCallErrorType)((userdata->value).call<int>("call_func", (uint32_t)p_argument_count));
	uint32_t *v = (uint32_t*)r_return;
	for (int i = 0; i < 6; i++) {
		v[i] = params_buffer[0][0][i];
	}
}
void js_callable_custom_create2(val info_js) {
	GDExtensionCallableCustomInfo2 info = {};
	JavascriptUserData *userdata = new JavascriptUserData();
	userdata->value = val(info_js);
	info.callable_userdata = userdata;

	if (info_js.hasOwnProperty("token")) info.token = (void *)(info_js["token"].as<uint32_t>());
	if (info_js.hasOwnProperty("object_id")) info.object_id = (uint64_t)(info_js["object_id"].as<uint32_t>());
	if (info_js.hasOwnProperty("call_func")) info.call_func = js_callable_call;
	/*if (info_js.hasOwnProperty("is_valid_func")) info.is_valid_func = js_callable_is_valid;
	if (info_js.hasOwnProperty("free_func")) info.free_func = js_callable_free;
	if (info_js.hasOwnProperty("hash_func")) info.hash_func = js_callable_hash;
	if (info_js.hasOwnProperty("equal_func")) info.equal_func = js_callable_equal;
	if (info_js.hasOwnProperty("less_than_func")) info.less_than_func = js_callable_less_than;
	if (info_js.hasOwnProperty("to_string_func")) info.to_string_func = js_callable_to_string;
	if (info_js.hasOwnProperty("get_argument_count_func")) info.get_argument_count_func = js_callable_get_argument_count;*/
	gdextension_callable_custom_create2(&result_buffer[0][0][0], &info);
}

uint32_t js_classdb_get_method_bind(uint32_t class_name, uint32_t method_name, double hash) {
	return (uint32_t)gdextension_classdb_get_method_bind((GDExtensionStringNamePtr)&class_name, (GDExtensionStringNamePtr)&method_name, *(int64_t*)&hash);
}
uint32_t js_classdb_get_class_tag(uint32_t name) {
	return (uint32_t)gdextension_classdb_get_class_tag(&name);
}

GDExtensionBool extension_class3_set_func(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionConstVariantPtr p_value) {
	val info = ((JavascriptUserData*)p_instance)->value;
	uint32_t *n = (uint32_t*)p_name;
	uint32_t *v = (uint32_t*)p_value;
	return (GDExtensionBool)info.call<bool>("set", n[0], v[0], v[1], v[2], v[3], v[4], v[5]);
}
GDExtensionBool extension_class3_get_func(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) {
	val info = ((JavascriptUserData*)p_instance)->value;
	uint32_t *n = (uint32_t*)p_name;
	bool valid = info.call<bool>("get", n[0]);
	uint32_t *v = (uint32_t*)r_ret;
	for (int i = 0; i < 6; i++) {
		v[i] = params_buffer[0][0][i];
	}
	return valid;
}
uint64_t extension_class3_get_rid_func(GDExtensionClassInstancePtr p_instance) {
	val info = ((JavascriptUserData*)p_instance)->value;
	double rid = info.call<double>("get_rid");
	return *(uint64_t *)&rid;
}

const GDExtensionPropertyInfo *extension_class3_get_property_list_func(GDExtensionClassInstancePtr p_instance, uint32_t *r_count) {
	val info = ((JavascriptUserData*)p_instance)->value;
	val list = info.call<val>("get_property_list");
	if (list.isArray()) {
		*r_count = list["length"].as<uint32_t>();
		size_t base = sizeof(GDExtensionPropertyInfo) * (*r_count);
		uint32_t *arena = (uint32_t*)gdextension_mem_alloc(base+sizeof(uintptr_t)*3*(*r_count));
		GDExtensionPropertyInfo *result = (GDExtensionPropertyInfo*)arena;
		for (int i = 0; i < *r_count; i++) {
			val prop = list[i];
			arena[base+i*3+0] = prop["name"].as<uint32_t>();
			arena[base+i*3+1] = prop["class_name"].as<uint32_t>();
			arena[base+i*3+2] = prop["hint_string"].as<uint32_t>();
			result[i].name = &arena[base+i*3+0];
			result[i].type = (GDExtensionVariantType)prop["type"].as<uint32_t>();
			result[i].class_name = &arena[base+i*3+1];
			result[i].hint = prop["hint"].as<uint32_t>();
			result[i].hint_string = &arena[base+i*3+2];
			result[i].usage = prop["usage"].as<uint32_t>();
		}
		return result;
	}
	return nullptr;
}
void extension_class3_free_property_list_func(GDExtensionClassInstancePtr p_instance, const GDExtensionPropertyInfo *p_list, uint32_t p_count) {
	gdextension_mem_free((void*)p_list);
}
GDExtensionBool extension_class3_property_can_revert_func(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name) {
	val info = ((JavascriptUserData*)p_instance)->value;
	uint32_t *n = (uint32_t*)p_name;
	return (GDExtensionBool)info.call<bool>("property_can_revert", n[0]);
}
GDExtensionBool extension_class3_property_get_revert_func(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) {
	val info = ((JavascriptUserData*)p_instance)->value;
	uint32_t *n = (uint32_t*)p_name;
	bool valid = info.call<bool>("property_get_revert", n[0]);
	uint32_t *v = (uint32_t*)r_ret;
	for (int i = 0; i < 6; i++) {
		v[i] = params_buffer[0][0][i];
	}
	return valid;
}
GDExtensionBool extension_class3_validate_property_func(GDExtensionClassInstancePtr p_instance, GDExtensionPropertyInfo *p_property) {
	val info = ((JavascriptUserData*)p_instance)->value;
	val prop = val::object();
	prop.set("name", p_property->name);
	prop.set("type", p_property->type);
	prop.set("class_name", p_property->class_name);
	prop.set("hint", p_property->hint);
	prop.set("hint_string", p_property->hint_string);
	prop.set("usage", p_property->usage);
	return (GDExtensionBool)info.call<bool>("validate_property", prop);
}
void extension_class3_notification_func(GDExtensionClassInstancePtr p_instance, int32_t p_what, GDExtensionBool p_reversed) {
	val info = ((JavascriptUserData*)p_instance)->value;
	info.call<void>("notification", p_what, (bool)p_reversed);
}
void extension_class3_to_string_func(GDExtensionClassInstancePtr p_instance, GDExtensionBool *r_is_valid, GDExtensionStringPtr p_out) {
	val info = ((JavascriptUserData*)p_instance)->value;
	bool result = info.call<bool>("to_string");
	if (result) {
		p_out = (GDExtensionStringPtr)params_buffer[0][0][0];
		*r_is_valid = true;
	} else {
		*r_is_valid = false;
	}
}
void extension_class3_reference_func(GDExtensionClassInstancePtr p_instance) {
	val info = ((JavascriptUserData*)p_instance)->value;
	info.call<void>("reference");
}
void extension_class3_unreference_func(GDExtensionClassInstancePtr p_instance) {
	val info = ((JavascriptUserData*)p_instance)->value;
	info.call<void>("unreference");
}
GDExtensionObjectPtr extension_class3_create_instance_func(void *p_class_userdata, GDExtensionBool post_notify) {
	val info = ((JavascriptUserData*)p_class_userdata)->value;
	uint32_t instance = info.call<uint32_t>("create_instance");
	return (GDExtensionObjectPtr)instance;
}
void extension_class3_free_instance_func(void *p_class_userdata, GDExtensionClassInstancePtr p_instance) {
	val info = ((JavascriptUserData*)p_instance)->value;
	info.call<void>("free", (uint32_t)p_instance);
	delete (JavascriptUserData*)p_instance;
}
void *extension_class3_get_virtual_call_data_func(void *p_class_userdata, GDExtensionConstStringNamePtr p_name, uint32_t hash) {
	val info = ((JavascriptUserData*)p_class_userdata)->value;
	uint32_t *n = (uint32_t*)p_name;
	return (void *)(uint32_t)info.call<uint32_t>("get_virtual_call_data", n[0]);
}
void extension_class3_call_virtual_with_data_func(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret) {
	val info = ((JavascriptUserData*)p_instance)->value;
	uint32_t *n = (uint32_t*)p_name;
	info.call<void>("call_virtual", n[0], (uint32_t)p_virtual_call_userdata, (uint32_t)p_args, (uint32_t)r_ret);
}

void js_classdb_register_extension_class_property(uint32_t token, uint32_t parent_class, val prop, uint32_t setter, uint32_t getter) {
	GDExtensionPropertyInfo info = {};
	uint32_t name = prop["name"].as<uint32_t>();
	uint32_t class_name = prop["class_name"].as<uint32_t>();
    uint32_t hint_string = prop["hint_string"].as<uint32_t>();
	info.name = (GDExtensionStringNamePtr)&name;
	info.type = (GDExtensionVariantType)prop["type"].as<uint32_t>();
	info.class_name = (GDExtensionStringNamePtr)&class_name;
	info.hint = prop["hint"].as<uint32_t>();
	info.hint_string = (GDExtensionStringNamePtr)&hint_string;
	info.usage = prop["usage"].as<uint32_t>();
	GDExtension::_register_extension_class_property(
		(GDExtensionClassLibraryPtr)token, (GDExtensionConstStringNamePtr)&parent_class, &info,
		(GDExtensionConstStringNamePtr)&setter,
		(GDExtensionConstStringNamePtr)&getter
	);
}

void js_classdb_register_extension_class_signal(uint32_t token, uint32_t parent_class, uint32_t signal_name, val list) {
	uint32_t r_count = list["length"].as<uint32_t>();
	size_t base = sizeof(GDExtensionPropertyInfo) * (r_count);
	uint32_t *arena = (uint32_t*)gdextension_mem_alloc(base+sizeof(uintptr_t)*3*(r_count));
	GDExtensionPropertyInfo *result = (GDExtensionPropertyInfo*)arena;
	for (int i = 0; i < r_count; i++) {
		val prop = list[i];
		arena[base+i*3+0] = prop["name"].as<uint32_t>();
		arena[base+i*3+1] = prop["class_name"].as<uint32_t>();
		arena[base+i*3+2] = prop["hint_string"].as<uint32_t>();
		result[i].name = &arena[base+i*3+0];
		result[i].type = (GDExtensionVariantType)prop["type"].as<uint32_t>();
		result[i].class_name = &arena[base+i*3+1];
		result[i].hint = prop["hint"].as<uint32_t>();
		result[i].hint_string = &arena[base+i*3+2];
		result[i].usage = prop["usage"].as<uint32_t>();
	}
	GDExtension::_register_extension_class_signal(
		(GDExtensionClassLibraryPtr)token, (GDExtensionConstStringNamePtr)&parent_class,
		(GDExtensionConstStringNamePtr)&signal_name, result, r_count
	);
	if (arena) gdextension_mem_free((void*)arena);
}

void js_classdb_register_extension_class3(uint32_t token, uint32_t class_name, uint32_t base_class_name, val info) {
	GDExtensionClassCreationInfo4 creation_info = {};
	if (info.hasOwnProperty("is_virtual")) creation_info.is_virtual = (GDExtensionBool)info["is_virtual"].as<bool>();
	if (info.hasOwnProperty("is_abstract")) creation_info.is_abstract = (GDExtensionBool)info["is_abstract"].as<bool>();
	if (info.hasOwnProperty("is_exposed")) creation_info.is_exposed = (GDExtensionBool)info["is_exposed"].as<bool>();
	if (info.hasOwnProperty("is_runtime")) creation_info.is_runtime = (GDExtensionBool)info["is_runtime"].as<bool>();
	creation_info.set_func = extension_class3_set_func;
	creation_info.get_func = extension_class3_get_func;
	creation_info.get_property_list_func = extension_class3_get_property_list_func;
	creation_info.free_property_list_func = extension_class3_free_property_list_func;
	creation_info.property_can_revert_func = extension_class3_property_can_revert_func;
	creation_info.property_get_revert_func = extension_class3_property_get_revert_func;
	creation_info.validate_property_func = extension_class3_validate_property_func;
	creation_info.notification_func = extension_class3_notification_func;
	creation_info.to_string_func = extension_class3_to_string_func;
	creation_info.reference_func = extension_class3_reference_func;
	creation_info.unreference_func = extension_class3_unreference_func;
	if (info.hasOwnProperty("create_instance")) creation_info.create_instance_func = extension_class3_create_instance_func;
	creation_info.free_instance_func = extension_class3_free_instance_func;
	creation_info.get_virtual_func = nullptr;
	if (info.hasOwnProperty("get_virtual_call_data")) creation_info.get_virtual_call_data_func = extension_class3_get_virtual_call_data_func;
	if (info.hasOwnProperty("get_virtual_call_data")) creation_info.call_virtual_with_data_func = extension_class3_call_virtual_with_data_func;
	//creation_info.get_rid_func = extension_class3_get_rid_func;
	JavascriptUserData *userdata = new JavascriptUserData();
	userdata->value = val(info);
	creation_info.class_userdata = userdata;
	GDExtension::_register_extension_class4(
		(GDExtensionClassLibraryPtr)token, (GDExtensionConstStringNamePtr)&class_name,
		(GDExtensionConstStringNamePtr)&base_class_name, &creation_info);
}

void class_method_call(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error) {
	JavascriptUserData *userdata = (JavascriptUserData*)method_userdata;
	JavascriptUserData *instance = (JavascriptUserData*)p_instance;
	for (int i = 0; i < p_argument_count; i++) {
		uint32_t *v = (uint32_t*)p_args[i];
		for (int j = 0; j < 6; j++) {
			result_buffer[0][i][j] = v[j];
		}
	}
	r_error->error = (GDExtensionCallErrorType)((userdata->value).call<int>("call", instance->value["ref"].as<uint32_t>(), (uint32_t)p_argument_count));
	uint32_t *v = (uint32_t*)r_return;
	for (int i = 0; i < 6; i++) {
		v[i] = params_buffer[0][0][i];
	}
}
void class_method_ptrcall(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret) {
	JavascriptUserData *userdata = (JavascriptUserData*)method_userdata;
	JavascriptUserData *instance = (JavascriptUserData*)p_instance;
	(userdata->value).call<void>("ptrcall", instance->value["ref"].as<uint32_t>(), (uint32_t)p_args, (uint32_t)r_ret);
}

void js_classdb_register_extension_class_method(uint32_t token, uint32_t class_name, val method_info) {
	GDExtensionPropertyInfo return_value_info = {};
	GDExtensionClassMethodInfo info = {};
	uint32_t name = method_info["name"].as<uint32_t>();
	JavascriptUserData *userdata = new JavascriptUserData();
	userdata->value = val(method_info);
	info.method_userdata = userdata;
	info.call_func = class_method_call;
	info.ptrcall_func = class_method_ptrcall;
	info.name = (GDExtensionStringNamePtr)&name;
	info.method_flags = method_info["method_flags"].as<uint32_t>();
	uint32_t return_name;
	uint32_t return_class_name;
    uint32_t return_hint_string;
	if (method_info.hasOwnProperty("return_value_info")) {
		val prop = method_info["return_value_info"];
		return_name = prop["name"].as<uint32_t>();
		return_class_name = prop["class_name"].as<uint32_t>();
	    return_hint_string = prop["hint_string"].as<uint32_t>();
		info.has_return_value = true;
		return_value_info.name = (GDExtensionStringNamePtr)&return_name;
		return_value_info.type = (GDExtensionVariantType)prop["type"].as<uint32_t>();
		return_value_info.class_name = (GDExtensionStringNamePtr)&return_class_name;
		return_value_info.hint = prop["hint"].as<uint32_t>();
		return_value_info.hint_string = (GDExtensionStringNamePtr)&return_hint_string;
		return_value_info.usage = prop["usage"].as<uint32_t>();
		info.return_value_info = &return_value_info;
		info.return_value_metadata = (GDExtensionClassMethodArgumentMetadata)method_info["return_value_metadata"].as<uint32_t>();
	}
	{
		val arguments_info = method_info["arguments_info"];
		val arguments_metadata = method_info["arguments_metadata"];
		uint32_t r_count = arguments_info["length"].as<uint32_t>();

		// Allocate memory properly with correct sizes
		GDExtensionPropertyInfo *result = (GDExtensionPropertyInfo *)gdextension_mem_alloc(sizeof(GDExtensionPropertyInfo) * r_count);
		GDExtensionClassMethodArgumentMetadata *metadata = (GDExtensionClassMethodArgumentMetadata *)gdextension_mem_alloc(sizeof(GDExtensionClassMethodArgumentMetadata) * r_count);

		// Separate memory allocation for string names (avoids alignment issues)
		GDExtensionStringNamePtr *name_storage = (GDExtensionStringNamePtr *)gdextension_mem_alloc(sizeof(GDExtensionStringNamePtr) * r_count);
		GDExtensionStringNamePtr *class_name_storage = (GDExtensionStringNamePtr *)gdextension_mem_alloc(sizeof(GDExtensionStringNamePtr) * r_count);
		GDExtensionStringNamePtr *hint_string_storage = (GDExtensionStringNamePtr *)gdextension_mem_alloc(sizeof(GDExtensionStringNamePtr) * r_count);

		for (uint32_t i = 0; i < r_count; i++) {
		    val prop = arguments_info[i];

		    // Store names in dedicated storage
		    name_storage[i] = (GDExtensionStringNamePtr)prop["name"].as<uint32_t>();
		    class_name_storage[i] = (GDExtensionStringNamePtr)prop["class_name"].as<uint32_t>();
		    hint_string_storage[i] = (GDExtensionStringNamePtr)prop["hint_string"].as<uint32_t>();

		    // Fill the result array with correct pointers
		    result[i].name = &name_storage[i];
		    result[i].type = (GDExtensionVariantType)prop["type"].as<uint32_t>();
		    result[i].class_name = &class_name_storage[i];
		    result[i].hint = prop["hint"].as<uint32_t>();
		    result[i].hint_string = &hint_string_storage[i];
		    result[i].usage = prop["usage"].as<uint32_t>();

		    // Fill the metadata array
		    metadata[i] = (GDExtensionClassMethodArgumentMetadata)arguments_metadata[i].as<uint32_t>();
		}

		// Assign properly to info
		info.argument_count = r_count;
		info.arguments_info = result;
		info.arguments_metadata = metadata;
	}
	{
		info.default_argument_count = method_info["default_argument_count"].as<uint32_t>();
		info.default_arguments = params_points;
	}
	GDExtension::_register_extension_class_method(
		(GDExtensionClassLibraryPtr)token, (GDExtensionConstStringNamePtr)&class_name, &info);
}

uint32_t js_get_library_path(uint32_t token) {
	uint32_t path;
	GDExtension::_get_library_path((GDExtensionClassLibraryPtr)token, &path);
	return path;
}
uint32_t js_classdb_construct_object(uint32_t class_name) {
	return (uint32_t)gdextension_classdb_construct_object(&class_name);
}

uint32_t js_memory_index(uint32_t addr, uint32_t n, uint32_t size) {
	uint8_t *ptr = (uint8_t*)addr;
	if (n >= 0 ) {
		ptr = ((uint8_t**)addr)[n];
	}
	uint8_t *src = (uint8_t*)ptr;
	uint8_t *dst = (uint8_t*)&result_buffer[0][0][0];
	for (uint32_t i = 0; i < size; i++) {
		dst[i] = src[i];
	}
	return (uint32_t)dst;
}

void js_memory_write(uint32_t addr, uint32_t size) {
	uint8_t *src = (uint8_t*)&params_buffer[0][0][0];
	uint8_t *dst = (uint8_t*)addr;
	for (uint32_t i = 0; i < size; i++) {
		dst[i] = src[i];
	}
}

EMSCRIPTEN_BINDINGS(my_module) {
	function("gdextension_mem_index", &js_memory_index);
	function("gdextension_mem_write", &js_memory_write);
	function("gdextension_read_result_buffer", &js_read_result_buffer);
	function("gdextension_write_params_buffer", &js_write_params_buffer);
	function("gdextension_write_params_buffer16", &js_write_params_buffer16);
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
	function("gdextension_string_operator_plus_eq_string", &js_string_operator_plus_eq_string);
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
	function("gdextension_object_free_instance_binding", &js_object_free_instance_binding);
	function("gdextension_object_set_instance", &js_object_set_instance);
	function("gdextension_object_get_class_name", &js_object_get_class_name);
	function("gdextension_object_cast_to", &js_object_cast_to);
	function("gdextension_object_get_instance_from_id", &js_object_get_instance_from_id);
	function("gdextension_object_get_instance_id", &js_object_get_instance_id);
	function("gdextension_object_has_script_method", &js_object_has_script_method);
	function("gdextension_object_call_script_method", &js_object_call_script_method);
	function("gdextension_ref_get_object", &js_ref_get_object);
	function("gdextension_ref_set_object", &js_ref_set_object);
	function("gdextension_script_instance_create3", &js_script_instance_create3);
	function("gdextension_placeholder_script_instance_create", &js_placeholder_script_instance_create);
	function("gdextension_placeholder_script_instance_update", &js_placeholder_script_instance_update);
	function("gdextension_object_get_script_instance", &js_object_get_script_instance);
	function("gdextension_callable_custom_create2", &js_callable_custom_create2);
	function("gdextension_classdb_get_class_tag", &js_classdb_get_class_tag);
	function("gdextension_classdb_get_method_bind", &js_classdb_get_method_bind);
	function("gdextension_classdb_register_extension_class3", &js_classdb_register_extension_class3);
	function("gdextension_get_library_path", &js_get_library_path);
	function("gdextension_classdb_register_extension_class_property", &js_classdb_register_extension_class_property);
	function("gdextension_classdb_construct_object", &js_classdb_construct_object);
	function("gdextension_classdb_register_extension_class_signal", &js_classdb_register_extension_class_signal);
	function("gdextension_classdb_register_extension_class_method", &js_classdb_register_extension_class_method);
}
