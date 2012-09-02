JsDbg
Scriptable Windows debugging with node.js


**** TODO ****

* handle debug events (39)

* breakpoints (43)

* crash_analysis (60)
** output thread context, addresses, disasm, ...

* process snapshoting (63)

* list of dangerous functions (66) ... and more

**** FUNCTIONS ****

address(es?) func_resolve(module_name, func_name)
bp_set(address(es?), description, handler)

bytes = read_process_memory(address, size)
write_process_memory(address, bytes)

set_callback(exception_type, callback)
