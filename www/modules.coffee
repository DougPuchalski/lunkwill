window.randomColor = ->
	hex = "0123456789abcdef"
	r=50+(Math.round(Math.random()*1000))%125;
	g=r*0.618;
	b=r-g;

	x=[r,g,b]
	y=[]

	for e, i in x
		if i is 0
			y[0] = e
		else
			rand = Math.floor(Math.random() * (i + 1))
			y[i] = y[rand]
			y[rand] = e
	r=y[0]
	g=y[1]
	b=y[2]		

	color = "#" + hex[Math.floor(r/16)] + hex[Math.floor(r)%16] + hex[Math.floor(g/16)] + hex[Math.floor(g)%16] + hex[Math.floor(b/16)] + hex[Math.floor(b)%16]

	document.body.style.backgroundColor = color

window.initRandomColor = ->
	CSS.add "body", "background:#FF6300; transition: background-color 2s linear; -webkit-transition: background-color 2s linear;
	-moz-transition: background-color 2s linear; -o-transition: background-color 2s linear; -ms-transition: background-color 2s linear;"
	
	CSS.flush()
	
	setInterval(window.randomColor, 10000)

	if not window.shuffleColor?
		setInterval(window.randomColor, 15000)
		window.shuffleColor=true;

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
