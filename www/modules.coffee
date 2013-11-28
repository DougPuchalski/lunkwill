window.lw_show_ModuleList = ->
	lw_mods=document.createElement("div")
	lw_mods.id='ModuleList'
	lw_mods.setAttribute 'style', 'display:inline-block;overflow-x:auto;margin-left:5px;width:90%'
	lw_mods.innerHTML=''

	for Num,Name of window.lw_ModuleList
		lw_mods.innerHTML+=
			"<a onclick=\"window.setCookie('module','#{Num}')\"
			 style='background:#aa2211;color:#FFF;margin-left:5px;'>
			 <div style='margin:1px 10px;display: inline-block;'>
			 #{Name}</div></a>" 

	window.EvalScriptElem.parentNode.insertBefore lw_mods, window.EvalScriptElem
