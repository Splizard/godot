/**************************************************************************/
/*  file_access_http.cpp                                                   */
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

#include "file_access_http.h"

#include "core/io/http_client.h"
#include "core/os/os.h"
#include "core/string/print_string.h"

#ifdef WEB_ENABLED
#include <emscripten.h>
#endif

Mutex FileAccessHTTP::registry_mutex;
HashMap<String, FileAccessHTTP::Remote *> FileAccessHTTP::registry;

bool FileAccessHTTP::is_http_path(const String &p_path) {
	return p_path.begins_with("http://") || p_path.begins_with("https://");
}

FileAccessHTTP::Remote *FileAccessHTTP::_get_remote(const String &p_url) {
	MutexLock lock(registry_mutex);
	Remote **existing = registry.getptr(p_url);
	if (existing) {
		return *existing;
	}
	Remote *r = memnew(Remote);
	r->url = p_url;
	registry.insert(p_url, r);
	return r;
}

void FileAccessHTTP::cleanup() {
	MutexLock lock(registry_mutex);
	for (KeyValue<String, Remote *> &kv : registry) {
		memdelete(kv.value);
	}
	registry.clear();
}

Error FileAccessHTTP::open_internal(const String &p_path, int p_mode_flags) {
	ERR_FAIL_COND_V_MSG(p_mode_flags & WRITE, ERR_UNAVAILABLE, "FileAccessHTTP is read-only.");
	remote = _get_remote(p_path);
	pos = 0;
	eof = false;
	last_error = OK;

	MutexLock lock(remote->mutex);
	if (!remote->probed) {
		Error err = _probe(remote);
		if (err != OK) {
			remote = nullptr;
			return err;
		}
		remote->probed = true;
		print_verbose(vformat("FileAccessHTTP: opened %s (%d bytes)", p_path, remote->length));
	}
	return OK;
}

bool FileAccessHTTP::is_open() const {
	return remote != nullptr;
}

void FileAccessHTTP::seek(uint64_t p_position) {
	pos = p_position;
	eof = false;
}

void FileAccessHTTP::seek_end(int64_t p_position) {
	ERR_FAIL_NULL(remote);
	pos = remote->length + p_position;
	eof = false;
}

uint64_t FileAccessHTTP::get_position() const {
	return pos;
}

uint64_t FileAccessHTTP::get_length() const {
	ERR_FAIL_NULL_V(remote, 0);
	return remote->length;
}

bool FileAccessHTTP::eof_reached() const {
	return eof;
}

Error FileAccessHTTP::get_error() const {
	return last_error;
}

bool FileAccessHTTP::file_exists(const String &p_name) {
	// Existence is meaningless without a probe; report false for arbitrary paths.
	return false;
}

