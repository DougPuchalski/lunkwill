window.lw_show_ModuleList = ->
	document.getElementById('HEAD').style.visibility = "visible";
	
	lw_mods=document.getElementById('HEAD')

	for Num,Name of window.lw_ModuleList
		lw_mods.innerHTML+=
			"<div class='module_list_entry' onclick=\"window.setCookie('module','#{Num}')\">
			<div class='module'>#{Name}</div></div>"

	lw_mods.innerHTML += "<div class='module_list_entry' style='float: right;'>Logout</div>"
	window.EvalScriptElem.parentNode.insertBefore lw_mods, window.EvalScriptElem
