#CSS creator
CSS = {}
CSS.add = (affects , content) ->
	CSS.data ?= []
	CSS.data.push "#{affects} { #{content}; }"
CSS.remove=(affects)->
	if CSS.data?
		CSS.data = 
			(data for data in CSS.data when(CSS.data.indexOf data is 0))
	
#Write data to window.document
flush = ->
	if CSS.data?

		#Create "style" tag

		head = document.getElementsByTagName('head')[0]
		if head?
			OldStyle?.parentNode?.removeChild?(OldStyle) for OldStyle in head.getElementsByTagName('style')

		style = document.createElement 'style'
		style.type = 'text/css';

		#loop through css data

		for css in CSS.data
			do (css) ->
				if style.styleSheet?
					style.styleSheet.cssText = css;
				else
					style.appendChild(document.createTextNode css )

				head.appendChild style

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
	location.hash="/"+projectCookie()+"/"+moduleCookie()+"/"

loginCookie= ->
	a=window.readCookie("login")
	return if( a? and a.length is 20) then a else  "AAAAAAAAAAAAAAAAAAAA"

projectCookie= ->
	a=window.readCookie("project")
	return if( a? and a.length is 4) then a else "AAAA"

moduleCookie= ->
	a=window.readCookie("module")
	return if( a? and a.length is 2) then a else "AA"

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
			eval(s.text) for s in document.getElementById("base").getElementsByTagName('script')

	document.body.style.visibility="hidden"
	HttpRequest.send()

#Add default css values
CSS.add "body", "background-color: #aa2211;
	font-family: Verdana, Helvetica, Arial"
		
CSS.add ".title", "font-size: 46px;
	color: #FFF;
	font-weight:bold;
	height:100px;
	background:#aa2211"

CSS.add ".menu", "width: 150px;
	overflow: hidden;
	text-align: center;
	background: #aa2211;
	font-weight: bold;
	height: 5px"

CSS.add ".base", "position:absolute;
	top:80px;
	bottom:50px;
	left:0px;
	right:0px;
	margin:auto;
	width:100%;
	height: 80%;
	background:#FFF;
	overflow: auto;
	white-space:nowrap"

CSS.add ".header", "margin-top: 10px;
	height: 35px"

CSS.add "a.silentlink", "color: #000000;
	text-decoration: none"

CSS.add "a:link", "color:#000;"
CSS.add "a:visited", "color:#000;"
CSS.add "a:active", "color:#000;"
CSS.add "a:hover", "color:#000;"

window.onhashchange =  ->
	a=window.loading
	window.loading=true
	b=location.hash.split('/')[1]
	c=location.hash.split('/')[2]
	window.setCookie "project", b
	window.setCookie "module",  c
	window.loading = a
	window.ajax '' if window.loading is false

window.onload = ->
	flush()
	window.ajax ""

