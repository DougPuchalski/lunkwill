window.randomColor = ->
	hex = "0123456789abcdef"
	color = "#" + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16] + hex[Math.round((Math.random()*16))%16]

	document.body.style.backgroundColor = color

window.initRandomColor = ->
	CSS.add "body", "background:#FF6300; transition: background-color 10s linear; -webkit-transition: background-color 10s linear;
	-moz-transition: background-color 10s linear; -o-transition: background-color 10s linear; -ms-transition: background-color 10s linear;"

	
	CSS.flush()
	
	
	setInterval(window.randomColor, 10000)


window.lw_show_ModuleList = ->
	# Init interface
	document.getElementById('HEAD').style.visibility = "visible";
	
	setTimeout(initRandomColor, 10000);
	
	
	lw_mods=document.getElementById('HEAD')

	for Num,Name of window.lw_ModuleList
		lw_mods.innerHTML+=
			"<div class=\"module_list_entry\" onclick=\"window.setCookie('module','#{Num}')\">
			<div class='module'>#{Name}</div></div>"

	lw_mods.innerHTML += "<div class='module_list_entry' onclick='logout()' style='float: right;'>Logout</div>"

	window.if = new window.main_interface 
	window.if.setTitle("SetTitle Test :-)")

	window.if.build()