uint64_t FileAccessHTTP::_read_range(uint64_t p_from, uint64_t p_to, uint8_t *p_dst) const {
	MutexLock lock(remote->mutex);
	p_to = MIN(p_to, remote->length);
	if (p_from >= p_to) {
		return 0;
	}
	const uint64_t first = p_from / BLOCK_SIZE;
	const uint64_t last = (p_to - 1) / BLOCK_SIZE;

	// Coalesce contiguous missing blocks into single Range fetches so a large
	// sequential read costs one request, not one-per-block.
	uint64_t b = first;
	while (b <= last) {
		if (remote->blocks.has(b)) {
			b++;
			continue;
		}
		uint64_t run_end = b;
		while (run_end + 1 <= last && !remote->blocks.has(run_end + 1)) {
			run_end++;
		}
		const uint64_t start = b * BLOCK_SIZE;
		const uint64_t end = MIN(remote->length, (run_end + 1) * BLOCK_SIZE);
		Vector<uint8_t> data;
		Error err = _fetch(remote, start, end - start, data);
		if (err != OK || (uint64_t)data.size() != end - start) {
			last_error = ERR_FILE_CANT_READ;
			return 0;
		}
		for (uint64_t i = b; i <= run_end; i++) {
			const uint64_t bs = (i - b) * BLOCK_SIZE;
			const uint64_t bl = MIN(BLOCK_SIZE, (uint64_t)data.size() - bs);
			Vector<uint8_t> blk;
			blk.resize(bl);
			memcpy(blk.ptrw(), data.ptr() + bs, bl);
			remote->blocks.insert(i, blk);
			remote->lru.push_front(i);
		}
		b = run_end + 1;
	}

	// Copy out of the (now resident) blocks.
	uint64_t copied = 0;
	for (uint64_t i = first; i <= last; i++) {
		const Vector<uint8_t> &blk = remote->blocks[i];
		const uint64_t block_start = i * BLOCK_SIZE;
		const uint64_t s = MAX(p_from, block_start) - block_start;
		const uint64_t e = MIN(p_to, block_start + (uint64_t)blk.size()) - block_start;
		if (e > s) {
			memcpy(p_dst + copied, blk.ptr() + s, e - s);
			copied += e - s;
		}
		remote->lru.erase(i);
		remote->lru.push_front(i);
	}

	// Evict LRU tail, never the blocks we just served.
	while ((uint64_t)remote->blocks.size() > MAX_BLOCKS && !remote->lru.is_empty()) {
		uint64_t victim = remote->lru.back()->get();
		remote->lru.pop_back();
		if (victim >= first && victim <= last) {
			// Currently in use; keep it, push to front so we try another.
			remote->lru.push_front(victim);
			break;
		}
		remote->blocks.erase(victim);
	}
	return copied;
}

uint64_t FileAccessHTTP::get_buffer(uint8_t *p_dst, uint64_t p_length) const {
	ERR_FAIL_NULL_V(remote, 0);
	if (p_length == 0) {
		return 0;
	}
	if (pos >= remote->length) {
		eof = true;
		return 0;
	}
	uint64_t want = p_length;
	if (pos + want > remote->length) {
		want = remote->length - pos;
		eof = true;
	}
	uint64_t got = _read_range(pos, pos + want, p_dst);
	pos += got;
	return got;
}

/* ------------------------------------------------------------------------ */
/* Transport                                                                 */
/* ------------------------------------------------------------------------ */

#ifdef WEB_ENABLED

// Synchronous XHR with a Range header. Only valid OFF the browser main thread,
// so the web template must be built with proxy_to_pthread=yes (engine loop on a
// worker). Sync XHR can't use responseType="arraybuffer", so we pull bytes out
// of responseText via the x-user-defined charset trick.
EM_JS(int, godot_http_range_sync, (const char *p_url, double p_start, double p_len, uint8_t *p_buf, double p_cap), {
	try {
		const url = UTF8ToString(p_url);
		const xhr = new XMLHttpRequest();
		xhr.open("GET", url, false);
		xhr.setRequestHeader("Range", "bytes=" + p_start + "-" + (p_start + p_len - 1));
		xhr.overrideMimeType("text/plain; charset=x-user-defined");
		xhr.send(null);
		if (xhr.status !== 206 && xhr.status !== 200) {
			return -1;
		}
		const t = xhr.responseText;
		const n = Math.min(t.length, p_cap);
		for (let i = 0; i < n; i++) {
			HEAPU8[p_buf + i] = t.charCodeAt(i) & 0xff;
		}
		return n;
	} catch (e) {
		return -2;
	}
});

// Probe via Range: bytes=0-0; parse total length from Content-Range "…/<total>".
EM_JS(double, godot_http_probe_sync, (const char *p_url), {
	try {
		const url = UTF8ToString(p_url);
		const xhr = new XMLHttpRequest();
		xhr.open("GET", url, false);
		xhr.setRequestHeader("Range", "bytes=0-0");
		xhr.overrideMimeType("text/plain; charset=x-user-defined");
		xhr.send(null);
		if (xhr.status === 206) {
			const cr = xhr.getResponseHeader("Content-Range");
			if (cr) {
				return parseFloat(cr.substring(cr.lastIndexOf("/") + 1));
			}
		}
		const cl = xhr.getResponseHeader("Content-Length");
		return cl ? parseFloat(cl) : -1;
	} catch (e) {
		return -2;
	}
});

