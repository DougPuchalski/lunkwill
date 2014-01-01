window.randomColor = ->
	hex = "0123456789abcdef"
	color = "#" + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16]

	document.getElementById('title').style.background = color
	document.body.style.backgroundColor = color


window.lw_show_ModuleList = ->
	# Init interface
	document.getElementById('HEAD').style.visibility = "visible";
	setInterval(window.randomColor, 10000)
	
	lw_mods=document.getElementById('HEAD')

	for Num,Name of window.lw_ModuleList
		lw_mods.innerHTML+=
			"<div class='module_list_entry' onclick=\"window.setCookie('module','#{Num}')\">
			<div class='module'>#{Name}</div></div>"

	lw_mods.innerHTML += "<div class='module_list_entry' style='float: right;'>Logout</div>"
	window.EvalScriptElem.parentNode.insertBefore lw_mods, window.EvalScriptElem
