#CSS creator
CSS = {}
CSS.add = (affects , content) ->
	CSS.data ?= []
	CSS.data.push "#{affects} { #{content}; }"
CSS.remove=(affects)->
	if CSS.data?
		CSS.data = 
			(data for data in CSS.data when(CSS.data.indexOf data is 0))

#Session management
SESSION={}
#SESSION.GET ->
	
	
#Write data to window.document
flush = ->
	if CSS.data?

		#Create "style" tag

		head = document.getElementsByTagName('head')[0]

		OldStyle=head.getElementsByTagName('style')
		if OldStyle.length>0
			OldStyle.parentNode.removeChild(OldStyle)

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


flush()	
