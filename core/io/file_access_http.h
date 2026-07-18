/**************************************************************************/
/*  file_access_http.h                                                     */
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

#pragma once

#include "core/io/file_access.h"
#include "core/os/mutex.h"
#include "core/templates/hash_map.h"
#include "core/templates/list.h"

// Read-only, seekable FileAccess backed by HTTP(S) Range requests.
//
// Lets a remote .pck be mounted and streamed: FileAccess::open() routes
// http(s):// READ opens here, so the stock PackedSourcePCK parses the pack
// directory and FileAccessPack reads individual files, all over Range — only
// the bytes actually touched are fetched.
//
// Bytes are cached per-URL (shared across all handles to the same pack) in
// fixed-size blocks, so repeated/overlapping reads don't re-hit the network.
class FileAccessHTTP : public FileAccess {
	GDSOFTCLASS(FileAccessHTTP, FileAccess);

public:
	// 256 KiB blocks: big enough to amortize request overhead, small enough
	// that touching one tiny resource doesn't drag in megabytes.
	static constexpr uint64_t BLOCK_SIZE = 256 * 1024;
	// Soft cap on cached blocks per URL (~64 MiB at 256 KiB) before LRU evict.
	static constexpr uint64_t MAX_BLOCKS = 256;

	// Per-URL shared state: probe result + block cache + connection. One
	// instance per distinct pack URL, reused by every FileAccessHTTP handle.
	struct Remote {
		String url;
		uint64_t length = 0;
		bool probed = false;
		String etag;
		String last_modified;

		Mutex mutex;
		HashMap<uint64_t, Vector<uint8_t>> blocks; // block index -> bytes
		List<uint64_t> lru; // most-recent at front

		// Diagnostics (handy while prototyping / for tests).
		uint64_t bytes_fetched = 0;
		uint64_t requests = 0;
	};

private:
	Remote *remote = nullptr; // owned by the static registry, not by us
	mutable uint64_t pos = 0;
	mutable bool eof = false;
	mutable Error last_error = OK;

	static Mutex registry_mutex;
	static HashMap<String, Remote *> registry;
	static Remote *_get_remote(const String &p_url);

	// Ensure cached coverage of [p_from, p_to) and copy into p_dst.
	uint64_t _read_range(uint64_t p_from, uint64_t p_to, uint8_t *p_dst) const;

	// --- transport (platform specific, defined in the .cpp) ---
	// Probe: discover total length (+ validators). Returns OK and sets fields.
	static Error _probe(Remote *p_remote);
	// Fetch [p_offset, p_offset+p_len) into p_out (resized to bytes read).
	static Error _fetch(Remote *p_remote, uint64_t p_offset, uint64_t p_len, Vector<uint8_t> &p_out);

public:
	static bool is_http_path(const String &p_path);
	static void cleanup();

	virtual Error open_internal(const String &p_path, int p_mode_flags) override;
	virtual bool is_open() const override;

	virtual void seek(uint64_t p_position) override;
	virtual void seek_end(int64_t p_position = 0) override;
	virtual uint64_t get_position() const override;
	virtual uint64_t get_length() const override;

	virtual bool eof_reached() const override;

	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const override;

	virtual Error get_error() const override;

	virtual Error resize(int64_t p_length) override { return ERR_UNAVAILABLE; }
	virtual void flush() override {}
	virtual bool store_buffer(const uint8_t *p_src, uint64_t p_length) override { return false; }

	virtual bool file_exists(const String &p_name) override;

	virtual uint64_t _get_modified_time(const String &p_file) override { return 0; }
	virtual uint64_t _get_access_time(const String &p_file) override { return 0; }
	virtual int64_t _get_size(const String &p_file) override { return -1; }

	virtual BitField<FileAccess::UnixPermissionFlags> _get_unix_permissions(const String &p_file) override { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, BitField<FileAccess::UnixPermissionFlags> p_permissions) override { return FAILED; }

	virtual bool _get_hidden_attribute(const String &p_file) override { return false; }
	virtual Error _set_hidden_attribute(const String &p_file, bool p_hidden) override { return ERR_UNAVAILABLE; }
	virtual bool _get_read_only_attribute(const String &p_file) override { return false; }
	virtual Error _set_read_only_attribute(const String &p_file, bool p_ro) override { return ERR_UNAVAILABLE; }

	virtual void close() override {}

	FileAccessHTTP() {}
};
