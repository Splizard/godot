/**
 * Projects exported for the Web expose the :js:class:`Engine` class to the JavaScript environment, that allows
 * fine control over the engine's start-up process.
 *
 * This API is built in an asynchronous manner and requires basic understanding
 * of `Promises <https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Using_promises>`__.
 *
 * @module Engine
 * @header Web export JavaScript reference
 */
const Engine = (function () {
	const preloader = new Preloader();

	let loadPromise = null;
	let loadPath = "";
	let initPromise = null;

	/**
	 * @classdesc The ``Engine`` class provides methods for loading and starting exported projects on the Web. For default export
	 * settings, this is already part of the exported HTML page. To understand practical use of the ``Engine`` class,
	 * see :ref:`Custom HTML page for Web export <doc_customizing_html5_shell>`.
	 *
	 * @description Create a new Engine instance with the given configuration.
	 *
	 * @global
	 * @constructor
	 * @param {EngineConfig} initConfig The initial config for this instance.
	 */
	function Engine(initConfig) {
		// eslint-disable-line no-shadow
		this.config = new InternalConfig(initConfig);
		this.rtenv = null;
	}

	/**
	 * Load the engine from the specified base path.
	 *
	 * @param {string} basePath Base path of the engine to load.
	 * @param {number=} [size=0] The file size if known.
	 * @returns {Promise} A Promise that resolves once the engine is loaded.
	 *
	 * @function Engine.load
	 */
	Engine.load = function (basePath, size) {
		if (loadPromise == null) {
			loadPath = basePath;
			loadPromise = preloader.loadPromise(`${loadPath}.wasm`, size, true);
			requestAnimationFrame(preloader.animateProgress);
		}
		return loadPromise;
	};

	/**
	 * Unload the engine to free memory.
	 *
	 * This method will be called automatically depending on the configuration. See :js:attr:`unloadAfterInit`.
	 *
	 * @function Engine.unload
	 */
	Engine.unload = function () {
		loadPromise = null;
	};

	/**
	 * Safe Engine constructor, creates a new prototype for every new instance to avoid prototype pollution.
	 * @ignore
	 * @constructor
	 */
	function SafeEngine(initConfig) {
		const proto = /** @lends Engine.prototype */ {
			/**
			 * Initialize the engine instance. Optionally, pass the base path to the engine to load it,
			 * if it hasn't been loaded yet. See :js:meth:`Engine.load`.
			 *
			 * @param {string=} basePath Base path of the engine to load.
			 * @return {Promise} A ``Promise`` that resolves once the engine is loaded and initialized.
			 */
			init: function (basePath) {
				if (initPromise) {
					return initPromise;
				}
				if (loadPromise == null) {
					if (!basePath) {
						initPromise = Promise.reject(
							new Error("A base path must be provided when calling `init` and the engine is not loaded."),
						);
						return initPromise;
					}
					Engine.load(basePath, this.config.fileSizes[`${basePath}.wasm`]);
				}
				const me = this;
				function doInit(promise) {
					// Care! Promise chaining is bogus with old emscripten versions.
					// This caused a regression with the Mono build (which uses an older emscripten version).
					// Make sure to test that when refactoring.
					return new Promise(function (resolve, reject) {
						promise.then(function (response) {
							const cloned = new Response(response.clone().body, {
								headers: [["content-type", "application/wasm"]],
							});
							Godot(me.config.getModuleConfig(loadPath, cloned)).then(function (module) {
								window.GD = {};
								for (const key in module) {
									if (key.startsWith("gd_")) {
										window.GD[key.slice(3)] = module[key];
									}
								}
								module.gdextension_javascript_set_get_proc_address(function (name) {
									return module["gdextension_" + name];
								});

								// Load Go WASM with direct access to Godot's gd_* exports
								const go = new Go();
								go.importObject.gd = {};
								// Cross-memory copy: Go WASM memory → Godot WASM memory (single JS call)
								go.importObject.gd.bulk_copy = function (godot_dst, go_src, len) {
									new Uint8Array(module.HEAPU8.buffer, godot_dst, len).set(
										new Uint8Array(go._inst.exports.mem.buffer, go_src, len),
									);
								};
								WebAssembly.compileStreaming(fetch("library.wasm")).then(function (goModule) {
									var goImportList = WebAssembly.Module.imports(goModule);
									for (var i = 0; i < goImportList.length; i++) {
										var imp = goImportList[i];
										if (imp.module !== "gd") continue;
										// Prefer raw WASM exports (EMSCRIPTEN_KEEPALIVE puts them on module directly)
										var rawFn = module["_wasm_gd_" + imp.name];
										if (rawFn) {
											go.importObject.gd[imp.name] = rawFn;
										} else if (module["gd_" + imp.name]) {
											// Fallback to embind wrapper with >>> 0 unsigned fixup
											go.importObject.gd[imp.name] = (function (fn) {
												return function () {
													var a = arguments;
													switch (a.length) {
														case 0:
															return fn();
														case 1:
															return fn(a[0] >>> 0);
														case 2:
															return fn(a[0] >>> 0, a[1] >>> 0);
														case 3:
															return fn(a[0] >>> 0, a[1] >>> 0, a[2] >>> 0);
														case 4:
															return fn(a[0] >>> 0, a[1] >>> 0, a[2] >>> 0, a[3] >>> 0);
														case 5:
															return fn(
																a[0] >>> 0,
																a[1] >>> 0,
																a[2] >>> 0,
																a[3] >>> 0,
																a[4] >>> 0,
															);
														case 6:
															return fn(
																a[0] >>> 0,
																a[1] >>> 0,
																a[2] >>> 0,
																a[3] >>> 0,
																a[4] >>> 0,
																a[5] >>> 0,
															);
													}
												};
											})(module["gd_" + imp.name]);
										}
									}
									// Ring flush JS fallback (used when Godot lacks the C++ raw export)
									if (!go.importObject.gd.ring_flush) {
										console.warn("ring_flush: using JS fallback (rebuild Godot for native path)");
										var ptrcall =
											module["_wasm_gd_object_unsafe_call"] || module.gd_object_unsafe_call;
										// Go WASM uses 8-byte uintptr: Object(0,8) Method(8,8) Shape(16,8) Args(24,256)
										go.importObject.gd.ring_flush = function (ring_base, entry_stride, tail, head) {
											var h32 = module.HEAPU32;
											for (var i = tail >>> 0; i !== head >>> 0; i = (i + 1) >>> 0) {
												var base = (ring_base + (i & 0xff) * entry_stride) >>> 0;
												var b = base >>> 2;
												ptrcall(
													h32[b],
													h32[b + 2],
													0,
													h32[b + 5],
													h32[b + 4],
													(base + 24) >>> 0,
												);
											}
										};
									}
									WebAssembly.instantiate(goModule, go.importObject).then(function (instance) {
										window.GDExtension = {
											"res://library.gdextension": {},
										};
										go.run(instance);
										// Wire all Go WASM exports as GO callbacks,
										// bypassing js.FuncOf runtime overhead.
										// Each wrapper calls resume() after the export
										// to drive the Go goroutine scheduler.
										var exports = go._inst.exports;
										var resume = exports.resume;
										for (var key in exports) {
											if (key.startsWith("gd_on_")) {
												(function (fn) {
													GD[key.substring(3)] = function () {
														var r = fn.apply(null, arguments);
														resume();
														return r;
													};
												})(exports[key]);
											}
										}
										// Hot-path callbacks use direct WASM export
										// without resume() to avoid allocation overhead.
										for (var key in exports) {
											if (
												key.startsWith("gd_on_") &&
												key !== "gd_on_engine_init" &&
												key !== "gd_on_engine_exit" &&
												key !== "gd_on_first_frame" &&
												key !== "gd_on_every_frame" &&
												key !== "gd_on_final_frame" &&
												key !== "gd_on_init"
											) {
												GD[key.substring(3)] = exports[key];
											}
										}
										const paths = me.config.persistentPaths;
										module["initFS"](paths).then(function (err) {
											me.rtenv = module;
											if (me.config.unloadAfterInit) {
												Engine.unload();
											}
											resolve();
										});
									});
								});
							});
						});
					});
				}
				preloader.setProgressFunc(this.config.onProgress);
				initPromise = doInit(loadPromise);
				return initPromise;
			},

			/**
			 * Load a file so it is available in the instance's file system once it runs. Must be called **before** starting the
			 * instance.
			 *
			 * If not provided, the ``path`` is derived from the URL of the loaded file.
			 *
			 * @param {string|ArrayBuffer} file The file to preload.
			 *
			 * If a ``string`` the file will be loaded from that path.
			 *
			 * If an ``ArrayBuffer`` or a view on one, the buffer will used as the content of the file.
			 *
			 * @param {string=} path Path by which the file will be accessible. Required, if ``file`` is not a string.
			 *
			 * @returns {Promise} A Promise that resolves once the file is loaded.
			 */
			preloadFile: function (file, path) {
				return preloader.preload(file, path, this.config.fileSizes[file]);
			},

			/**
			 * Start the engine instance using the given override configuration (if any).
			 * :js:meth:`startGame <Engine.prototype.startGame>` can be used in typical cases instead.
			 *
			 * This will initialize the instance if it is not initialized. For manual initialization, see :js:meth:`init <Engine.prototype.init>`.
			 * The engine must be loaded beforehand.
			 *
			 * Fails if a canvas cannot be found on the page, or not specified in the configuration.
			 *
			 * @param {EngineConfig} override An optional configuration override.
			 * @return {Promise} Promise that resolves once the engine started.
			 */
			start: function (override) {
				this.config.update(override);
				const me = this;
				return me.init().then(function () {
					if (!me.rtenv) {
						return Promise.reject(new Error("The engine must be initialized before it can be started"));
					}

					let config = {};
					try {
						config = me.config.getGodotConfig(function () {
							me.rtenv = null;
						});
					} catch (e) {
						return Promise.reject(e);
					}
					// Godot configuration.
					me.rtenv["initConfig"](config);

					// Preload GDExtension libraries.
					if (me.config.gdextensionLibs.length > 0 && !me.rtenv["loadDynamicLibrary"]) {
						return Promise.reject(
							new Error(
								"GDExtension libraries are not supported by this engine version. " +
									'Enable "Extensions Support" for your export preset and/or build your custom template with "dlink_enabled=yes".',
							),
						);
					}
					return new Promise(function (resolve, reject) {
						for (const file of preloader.preloadedFiles) {
							me.rtenv["copyToFS"](file.path, file.buffer);
						}
						preloader.preloadedFiles.length = 0; // Clear memory
						me.rtenv["callMain"](me.config.args);
						initPromise = null;
						me.installServiceWorker();
						resolve();
					});
				});
			},

			/**
			 * Start the game instance using the given configuration override (if any).
			 *
			 * This will initialize the instance if it is not initialized. For manual initialization, see :js:meth:`init <Engine.prototype.init>`.
			 *
			 * This will load the engine if it is not loaded, and preload the main pck.
			 *
			 * This method expects the initial config (or the override) to have both the :js:attr:`executable` and :js:attr:`mainPack`
			 * properties set (normally done by the editor during export).
			 *
			 * @param {EngineConfig} override An optional configuration override.
			 * @return {Promise} Promise that resolves once the game started.
			 */
			startGame: function (override) {
				this.config.update(override);
				// Add main-pack argument.
				const exe = this.config.executable;
				const pack = this.config.mainPack || `${exe}.pck`;
				this.config.args = ["--main-pack", pack].concat(this.config.args);
				// Start and init with execName as loadPath if not inited.
				const me = this;
				return Promise.all([this.init(exe), this.preloadFile(pack, pack)]).then(function () {
					return me.start.apply(me);
				});
			},

			/**
			 * Create a file at the specified ``path`` with the passed as ``buffer`` in the instance's file system.
			 *
			 * @param {string} path The location where the file will be created.
			 * @param {ArrayBuffer} buffer The content of the file.
			 */
			copyToFS: function (path, buffer) {
				if (this.rtenv == null) {
					throw new Error("Engine must be inited before copying files");
				}
				this.rtenv["copyToFS"](path, buffer);
			},

			/**
			 * Request that the current instance quit.
			 *
			 * This is akin the user pressing the close button in the window manager, and will
			 * have no effect if the engine has crashed, or is stuck in a loop.
			 *
			 */
			requestQuit: function () {
				if (this.rtenv) {
					this.rtenv["request_quit"]();
				}
			},

			/**
			 * Install the progressive-web app service worker.
			 * @returns {Promise} The service worker registration promise.
			 */
			installServiceWorker: function () {
				if (this.config.serviceWorker && "serviceWorker" in navigator) {
					try {
						return navigator.serviceWorker.register(this.config.serviceWorker);
					} catch (e) {
						return Promise.reject(e);
					}
				}
				return Promise.resolve();
			},
		};

		Engine.prototype = proto;
		// Closure compiler exported instance methods.
		Engine.prototype["init"] = Engine.prototype.init;
		Engine.prototype["preloadFile"] = Engine.prototype.preloadFile;
		Engine.prototype["start"] = Engine.prototype.start;
		Engine.prototype["startGame"] = Engine.prototype.startGame;
		Engine.prototype["copyToFS"] = Engine.prototype.copyToFS;
		Engine.prototype["requestQuit"] = Engine.prototype.requestQuit;
		Engine.prototype["installServiceWorker"] = Engine.prototype.installServiceWorker;
		// Also expose static methods as instance methods
		Engine.prototype["load"] = Engine.load;
		Engine.prototype["unload"] = Engine.unload;
		return new Engine(initConfig);
	}

	// Closure compiler exported static methods.
	SafeEngine["load"] = Engine.load;
	SafeEngine["unload"] = Engine.unload;

	// Feature-detection utilities.
	SafeEngine["isWebGLAvailable"] = Features.isWebGLAvailable;
	SafeEngine["isFetchAvailable"] = Features.isFetchAvailable;
	SafeEngine["isSecureContext"] = Features.isSecureContext;
	SafeEngine["isCrossOriginIsolated"] = Features.isCrossOriginIsolated;
	SafeEngine["isSharedArrayBufferAvailable"] = Features.isSharedArrayBufferAvailable;
	SafeEngine["isAudioWorkletAvailable"] = Features.isAudioWorkletAvailable;
	SafeEngine["getMissingFeatures"] = Features.getMissingFeatures;

	return SafeEngine;
})();
if (typeof window !== "undefined") {
	window["Engine"] = Engine;
}
