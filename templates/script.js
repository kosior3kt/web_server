/* Filename: script.js*/

var state = false;
function myFun () {
	if(state) {
		document.getElementById('twoja_stara')
			.innerHTML = 'HELL YEAH - state true!'
		state = false;
	} else {
		document.getElementById('twoja_stara')
			.innerHTML = 'state false!'
		state = true;
	}
}
