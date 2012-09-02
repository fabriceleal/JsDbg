var jsdbg = require('./Debug/jsdbg').JsDbg;
var EventEmitter = require('events').EventEmitter;

// Extend prototypes so one can listen to events
for(var s in EventEmitter.prototype){
	jsdbg.prototype[s] = EventEmitter.prototype[s];
}

var j = new jsdbg();

j.on('Loaded', function(){
	console.log('Js: Loaded!');

	// list threads!
	j.listThreads(function(threads){
		console.log('Js: I have threads for you!');
		console.log('Js: Count = ' + threads.length);
		
		// Get context of first thread
		var ctx = threads[0].getContext();
		console.log('Js: Context = ' + JSON.stringify(ctx));
		
		// Detach process. Otherwise process will terminate abnormally!
		//j.detach();
	});

	var add = j.funcResolve("msvcrt.dll", "printf");
	console.log("Address of printf is " + add);
	j.setBp(add);
});

j.on('ExitProcess', function(){
	console.log('Js: Ended!');
	
})
j.on('LoadDll', function(){
	console.log('Js: LoadDLL !!!');
	
});
j.on('Exception', function(o){
	console.log('Js: Unknown Exception!!!');
	
});
j.on('AccessViolation', function(o){
	console.log('Js: access violation...');
});
j.on('Breakpoint', function(o){
	console.log('Js: Breakpoint...');
	console.log(JSON.stringify(o));
	//console.log('Js: ... at address: ' + o.Address)
});
j.on('SingleStep', function(o){
	console.log('Js: SingleStep...');
});
j.on('GuardPage', function(o){
	console.log('Js: GuardPage...');
});

//j.load("C:\\Windows\\system32\\calc.exe");
j.load("L:\\testefx\\testconsole\\Debug\\testconsole.exe");

j.run();

//j.detach();

console.log('Js: end of "main thread"');
