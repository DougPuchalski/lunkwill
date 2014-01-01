#CSS creator
CSS = {}
CSS.add = (affects , content) ->
	CSS.data ?= []
	CSS.data.push "#{affects} { #{content}; }"
CSS.remove=(affects)->
	if CSS.data?
		CSS.data = 
			(data for data in CSS.data when(CSS.data.indexOf data is 0))

CSS.flush = ->
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
	
#Add default css values
CSS.add "body", "background-color: #FF6300;
	font-family: Verdana, Helvetica, Arial"
		
CSS.add ".title", "font-size: 46px;
	color: #FFF;
	font-weight:bold;
	height:100px;
	background:#FF6300; transition: background-color 10s linear; -webkit-transition: background-color 10s linear;
	-moz-transition: background-color 10s linear; -o-transition: background-color 10s linear; -ms-transition: background-color 10s linear;"

CSS.add "body", "background:#FF6300; transition: background-color 10s linear; -webkit-transition: background-color 10s linear;
	-moz-transition: background-color 10s linear; -o-transition: background-color 10s linear; -ms-transition: background-color 10s linear;"


CSS.add ".menu", "width: 150px;
	overflow: hidden;
	text-align: center;
	background: #aa2211;
	font-weight: bold;
	height: 5px"

CSS.add ".menu:hover", "background-color: #FF6300"

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
	white-space:nowrap;
	line-height: 150%"

CSS.add "div.module_list_entry", "display: inline-block; width: 100px; height: 20px; text-align: center;
	user-select: none; padding: 5px; user-select: none; -moz-user-select: none; -o-user-select: none;
	-webkit-user-select: none; -ms-user-select: none; float: left"
CSS.add "div.module_list_entry:hover", "background-color: #E6E6E6; cursor: pointer; font-weight: bold;"

CSS.add ".header", "height: 30px; background-color: #F0F0F0;  visibility: hidden; white-space:nowrap"

CSS.add "a.silentlink", "color: #000000; text-decoration: none"

CSS.add "a:link", "color:#000"
CSS.add "a:visited", "color:#000"
CSS.add "a:active", "color:#000"
CSS.add "a:hover", "color:#000"

CSS.add "div.module_title", "font-size: 28pt; margin-bottom: 20px;"
CSS.add "div.sidebar", "padding-top: 55px; padding-left: 20px"
