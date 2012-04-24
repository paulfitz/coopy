Module['preRun'] = function() {
    FS.createDataFile(
	'/',
	'broken_bridge.csv',
	Module['intArrayFromString'](Module['local']),
	true,
	true);
    FS.createDataFile(
	'/',
	'bridge.csv',
	Module['intArrayFromString'](Module['remote']),
      true,
	true);
};
Module['arguments'] = [ 'broken_bridge.csv', 'bridge.csv'];
Module['return'] = '';
Module['print'] = function(text) {
    Module['return'] += text + '\n';
};