Error FileAccessHTTP::_probe(Remote *p_remote) {
	double total = godot_http_probe_sync(p_remote->url.utf8().get_data());
	if (total < 0) {
		ERR_PRINT(vformat("FileAccessHTTP: probe failed for %s (code %d)", p_remote->url, (int)total));
		return ERR_CANT_OPEN;
	}
	p_remote->length = (uint64_t)total;
	return OK;
}

Error FileAccessHTTP::_fetch(Remote *p_remote, uint64_t p_offset, uint64_t p_len, Vector<uint8_t> &p_out) {
	p_out.resize(p_len);
	int n = godot_http_range_sync(p_remote->url.utf8().get_data(), (double)p_offset, (double)p_len, p_out.ptrw(), (double)p_len);
	if (n < 0) {
		ERR_PRINT(vformat("FileAccessHTTP: range fetch failed for %s @%d+%d (code %d)", p_remote->url, p_offset, p_len, n));
		return ERR_FILE_CANT_READ;
	}
	p_out.resize(n);
	p_remote->bytes_fetched += n;
	p_remote->requests++;
	return OK;
}

#else // !WEB_ENABLED — desktop / default transport via Godot HTTPClient.

namespace {
struct ParsedURL {
	bool tls = false;
	String host;
	int port = 80;
	String path;
};

static bool _parse_url(const String &p_url, ParsedURL &r_out) {
	String rest;
	if (p_url.begins_with("https://")) {
		r_out.tls = true;
		r_out.port = 443;
		rest = p_url.substr(8);
	} else if (p_url.begins_with("http://")) {
		r_out.tls = false;
		r_out.port = 80;
		rest = p_url.substr(7);
	} else {
		return false;
	}
	int slash = rest.find("/");
	String authority = slash == -1 ? rest : rest.substr(0, slash);
	r_out.path = slash == -1 ? "/" : rest.substr(slash);
	int colon = authority.find(":");
	if (colon != -1) {
		r_out.host = authority.substr(0, colon);
		r_out.port = authority.substr(colon + 1).to_int();
	} else {
		r_out.host = authority;
	}
	return !r_out.host.is_empty();
}

// Blocking ranged GET (or probe with p_len==0). Follows redirects. Fills
// r_body with the response bytes and r_total with the resource's full size
// (parsed from Content-Range, else Content-Length).
static Error _http_range(const String &p_url, uint64_t p_start, uint64_t p_len, Vector<uint8_t> &r_body, uint64_t &r_total) {
	String url = p_url;
	for (int redirect = 0; redirect < 8; redirect++) {
		ParsedURL u;
		if (!_parse_url(url, u)) {
			return ERR_INVALID_PARAMETER;
		}
		Ref<HTTPClient> client = Ref<HTTPClient>(HTTPClient::create());
		Ref<TLSOptions> tls = u.tls ? TLSOptions::client() : Ref<TLSOptions>();
		Error err = client->connect_to_host(u.host, u.port, tls);
		if (err != OK) {
			return err;
		}
		// Wall-clock deadline so an unreachable/stalled host can't hang the
		// caller forever (FileAccess reads are synchronous and on a hot path).
		const uint64_t deadline = OS::get_singleton()->get_ticks_msec() + 30000;
		while ((client->get_status() == HTTPClient::STATUS_CONNECTING || client->get_status() == HTTPClient::STATUS_RESOLVING) && OS::get_singleton()->get_ticks_msec() < deadline) {
			client->poll();
			OS::get_singleton()->delay_usec(1000);
		}
		if (client->get_status() != HTTPClient::STATUS_CONNECTED) {
			return ERR_CANT_CONNECT;
		}

		Vector<String> headers;
		headers.push_back("User-Agent: GodotEngine/FileAccessHTTP");
		headers.push_back("Accept: */*");
		const uint64_t end = p_start + (p_len == 0 ? 1 : p_len) - 1;
		headers.push_back(vformat("Range: bytes=%d-%d", p_start, end));

		err = client->request(HTTPClient::METHOD_GET, u.path, headers, nullptr, 0);
		if (err != OK) {
			return err;
		}
		while (client->get_status() == HTTPClient::STATUS_REQUESTING && OS::get_singleton()->get_ticks_msec() < deadline) {
			client->poll();
			OS::get_singleton()->delay_usec(1000);
		}
		if (!client->has_response()) {
			return ERR_CANT_CONNECT;
		}

		const int code = client->get_response_code();
		List<String> rh;
		client->get_response_headers(&rh);

		// Redirect.
		if (code == 301 || code == 302 || code == 303 || code == 307 || code == 308) {
			String location;
			for (const String &h : rh) {
				if (h.to_lower().begins_with("location:")) {
					location = h.substr(h.find(":") + 1).strip_edges();
					break;
				}
			}
			if (location.is_empty()) {
				return ERR_CANT_RESOLVE;
			}
			if (location.begins_with("/")) {
				location = (u.tls ? "https://" : "http://") + u.host + ":" + itos(u.port) + location;
			}
			url = location;
			continue;
		}
		if (code != 206 && code != 200) {
			ERR_PRINT(vformat("FileAccessHTTP: unexpected status %d for %s", code, url));
			return ERR_FILE_CANT_READ;
		}

		// Total size: prefer Content-Range "bytes a-b/total", else Content-Length.
		r_total = 0;
		for (const String &h : rh) {
			String l = h.to_lower();
			if (l.begins_with("content-range:")) {
				int slash = h.find("/");
				if (slash != -1) {
					r_total = h.substr(slash + 1).strip_edges().to_int();
				}
			} else if (r_total == 0 && l.begins_with("content-length:") && code == 200) {
				r_total = h.substr(h.find(":") + 1).strip_edges().to_int();
			}
		}

		// Read the body. The deadline is extended on every chunk so a slow but
		// progressing transfer isn't cut off, while a stalled one still bails.
		r_body.clear();
		uint64_t body_deadline = OS::get_singleton()->get_ticks_msec() + 30000;
		while (client->get_status() == HTTPClient::STATUS_BODY) {
			client->poll();
			PackedByteArray chunk = client->read_response_body_chunk();
			if (chunk.size() > 0) {
				int base = r_body.size();
				r_body.resize(base + chunk.size());
				memcpy(r_body.ptrw() + base, chunk.ptr(), chunk.size());
				body_deadline = OS::get_singleton()->get_ticks_msec() + 30000;
			} else {
				if (OS::get_singleton()->get_ticks_msec() >= body_deadline) {
					ERR_PRINT(vformat("FileAccessHTTP: body read stalled for %s", url));
					return ERR_FILE_CANT_READ;
				}
				OS::get_singleton()->delay_usec(1000);
			}
		}

		// A 200 (no range support) means we got the whole file; slice it.
		if (code == 200 && p_len > 0) {
			if ((uint64_t)r_body.size() < p_start + p_len) {
				return ERR_FILE_CANT_READ;
			}
			Vector<uint8_t> slice;
			slice.resize(p_len);
			memcpy(slice.ptrw(), r_body.ptr() + p_start, p_len);
			r_body = slice;
		}
		return OK;
	}
	return ERR_CANT_RESOLVE;
}
} // namespace

Error FileAccessHTTP::_probe(Remote *p_remote) {
	Vector<uint8_t> body;
	uint64_t total = 0;
	Error err = _http_range(p_remote->url, 0, 0, body, total);
	if (err != OK) {
		return err;
	}
	if (total == 0) {
		ERR_PRINT(vformat("FileAccessHTTP: could not determine length of %s", p_remote->url));
		return ERR_CANT_OPEN;
	}
	p_remote->length = total;
	return OK;
}

Error FileAccessHTTP::_fetch(Remote *p_remote, uint64_t p_offset, uint64_t p_len, Vector<uint8_t> &p_out) {
	uint64_t total = 0;
	Error err = _http_range(p_remote->url, p_offset, p_len, p_out, total);
	if (err != OK) {
		return err;
	}
	p_remote->bytes_fetched += p_out.size();
	p_remote->requests++;
	return OK;
}

#endif // WEB_ENABLED
