window.readCookie = (name) ->
	nameEQ = name + "="
	ca = document.cookie.split(";")
	i = 0
	while i < ca.length
		c = ca[i]
		c = c.substring(1, c.length)  while c.charAt(0) is " "
		return c.substring(nameEQ.length, c.length).replace(/"/g, '')  if c.indexOf(nameEQ) is 0
		i++
	ca

window.setCookie = (cookieName, cookieValue) ->
	today = new Date()
	expire = new Date(today.getTime()+6000000)
	document.cookie = cookieName + "=" + escape(cookieValue) + ";expires=" + expire.toGMTString();

	d=location.hash.split('/')[3]
	location.hash="/"+projectCookie()+"/"+moduleCookie()+"/"+d

loginCookie= ->
	a=window.readCookie("login")
	return if( a? and a.length is 20) then a else  "AAAAAAAAAAAAAAAAAAAA"

projectCookie= ->
	a=window.readCookie("project")
	return if( a? and a.length is 4) then a else "AAAA"

moduleCookie= ->
	a=window.readCookie("module")
	return if( a? and a.length is 2) then a else "AA"

window.selectColor= ->
	document.getElementById('title').style.background = document.getElementById('selected_color').value
	document.body.style.background = document.getElementById('selected_color').value
	
	# Set color to cookie (only for user selected color)
	#window.setCookie("background_color", document.getElementById('selected_color').value)


window.ajax= (url) ->
	window.loading= true
	HttpRequest = new XMLHttpRequest()
	req="/"+projectCookie()+"/"+moduleCookie()+"/"+btoa url
	HttpRequest.open "GET", "/"+loginCookie()+req, false
	location.hash=req

	HttpRequest.addEventListener 'readystatechange', ->
		document.body.style.visibility="visible"
		if HttpRequest.readyState is 4
			window.loading = false
			document.getElementById("base").innerHTML=HttpRequest.responseText
			for s in document.getElementById("base").getElementsByTagName('script')
				window.EvalScriptElem=s
				eval(s.text)

	document.body.style.visibility="hidden"
	HttpRequest.send()
