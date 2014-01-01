window.lw_show_ModuleList = ->
	lw_mods=document.createElement("div")
	lw_mods.id='ModuleList'
	lw_mods.setAttribute 'style', 'display:inline-block;overflow-x:auto;margin-left:5px;width:90%'
	lw_mods.innerHTML=''

	for Num,Name of window.lw_ModuleList
		lw_mods.innerHTML+=
			"<a onclick=\"window.setCookie('module','#{Num}')\"
			 class='module'>
			 <div class='module_list_entry'>
			 #{Name}</div></a>" 

	window.EvalScriptElem.parentNode.insertBefore lw_mods, window.EvalScriptElem
