/**************************************************************************/
/*  gdextension_library_loader.h                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "core/extension/gdextension_loader.h"
#include "gdextension_library_loader_web.h"
#include "core/extension/gdextension.h"
#include <emscripten.h>
#include <emscripten/bind.h>

emscripten::val gdextension_javascript_get_proc_address;

struct JsWrapper {
	emscripten::val value;
};

void gdextension_javascript_set_get_proc_address(emscripten::val get_proc_address) {
	gdextension_javascript_get_proc_address = get_proc_address;
}

void gdextension_javascript_initialize(void* userdata, GDExtensionInitializationLevel level) {
	JsWrapper* wrapper = static_cast<JsWrapper*>(userdata);
	wrapper->value.call<void>("initialize", (uint32_t)level);
}

void gdextension_javascript_deinitialize(void* userdata, GDExtensionInitializationLevel level) {
	JsWrapper* wrapper = static_cast<JsWrapper*>(userdata);
	wrapper->value.call<void>("deinitialize", (uint32_t)level);
	delete wrapper;
}

Error GDExtensionJavascriptLoader::open_library(const String &p_path) {
	path = p_path;
	return OK;
}

Error GDExtensionJavascriptLoader::initialize(GDExtensionInterfaceGetProcAddress p_get_proc_address, const Ref<GDExtension> &p_extension, GDExtensionInitialization *r_initialization) {
	emscripten::val init = emscripten::val::object();
	init.set("minimum_initialization_level", 0);
	init.set("initialize", emscripten::val::null());
	init.set("deinitialize", emscripten::val::null());
	Error err = (Error)emscripten::val::global("GDExtension").call<uint32_t>(path.utf8().get_data(), gdextension_javascript_get_proc_address, (uint32_t)(uintptr_t)p_extension.ptr(), init);
	JsWrapper* wrapper = new JsWrapper{init};
	r_initialization->userdata = wrapper;
	if (init["initialize"] != emscripten::val::null()) r_initialization->initialize = gdextension_javascript_initialize;
	if (init["deinitialize"] != emscripten::val::null()) r_initialization->deinitialize = gdextension_javascript_deinitialize;
	return err;
}
void GDExtensionJavascriptLoader::close_library() {}
bool GDExtensionJavascriptLoader::is_library_open() const {return true;}
bool GDExtensionJavascriptLoader::has_library_changed() const {return false;}
bool GDExtensionJavascriptLoader::library_exists() const {return true;}

EMSCRIPTEN_BINDINGS(gdextension_javascript_loader) {
	function("gdextension_javascript_set_get_proc_address", &gdextension_javascript_set_get_proc_address);
}
