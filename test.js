var jsdbg = require('./Debug/jsdbg').JsDbg;
var j = new jsdbg();

j.load("C:\\Windows\\system32\\calc.exe");

// No need to attach
//j.attach(process.pid);

// list threads!
j.listThreads(function(threads){
	console.log('Js: I have threads for you!');
	console.log('Js: Count = ' + threads.length);
	
	// Get context of first thread
	var ctx = threads[0].getContext();
	console.log('Js: Context = ' + JSON.stringify(ctx));
	
	// Detach process. Otherwise process will terminate abnormally!
	j.detach();
});
console.log('Js: end of "main thread"');